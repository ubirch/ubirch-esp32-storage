/*!
 * @file    storage.c
 * @brief TODO: ${FILE}
 *
 * ...
 *
 * @author Waldemar Gruenwald
 * @date   2018-11-14
 *
 * @copyright &copy; 2018 ubirch GmbH (https://ubirch.com)
 *
 * ```
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ```
 */

#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG


#include <esp_log.h>
#include <nvs.h>
#include <nvs_flash.h>

#include "storage.h"

static const char *TAG = "storage";

void init_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}


/*!
 * Cleanup the nvs storage handle after using it.
 *
 * @param[in]   handle  handle of the nvs
 * @param[in]   err     error during nvs operations
 * @return      error, which was passed into
 */
static esp_err_t storage_cleanup(nvs_handle handle, esp_err_t err) {
    ESP_LOGD(TAG, "cleanup");
    nvs_close(handle);
    return err;
}

/*!
 * code example:
 *
 * char *array = "this is a char array"
 * size_t a_len = sizeof(array)
 * kv_store("region", "key", array, a_len);
 *
 * size_t da_len = 5;
 * char defined_array[da_len] = "hello";
 * kv_store("region", "def_key", defined_array, da_len);
 *
 * // now load the data:
 * // uninitialized
 * char *load array;
 * size_t l_len = 0;
 * kv_load("region", "key", (void **)&load_array, &l_len);
 *
 * //already initialized:
 * char *p_defined_array = defined_array;
 * kv_load("region", "dev_key", (void **)&p_defined_array, &da_len);
 *
 * */


esp_err_t kv_store(char *region, char *key, void *val, size_t len) {
    nvs_handle storage_handle;
    ESP_LOGD(__func__, "%s", key);
    //open the memory
    esp_err_t err = nvs_open(region, NVS_READWRITE, &storage_handle);
    if (err != ESP_OK) return err;
    // get the length of the current stored value
    size_t temp_len;
    err = nvs_get_blob(storage_handle, key, NULL, &temp_len);
    if ((err != ESP_OK) && (err != ESP_ERR_NVS_NOT_FOUND)) return storage_cleanup(storage_handle, err);

    // delete the element, if the length is different from the previous length
    if (temp_len != len) {
        err = nvs_erase_key(storage_handle, key);
        if ((err != ESP_OK) && (err != ESP_ERR_NVS_NOT_FOUND)) return storage_cleanup(storage_handle, err);
    }
    ESP_LOG_BUFFER_HEXDUMP(TAG, val, (uint16_t) len, ESP_LOG_DEBUG);
    err = nvs_set_blob(storage_handle, key, val, len);
    if (err != ESP_OK) return storage_cleanup(storage_handle, err);

    err = nvs_commit(storage_handle);
    return storage_cleanup(storage_handle, err);
}


esp_err_t kv_load(char *region, char *key, void **val, size_t *len) {
    nvs_handle storage_handle;  //!< storage handle
    size_t blob_len = 0;        //!< actual length of the blob to read
    ESP_LOGD(__func__, "%s", key);

    // check the length, if bigger then 4 MB, return invalid length
    if(*len >  0x400000){
        return ESP_ERR_NVS_INVALID_LENGTH;
    }

    //open the memory
    ESP_LOGD(__func__, "open");
    esp_err_t err = nvs_open(region, NVS_READONLY, &storage_handle);
    if (err != ESP_OK) return err;
    // get the length of the current stored value
    ESP_LOGD(__func__, "get length");
    err = nvs_get_blob(storage_handle, key, NULL, &blob_len);
    if (err != ESP_OK) return storage_cleanup(storage_handle, err);
    ESP_LOGD(__func__, "blob_len = %d , len = %d", blob_len, *len);
    // check if TODO
    if (*len == 0) {  // no val buffer yet, allocate new memory space
        ESP_LOGD(__func__, "l == 0");
        *val = malloc(blob_len);
    } else if (blob_len > *len) {
        return storage_cleanup(storage_handle, ESP_ERR_NVS_INVALID_LENGTH);
    }
    *len = blob_len;    // set the correct length and get the data
    ESP_LOGD(__func__, "get blob");
    err = nvs_get_blob(storage_handle, key, *val, len);
    ESP_LOG_BUFFER_HEXDUMP(TAG, *val, (uint16_t) *len, ESP_LOG_DEBUG);
    return storage_cleanup(storage_handle, err);
}


/*
 * check for possible memory handling errors
 *
 * return error: true, if memory error, false otherwise
 */
esp_err_t memory_error_check(esp_err_t err) {
    bool error = true;
    switch (err) {
        case ESP_OK:
            error = false;
            break;
        case ESP_ERR_NVS_INVALID_HANDLE:
            ESP_LOGW(TAG, "memory invalid handle");
            break;
        case ESP_ERR_NVS_READ_ONLY:
            ESP_LOGW(TAG, "memory read only");
            break;
        case ESP_ERR_NVS_INVALID_NAME:
            ESP_LOGW(TAG, "memory invalid name");
            break;
        case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
            ESP_LOGW(TAG, "memory not enoug space");
            break;
        case ESP_ERR_NVS_REMOVE_FAILED:
            ESP_LOGW(TAG, "memory remove failed");
            break;
        case ESP_ERR_NVS_VALUE_TOO_LONG:
            ESP_LOGW(TAG, "memory value too long");
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            ESP_LOGW(TAG, "key not found");
        case ESP_ERR_NVS_INVALID_LENGTH:
            ESP_LOGW(TAG, "value does not fit into buffer");
        default:
            ESP_LOGW(TAG, "no handle for %d", err);
            break;
    }
    return error;
}

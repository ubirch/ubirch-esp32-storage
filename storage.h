/*!
 * @file    storage.h
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


#ifndef EXAMPLE_ESP32_STORAGE_H
#define EXAMPLE_ESP32_STORAGE_H

#include <esp_err.h>

void init_nvs(void);

//todo, check if this has to be here and if necessary
struct storage_element {
    char *key;
    void *value;
    size_t len;
    struct storage_element *next_element;
};

/*!
 * Store a key value pair in the flash memory.
 *
 * @param[in]   region  string of the memory region to write to (max 15 characters)
 * @param[in]   key     string of the specified key (max. 15 characters)
 * @param[in]   val     value to store, can be any data type
 * @param[in]   len     length of value to store in bytes
 *
 * @return
 *             - ESP_OK if value was set successfully
 *             - ESP_ERR_... (for details, see nvs.h)
 */
esp_err_t kv_store(char *region, char *key, void *val, size_t len);

/*!
 * Load a key value pair from the flash memory.
 *
 * @param[in]       region  string of the memory region to read from (max 15 characters)
 * @param[in]       key     string of the specified key (max. 15 characters)
 * @param[inout]    val     pointer to value to load, can be any data type.
 *                          @note: memory will be allocated for this value and returned.
 *                          use 'free(val)' to deallocate the memory after val is not needed anymore.
 * @param[out]      len     pointer to length of value to load in bytes.
 *                          @note: this value will be set to the correct length of the value.
 *
 * @return
 *             - ESP_OK if value was set successfully
 *             - ESP_ERR_... (for details, see nvs.h)
 */
esp_err_t kv_load(char *region, char *key, void **val, size_t *len);

esp_err_t memory_error_check(esp_err_t err);



#endif //EXAMPLE_ESP32_STORAGE_H

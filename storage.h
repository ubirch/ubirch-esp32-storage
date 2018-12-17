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

/*!
 * Initialize the non volatile storage
 */
void init_nvs(void);


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
 * @param[in,out]   val     pointer to value to load, can be any data type.
 *                          @note:
 *                          If input parameter len = 0,
 *                              memory will be allocated for this value and returned.
 *                              use 'free(val)' to deallocate the memory after val is not needed anymore.
*                           If input parameter len > 0,
 *                              no memory will be allocated, so the val buffer has to be big enough to load the value
 *                              into it.
 * @param[in,out]   len     pointer to length of value to load in bytes.
 *                          @note:
 *                          If len > actual length of the value in flash,
 *                              it will be set to the actual length of the value in the flash.
 *                          If len = 0,
 *                              memory space for val will be allocated. See param val.
 *
 * @return
 *             - ESP_OK if value was set successfully
 *             - ESP_ERR_... (for details, see nvs.h)
 */
esp_err_t kv_load(char *region, char *key, void **val, size_t *len);

/*!
 * Log output for any memory handling error
 *
 * @param err error, which occured during memory operation
 *
 * @return error for further error handling
 */
esp_err_t memory_error_check(esp_err_t err);

#endif //EXAMPLE_ESP32_STORAGE_H

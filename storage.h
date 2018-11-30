//
// Created by wowa on 14.11.18.
//

#ifndef EXAMPLE_ESP32_STORAGE_H
#define EXAMPLE_ESP32_STORAGE_H

#include <esp_err.h>

void init_nvs(void);

struct storage_element {
    char *key;
    void *value;
    size_t len;
    struct storage_element *next_element;
};

esp_err_t kv_store(char *region, char *key, void *val, size_t len);

esp_err_t kv_load(char *region, char *key, void **val, size_t *len);

//bool store_wifi_login(struct Wifi_login wifi);
//
//bool load_wifi_login(struct Wifi_login *wifi);
/*!
 * Load the last signature
 *
 * @param   signature pointer to signature array
 *
 * @return  true, if something went wrong,
 * @return  false if keys were successfully stored
 */
bool load_signature(unsigned char *signature);

/*!
 * Store the last signature.
 *
 * @param signature pointer to signature array
 * @param size_sig size of the signature array
 *
 * @return error: true if signature could not be stored, false otherwise
 */
bool store_signature(const unsigned char *signature, size_t size_sig);


#endif //EXAMPLE_ESP32_STORAGE_H

![ubirch logo](https://ubirch.de/wp-content/uploads/2018/10/cropped-uBirch_Logo.png)

# ESP32 ubirch storage functionality

This is an implementation of the ESP32 storage load and store functionality,
see `kv_load()` and `kv_store()`

## Example

Store and load of **initialized buffer**:

```c
...
// store data into flash region "region", with the key "def_key"
size_t da_len = 5;
char defined_array[da_len] = "hello";
kv_store("region", "def_key", defined_array, da_len);
...
// now load the data into an already initialized buffer:
char *p_defined_array = defined_array;
kv_load("region", "def_key", (void **)&p_defined_array, &da_len);
...
```

Store and load of **unitilized buffer**:

```c
...
char *array = "this is a char array"
size_t a_len = sizeof(array)
kv_store("region", "un_key", array, a_len);
...
// now load the data into an uninitialized buffer
char *load array;
size_t l_len = 0;
kv_load("region", "un_key", (void **)&load_array, &l_len);
...
// don't forget to free the memory, if load_array is not needed anymore
free(load_array);
...
```

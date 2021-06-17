idf_component_register(SRCS "esp_hidd_prf_api.c"
                            "hid_dev.c"
                            "hid_device_le_prf.c"
                            "ble_init.c"
                            "BLE_HID.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES log bt)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-const-variable)
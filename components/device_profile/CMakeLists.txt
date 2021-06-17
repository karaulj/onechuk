idf_component_register(SRCS "IDeviceProfile.cpp"
                            "RedDeviceProfile.cpp"
                            "GreenDeviceProfile.cpp"
                            "BlueDeviceProfile.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES log main ble_hid tinypico rgb_led nunchuk)
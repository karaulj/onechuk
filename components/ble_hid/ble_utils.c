/*
 * ble_utils.c
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#include "ble_utils.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "esp_hidd_prf_api.h"
#include "esp_bt_defs.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "driver/gpio.h"
#include "hid_dev.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char *TAG = "bleInit";


void bleInit()
{
    esp_err_t ret;

    // Initialize NVS.
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(TAG, "initialize controller failed: 0x%X\n", ret);
        return;
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_BLE)) != ESP_OK) {
        ESP_LOGE(TAG, "enable controller failed: 0x%X\n", ret);
        return;
    }

    if ((ret = esp_bluedroid_init()) != ESP_OK) {
        ESP_LOGE(TAG, "init bluedroid failed: 0x%X\n", ret);
        return;
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(TAG, "init bluedroid failed: 0x%X\n", ret);
        return;
    }

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "init successful");
    }
}   


void bleEnable()
{
    esp_bluedroid_enable();
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
}


void bleDisable()
{
    esp_bluedroid_disable();
    esp_bt_controller_disable();
    
}


#ifdef __cplusplus
}
#endif
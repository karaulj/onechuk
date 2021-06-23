/*
 * BLE_HID.cpp
 *
 *  Created on: Jun 13, 2021
 *      Author: Jacob
 */

#include "BLE_HID.h"
#include "queues.h"
#include "rgb_commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_err.h"
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

static const char *TAG = "BLE_HID";


volatile uint8_t bleConnected = 0;
static uint16_t hid_conn_id = 0;
static bool sec_conn = false;
static uint8_t hidd_service_uuid128[] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    //first uuid, 16bit, [12],[13] is the value
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x12, 0x18, 0x00, 0x00,
};

static esp_ble_adv_data_t hidd_adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006, //slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0010, //slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x03c0,       //HID Generic,
    .manufacturer_len = 0,
    .p_manufacturer_data =  NULL,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(hidd_service_uuid128),
    .p_service_uuid = hidd_service_uuid128,
    .flag = 0x6,
};

static esp_ble_adv_params_t hidd_adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x30,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    //.peer_addr            =
    //.peer_addr_type       =
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};


static void hidd_event_callback(esp_hidd_cb_event_t event, esp_hidd_cb_param_t *param)
{
    switch(event) {
        case ESP_HIDD_EVENT_REG_FINISH: {
            if (param->init_finish.state == ESP_HIDD_INIT_OK) {
                //esp_bd_addr_t rand_addr = {0x04,0x11,0x11,0x11,0x11,0x05};
                esp_ble_gap_set_device_name(BLE_HID_DEVICE_NAME);
                esp_ble_gap_config_adv_data(&hidd_adv_data);

            }
            break;
        }
        case ESP_BAT_EVENT_REG: {
            break;
        }
        case ESP_HIDD_EVENT_DEINIT_FINISH:
	     break;
		case ESP_HIDD_EVENT_BLE_CONNECT: {
            bleConnected = 1;
            ESP_LOGI(TAG, "ESP_HIDD_EVENT_BLE_CONNECT");
            hid_conn_id = param->connect.conn_id;
            // blink led
            xQueueSend(
                ledCmdQueue,
                (void*)&RGB_CMD_BLE_CONNECTED,
                1/portTICK_PERIOD_MS
            );
            break;
        }
        case ESP_HIDD_EVENT_BLE_DISCONNECT: {
            bleConnected = 0;
            sec_conn = false;
            ESP_LOGI(TAG, "ESP_HIDD_EVENT_BLE_DISCONNECT");
            esp_ble_gap_start_advertising(&hidd_adv_params);
            // blink led
            xQueueSend(
                ledCmdQueue,
                (void*)&RGB_CMD_BLE_DISCONNECTED,
                1/portTICK_PERIOD_MS
            );
            break;
        }
        case ESP_HIDD_EVENT_BLE_VENDOR_REPORT_WRITE_EVT: {
            ESP_LOGI(TAG, "%s, ESP_HIDD_EVENT_BLE_VENDOR_REPORT_WRITE_EVT", __func__);
            ESP_LOG_BUFFER_HEX(TAG, param->vendor_write.data, param->vendor_write.length);
        }
        default:
            break;
    }
    return;
}

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        esp_ble_gap_start_advertising(&hidd_adv_params);
        break;
     case ESP_GAP_BLE_SEC_REQ_EVT:
        for(int i = 0; i < ESP_BD_ADDR_LEN; i++) {
             ESP_LOGD(TAG, "%x:",param->ble_security.ble_req.bd_addr[i]);
        }
        esp_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);
	 break;
     case ESP_GAP_BLE_AUTH_CMPL_EVT:
        sec_conn = true;
        esp_bd_addr_t bd_addr;
        memcpy(bd_addr, param->ble_security.auth_cmpl.bd_addr, sizeof(esp_bd_addr_t));
        ESP_LOGI(TAG, "remote BD_ADDR: %08x%04x",\
                (bd_addr[0] << 24) + (bd_addr[1] << 16) + (bd_addr[2] << 8) + bd_addr[3],
                (bd_addr[4] << 8) + bd_addr[5]);
        ESP_LOGI(TAG, "address type = %d", param->ble_security.auth_cmpl.addr_type);
        ESP_LOGI(TAG, "pair status = %s",param->ble_security.auth_cmpl.success ? "success" : "fail");
        if(!param->ble_security.auth_cmpl.success) {
            ESP_LOGE(TAG, "fail reason = 0x%x",param->ble_security.auth_cmpl.fail_reason);
        }
        break;
    default:
        break;
    }
}


static void disablePairingCallback(TimerHandle_t xTimer)
{
    BLE_HID::getInstance()->setPairable(0);
}


BLE_HID* BLE_HID::hid = nullptr;


BLE_HID* BLE_HID::getInstance()
{
    if (hid == nullptr) {
        hid = new BLE_HID();
    }
    return hid;
}


BLE_HID::BLE_HID()
{
    esp_err_t ret;
    // HID init
    if((ret = esp_hidd_profile_init()) != ESP_OK) {
        ESP_LOGE(TAG, "hidd profile init failed: 0x%X\n", ret);
    }
    esp_ble_gap_register_callback(gap_event_handler);
    esp_hidd_register_callbacks(hidd_event_callback);

    // secure connections only
    esp_ble_auth_req_t auth_req = ESP_LE_AUTH_BOND | ESP_LE_AUTH_REQ_SC_ONLY | ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_ENABLE;
    esp_ble_io_cap_t iocap = ESP_IO_CAP_NONE;
    uint8_t key_size = 16;
    uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));

}

void BLE_HID::setPairable(uint8_t isPairable)
{
    esp_ble_io_cap_t iocap;
    if (isPairable) {
        iocap = ESP_IO_CAP_NONE;
    } else {
        iocap = ESP_IO_CAP_IN;
    }
    // From https://github.com/jakkra/Gesture-Detecting-Macro-Keyboard
    // A bit hacky, but cannot find any other way to have pairing enabled by a button.
    // Doing it by using whitelisting (adv_filter_policy) does not work and no way to enable/disable pairing.
    // When pairing is disabled, set IOCAP to keyboard and just reject the passkey request.
    esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
    ESP_LOGD(TAG, "set pairable: %d", isPairable);
}


void BLE_HID::startPairableWindow(uint32_t pairableTimeoutMs)
{
    if (!pairWindowTimerInit)
    {
        timerHandle = xTimerCreate(
            BLE_HID_PAIR_TIMER_NAME,
            pdMS_TO_TICKS(pairableTimeoutMs),
            pdFALSE, (void*)0, disablePairingCallback
        );
        pairWindowTimerInit = 1;
    }
    xTimerStop(timerHandle, portMAX_DELAY);
    
    setPairable(1);
    xTimerStart(timerHandle, portMAX_DELAY);
}


void BLE_HID::send(uint8_t keyCmd)
{
    esp_hidd_send_consumer_value(hid_conn_id, keyCmd, true);
    esp_hidd_send_consumer_value(hid_conn_id, keyCmd, false);
    vTaskDelay(5 / portTICK_PERIOD_MS);
}
void BLE_HID::sendPlayPause()
{
    send( (paused=!paused) ? HID_CONSUMER_PLAY : HID_CONSUMER_PAUSE );
}
void BLE_HID::sendVolumeUp()
{
    send(HID_CONSUMER_VOLUME_UP);
}
void BLE_HID::sendVolumeDown()
{
    send(HID_CONSUMER_VOLUME_DOWN);
}
void BLE_HID::sendNextTrack()
{
    send(HID_CONSUMER_SCAN_NEXT_TRK);
}
void BLE_HID::sendPrevTrack()
{
    send(HID_CONSUMER_SCAN_PREV_TRK);
}
void BLE_HID::sendShufflePlay()
{
    send(HID_CONSUMER_RANDOM_PLAY);
}
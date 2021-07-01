/*
 * GreenDeviceProfile.cpp
 * 
 *  Created on: Jun 13, 2021
 *      Author: Jacob
 */

#include "GreenDeviceProfile.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "queues.h"
#include "rgb_commands.h"
#include "BLE_HID.h"


static const char *TAG = "GreenDeviceProfile";


void GreenDeviceProfile::profileSelectedCallback()
{
    ESP_LOGI(TAG, "Green profile selected");
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GREEN_PROFILE, 1/portTICK_PERIOD_MS);
    BLE_HID* hid = BLE_HID::getInstance();      // initialize on switch to profile
}


void GreenDeviceProfile::gestureClickCallback()
{
    ESP_LOGI(TAG, "%s: sendPlayPause", __func__);
    BLE_HID* hid = BLE_HID::getInstance();
    hid->sendPlayPause();
}


void GreenDeviceProfile::gestureUpCallback()
{
    ESP_LOGI(TAG, "%s: sendVolumeUp", __func__);
    BLE_HID* hid = BLE_HID::getInstance();
    hid->sendVolumeUp();
}


void GreenDeviceProfile::gestureDownCallback()
{
    ESP_LOGI(TAG, "%s: sendVolumeDown", __func__);
    BLE_HID* hid = BLE_HID::getInstance();
    hid->sendVolumeDown();
}


void GreenDeviceProfile::gestureLeftCallback()
{
    ESP_LOGI(TAG, "%s: sendPrevTrack", __func__);
    BLE_HID* hid = BLE_HID::getInstance();
    hid->sendPrevTrack();
}


void GreenDeviceProfile::gestureRightCallback()
{
    ESP_LOGI(TAG, "%s: sendNextTrack", __func__);
    BLE_HID* hid = BLE_HID::getInstance();
    hid->sendNextTrack();
}


void GreenDeviceProfile::gestureCircleCallback()
{
    ESP_LOGI(TAG, "%s: startPairableWindow", __func__);
    BLE_HID* hid = BLE_HID::getInstance();
    hid->startPairableWindow();
    xQueueSend(
        ledCmdQueue,
        (void*)&RGB_CMD_BLE_PAIRING_MODE_ACTIVE,
        1/portTICK_PERIOD_MS
    );
}
/*
 * DeviceProfile.cpp
 * 
 *  Created on: Jun 13, 2021
 *      Author: Jacob
 */

#include "DeviceProfile.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "queues.h"
#include "rgb_commands.h"


static const char *TAG = "DeviceProfile";


uint32_t DeviceProfile::cnt = 0;


void DeviceProfile::gestureClickCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}


void DeviceProfile::gestureUpCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}


void DeviceProfile::gestureDownCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}


void DeviceProfile::gestureLeftCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}


void DeviceProfile::gestureRightCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}


void DeviceProfile::gestureCircleCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}
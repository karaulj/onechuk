/*
 * IDeviceProfile.cpp
 * 
 *  Created on: Jun 13, 2021
 *      Author: Jacob
 */

#include "IDeviceProfile.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "queues.h"
#include "rgb_commands.h"


static const char *TAG = "IDeviceProfile";


uint32_t IDeviceProfile::cnt = 0;


void IDeviceProfile::profileSelectedCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}


void IDeviceProfile::gestureClickCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}


void IDeviceProfile::gestureUpCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}


void IDeviceProfile::gestureDownCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}


void IDeviceProfile::gestureLeftCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}


void IDeviceProfile::gestureRightCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}


void IDeviceProfile::gestureCircleCallback()
{
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
    ESP_LOGW(TAG, "%s not implemented for profile with id %d", __func__, id);
}
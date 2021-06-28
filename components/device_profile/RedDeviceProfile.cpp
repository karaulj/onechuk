/*
 * RedDeviceProfile.cpp
 * 
 *  Created on: Jun 14, 2021
 *      Author: Jacob
 */

#include "RedDeviceProfile.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"         // restart
#include "esp_log.h"

#include "IDeviceProfile.h"
#include "tasks.h"              // suspend/resume inference task
#include "queues.h"
#include "rgb_commands.h"
#include "BLE_HID.h"            // BLE status
#include "NunchukController.h"  // accel/joystick fade
#include "TinyPICO.h"           // battery

static const char *TAG = "RedDeviceProfile";


void RedDeviceProfile::profileSelectedCallback()
{
    ESP_LOGI(TAG, "Red profile selected");
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_RED_PROFILE, 1/portTICK_PERIOD_MS);
}


void RedDeviceProfile::gestureClickCallback()
{
    ESP_LOGI(TAG, "%s: batteryLevel", __func__);
    // battery level
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_BATTERY_LEVEL, 1/portTICK_PERIOD_MS);
}


void RedDeviceProfile::gestureUpCallback()
{
    ESP_LOGI(TAG, "%s: bleStatus", __func__);
    // BLE HID status
    xQueueSend(
        ledCmdQueue,
        (void*)&((bleConnected) ? 
            RGB_CMD_BLE_CONNECTED : RGB_CMD_BLE_DISCONNECTED), 
        1/portTICK_PERIOD_MS
    );
}


void RedDeviceProfile::gestureDownCallback()
{
    ESP_LOGI(TAG, "%s: batteryChargingStatus", __func__);
    // battery charging status
    xQueueSend(
        ledCmdQueue,
        (void*)&((TinyPICO::getInstance()->isBatteryCharging()) ? 
            RGB_CMD_BATTERY_CHARGING : RGB_CMD_BATTERY_NOT_CHARGING), 
        1/portTICK_PERIOD_MS
    );
}


void RedDeviceProfile::gestureLeftCallback()
{
    ESP_LOGI(TAG, "%s: accelLEDFade", __func__);

    // turn off inference task so we can use the Z button
    vTaskSuspend(joystickInferenceTaskHandle);

    // do an LED thing based on accel values
    NunchukController* nunchuk = NunchukController::getInstance();
    while ( !nunchuk->getZButton() )
    {
        xQueueSend(
            ledCmdQueue,
            (void*)&RGB_CMD_ACCEL_FADE_10_MS,
            1/portTICK_PERIOD_MS
        );
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    while (nunchuk->getZButton()) { vTaskDelay(1 / portTICK_PERIOD_MS); }
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_CLEAR_ALL, 1/portTICK_PERIOD_MS);

    vTaskResume(joystickInferenceTaskHandle);
}


void RedDeviceProfile::gestureRightCallback()
{
    ESP_LOGI(TAG, "%s: joystickLEDFade", __func__);

    // use Z button
    vTaskSuspend(joystickInferenceTaskHandle);
    
    uint8_t colorCombo = 0;
    NunchukController* nunchuk = NunchukController::getInstance();
    while ( !nunchuk->getZButton() )
    {
        if ( nunchuk->getCButton() ) {  // cycle joystick color mappings with C button
            colorCombo = (colorCombo >= 2) ? 0 : colorCombo+1;
        }
        while ( nunchuk->getCButton() ) {}    // wait for button release

        switch (colorCombo)
        {
        case 0:
            xQueueSend(ledCmdQueue, (void*)&RGB_CMD_JOYSTICK_RG_FADE, 1/portTICK_PERIOD_MS);
            break;
        case 1:
            xQueueSend(ledCmdQueue, (void*)&RGB_CMD_JOYSTICK_RB_FADE, 1/portTICK_PERIOD_MS);
            break;
        case 2:
            xQueueSend(ledCmdQueue, (void*)&RGB_CMD_JOYSTICK_GB_FADE, 1/portTICK_PERIOD_MS);
            break;
        default:
            colorCombo = 0;
            break;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    while (nunchuk->getZButton()) { vTaskDelay(10 / portTICK_PERIOD_MS); }
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_CLEAR_ALL, 1/portTICK_PERIOD_MS);

    vTaskResume(joystickInferenceTaskHandle);
}


void RedDeviceProfile::gestureCircleCallback()
{
    ESP_LOGI(TAG, "%s: restarting in 3 seconds", __func__);
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_RESTART, 1/portTICK_PERIOD_MS);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    // restart ESP
    fflush(stdout);
    esp_restart();
}

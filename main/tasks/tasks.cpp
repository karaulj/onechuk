/*
 * tasks.cpp
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#include "tasks.h"
#include "queues.h"
#include "settings.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
// common
#include "NunchukController.h"
#include "RGB_CC_LED.h"
#include "rgb_commands.h"
// ble
#include "BLE_HID.h"
//
#include "TinyPICO.h"
// tflite
#include "gesture_model_tflite.h"


static const char *TAG = "tasks";


TaskHandle_t nunchukReadTaskHandle = NULL;
TaskHandle_t ledCmdTaskHandle = NULL;
TaskHandle_t trainingModeTaskHandle = NULL;
TaskHandle_t joystickInferenceTaskHandle = NULL;
TaskHandle_t joystickCmdTaskHandle = NULL;

// inference raw data
static uint8_t dataMatrix[INPUT_SHAPE_DIM][INPUT_SHAPE_DIM];


void nunchukReadTask(void *pvParameter)
{
    ESP_LOGI(TAG, "start %s", NUNCHUK_READ_TASK);
    NunchukController* nunchuk = NunchukController::getInstance(); 
    vTaskDelay(1 / portTICK_PERIOD_MS);
    while(true) {
        nunchuk->fetchLatestReadings();
    }
}


void ledCmdTask(void *pvParameter)
{
    ESP_LOGI(TAG, "start %s", LED_CMD_TASK);
    RGB_CC_LED* led = (RGB_CC_LED*) pvParameter;
    
    rgb_cmd_t ledCmd;
    while (true)
    {
        while (xQueueReceive(ledCmdQueue, (void*)&ledCmd, portMAX_DELAY) == pdTRUE)
        {
            switch(ledCmd)
            {
            case RGB_CMD_SYSTEM_INIT:
                rgbcSystemInit(led);
                break;
            case RGB_CMD_RESTART:
                rgbcRestart(led);
                break;
            case RGB_CMD_CLEAR_ALL:
                rgbcClearAll(led);
                break;
            case RGB_CMD_BLE_CONNECTED:
                rgbcBleConnected(led);
                break;
            case RGB_CMD_BLE_DISCONNECTED:
                rgbcBleDisconnected(led);
                break;
            case RGB_CMD_BLE_PAIRING_MODE_ACTIVE:
                rgbcBlePairingModeActive(led);
                break;
            case RGB_CMD_DEVICE_MODE:
                rgbcDeviceMode(led);
                break;
            case RGB_CMD_GESTURE_ACTIVE:
                rgbcGestureActive(led);
                break;
            case RGB_CMD_GESTURE_FOUND:
                rgbcGestureFound(led);
                break;
            case RGB_CMD_GESTURE_NOT_FOUND:
                rgbcGestureNotFound(led);
                break;
            case RGB_CMD_GESTURE_NOT_IMPL:
                rgbcGestureNotImpl(led);
                break;
            case RGB_CMD_CANCEL_GESTURE:
                rgbcCancelGesture(led);
                break;
            case RGB_CMD_TRAINING_MODE:
                rgbcTrainingMode(led);
                break;
            case RGB_CMD_BATTERY_RANGE:
                rgbcBatteryRange(led);
                break;
            case RGB_CMD_BATTERY_LEVEL:
                {
                    float batVoltage = TinyPICO::getInstance()->getBatteryVoltage();
                    batVoltage = (batVoltage < BAT_VOLTAGE_MIN) ? BAT_VOLTAGE_MIN : batVoltage;
                    batVoltage = (batVoltage > BAT_VOLTAGE_MAX) ? BAT_VOLTAGE_MAX : batVoltage;
                    color_level_t batLevel = (color_level_t)(
                        (float)RGB_CC_LED_MAX * ((batVoltage-BAT_VOLTAGE_MIN)/BAT_VOLTAGE_RANGE)
                    );
                    rgbcBatteryLevel(led, batLevel);
                }
                break;
            case RGB_CMD_BATTERY_CHARGING:
                rgbcBatteryCharging(led);
                break;
            case RGB_CMD_BATTERY_NOT_CHARGING:
                rgbcBatteryNotCharging(led);
                break;
            case RGB_CMD_ACCEL_FADE:
                rgbcAccelFade(led);
                break;
            case RGB_CMD_ACCEL_FADE_5_MS:
                rgbcAccelFade5ms(led);
                break;
            case RGB_CMD_ACCEL_FADE_10_MS:
                rgbcAccelFade10ms(led);
                break;
            case RGB_CMD_ACCEL_FADE_20_MS:
                rgbcAccelFade20ms(led);
                break;
            case RGB_CMD_ACCEL_FADE_50_MS:
                rgbcAccelFade50ms(led);
                break;
            case RGB_CMD_ACCEL_FADE_100_MS:
                rgbcAccelFade100ms(led);
                break;
            case RGB_CMD_JOYSTICK_RG_FADE:
                rgbcJoystick_RG_Fade(led);
                break;
            case RGB_CMD_JOYSTICK_RB_FADE:
                rgbcJoystick_RB_Fade(led);
                break;
            case RGB_CMD_JOYSTICK_GB_FADE:
                rgbcJoystick_GB_Fade(led);
                break;
            default:
                ESP_LOGW(TAG, "uknonwn LED cmd: %d", ledCmd);
                break;
            }
        }
    }
}


void trainingModeTask(void *pvParameter)
{
    ESP_LOGI(TAG, "start %s", TRAINING_MODE_TASK);

    NunchukController* nunchuk = NunchukController::getInstance();
    uint8_t isDrawing = 0;
    uint8_t zButton = nunchuk->getZButton();

    while (true)
    {
        nunchuk->fetchLatestReadings();
        zButton = nunchuk->getZButton();
        if (!zButton && isDrawing) {            // finish drawing
            printf("\n");
            xQueueSend(ledCmdQueue, (void*)&RGB_CMD_CLEAR_ALL, 1/portTICK_PERIOD_MS);
        } else if (zButton && !isDrawing) {    // start new drawing
            printf("START:");
            xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_ACTIVE, 1/portTICK_PERIOD_MS);
        }
        if (zButton) {
            printf(
                "%d,%d,",
                nunchuk->getJoystickX()/INPUT_PRESCALER,
                nunchuk->getJoystickY()/INPUT_PRESCALER
            );
        }
        isDrawing = zButton;
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}


void joystickInferenceTask(void *pvParameter)
{
    ESP_LOGI(TAG, "start %s", JOYSTICK_INFERENCE_TASK);

    NunchukController* nunchuk = NunchukController::getInstance();
    nunchuk->fetchLatestReadings();

    uint8_t cancelled = 0;
    uint8_t x, y;

    while (true)
    {
        nunchuk->fetchLatestReadings();
        if ( nunchuk->getCButton() || !nunchuk->getZButton() ) { continue; }
        xQueueSend(
            ledCmdQueue, 
            (void*)&RGB_CMD_GESTURE_ACTIVE,
            1/portTICK_PERIOD_MS
        );
        // get input 
        while (nunchuk->getZButton())
        {
            nunchuk->fetchLatestReadings();
            if (nunchuk->getCButton()) {    // press C button mid-gesture to cancel
                xQueueSend(
                    ledCmdQueue, 
                    (void*)&RGB_CMD_CANCEL_GESTURE,
                    1/portTICK_PERIOD_MS
                );
                cancelled = 1;
                break;
            }
            x = nunchuk->getJoystickX() / INPUT_PRESCALER;
            y = nunchuk->getJoystickY() / INPUT_PRESCALER;
            dataMatrix[x][y] = 1;
            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
        while (nunchuk->getCButton() || nunchuk->getZButton()) { nunchuk->fetchLatestReadings(); }
        if (cancelled) {
            cancelled = 0;
            continue;
        } 
        xQueueSend(
            ledCmdQueue, 
            (void*)&RGB_CMD_CLEAR_ALL,
            1/portTICK_PERIOD_MS
        );

        // run inference



        // send command (if any) down queue

        // reset input vars
        for (int i=0; i<INPUT_SHAPE_DIM; i++) {
            for (int j=0; j<INPUT_SHAPE_DIM; j++) {
                dataMatrix[i][j] = 0;
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void joystickCmdTask(void *pvParameter)
{
    ESP_LOGI(TAG, "start %s", JOYSTICK_CMD_TASK);

    NunchukController* nunchuk = NunchukController::getInstance();

    while (true)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    /*
    while (true)
    {
        nunchuk->fetchLatestReadings();
        if (nunchuk->getZButton()) {
            float batLevel = TinyPICO::getInstance()->getBatteryVoltage();
            uint8_t isCharging = TinyPICO::getInstance()->isBatteryCharging();
            ESP_LOGI(TAG, "batLevel = %f", batLevel);
            ESP_LOGI(TAG, "charging = %d", isCharging);
            uint8_t vPart = (uint8_t)batLevel;
            uint8_t mvPart = (uint8_t)((batLevel-vPart)*10.0);
            for (int i=0; i<vPart; i++) {
                xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_NOT_IMPL, 1/portTICK_PERIOD_MS);
                vTaskDelay(750 / portTICK_PERIOD_MS);
            }
            for (int i=0; i<mvPart; i++) {
                xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_ACTIVE, 1/portTICK_PERIOD_MS);
                vTaskDelay(400 / portTICK_PERIOD_MS);
                xQueueSend(ledCmdQueue, (void*)&RGB_CMD_CLEAR_ALL, 1/portTICK_PERIOD_MS);
                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        while (nunchuk->getZButton()) { vTaskDelay(1 / portTICK_PERIOD_MS); }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    */

}




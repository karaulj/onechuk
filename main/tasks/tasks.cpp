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

#include "NunchukController.h"
#include "RGB_CC_LED.h"
#include "rgb_commands.h"
#include "BLE_HID.h"

#include "esp_log.h"


static const char *TAG = "tasks";


void nunchukReadTask(void *pvParameter)
{
    ESP_LOGI(TAG, "start %s", NUNCHUK_READ_TASK);

    vTaskDelay(1 / portTICK_PERIOD_MS);
    NunchukController* nunchuk = (NunchukController*) pvParameter; 
    while(true)
    {
        nunchuk->fetchLatestReadings();
        xQueueSend(nunchukDataQueue, (void*)nunchuk->getLatestData(), 1/portTICK_PERIOD_MS);
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
            switch(ledCmd) {
            case RGB_CMD_SYSTEM_INIT:
                rgbcSystemInit(led);
                break;
            case RGB_CMD_RESTART:
                rgbcRestart(led);
                break;
            case RGB_CMD_BLE_CONNECTED:
                rgbcBleConnected(led);
                break;
            case RGB_CMD_BLE_DISCONNECTED:
                rgbcBleDisconnected(led);
                break;
            case RGB_CMD_GESTURE_MODE:
                rgbcGestureMode(led);
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
            case RGB_CMD_CLEAR_ALL:
                rgbcClearAll(led);
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

    nunchuk_data_t nunchukData;
    uint8_t isDrawing = 0;

    while (true)
    {
        while (xQueueReceive(nunchukDataQueue, (void*)&nunchukData, portMAX_DELAY) == pdTRUE)
        {
            if (!nunchukData.zButton && isDrawing) {            // finish drawing
                printf("\n");
                xQueueSend(ledCmdQueue, (void*)&RGB_CMD_CLEAR_ALL, 1/portTICK_PERIOD_MS);
            } else if (nunchukData.zButton && !isDrawing) {    // start new drawing
                printf("START:");
                xQueueSend(ledCmdQueue, (void*)&RGB_CMD_GESTURE_MODE, 1/portTICK_PERIOD_MS);
            }
            if (nunchukData.zButton) {
                printf(
                    "%d,%d,",
                    nunchukData.joystickX/INPUT_PRESCALER,
                    nunchukData.joystickY/INPUT_PRESCALER
                );
            }
            isDrawing = nunchukData.zButton;
        }
    }
}


void joystickCmdTask(void *pvParameter)
{
    ESP_LOGI(TAG, "start %s", JOYSTICK_CMD_TASK);

    uint8_t cycledCommands = 0;
    nunchuk_data_t nunchukData;
    uint8_t released = 1;
    //uint8_t currCmd = 0;

    BLE_HID* hid = BLE_HID::getInstance();
    hid->startPairableWindow();

    uint8_t lastBleStatus = bleConnected;
    while (true)
    {
        if (bleConnected != lastBleStatus) {
            if (bleConnected) {
                xQueueSend(ledCmdQueue, (void*)&RGB_CMD_BLE_CONNECTED, 1/portTICK_PERIOD_MS);
            } else {
                xQueueSend(ledCmdQueue, (void*)&RGB_CMD_BLE_DISCONNECTED, 1/portTICK_PERIOD_MS);
            }
            lastBleStatus = bleConnected;
        }

        xQueueReset(nunchukDataQueue);
        xQueueReceive(nunchukDataQueue, (void*)&nunchukData, 1/portTICK_PERIOD_MS);
        if (released) {
            if (nunchukData.zButton && (nunchukData.joystickY>250)) {
                hid->sendVolumeUp();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                released = 0;
            } else if (nunchukData.zButton && (nunchukData.joystickY<15)) {
                hid->sendVolumeDown();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                released = 0;
            } else if (nunchukData.zButton && (nunchukData.joystickX>250)) {
                hid->sendNextTrack();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                released = 0;
            } else if (nunchukData.zButton && (nunchukData.joystickX<15)) {
                hid->sendPrevTrack();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                released = 0;
            } else if (nunchukData.zButton) {
                hid->sendPlayPause();
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                released = 0;
            }
        } else { released = !nunchukData.zButton; }
        /*
        if (!cycledCommands && bleConnected)
        {
            vTaskDelay(3000 / portTICK_PERIOD_MS);
            ESP_LOGI(TAG, "sending volumeUp");
            hid->sendVolumeUp();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            ESP_LOGI(TAG, "sending volumeDown");
            hid->sendVolumeDown();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            ESP_LOGI(TAG, "sending nextTrack");
            hid->sendNextTrack();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            ESP_LOGI(TAG, "sending prevTrack");
            hid->sendPrevTrack();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            ESP_LOGI(TAG, "sending shufflePlay");
            hid->sendShufflePlay();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            ESP_LOGI(TAG, "sending playPause");
            hid->sendPlayPause();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            ESP_LOGI(TAG, "done");
            cycledCommands = 1;
        }
        */
           
        //vTaskDelay(10 / portTICK_PERIOD_MS);
    }

}




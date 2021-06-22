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
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
// common
#include "NunchukController.h"
#include "RGB_CC_LED.h"
#include "rgb_commands.h"
#include "TinyPICO.h"
// ble
#include "BLE_HID.h"
// tensorflow
#include "Joystick_CNN_Model.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
// profiles
#include "IDeviceProfile.h"
#include "RedDeviceProfile.h"
#include "GreenDeviceProfile.h"
#include "BlueDeviceProfile.h"


static const char *TAG = "tasks";


TaskHandle_t nunchukReadTaskHandle = NULL;
TaskHandle_t ledCmdTaskHandle = NULL;
TaskHandle_t trainingModeTaskHandle = NULL;
TaskHandle_t joystickInferenceTaskHandle = NULL;
TaskHandle_t joystickCmdTaskHandle = NULL;

// inference raw data
static float dataMatrix[INPUT_SHAPE_DIM][INPUT_SHAPE_DIM];


void nunchukReadTask(void *pvParameter)
{
    ESP_LOGI(TAG, "start %s", NUNCHUK_READ_TASK);
    NunchukController* nunchuk = NunchukController::getInstance(); 
    while(true) {
        nunchuk->fetchLatestReadings();
        vTaskDelay(1 / portTICK_PERIOD_MS);
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
                        (float)RGB_CC_LED_MAX * ((batVoltage-(float)BAT_VOLTAGE_MIN)/(float)BAT_VOLTAGE_RANGE)
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
            case RGB_CMD_RED_PROFILE:
                rgbcRedProfile(led);
                break;
            case RGB_CMD_GREEN_PROFILE:
                rgbcGreenProfile(led);
                break;
            case RGB_CMD_BLUE_PROFILE:
                rgbcBlueProfile(led);
                break;
            default:
                ESP_LOGW(TAG, "uknonwn LED cmd: %d", ledCmd);
                break;
            }
            vTaskDelay(1 / portTICK_PERIOD_MS);
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

    // tensorflow setup
    Joystick_CNN_Model* joystickModel = new Joystick_CNN_Model(INPUT_SHAPE_DIM, INPUT_SHAPE_DIM);
    prediction_result_t res;
    gesture_label_t label;

    uint8_t cancelled;
    uint8_t x, y;

    while (true)
    {
        vTaskDelay(10/portTICK_PERIOD_MS);

        // wait for ONLY Z-button press 
        if (nunchuk->getCButton() || !nunchuk->getZButton()) { continue; }
        xQueueSend(
            ledCmdQueue, 
            (void*)&RGB_CMD_GESTURE_ACTIVE,
            1/portTICK_PERIOD_MS
        );
        ESP_LOGI(TAG, "Joystick read started");
        vTaskSuspend(joystickCmdTaskHandle);

        // get input 
        cancelled = 0;
        while (nunchuk->getZButton())
        {
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
            dataMatrix[y-1][x-1] = 1;
            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
        ESP_LOGI(TAG, "Joystick read ended");
        while (nunchuk->getCButton() || nunchuk->getZButton()) { vTaskDelay(10/portTICK_PERIOD_MS); }
        vTaskResume(joystickCmdTaskHandle);
        
        if (cancelled) { continue; } 
        xQueueSend(
            ledCmdQueue, 
            (void*)&RGB_CMD_CLEAR_ALL,
            1/portTICK_PERIOD_MS
        );

        for (int i=0; i<INPUT_SHAPE_DIM; i++) {
            for (int j=0; j<INPUT_SHAPE_DIM; j++) {
                printf("%d", (int)dataMatrix[i][j]);
            }
            printf("\n");
        }
        // run inference
        joystickModel->makePrediction(
            (float*)dataMatrix, 
            INPUT_SHAPE_DIM*INPUT_SHAPE_DIM*sizeof(float),
            &res
        );
        label = res.label;

        // send command (if any) down queue
        if (res.prob >= 0.95f) {
            ESP_LOGI(TAG, "Got gesture %d with prob %f", label, res.prob);
            xQueueSend(
                joystickCmdQueue, 
                (void*)&label,
                1/portTICK_PERIOD_MS
            );
        } else {
            ESP_LOGW(TAG, "Gesture %d prob %f too low; skipping", label, res.prob);
        }

        // reset input vars
        memset(&dataMatrix, 0, sizeof(dataMatrix));
        /*
        for (int i=0; i<INPUT_SHAPE_DIM; i++) {
            for (int j=0; j<INPUT_SHAPE_DIM; j++) {
                dataMatrix[i][j] = 0;
            }
        }
        */
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void joystickCmdTask(void *pvParameter)
{
    ESP_LOGI(TAG, "start %s", JOYSTICK_CMD_TASK);

    NunchukController* nunchuk = NunchukController::getInstance();
    gesture_label_t gesture;

    // profiles
    RedDeviceProfile* redProfile = new RedDeviceProfile;
    GreenDeviceProfile* greenProfile = new GreenDeviceProfile;
    //BlueDeviceProfile* blueProfile = new BlueDeviceProfile;
    IDeviceProfile* profiles[] = {
        redProfile,
        greenProfile
        //blueProfile
    };
    uint8_t profileIdx = 0;

    while (true)
    {
        if (nunchuk->getCButton())
        {
            profileIdx = (profileIdx == IDeviceProfile::count()-1) ? 0 : profileIdx+1;
            profiles[profileIdx]->profileSelectedCallback();
            while (nunchuk->getCButton()) { vTaskDelay(10/portTICK_PERIOD_MS); }
            ESP_LOGI(TAG, "Selected profile %d", profileIdx);
        }
        
        while (xQueueReceive(joystickCmdQueue, (void*)&gesture, 1/portTICK_PERIOD_MS) == pdTRUE)
        {
            ESP_LOGI(TAG, "Gesture: %s", getNameOfPrediction(gesture));
            switch (gesture)
            {
            case LABEL_RGESTURE_CIRCLE:
                profiles[profileIdx]->gestureCircleCallback();
                break;
            case LABEL_RGESTURE_UP_FLICK:
                profiles[profileIdx]->gestureUpCallback();
                break;
            case LABEL_RGESTURE_CENTER_CLICK:
                profiles[profileIdx]->gestureClickCallback();
                break;
            case LABEL_RGESTURE_LEFT_FLICK:
                profiles[profileIdx]->gestureLeftCallback();
                break;
            case LABEL_RGESTURE_DOWN_FLICK:
                profiles[profileIdx]->gestureDownCallback();
                break;
            case LABEL_RGESTURE_RIGHT_FLICK:
                profiles[profileIdx]->gestureRightCallback();
                break;
            default:
                ESP_LOGW(TAG, "Unknown gesture: %d", gesture);
                break;    
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}




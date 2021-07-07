/*
 * main.cpp
 * 
 *  Onechuk main program file.
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#include "settings.h"
#include "tasks.h"
#include "queues.h"

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
// i2c
#include "hal/gpio_types.h"
#include "hal/i2c_types.h"
#include "driver/i2c.h"
// ble
#include "ble_utils.h"
// touch/sleep
#include "driver/touch_pad.h"
#include "esp_sleep.h"
// main components
#include "NunchukController.h"
#include "RGB_CC_LED.h"
#include "TinyPICO.h"
#include "rgb_commands.h"
#include "BLE_HID.h"

static const char *TAG = "main";

extern "C" void app_main(void);


static void i2cInit()
{
    i2c_config_t i2cConfig;
	i2cConfig.mode = I2C_MODE_MASTER;
	i2cConfig.scl_io_num = I2C_SCL_PIN;
	i2cConfig.sda_io_num = I2C_SDA_PIN;
	i2cConfig.scl_pullup_en = GPIO_PULLUP_ENABLE;
	i2cConfig.sda_pullup_en = GPIO_PULLUP_ENABLE;
	i2cConfig.clk_flags = 0;
	i2cConfig.master.clk_speed = I2C_FREQ_HZ;
	ESP_ERROR_CHECK(i2c_param_config(I2C_PORT_NUM, &i2cConfig));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT_NUM, I2C_MODE_MASTER, 0, 0, 0));
}


static void touchInit()
{
    ESP_ERROR_CHECK(touch_pad_init());
    ESP_ERROR_CHECK(touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER));

    // from ESP IDF deep sleep example
    // Set reference voltage for charging/discharging
    // In this case, the high reference valtage will be 2.4V - 1V = 1.4V
    // The low reference voltage will be 0.5
    // The larger the range, the larger the pulse count value.
    ESP_ERROR_CHECK(touch_pad_set_voltage(TOUCH_HVOLT_2V4, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V));
    ESP_ERROR_CHECK(touch_pad_config(DEEP_SLEEP_TOUCH_PAD_NUM, 0));

    // calibrate touch pad - don't touch during init
    int avg = 0;
    const size_t calibration_count = 128;
    for (int i = 0; i < calibration_count; ++i) {
        uint16_t val;
        touch_pad_read(DEEP_SLEEP_TOUCH_PAD_NUM, &val);
        avg += val;
    }
    avg /= calibration_count;
    const int min_reading = 300;
    if (avg < min_reading) {
        ESP_LOGW(TAG, "Touch pad #%d average reading too low for deep sleep setup: %d",
            DEEP_SLEEP_TOUCH_PAD_NUM, avg
        );
        ESP_ERROR_CHECK(touch_pad_config(DEEP_SLEEP_TOUCH_PAD_NUM, 0));
    } else {
        int threshold = avg - 100;
        ESP_LOGI(TAG, "Touch pad #%d threshold set to %d", DEEP_SLEEP_TOUCH_PAD_NUM, threshold);
        ESP_ERROR_CHECK(touch_pad_config(DEEP_SLEEP_TOUCH_PAD_NUM, threshold));
    }
    ESP_ERROR_CHECK(esp_sleep_enable_touchpad_wakeup());
    ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON));
}


void app_main(void)
{
    /*
     *  SYSTEM INIT
     */
    // queues - init done at compile time, check status now
    queuesCheck();

    // peripherals init - configure in settings.h
	i2cInit();
    touchInit();

    // main component classes init
    TinyPICO* tinypico = TinyPICO::getInstance();
    RGB_CC_LED mainLED = RGB_CC_LED(R_PIN, G_PIN, B_PIN);
    NunchukController* nunchuk = NunchukController::getInstance(I2C_PORT_NUM);

    // get init params from nunchuk buttons
    nunchuk->fetchLatestReadings();
    uint8_t cButtonPressed = nunchuk->getCButton();
    uint8_t zButtonPressed = nunchuk->getZButton();
    enum task_init_t {
        INIT_MODE_NULL,
        INIT_MODE_TRAINING,
        INIT_MODE_DEVICE
    } initType;
    if (cButtonPressed && !zButtonPressed) {            // C-button: training mode
        ESP_LOGI(TAG, "TRAINING mode selected");
        initType = INIT_MODE_TRAINING;
    } else if (!cButtonPressed && !zButtonPressed) {    // default: device mode
        ESP_LOGI(TAG, "DEVICE mode selected");
        initType = INIT_MODE_DEVICE;
    } else {
        ESP_LOGI(TAG, "no mode selected");
        initType = INIT_MODE_NULL;
    }

    // system init cplt
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_INIT, 1/portTICK_PERIOD_MS);


    /*
     *  TASKS INIT
     */
    // basic I/O tasks
    xTaskCreate(&touchDeepSleepTask, TOUCH_DEEP_SLEEP_TASK, 2048,
        NULL, tskIDLE_PRIORITY, &touchDeepSleepTaskHandle);
    xTaskCreate(&ledCmdTask, LED_CMD_TASK, 2048,
        (void*)&mainLED, tskIDLE_PRIORITY, &ledCmdTaskHandle);  // min priority
    xTaskCreate(&nunchukReadTask, NUNCHUK_READ_TASK, 4096,
        NULL, tskIDLE_PRIORITY, &nunchukReadTaskHandle);        // min priority
    
    switch (initType) {

    // training mode sends joystick values over serial if Z button pressed
    case INIT_MODE_TRAINING:
        xQueueSend(ledCmdQueue, (void*)&RGB_CMD_TRAINING_MODE, 1/portTICK_PERIOD_MS);
        xTaskCreate(&trainingModeTask, TRAINING_MODE_TASK, 4096,
            NULL, 12, &trainingModeTaskHandle);                 // med priority
        break;

    // device mode (default) performs actions from joystick gesture commands
    case INIT_MODE_DEVICE:
        bleInit();
        xQueueSend(ledCmdQueue, (void*)&RGB_CMD_DEVICE_MODE, 1/portTICK_PERIOD_MS);
        xTaskCreate(&joystickCmdTask, JOYSTICK_CMD_TASK, 4096,
            NULL, 12, &joystickCmdTaskHandle);	                // med priority
        xTaskCreate(&joystickInferenceTask, JOYSTICK_INFERENCE_TASK, 65536,
            NULL, 24, &joystickInferenceTaskHandle);	        // max priority
        break;

    case INIT_MODE_NULL:
        break;
    }


    // Run Forrest, run
    while(true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
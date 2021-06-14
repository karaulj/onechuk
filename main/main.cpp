/*
 * main.cpp
 * 
 *  Funchuk main program file.
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
// i2c
#include "hal/gpio_types.h"
#include "hal/i2c_types.h"
#include "driver/i2c.h"
// ble
#include "ble_init.h"

#include "settings.h"
#include "tasks.h"
#include "queues.h"
#include "NunchukController.h"
#include "RGB_CC_LED.h"
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
	i2cConfig.master.clk_speed = I2C_FREQ_HZ;
	ESP_ERROR_CHECK(i2c_param_config(I2C_PORT_NUM, &i2cConfig));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT_NUM, I2C_MODE_MASTER, 0, 0, 0));
}


void app_main(void)
{
    /*
     *  SYSTEM INIT
     */
    // queues - init already done, check status
    queuesCheck();

    // peripherals init - configure in settings.h
	i2cInit();

    // device class init
    RGB_CC_LED mainLED = RGB_CC_LED(R_PIN, G_PIN, B_PIN);
    NunchukController nunchuk = NunchukController(I2C_PORT_NUM);

    // get init params from nunchuk buttons
    nunchuk.fetchLatestReadings();
    uint8_t cButtonPressed = nunchuk.getCButton();
    uint8_t zButtonPressed = nunchuk.getZButton();
    enum task_init_t {
        INIT_MODE_NONE = 0,
        INIT_MODE_TRAINING,
        INIT_MODE_DEVICE
    } initType;

    if (cButtonPressed && !zButtonPressed) {            // C-button for training mode
        ESP_LOGI(TAG, "TRAINING mode selected");
        initType = INIT_MODE_TRAINING;
    } else if (!cButtonPressed && !zButtonPressed) {    // nothing for device mode
        ESP_LOGI(TAG, "DEVICE mode selected");
        initType = INIT_MODE_DEVICE;
    } else {
        ESP_LOGI(TAG, "NONE mode selected");
        initType = INIT_MODE_NONE;
    }

    // system init cplt
    xQueueSend(ledCmdQueue, (void*)&RGB_CMD_SYSTEM_INIT, 1/portTICK_PERIOD_MS);

    /*
     *  TASKS INIT
     */
    // basic I/O tasks
    xTaskCreate(&nunchukReadTask, NUNCHUK_READ_TASK, 2048,
        (void*)&nunchuk, 24, NULL);	                // max priority
    xTaskCreate(&ledCmdTask, LED_CMD_TASK, 2048,
        (void*)&mainLED, tskIDLE_PRIORITY, NULL);   // min priority
    // dynamically choose which tasks to run from init params
    switch (initType) {

    case INIT_MODE_TRAINING:
        xQueueSend(ledCmdQueue, (void*)&RGB_CMD_TRAINING_MODE, 1/portTICK_PERIOD_MS);
        xTaskCreate(&trainingModeTask, TRAINING_MODE_TASK, 4096,
            NULL, 12, NULL);                        // med priority
        break;

    case INIT_MODE_DEVICE:
        // BLE HID init
        bleInit();
        //BLE_HID* bleHid = BLE_HID::getInstance();
        xTaskCreate(&joystickCmdTask, JOYSTICK_CMD_TASK, 4096,
        NULL, 12, NULL);	                    // med priority
        break;

    case INIT_MODE_NONE:
        break;
    }

    while(true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
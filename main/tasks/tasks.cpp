/*
 * tasks.cpp
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#include "tasks.h"
#include "queues.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "NunchukController.h"
#include "rgb_cc_led.h"

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
        //vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}


void printTask(void *pvParameter)
{
    ESP_LOGI(TAG, "start %s", PRINT_TASK);
    
    nunchuk_data_t nunchukData;
    //uint8_t joystickX, joystickY;
    //uint8_t cButton, zButton;

    //uint32_t nextSecTicks = xTaskGetTickCount() + (1000/portTICK_PERIOD_MS);
    //uint32_t cnt = 0;

    while (true)
    {
        while (xQueueReceive(nunchukDataQueue, (void*)&nunchukData, portMAX_DELAY) == pdTRUE)
        {
           //joystickX = nunchukData.joystickX;
           //joystickY = nunchukData.joystickY;
           printf("%3dX%3dY\n", nunchukData.joystickX, nunchukData.joystickY);
           //cnt++;

           /*
           if (xTaskGetTickCount() >= nextSecTicks)
            {
                printf("\n%d\n\n", cnt);
                cnt = 0;
                nextSecTicks = xTaskGetTickCount() + (1000/portTICK_PERIOD_MS);
            }
            */
        }

        //nunchuk->fetchLatestReadings();
        //vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}


void fadeLEDTask(void *pvParameter)
{
    ESP_LOGI(TAG, "start %s", FADE_LED_TASK);
    RGB_CC_LED* mainLED = (RGB_CC_LED*) pvParameter; 

    uint8_t high = 0x0A;
    while (true) {
        for (int i=1; i<4; i++)
        {
            //printf("fade factor: %d\n", i);
            int fadeFactor = (i*150)-100;

            //printf("R getting brighter\n");
            mainLED->setColor(high, 0, 0, fadeFactor, fadeFactor, fadeFactor);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            //printf("G getting brighter\n");
            mainLED->setColor(0, high, 0, fadeFactor, fadeFactor, fadeFactor);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            //printf("B getting brighter\n");
            mainLED->setColor(0, 0, high, fadeFactor, fadeFactor, fadeFactor);
            vTaskDelay(1000 / portTICK_PERIOD_MS);

            //printf("All getting brighter\n");
            mainLED->setColor(high, high, high, fadeFactor, fadeFactor, fadeFactor);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            //printf("All getting dimmer\n");
            mainLED->setColor(0, 0, 0, fadeFactor, fadeFactor, fadeFactor);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}
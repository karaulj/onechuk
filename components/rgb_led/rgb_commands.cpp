/*
 * RGBCommands.cpp
 *
 *  Created on: Jun 12, 2021
 *      Author: Jacob
 */

#include "rgb_commands.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "RGB_CC_LED.h"

//namespace RGBc {

void rgbcSystemInit(RGB_CC_LED* led)
{
    led->setColor(RGB_CC_LED_MAX, RGB_CC_LED_MAX, RGB_CC_LED_MAX, 250, 250, 250);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    led->setColor(0, 0, 0, 250, 250, 250);
    vTaskDelay(300 / portTICK_PERIOD_MS);
}

void rgbcRestart(RGB_CC_LED* led)
{
    led->setColor(RGB_CC_LED_MAX, 0, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    led->setColor(0, RGB_CC_LED_MAX, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    led->setColor(0, 0, RGB_CC_LED_MAX);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    rgbcClearAll(led);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void rgbcBleConnected(RGB_CC_LED* led)
{
    led->setColor(0, 0, RGB_CC_LED_MAX/2);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    led->setColor(0, RGB_CC_LED_MAX/2, 0, 300, 300, 300);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    for (int i=0; i<3; i++)
    {
        led->setColor(0, RGB_CC_LED_MAX/2, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        rgbcClearAll(led);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void rgbcBleDisconnected(RGB_CC_LED* led)
{
    led->setColor(0, 0, RGB_CC_LED_MAX/2);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    led->setColor(RGB_CC_LED_MAX/2, 0, 0, 300, 300, 300);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    for (int i=0; i<3; i++)
    {
        led->setColor(RGB_CC_LED_MAX/2, 0, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        rgbcClearAll(led);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void rgbcGestureMode(RGB_CC_LED* led)
{
    led->setColor(0, 0, RGB_CC_LED_MAX/4);
}

void rgbcGestureFound(RGB_CC_LED* led)
{
    for (int i=0; i<2; i++)
    {
        led->setColor(0, RGB_CC_LED_MAX/2, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        rgbcClearAll(led);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void rgbcGestureNotFound(RGB_CC_LED* led)
{
    for (int i=0; i<2; i++)
    {
        led->setColor(RGB_CC_LED_MAX/2, 0, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        rgbcClearAll(led);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void rgbcGestureNotImpl(RGB_CC_LED* led)
{
    led->setColor(RGB_CC_LED_MAX/4, 0, RGB_CC_LED_MAX/2);   // dark purple
    vTaskDelay(600 / portTICK_PERIOD_MS);
    rgbcClearAll(led);
    vTaskDelay(150 / portTICK_PERIOD_MS);
}

void rgbcCancelGesture(RGB_CC_LED* led)
{
    led->setColor(RGB_CC_LED_MAX/2, 0, 0);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    rgbcClearAll(led);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void rgbcTrainingMode(RGB_CC_LED* led)
{
    for (int i=0; i<3; i++)
    {
        led->setColor(RGB_CC_LED_MAX, RGB_CC_LED_MAX/2, 0); // dark orange
        vTaskDelay(100 / portTICK_PERIOD_MS);
        rgbcClearAll(led);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void rgbcClearAll(RGB_CC_LED* led)
{
    led->setColor(0, 0, 0);
}

//}
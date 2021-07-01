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
#include "NunchukController.h"

//namespace RGBc {

void rgbcSystemInit(RGB_CC_LED* led)
{
    led->setColor(RGB_CC_LED_MAX, RGB_CC_LED_MAX, RGB_CC_LED_MAX, 100, 100, 100);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    led->setColor(0, 0, 0, 200, 200, 200);
    vTaskDelay(250 / portTICK_PERIOD_MS);
}
void rgbcRestart(RGB_CC_LED* led)
{
    for (int i=0; i<3; i++)
    {
        led->setColor(RGB_CC_LED_MAX, 0, 0);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        led->setColor(0, RGB_CC_LED_MAX, 0);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        led->setColor(0, 0, RGB_CC_LED_MAX);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        rgbcClearAll(led);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
void rgbcDeepSleepStart(RGB_CC_LED* led)
{
    for (int i=2; i>=0; i--) {      // brighter
        led->setColor(RGB_CC_LED_MAX/(1<<i), RGB_CC_LED_MAX/(1<<i), RGB_CC_LED_MAX/(1<<i),
            300, 300, 300);
        vTaskDelay(300 / portTICK_PERIOD_MS);
        led->setColor(0, 0, 0,
            200, 200, 200);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
void rgbcClearAll(RGB_CC_LED* led)
{
    led->setColor(0, 0, 0);
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
void rgbcBlePairingModeActive(RGB_CC_LED* led)
{
    for (int i=0; i<3; i++)
    {
        led->setColor(RGB_CC_LED_MAX/2, 0, 0, 500, 500, 500);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        led->setColor(0, 0, RGB_CC_LED_MAX/2, 500, 500, 500);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    rgbcClearAll(led);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void rgbcDeviceMode(RGB_CC_LED* led)
{
    for (int i=0; i<3; i++)
    {
        led->setColor(0, RGB_CC_LED_MAX/2, RGB_CC_LED_MAX/2); // cyan
        vTaskDelay(100 / portTICK_PERIOD_MS);
        rgbcClearAll(led);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
void rgbcGestureActive(RGB_CC_LED* led)
{
    led->setColor(RGB_CC_LED_MAX/8, RGB_CC_LED_MAX/8, RGB_CC_LED_MAX/8);
}
void rgbcGestureFound(RGB_CC_LED* led)
{
    led->setColor(0, RGB_CC_LED_MAX/4, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    rgbcClearAll(led);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
void rgbcGestureNotFound(RGB_CC_LED* led)
{
    led->setColor(RGB_CC_LED_MAX/4, 0, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    rgbcClearAll(led);
    vTaskDelay(100 / portTICK_PERIOD_MS);
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
    led->setColor(RGB_CC_LED_MAX/4, 0, 0);
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

void rgbcBatteryRange(RGB_CC_LED* led)
{
    led->setColor(RGB_CC_LED_MAX, 0, 0);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    led->setColor(0, RGB_CC_LED_MAX, 0, 1000, 1000, 1000);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    rgbcClearAll(led);
}
void rgbcBatteryLevel(RGB_CC_LED* led, color_level_t level)
{
    led->setColor(RGB_CC_LED_MAX, 0, 0);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    led->setColor(
        (level > RGB_CC_LED_MAX/2) ? 2*(RGB_CC_LED_MAX-level) : RGB_CC_LED_MAX, 
        (level > RGB_CC_LED_MAX/2) ? RGB_CC_LED_MAX : 2*level, 
        0, 
        1000, 1000, 1000
    );
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    rgbcClearAll(led);
}
void rgbcBatteryCharging(RGB_CC_LED* led)
{
    for (int i=0; i<3; i++)
    {
        led->setColor(0, RGB_CC_LED_MAX, 0);
        vTaskDelay(250 / portTICK_PERIOD_MS);
        rgbcClearAll(led);
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}
void rgbcBatteryNotCharging(RGB_CC_LED* led)
{
    for (int i=0; i<3; i++)
    {
        led->setColor(RGB_CC_LED_MAX, 0, 0);
        vTaskDelay(250 / portTICK_PERIOD_MS);
        rgbcClearAll(led);
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

static void rgbcAccelFadeWithDelay(RGB_CC_LED* led, uint32_t rDelay, uint32_t gDelay, uint32_t bDelay)
{
    NunchukController* nunchuk = NunchukController::getInstance();
    color_level_t rVal = (color_level_t)
        ( (RGB_CC_LED_MAX*nunchuk->getAccelX()) / NUNCHUK_CONTROLLER_ACCEL_MAX );
    color_level_t gVal = (color_level_t)
        ( (RGB_CC_LED_MAX*nunchuk->getAccelY()) / NUNCHUK_CONTROLLER_ACCEL_MAX );
    color_level_t bVal = (color_level_t)
        ( (RGB_CC_LED_MAX*nunchuk->getAccelZ()) / NUNCHUK_CONTROLLER_ACCEL_MAX );
    led->setColor(rVal, gVal, bVal, rDelay, gDelay, bDelay);
}
static void rgbcAccelFadeWithCommonDelay(RGB_CC_LED* led, uint32_t commonDelay)
{
    rgbcAccelFadeWithDelay(led, commonDelay, commonDelay, commonDelay);
}
void rgbcAccelFade(RGB_CC_LED* led)         { rgbcAccelFadeWithCommonDelay(led, RGB_CC_LED_FADE_TIME_MS); }
void rgbcAccelFade5ms(RGB_CC_LED* led)      { rgbcAccelFadeWithCommonDelay(led, 5); }
void rgbcAccelFade10ms(RGB_CC_LED* led)     { rgbcAccelFadeWithCommonDelay(led, 10); }
void rgbcAccelFade20ms(RGB_CC_LED* led)     { rgbcAccelFadeWithCommonDelay(led, 20); }
void rgbcAccelFade50ms(RGB_CC_LED* led)     { rgbcAccelFadeWithCommonDelay(led, 50); }
void rgbcAccelFade100ms(RGB_CC_LED* led)    { rgbcAccelFadeWithCommonDelay(led, 100); }

// mask: 0x01 = RG, 0x02 = RB, 0x04 = GB
static void rgbcJoystick_Fade(RGB_CC_LED* led, uint8_t rgbMask)
{
    NunchukController* nunchuk = NunchukController::getInstance();
    color_level_t xVal = (color_level_t)
        ( (RGB_CC_LED_MAX*nunchuk->getJoystickX()) / NUNCHUK_CONTROLLER_JOYSTICK_MAX );
    color_level_t yVal = (color_level_t)
        ( (RGB_CC_LED_MAX*nunchuk->getJoystickY()) / NUNCHUK_CONTROLLER_JOYSTICK_MAX );
    
    led->setColor(
        (rgbMask & 0x03) ? xVal : 0,
        (rgbMask & 0x05) ? ((rgbMask == 0x04) ? xVal  : yVal) : 0, 
        (rgbMask & 0x06) ? yVal : 0
    );
}
void rgbcJoystick_RG_Fade(RGB_CC_LED* led) { rgbcJoystick_Fade(led, 0x01); }
void rgbcJoystick_RB_Fade(RGB_CC_LED* led) { rgbcJoystick_Fade(led, 0x02); }
void rgbcJoystick_GB_Fade(RGB_CC_LED* led) { rgbcJoystick_Fade(led, 0x04); }

void rgbcRedProfile(RGB_CC_LED* led)
{
    for (int i=0; i<2; i++)
    {
        led->setColor(RGB_CC_LED_MAX/4, 0, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        rgbcClearAll(led);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
void rgbcGreenProfile(RGB_CC_LED* led)
{
    for (int i=0; i<2; i++)
    {
        led->setColor(0, RGB_CC_LED_MAX/4, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        rgbcClearAll(led);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
void rgbcBlueProfile(RGB_CC_LED* led)
{
    for (int i=0; i<2; i++)
    {
        led->setColor(0, 0, RGB_CC_LED_MAX/4);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        rgbcClearAll(led);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
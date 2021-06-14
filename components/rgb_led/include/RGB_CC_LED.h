/*
 * RGB_CC_LED.h
 * 
 * A class for driving a Common-Cathode RGB LED.
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_RGB_CC_LED_INCLUDE_RGB_CC_LED_H_
#define COMPONENTS_RGB_CC_LED_INCLUDE_RGB_CC_LED_H_

#include "hal/gpio_types.h"
#include "driver/ledc.h"


// ledc_timer_config_t params
#define RGB_CC_LED_DC_RES           LEDC_TIMER_12_BIT
#define RGB_CC_LED_FREQ_HZ          10000U
#define RGB_CC_LED_SPEED_MODE       LEDC_HIGH_SPEED_MODE
#define RGB_CC_LED_TIMER_NUM        LEDC_TIMER_0
#define RGB_CC_LED_CLK_CFG          LEDC_AUTO_CLK

// ledc_channel_config_t params
#define RGB_CC_LED_R_CHANNEL        LEDC_CHANNEL_0
#define RGB_CC_LED_G_CHANNEL        LEDC_CHANNEL_1
#define RGB_CC_LED_B_CHANNEL        LEDC_CHANNEL_2
#define RGB_CC_LED_INTR_TYPE        LEDC_INTR_DISABLE
#define RGB_CC_LED_HPOINT           0

// drive params
#define RGB_CC_LED_FADE_TIME_MS     40
#define RGB_CC_LED_FADE_MODE        LEDC_FADE_NO_WAIT

// general
#define RGB_CC_LED_MAX              4095U        // 2^12 - 1

// duty cycle param type of a single LED channel, supplied by user
// must be >= RGB_CC_LED_DC_RES
typedef uint16_t color_level_t;


class RGB_CC_LED
{
public:
    const gpio_num_t r, g, b;
    RGB_CC_LED(uint8_t rPin, uint8_t gPin, uint8_t bPin);
    ~RGB_CC_LED();

    void setColor(
        color_level_t rVal,
        color_level_t gVal,
        color_level_t bVal,
        uint32_t rFadeTimeMs = RGB_CC_LED_FADE_TIME_MS,
        uint32_t gFadeTimeMs = RGB_CC_LED_FADE_TIME_MS,
        uint32_t bFadeTimeMs = RGB_CC_LED_FADE_TIME_MS
    );
    void turnOff();
    void turnOn();
protected:
    void fadeAll(
        color_level_t rVal,
        color_level_t gVal,
        color_level_t bVal,
        uint32_t rFadeTimeMs = RGB_CC_LED_FADE_TIME_MS,
        uint32_t gFadeTimeMs = RGB_CC_LED_FADE_TIME_MS,
        uint32_t bFadeTimeMs = RGB_CC_LED_FADE_TIME_MS
    );
private:
    uint8_t ledInit = 0;
    uint8_t ledOn = 0;
    color_level_t colorLevels[3] = {0, 0, 0};
};


#endif /* COMPONENTS_RGB_CC_LED_INCLUDE_RGB_CC_LED_H_ */
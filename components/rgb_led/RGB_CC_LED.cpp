/*
 * RGB_CC_LED.cpp
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#include "RGB_CC_LED.h"

// gpio setup
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "driver/ledc.h"
// logging
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "RGB_CC_LED";


RGB_CC_LED::RGB_CC_LED(uint8_t rPin, uint8_t gPin, uint8_t bPin,
	ledc_channel_t rChannel, ledc_channel_t gChannel, ledc_channel_t bChannel,
	ledc_mode_t speedModeAll
    ) : r((gpio_num_t)rPin), g((gpio_num_t)gPin), b((gpio_num_t)bPin),
		rChan(rChannel), gChan(gChannel), bChan(bChannel), speedMode(speedModeAll)
{
	esp_err_t ret;

	// ledc timer setup
	ledc_timer_config_t ledc_timer = {
		.speed_mode				= RGB_CC_LED_SPEED_MODE,
		.duty_resolution		= RGB_CC_LED_DC_RES,
		.timer_num				= RGB_CC_LED_TIMER_NUM,
		.freq_hz				= RGB_CC_LED_FREQ_HZ,
		.clk_cfg				= RGB_CC_LED_CLK_CFG,
	};
	ret = ledc_timer_config(&ledc_timer);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Could not initialize ledc timer: %d", (int)ret);
	}

	// R ledc channel setup
	ledc_channel_config_t ledc_channel = {
		.gpio_num				= r,
		.speed_mode				= speedMode,
		.channel				= rChan,
		.intr_type				= RGB_CC_LED_INTR_TYPE,
		.timer_sel				= RGB_CC_LED_TIMER_NUM,
		.duty					= 0,
		.hpoint					= RGB_CC_LED_HPOINT,
	};
	ret = ledc_channel_config(&ledc_channel); 
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Could not initialize R ledc channel: %d", (int)ret);
	}
	// G 
	ledc_channel.channel	= gChan;
	ledc_channel.gpio_num	= g;
	ret = ledc_channel_config(&ledc_channel); 
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Could not initialize G ledc channel: %d", (int)ret);
	}
	// B 
	ledc_channel.channel	= bChan;
	ledc_channel.gpio_num	= b;
	ret = ledc_channel_config(&ledc_channel); 
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Could not initialize B ledc channel: %d", (int)ret);
	}

	// ledc fade setup
	ret = ledc_fade_func_install(0);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Could not install fade func: %d", (int)ret);
	}

	// init complete
	ledInit = 1;
	ESP_LOGI(TAG, "initialized with pins r=%d,g=%d,b=%d", (int)r, (int)g, (int)b);
	turnOff();
}


void RGB_CC_LED::deInit()
{
	if (ledInit)
	{
		ledc_stop(speedMode, rChan, 0);
		ledc_stop(speedMode, gChan, 0);
		ledc_stop(speedMode, bChan, 0);

		ledc_fade_func_uninstall();

		ledInit = 0;
	}
}


RGB_CC_LED::~RGB_CC_LED()
{
	if (ledInit)
	{
		deInit();
	}
}


void RGB_CC_LED::fadeAll(color_level_t rVal, color_level_t gVal, color_level_t bVal,
        uint32_t rFadeTimeMs, uint32_t gFadeTimeMs, uint32_t bFadeTimeMs)
{
	ledc_set_fade_time_and_start(
		RGB_CC_LED_SPEED_MODE, 
		RGB_CC_LED_R_CHANNEL,
		rVal,
		rFadeTimeMs,
		RGB_CC_LED_FADE_MODE
	);
	ledc_set_fade_time_and_start(
		RGB_CC_LED_SPEED_MODE, 
		RGB_CC_LED_G_CHANNEL,
		gVal,
		gFadeTimeMs,
		RGB_CC_LED_FADE_MODE
	);
	ledc_set_fade_time_and_start(
		RGB_CC_LED_SPEED_MODE, 
		RGB_CC_LED_B_CHANNEL,
		bVal,
		bFadeTimeMs,
		RGB_CC_LED_FADE_MODE
	);
	colorLevels[0] = rVal;
	colorLevels[1] = gVal;
	colorLevels[2] = bVal;
}


void RGB_CC_LED::setColor(color_level_t rVal, color_level_t gVal, color_level_t bVal,
        uint32_t rFadeTimeMs, uint32_t gFadeTimeMs, uint32_t bFadeTimeMs)
{
	if (ledInit)
	{
		return fadeAll(rVal, gVal, bVal, rFadeTimeMs, gFadeTimeMs, bFadeTimeMs);
	}
}


void RGB_CC_LED::turnOff()
{
	if (ledInit)
	{
		ledOn = 0;
		setColor(0,0,0, 0,0,0);
	}
}


void RGB_CC_LED::turnOn()
{
	if (ledInit)
	{
		ledOn = 1;
		setColor(colorLevels[0], colorLevels[1], colorLevels[2]);
	}
}

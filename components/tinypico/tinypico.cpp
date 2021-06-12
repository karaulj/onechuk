/*
 * tinypico.cpp
 *
 *  Created on: Mar 17, 2021
 *      Author: Jacob
 */

#include "tinypico.h"

#include <stdio.h>
// for delay/getting delay in ms
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// gpio setup
#include "driver/gpio.h"
#include "hal/gpio_types.h"
// logging
#include "esp_log.h"

static const char *TAG = "tinypico";


TinyPICO* TinyPICO::tp = nullptr;


TinyPICO::TinyPICO()
{
	// LED power pin
	gpio_config_t ledPwrConfig = {
		.pin_bit_mask = 1ULL<<DOTSTAR_PWR,
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = GPIO_PULLUP_DISABLE,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.intr_type = GPIO_INTR_DISABLE
	};
	if(gpio_config(&ledPwrConfig) != ESP_OK)
	{
		ESP_LOGE(TAG, "Could not initialize TinyPICO LED");
	}
	else
	{
		ESP_LOGI(TAG, "TinyPICO LED initialized");
		ledInit = 1;
	}
	turnOffLED();
}


TinyPICO* TinyPICO::getInstance()
{
	if (tp == nullptr)
	{
		tp = new TinyPICO();
	}
	return tp;
}


void TinyPICO::swspiOut(uint8_t n)
{
	ESP_LOGV(TAG, "Sending frame %d", n);

	for(uint8_t i=8; i--; n <<= 1)
	{
		if (n & 0x80)
		{
			gpio_set_level((gpio_num_t)DOTSTAR_DATA, 1);
		}
		else
		{
			gpio_set_level((gpio_num_t)DOTSTAR_DATA, 0);
		}
		gpio_set_level((gpio_num_t)DOTSTAR_CLK, 1);
		gpio_set_level((gpio_num_t)DOTSTAR_CLK, 0);
	}
	vTaskDelay(1 / portTICK_PERIOD_MS);
}


void TinyPICO::showLED()
{
	if (ledOn && ledInit)
	{
		// start-frame marker
		for( int i=0; i<4; i++)
		{
			swspiOut(0x00);
		}

		// pixel start
		swspiOut(0xFF);

		// send pixel data
		for( int i=2; i>=0; i--)
		{
			swspiOut(ledColors[i]); // order: B, G, R
		}

		// end-frame marker
		swspiOut(0xFF);
	}
}


void TinyPICO::setLEDColor(uint8_t r, uint8_t g, uint8_t b)
{
	ledColors[0] = r;
	ledColors[1] = g;
	ledColors[2] = b;

	ESP_LOGD(TAG, "Setting r,g,b to %d,%d,%d", r, g, b);

	if (!ledOn)
	{
		turnOnLED();
	}
	else
	{
		showLED();
	}
}


void TinyPICO::turnOffLED()
{
	if (ledInit)
	{
		ESP_LOGI(TAG, "Turning off LED");
		ledOn = 0;

		gpio_set_level((gpio_num_t)DOTSTAR_PWR, 1);

		// change pins to input w/ pulldown
		gpio_config(&swspiOffConfig);
	}
}


void TinyPICO::turnOnLED()
{
	if (ledInit)
	{
		ESP_LOGI(TAG, "Turning on LED");
		ledOn = 1;

		// change pins to output w/o pulldown
		gpio_config(&swspiOnConfig);

		gpio_set_level((gpio_num_t)DOTSTAR_PWR, 0);

		// reload stored colors
		showLED();
	}
}


/*
 * TinyPICO.cpp
 *
 *  Created on: Mar 17, 2021
 *      Author: Jacob
 */

#include "TinyPICO.h"

#include <stdio.h>
// for delay/getting delay in ms
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// gpio
#include "driver/gpio.h"
#include "hal/gpio_types.h"
// adc
#include "driver/adc.h"
#include "esp_adc_cal.h"
// logging
#include "esp_log.h"

static const char *TAG = "tinypico";


static esp_adc_cal_characteristics_t adcChars;

TinyPICO* TinyPICO::tp = nullptr;


TinyPICO::TinyPICO()
{
	// LED power pin
	gpio_config_t pinConfig = {
		.pin_bit_mask = 1ULL<<DOTSTAR_PWR,
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = GPIO_PULLUP_DISABLE,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.intr_type = GPIO_INTR_DISABLE
	};
	if(gpio_config(&pinConfig) != ESP_OK) {
		ESP_LOGE(TAG, "Could not initialize TinyPICO LED");
	} else {
		ESP_LOGI(TAG, "TinyPICO LED initialized");
		ledInit = 1;
	}
	turnOffLED();
	// Bat charge pin
	pinConfig.mode = GPIO_MODE_INPUT;
	pinConfig.pin_bit_mask = (uint64_t)1UL<<BAT_CHARGE;
	if(gpio_config(&pinConfig) != ESP_OK) {
		ESP_LOGE(TAG, "Could not initialize TinyPICO BAT_CHARGE pin");
	} else {
		ESP_LOGI(TAG, "TinyPICO BAT_CHARGE pin initialized");
		batChargeInit = 1;
	}
	// Bat voltage pin
	batVoltageInit = 1;
	if (adc1_config_width((adc_bits_width_t)BAT_VOLTAGE_ADC_WIDTH) != ESP_OK) {
		ESP_LOGE(TAG, "Could not set TinyPICO BAT_VOLTAGE pin width");
		batVoltageInit = 0;
	}
	if (adc1_config_channel_atten((adc1_channel_t)BAT_VOLTAGE_ADC_CHANNEL,
				(adc_atten_t)BAT_VOLTAGE_ADC_ATTEN ) != ESP_OK) {
		ESP_LOGE(TAG, "Could not set TinyPICO BAT_VOLTAGE channel attenuation");
		batVoltageInit = 0;
	}
	if (batVoltageInit) {
		ESP_LOGI(TAG, "TinyPICO BAT_VOLTAGE pin initialized");
	}
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


uint8_t TinyPICO::isBatteryCharging()
{
	uint8_t ret = 0;
	if (batChargeInit) {
		for (int i=0; i<10; i++) {
			ret = ret || gpio_get_level((gpio_num_t)BAT_CHARGE);
			//vTaskDelay(1 / portTICK_PERIOD_MS);
		}
	} else {
		ESP_LOGE(TAG, "battery charging pin not set up; sending %d", ret);
	}
	return !ret;
}


float TinyPICO::getBatteryVoltage()
{
	float ret = -1.0;
	if (batVoltageInit) {
		uint32_t rawReading = adc1_get_raw(
			(adc1_channel_t)BAT_VOLTAGE_ADC_CHANNEL
		);
		esp_adc_cal_characterize(
			(adc_unit_t)BAT_VOLTAGE_ADC_UNIT,
			(adc_atten_t)BAT_VOLTAGE_ADC_ATTEN,
			(adc_bits_width_t)BAT_VOLTAGE_ADC_WIDTH,
			BAT_VOLTAGE_VREF,
			&adcChars
		);
		uint32_t mvReading = esp_adc_cal_raw_to_voltage(
			rawReading,
			(const esp_adc_cal_characteristics_t*)&adcChars
		);
		mvReading *= (BAT_VOLTAGE_LOWER_DIVIDER+BAT_VOLTAGE_UPPER_DIVIDER) / BAT_VOLTAGE_LOWER_DIVIDER;
		ret = (float)mvReading / 1000.0;
		ret += 0.8; // readings tend to be ~0.8V lower than actual
	} else {
		ESP_LOGE(TAG, "battery voltage pin not set up; sending %f", ret);
	}
	return ret;
}


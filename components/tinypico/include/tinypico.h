/*
 * TinyPICO.h
 *
 * TinyPICO utilizes the singleton design pattern.
 *
 *  Created on: Mar 17, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_TINYPICO_INCLUDE_TINYPICO_H_
#define COMPONENTS_TINYPICO_INCLUDE_TINYPICO_H_

#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"


// APA102 Dotstar
#define DOTSTAR_PWR 				13
#define DOTSTAR_DATA 				2
#define DOTSTAR_CLK 				12
#define SWSPI_PIN_BIT_MASK			(uint64_t)((1ULL<<DOTSTAR_CLK) | (1ULL<<DOTSTAR_DATA))
// Battery
#define BAT_CHARGE					34
#define BAT_VOLTAGE					35
#define BAT_VOLTAGE_MIN				3.4
#define BAT_VOLTAGE_MAX				4.2
#define BAT_VOLTAGE_RANGE			BAT_VOLTAGE_MAX - BAT_VOLTAGE_MIN
#define BAT_VOLTAGE_VREF			1100
#define BAT_VOLTAGE_UPPER_DIVIDER	442
#define BAT_VOLTAGE_LOWER_DIVIDER	160
const adc1_channel_t BAT_VOLTAGE_ADC_CHANNEL	= ADC1_CHANNEL_7;
const adc_bits_width_t BAT_VOLTAGE_ADC_WIDTH	= ADC_WIDTH_BIT_12;
const adc_atten_t BAT_VOLTAGE_ADC_ATTEN			= ADC_ATTEN_11db;
const adc_unit_t BAT_VOLTAGE_ADC_UNIT			= ADC_UNIT_1;


class TinyPICO {

public:
	static TinyPICO *getInstance();

	void turnOffLED();
	void turnOnLED();
	void showLED();
	void setLEDColor(uint8_t r, uint8_t g, uint8_t b);

	float getBatteryVoltage();
	uint8_t isBatteryCharging();
protected:
	TinyPICO();
	void swspiOut(uint8_t n);

	static TinyPICO* tp;
private:
	uint8_t ledInit = 0;
	uint8_t ledOn = 0;
	uint8_t ledColors[3] = {0x00, 0x00, 0x00};
	const gpio_config_t swspiOnConfig = {
			.pin_bit_mask = SWSPI_PIN_BIT_MASK,
			.mode = GPIO_MODE_OUTPUT,
			.pull_up_en = GPIO_PULLUP_DISABLE,
			.pull_down_en = GPIO_PULLDOWN_DISABLE,
			.intr_type = GPIO_INTR_DISABLE
	};
	const gpio_config_t swspiOffConfig = {
			.pin_bit_mask = SWSPI_PIN_BIT_MASK,
			.mode = GPIO_MODE_INPUT,
			.pull_up_en = GPIO_PULLUP_DISABLE,
			.pull_down_en = GPIO_PULLDOWN_ENABLE,
			.intr_type = GPIO_INTR_DISABLE
	};

	//esp_adc_cal_characteristics_t* adcChars;
	uint8_t batChargeInit = 0;
	uint8_t batVoltageInit = 0;
};


#endif /* COMPONENTS_TINYPICO_INCLUDE_TINYPICO_H_ */

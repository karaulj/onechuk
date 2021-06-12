/*
 * tinypico.h
 *
 * TinyPICO utilizes the singleton design pattern.
 *
 *  Created on: Mar 17, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_TINYPICO_INCLUDE_TINYPICO_H_
#define COMPONENTS_TINYPICO_INCLUDE_TINYPICO_H_

#include "hal/gpio_types.h"


// APA102 Dotstar
#define DOTSTAR_PWR 		13
#define DOTSTAR_DATA 		2
#define DOTSTAR_CLK 		12

#define SWSPI_PIN_BIT_MASK	(uint64_t)((1ULL<<DOTSTAR_CLK) | (1ULL<<DOTSTAR_DATA))


class TinyPICO {

public:
	static TinyPICO *getInstance();

	void turnOffLED();
	void turnOnLED();
	void showLED();
	void setLEDColor(uint8_t r, uint8_t g, uint8_t b);
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

};

#endif /* COMPONENTS_TINYPICO_INCLUDE_TINYPICO_H_ */

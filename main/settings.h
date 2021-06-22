/*
 * settings.h
 * 
 *  All relevant user settings.
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#ifndef MAIN_SETTINGS_H_
#define MAIN_SETTINGS_H_

#include "freertos/FreeRTOS.h"


// I2C
#define I2C_SDA_PIN             21
#define I2C_SCL_PIN             22
#define I2C_FREQ_HZ             100000
#define I2C_PORT_NUM            0

// nunchuk
#if configTICK_RATE_HZ > 100
#define NUNCHUK_READ_RATE_HZ    500
#else
#define NUNCHUK_READ_RATE_HZ    100
#endif

// RGB LED
#define R_PIN                   25
#define G_PIN                   26
#define B_PIN                   27

// Home Assistant API
#define HA_API_URL              "<HA url>/api"
#define HA_ACCESS_TOKEN         "<paste long-lived HA access token here>"

// TF Lite Micro
#define INPUT_SHAPE_DIM         16
#define INPUT_PRESCALER         16      // INPUT_SHAPE_DIM*INPUT_PRESCALER = 256


#endif /* MAIN_SETTINGS_H_ */
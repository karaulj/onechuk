/*
 * tasks.h
 * 
 * All FreeRTOS tasks defined here.
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_MAIN_TASKS_H_
#define COMPONENTS_MAIN_TASKS_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


// Constantly reads nunchuk data over I2C and posts over queue
const char* const NUNCHUK_READ_TASK = "nunchukReadTask";
void nunchukReadTask(void *pvParameter);
extern TaskHandle_t nunchukReadTaskHandle;

// Drives LED from commands received over queue
const char* const LED_CMD_TASK = "ledCmdTask";
void ledCmdTask(void *pvParameter);
extern TaskHandle_t ledCmdTaskHandle;

// Prints nunchuk values over serial when button is pressed
const char* const TRAINING_MODE_TASK = "trainingModeTask";
void trainingModeTask(void *pvParameter);
extern TaskHandle_t trainingModeTaskHandle;

// Receives joystick commands over queue and tracks active RGB profile
const char* const JOYSTICK_CMD_TASK = "joystickCmdTask";
void joystickCmdTask(void *pvParameter);
extern TaskHandle_t joystickCmdTaskHandle;

// Runs inference on joystick commands and sends successful results over queue
const char* const JOYSTICK_INFERENCE_TASK = "joystickInferenceTask";
void joystickInferenceTask(void *pvParameter);
extern TaskHandle_t joystickInferenceTaskHandle;


#endif /* COMPONENTS_MAIN_TASKS_H_ */

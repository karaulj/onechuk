/*
 * tasks.h
 * 
 * All FreeRTOS tasks housed here.
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_MAIN_TASKS_H_
#define COMPONENTS_MAIN_TASKS_H_


// Constantly reads nunchuk data over I2C and posts over queue
const char* const NUNCHUK_READ_TASK = "nunchukReadTask";
void nunchukReadTask(void *pvParameter);

// Drives LED from commands received over queue
const char* const LED_CMD_TASK = "ledCmdTask";
void ledCmdTask(void *pvParameter);

// Prints nunchuk values over serial when button is held down
const char* const TRAINING_MODE_TASK = "trainingModeTask";
void trainingModeTask(void *pvParameter);

// Runs inference on joystick commands and sends successful results over queue
const char* const JOYSTICK_INFERENCE_TASK = "joystickInferenceTask";
void joystickInferenceTask(void *pvParameter);

// Receives joystick commands over queue and tracks active RGB profile
const char* const JOYSTICK_CMD_TASK = "joystickCmdTask";
void joystickCmdTask(void *pvParameter);


#endif /* COMPONENTS_MAIN_TASKS_H_ */

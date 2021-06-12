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


const char* const NUNCHUK_READ_TASK = "nunchukReadTask";
void nunchukReadTask(void *pvParameter);

const char* const FADE_LED_TASK = "fadeLEDTask";
void fadeLEDTask(void *pvParameter);

const char* const PRINT_TASK = "printTask";
void printTask(void *pvParameter);


#endif /* COMPONENTS_MAIN_TASKS_H_ */

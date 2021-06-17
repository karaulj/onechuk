/*
 * queues.h
 * 
 * All FreeRTOS queues housed here.
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#ifndef MAIN_QUEUES_QUEUES_H_
#define MAIN_QUEUES_QUEUES_H_


#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "rgb_commands.h"
#include "gesture_model_tflite.h"


extern void queuesCheck();


// 'ledCmdTask' receives LED commands over this queue
#define LED_CMD_QUEUE_NAME              "ledCmdQueue"
#define LED_CMD_QUEUE_NUM_ITEMS         10
#define LED_CMD_QUEUE_ITEM_SZ           sizeof(rgb_cmd_t)
extern QueueHandle_t ledCmdQueue;

// 'joystickCmdQueue' receives recognized commands over this queue
#define JOYSTICK_CMD_QUEUE_NAME         "joystickCmdQueue"
#define JOYSTICK_CMD_QUEUE_NUM_ITEMS    10
#define JOYSTICK_CMD_QUEUE_ITEM_SZ      sizeof(gesture_label_t)
//#define JOYSTICK_CMD_QUEUE_ITEM_SZ      sizeof(uint32_t)
extern QueueHandle_t joystickCmdQueue;


#endif /* MAIN_QUEUES_QUEUES_H_ */
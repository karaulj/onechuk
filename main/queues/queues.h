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

#include "NunchukController.h"
#include "rgb_commands.h"


extern void queuesCheck();

// 'nunchukReadTask' sends latest nunchuk data over this queue
#define NUNCHUK_DATA_QUEUE_NAME         "nunchukDataQueue"
#define NUNCHUK_DATA_QUEUE_NUM_ITEMS    10
#define NUNCHUK_DATA_QUEUE_ITEM_SZ      sizeof(nunchuk_data_t)
extern QueueHandle_t nunchukDataQueue;

// 'ledCmdTask' receives LED commands over this queue
#define LED_CMD_QUEUE_NAME              "ledCmdQueue"
#define LED_CMD_QUEUE_NUM_ITEMS         10
#define LED_CMD_QUEUE_ITEM_SZ           sizeof(rgb_cmd_t)
extern QueueHandle_t ledCmdQueue;


#endif /* MAIN_QUEUES_QUEUES_H_ */
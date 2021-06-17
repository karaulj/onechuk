/*
 * queues.cpp
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#include "queues.h"

#include "freertos/queue.h"
#include "esp_log.h"

static const char *TAG = "queues";


QueueHandle_t ledCmdQueue = xQueueCreate(LED_CMD_QUEUE_NUM_ITEMS,
										LED_CMD_QUEUE_ITEM_SZ);


QueueHandle_t joystickCmdQueue = xQueueCreate(JOYSTICK_CMD_QUEUE_NUM_ITEMS,
										JOYSTICK_CMD_QUEUE_ITEM_SZ);



void queuesCheck()
{
	if (ledCmdQueue == NULL) {
		ESP_LOGE(TAG, "%s could not be created", LED_CMD_QUEUE_NAME);
	} else {
		ESP_LOGI(TAG, "%s created", LED_CMD_QUEUE_NAME);	
	}
	if (joystickCmdQueue == NULL) {
		ESP_LOGE(TAG, "%s could not be created", JOYSTICK_CMD_QUEUE_NAME);
	} else {
		ESP_LOGI(TAG, "%s created", JOYSTICK_CMD_QUEUE_NAME);	
	}
}



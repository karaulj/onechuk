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


QueueHandle_t nunchukDataQueue = xQueueCreate(NUNCHUK_DATA_QUEUE_NUM_ITEMS,
											NUNCHUK_DATA_QUEUE_ITEM_SZ);


void queuesCheck()
{
	if (nunchukDataQueue == NULL) {
		ESP_LOGE(TAG, "%s could not be created", NUNCHUK_DATA_QUEUE_NAME);
	} else {
		ESP_LOGI(TAG, "%s created", NUNCHUK_DATA_QUEUE_NAME);	
	}
}


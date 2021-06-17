/*
 * BlueDeviceProfile.cpp
 * 
 *  Created on: Jun 14, 2021
 *      Author: Jacob
 */

#include "BlueDeviceProfile.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "queues.h"
#include "rgb_commands.h"


static const char *TAG = "BlueDeviceProfile";


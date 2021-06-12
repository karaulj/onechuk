/*
 * NunchukController.cpp
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#include "NunchukController.h"

//#include "queues.h"
//#include "freertos/queue.h"
#include <stdio.h>

#include "driver/i2c.h"
#include "hal/i2c_types.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "NunchukController";

// for nunchuk I2C data 
static volatile uint8_t nunchukBuffer[NUNCHUK_CONTROLLER_BUFFER_SZ];


NunchukController::NunchukController(i2c_port_t i2cPort, uint8_t i2cAddr
    ) : port(i2cPort), addr(i2cAddr)
{
    // initialize, disable encryption
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr<<1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0xF0, true);
    i2c_master_write_byte(cmd, 0x55, true);
    i2c_master_write_byte(cmd, 0xFB, true);
    i2c_master_write_byte(cmd, 0x00, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(port, cmd, 10/portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    // fetch once
    fetchLatestReadings();

    ESP_LOGI(TAG, "initialized with port=%d, addr=%X", (int)port, (int)addr);
}


/*
 *  can get ~500 readings/sec if FreeRTOS tick rate is set to 1000Hz
 *  only ~100 readings/sec at 100Hz
 */ 
void NunchukController::fetchLatestReadings()
{
    // read latest data
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr<<1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, (uint8_t*)&nunchukBuffer[0], NUNCHUK_CONTROLLER_BUFFER_SZ-1, I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, (uint8_t*)&nunchukBuffer[NUNCHUK_CONTROLLER_BUFFER_SZ-1], I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(port, cmd, 1/portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    // send
    if (ret == ESP_OK) {
        btnAccelByte = nunchukBuffer[NUNCHUK_CONTROLLER_BTN_ACCEL_IDX];
        latestNunchukData = {
            .joystickX = nunchukBuffer[NUNCHUK_CONTROLLER_JOYSTICK_X_IDX],
            .joystickY = nunchukBuffer[NUNCHUK_CONTROLLER_JOYSTICK_Y_IDX],
            .accelX = (uint16_t)((nunchukBuffer[NUNCHUK_CONTROLLER_ACCEL_X_IDX] << 2) | 
                        ((btnAccelByte & 0b11000000) >> 6)),
            .accelY = (uint16_t)((nunchukBuffer[NUNCHUK_CONTROLLER_ACCEL_Y_IDX] << 2) | 
                        ((btnAccelByte & 0b00110000) >> 4)),
            .accelZ = (uint16_t)((nunchukBuffer[NUNCHUK_CONTROLLER_ACCEL_Z_IDX] << 2) | 
                        ((btnAccelByte & 0b00001100) >> 2)),
            .cButton = (uint8_t)(1 - ((btnAccelByte & 0b10) >> 1)),
            .zButton = (uint8_t)(1 - (btnAccelByte & 0b1))
        };
    }
    
    // request data for next time
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr<<1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x00, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(port, cmd, 1/portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}


nunchuk_data_t* NunchukController::getLatestData()   { return &latestNunchukData; }
uint8_t NunchukController::getJoystickX()           { return latestNunchukData.joystickX; }
uint8_t NunchukController::getJoystickY()           { return latestNunchukData.joystickY; }
uint16_t NunchukController::getAccelX()             { return latestNunchukData.accelX; }
uint16_t NunchukController::getAccelY()             { return latestNunchukData.accelY; }
uint16_t NunchukController::getAccelZ()             { return latestNunchukData.accelZ; }
uint8_t NunchukController::getCButton()             { return latestNunchukData.cButton; }
uint8_t NunchukController::getZButton()             { return latestNunchukData.zButton; }
/*
 * NunchukController.h
 *
 * Class for reading values from a Nintendo Wii nunchuk.
 * 
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_NUNCHUK_INCLUDE_NUNCHUK_CONTROLLER_H_
#define COMPONENTS_NUNCHUK_INCLUDE_NUNCHUK_CONTROLLER_H_

#include "hal/i2c_types.h"

// I2C
#define NUNCHUK_CONTROLLER_I2C_PORT_NUM     0
#define NUNCHUK_CONTROLLER_I2C_ADDR         0x52
#define NUNCHUK_CONTROLLER_BUFFER_SZ        6
#define NUNCHUK_CONTROLLER_JOYSTICK_X_IDX   0
#define NUNCHUK_CONTROLLER_JOYSTICK_Y_IDX   1
#define NUNCHUK_CONTROLLER_ACCEL_X_IDX      2
#define NUNCHUK_CONTROLLER_ACCEL_Y_IDX      3
#define NUNCHUK_CONTROLLER_ACCEL_Z_IDX      4
#define NUNCHUK_CONTROLLER_BTN_ACCEL_IDX    5

// General
#define NUNCHUK_CONTROLLER_JOYSTICK_MAX     0xFF
#define NUNCHUK_CONTROLLER_ACCEL_MAX        0x3FF   // 10-bit

/*
// struct for holding nunchuk data
typedef struct {
    uint8_t joystickX; 
    uint8_t joystickY;
    uint16_t accelX;
    uint16_t accelY;
    uint16_t accelZ;
    uint8_t cButton;
    uint8_t zButton;
} nunchuk_data_t;
*/

class NunchukController
{
public:
    static NunchukController* getInstance(
        i2c_port_t i2cPort = NUNCHUK_CONTROLLER_I2C_PORT_NUM,
        uint8_t i2cAddr = NUNCHUK_CONTROLLER_I2C_ADDR
    );
    
    void fetchLatestReadings();
    //nunchuk_data_t* getLatestData();
    uint8_t getJoystickX();
    uint8_t getJoystickY();
    uint16_t getAccelX();
    uint16_t getAccelY();
    uint16_t getAccelZ();
    uint8_t getCButton();
    uint8_t getZButton();
protected:
    NunchukController(
        i2c_port_t i2cPort,
        uint8_t i2cAddr
    );
    static NunchukController* nunchuk;
private:
    const i2c_port_t port;
    const uint8_t addr;
    uint8_t joystickX, joystickY;
    uint16_t accelX, accelY, accelZ;
    uint8_t cButton, zButton;
    /*
    nunchuk_data_t latestNunchukData = {
        .joystickX = NUNCHUK_CONTROLLER_JOYSTICK_MAX/2,
        .joystickY = NUNCHUK_CONTROLLER_JOYSTICK_MAX/2,
        .accelX = NUNCHUK_CONTROLLER_ACCEL_MAX/2,
        .accelY = NUNCHUK_CONTROLLER_ACCEL_MAX/2,
        .accelZ = NUNCHUK_CONTROLLER_ACCEL_MAX/2,
        .cButton = 0,
        .zButton = 0
    };
    */
    uint8_t btnAccelByte;
};


#endif /* COMPONENTS_NUNCHUK_INCLUDE_NUNCHUK_CONTROLLER_H_ */

/*
 * rgb_commands.h
 * 
 *  Single CC RGB LED animation functions and command types.
 *
 *  Created on: Jun 12, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_RGB_CC_LED_INCLUDE_RGB_COMMANDS_H_
#define COMPONENTS_RGB_CC_LED_INCLUDE_RGB_COMMANDS_H_

#include "RGB_CC_LED.h"


typedef uint8_t rgb_cmd_t;
// system
const rgb_cmd_t RGB_CMD_SYSTEM_INIT         = 0x00;
const rgb_cmd_t RGB_CMD_RESTART             = 0x01;
// ble
const rgb_cmd_t RGB_CMD_BLE_CONNECTED       = 0x10;
const rgb_cmd_t RGB_CMD_BLE_DISCONNECTED    = 0x11;
// gesture mode
const rgb_cmd_t RGB_CMD_GESTURE_MODE        = 0x20;
const rgb_cmd_t RGB_CMD_GESTURE_FOUND       = 0x21;
const rgb_cmd_t RGB_CMD_GESTURE_NOT_FOUND   = 0x22;
const rgb_cmd_t RGB_CMD_GESTURE_NOT_IMPL    = 0x23;
const rgb_cmd_t RGB_CMD_CANCEL_GESTURE      = 0x24;
// training mode
const rgb_cmd_t RGB_CMD_TRAINING_MODE       = 0x30;
// clear
const rgb_cmd_t RGB_CMD_CLEAR_ALL           = 0xFF;


// system
extern void rgbcSystemInit(RGB_CC_LED* led);
extern void rgbcRestart(RGB_CC_LED* led);
// ble
extern void rgbcBleConnected(RGB_CC_LED* led);
extern void rgbcBleDisconnected(RGB_CC_LED* led);
// gesture mode
extern void rgbcGestureMode(RGB_CC_LED* led);
extern void rgbcGestureFound(RGB_CC_LED* led);
extern void rgbcGestureNotFound(RGB_CC_LED* led);
extern void rgbcGestureNotImpl(RGB_CC_LED* led);
extern void rgbcCancelGesture(RGB_CC_LED* led);
// training mode
extern void rgbcTrainingMode(RGB_CC_LED* led);
// clear
extern void rgbcClearAll(RGB_CC_LED* led);


#endif /* COMPONENTS_RGB_CC_LED_INCLUDE_RGB_COMMANDS_H_ */
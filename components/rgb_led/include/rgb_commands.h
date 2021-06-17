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


// All RGB commands defined here
typedef uint8_t rgb_cmd_t;
// system
const rgb_cmd_t RGB_CMD_SYSTEM_INIT             = 0x00;
const rgb_cmd_t RGB_CMD_RESTART                 = 0x01;
const rgb_cmd_t RGB_CMD_CLEAR_ALL               = 0x0F;
// ble
const rgb_cmd_t RGB_CMD_BLE_CONNECTED           = 0x10;
const rgb_cmd_t RGB_CMD_BLE_DISCONNECTED        = 0x11;
const rgb_cmd_t RGB_CMD_BLE_PAIRING_MODE_ACTIVE = 0x12;
// device mode - joystick gesture recognition
const rgb_cmd_t RGB_CMD_DEVICE_MODE             = 0x20;
const rgb_cmd_t RGB_CMD_GESTURE_ACTIVE          = 0x21;
const rgb_cmd_t RGB_CMD_GESTURE_FOUND           = 0x22;
const rgb_cmd_t RGB_CMD_GESTURE_NOT_FOUND       = 0x23;
const rgb_cmd_t RGB_CMD_GESTURE_NOT_IMPL        = 0x24;
const rgb_cmd_t RGB_CMD_CANCEL_GESTURE          = 0x25;
// training mode
const rgb_cmd_t RGB_CMD_TRAINING_MODE           = 0x30;
// battery
const rgb_cmd_t RGB_CMD_BATTERY_RANGE           = 0x60;
const rgb_cmd_t RGB_CMD_BATTERY_LEVEL           = 0x61;
const rgb_cmd_t RGB_CMD_BATTERY_CHARGING        = 0x62;
const rgb_cmd_t RGB_CMD_BATTERY_NOT_CHARGING    = 0x63;
// accel fade
const rgb_cmd_t RGB_CMD_ACCEL_FADE              = 0x70;
const rgb_cmd_t RGB_CMD_ACCEL_FADE_5_MS         = 0x71;
const rgb_cmd_t RGB_CMD_ACCEL_FADE_10_MS        = 0x72;
const rgb_cmd_t RGB_CMD_ACCEL_FADE_20_MS        = 0x73;
const rgb_cmd_t RGB_CMD_ACCEL_FADE_50_MS        = 0x74;
const rgb_cmd_t RGB_CMD_ACCEL_FADE_100_MS       = 0x75;
// joystick fade
const rgb_cmd_t RGB_CMD_JOYSTICK_RG_FADE        = 0x80;
const rgb_cmd_t RGB_CMD_JOYSTICK_RB_FADE        = 0x81;
const rgb_cmd_t RGB_CMD_JOYSTICK_GB_FADE        = 0x82;


// All RGB command functions declared here
// system
extern void rgbcSystemInit(RGB_CC_LED* led);
extern void rgbcRestart(RGB_CC_LED* led);
extern void rgbcClearAll(RGB_CC_LED* led);
// ble
extern void rgbcBleConnected(RGB_CC_LED* led);
extern void rgbcBleDisconnected(RGB_CC_LED* led);
extern void rgbcBlePairingModeActive(RGB_CC_LED* led);
// device mode
extern void rgbcDeviceMode(RGB_CC_LED* led);
extern void rgbcGestureActive(RGB_CC_LED* led);
extern void rgbcGestureFound(RGB_CC_LED* led);
extern void rgbcGestureNotFound(RGB_CC_LED* led);
extern void rgbcGestureNotImpl(RGB_CC_LED* led);
extern void rgbcCancelGesture(RGB_CC_LED* led);
// training mode
extern void rgbcTrainingMode(RGB_CC_LED* led);
// battery
extern void rgbcBatteryRange(RGB_CC_LED* led);
extern void rgbcBatteryLevel(RGB_CC_LED* led, color_level_t level);
extern void rgbcBatteryCharging(RGB_CC_LED* led);
extern void rgbcBatteryNotCharging(RGB_CC_LED* led);
// accel fade
extern void rgbcAccelFade(RGB_CC_LED* led);
extern void rgbcAccelFade5ms(RGB_CC_LED* led);
extern void rgbcAccelFade10ms(RGB_CC_LED* led);
extern void rgbcAccelFade20ms(RGB_CC_LED* led);
extern void rgbcAccelFade50ms(RGB_CC_LED* led);
extern void rgbcAccelFade100ms(RGB_CC_LED* led);
// joystick fade
extern void rgbcJoystick_RG_Fade(RGB_CC_LED* led);
extern void rgbcJoystick_RB_Fade(RGB_CC_LED* led);
extern void rgbcJoystick_GB_Fade(RGB_CC_LED* led);


#endif /* COMPONENTS_RGB_CC_LED_INCLUDE_RGB_COMMANDS_H_ */
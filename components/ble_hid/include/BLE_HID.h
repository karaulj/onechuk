/*
 * BLE_HID.h
 *
 *  Bluetooth LE Human Interface Device. Uses Singleton pattern.
 * 
 *  Created on: Jun 13, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_BLE_HID_INCLUDE_BLE_HID_H_
#define COMPONENTS_BLE_HID_INCLUDE_BLE_HID_H_

#define BLE_HID_TIMEOUT_MS          30000
#define BLE_HID_DEVICE_NAME         "Funchuk"
#define BLE_HID_PAIR_TIMER_NAME     "BLE Pair Timeout"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"


extern volatile uint8_t bleConnected;

class BLE_HID {
public:
    static BLE_HID* getInstance();

    void setPairable(uint8_t isPairable);
    void startPairableWindow(uint32_t pairableTimeoutMs = BLE_HID_TIMEOUT_MS);

    void send(uint8_t keyCmd);
    void sendPlayPause();
    void sendVolumeUp();
    void sendVolumeDown();
    void sendNextTrack();
    void sendPrevTrack();
    void sendShufflePlay();
protected:
    BLE_HID();
    static BLE_HID* hid;
    TimerHandle_t timerHandle;
private:
    uint8_t pairWindowTimerInit = 0;
    uint8_t paused = 0;
};


#endif
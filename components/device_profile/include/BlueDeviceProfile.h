/*
 * BlueDeviceProfile.h
 *
 *  B device profile - User-defined, default is key combos via BLE HID
 * 
 *  Created on: Jun 14, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_MAIN_PROFILES_BLUE_DEVICE_PROFILE_H_
#define COMPONENTS_MAIN_PROFILES_BLUE_DEVICE_PROFILE_H_

#include "IDeviceProfile.h"


class BlueDeviceProfile: public IDeviceProfile
{
public:
    void profileSelectedCallback();
    void gestureClickCallback();
    void gestureUpCallback();
    void gestureDownCallback();
    void gestureLeftCallback();
    void gestureRightCallback();
    void gestureCircleCallback();
};


#endif /* COMPONENTS_MAIN_PROFILES_BLUE_DEVICE_PROFILE_H_ */
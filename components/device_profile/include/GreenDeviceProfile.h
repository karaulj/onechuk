/*
 * GreenDeviceProfile.h
 *
 *  G device profile - Media keys via BLE HID
 * 
 *  Created on: Jun 14, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_MAIN_PROFILES_GREEN_DEVICE_PROFILE_H_
#define COMPONENTS_MAIN_PROFILES_GREEN_DEVICE_PROFILE_H_

#include "IDeviceProfile.h"


class GreenDeviceProfile : public IDeviceProfile
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


#endif /* COMPONENTS_MAIN_PROFILES_GREEN_DEVICE_PROFILE_H_ */
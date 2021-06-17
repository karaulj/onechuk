/*
 * IDeviceProfile.h
 *
 *  Class representing a profile in device mode.
 * 
 *  Created on: Jun 13, 2021
 *      Author: Jacob
 */

#ifndef COMPONENTS_MAIN_PROFILES_IDEVICE_PROFILE_H_
#define COMPONENTS_MAIN_PROFILES_IDEVICE_PROFILE_H_

#include <stdint.h>


class IDeviceProfile
{
public:
    static uint32_t count() {return cnt;};
    const uint32_t id = cnt++;
    virtual ~IDeviceProfile() {cnt--;};

    virtual void gestureClickCallback();
    virtual void gestureUpCallback();
    virtual void gestureDownCallback();
    virtual void gestureLeftCallback();
    virtual void gestureRightCallback();
    virtual void gestureCircleCallback();
private:
    static uint32_t cnt;
};


#endif /* COMPONENTS_MAIN_PROFILES_IDEVICE_PROFILE_H_ */
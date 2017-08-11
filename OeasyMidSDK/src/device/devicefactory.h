#ifndef DEVICE_FACTORY_H
#define DEVICE_FACTORY_H
#include "icamera.h"
#include "iintercom.h"
#include "ialarmhost.h"

#include "hkcamera.h"
#include "dhcamera.h"
#include "hkalarmhost.h"
#include "oeasyintercom.h"
#include "OeasyMidDefines.h"

#include <stdio.h>

using namespace OeasyMid_DH;
using namespace OeasyMid_HK;

class DeviceFactory
{
public:
	DeviceFactory();
	virtual ~DeviceFactory();
	//create 的 device需要自己释放内存
	virtual IDevice* createDevice(DEVICETYPE deviceType, int deviceModel);
private:
	virtual ICamera* createCamera(CAMERATYPE cameraType);
	virtual IItercom* CreateInterCom(INTERCOMTYPE intercomType);
	virtual IAlarmHost* CreateAlarmHost(ALARMHOSTTYPE intercomType);
};

#endif //DEVICE_FACTORY_H
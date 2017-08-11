#include "devicefactory.h"

DeviceFactory::DeviceFactory()
{

}

DeviceFactory::~DeviceFactory()
{

}

ICamera* DeviceFactory::createCamera( CAMERATYPE cameraType )
{
	ICamera *camera = NULL;
	switch(cameraType)
	{
	case _HIKVISION_IPC:
		{
			HKCamera *hk = new HKCamera();
			camera = hk;
		}

		break;
	case _DAHUA_IPC:
		{
			//DHCamera *dh = new DHCamera();
			//camera = dh;
		}
	
		break;
	case _TIANDY_IPC:
		break;
	case _OTHER_IPC:
		break;
	default:
		break;
	}
	return camera;
}

IItercom* DeviceFactory::CreateInterCom( INTERCOMTYPE intercomType )
{
	IItercom *intercom = NULL;
	switch(intercomType)
	{
	case _OEASYLOG_INTERCOM:
		{
			//OeasyIntercom *oeasy = new OeasyIntercom();
			//intercom = oeasy;
		}
		break;
	case _OTHER_INERCOM:
		break;
	default:
		break;
	}
	return intercom;
}

IAlarmHost* DeviceFactory::CreateAlarmHost(ALARMHOSTTYPE alarmhostType )
{
	IAlarmHost *alarmhost = NULL;
	switch(alarmhostType)
	{
	case _HIKVISION_ALARMHOST:
		{
			//HKAlarmHost *hkalarm = new HKAlarmHost();
			//alarmhost = hkalarm;
		}
		break;
	case _OTHER_ALARMHOST:
		break;
	default:
		break;
	}
	return alarmhost;
}

IDevice* DeviceFactory::createDevice( DEVICETYPE deviceType, int deviceModel )
{
	IDevice *device = NULL;
	switch(deviceType)
	{
	case CAMERA:
		{
			device = createCamera((CAMERATYPE)deviceModel);
		}
		break;
	case INTERCOM:
		{
			device = CreateInterCom((INTERCOMTYPE)deviceModel);
		}
		break;
	case ALARMHOST:
		{
			device = CreateAlarmHost((ALARMHOSTTYPE)deviceModel);
		}
		break;
	default:
		break;
	}
	return device;
}


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
	case HIKVISION:
		{
			HKCamera *hk = new HKCamera();
			camera = hk;
		}

		break;
	case DAHUA:
		{
			DHCamera *dh = new DHCamera();
			camera = dh;
		}
	
		break;
	case TIANDIWEIYE:
		break;
	case OTHERCAMERA:
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
	case OEASY:
		{
			OeasyIntercom *oeasy = new OeasyIntercom();
			intercom = oeasy;
		}
		break;
	case OTHERINERCOM:
		break;
	default:
		break;
	}
	return intercom;
}


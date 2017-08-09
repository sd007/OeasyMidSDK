#include "OeasyMidSdk.h"
#include "devicefactory.h"
#include "BaseType.h"
#include "log4cpp.h"
using namespace std;


BOOL APIENTRY DllMain( HANDLE hModule, 
	DWORD  ul_reason_for_call, 
	LPVOID lpReserved
	)
{
	OEASY_I("DllMain");

	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		{
			SOCK_INIT
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{
			SOCK_DEINIT
		}
		break;
	}
	return TRUE;
}

OEASY_API int stdcall OeasyMid::Oeasy_init()
{
	DeviceFactory *factory = new DeviceFactory();
	ICamera *camera = factory->createCamera(HIKVISION);
	ICamera *camera1 = factory->createCamera(DAHUA);
	//cout<<"init---"<<camera->getCameraType() << camera1->getCameraType();
	IItercom *inercom = factory->CreateInterCom(OEASY);
	cout<<"init22---"<<inercom->getIntercomType();
	int type1 = camera->getCameraType();
	OEASY_I("init---%d, %d",type1, 66);
	OEASY_I("test %d",666);
	delete factory;
	delete camera;
	delete camera1;
	delete inercom;
	return 0;
}

OEASY_API void  stdcall OeasyMid::Oeasy_deinit()
{
	logInstance.destory();
}

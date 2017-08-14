#include <iostream>
#include "OeasyMidSdk.h"
#include "OeasyMidDefines.h"

#ifdef _WIN32
#include <windows.h> 
#define SLEEP(X) Sleep(X)
#else
#include <unistd.h> 
#define SLEEP(X) sleep(X)
#endif

using namespace OeasyMidIPC;
using namespace std;

void videoDataCallBack(long lPlayHandle,  unsigned char *pBuffer, unsigned long bufferSize, void *pUSer)
{
	cout<<"videoDataCallBack  get ....................... "<<bufferSize<<endl;
}

int main()
{
	Oeasy_InitAll();
	char version[32] = {0};
	Oeasy_GetOeasyMidVersion(version);
	cout<<"version == "<<version<<endl;
	OEASYHANDLE handle = Oeasy_CreateHandle();


	int ret2 = Oeasy_IPC_Create(handle, HIKVISION_IPC);

	OEASY_LOGINFO loginfo;
	memset(&loginfo, 0 ,sizeof(OEASY_LOGINFO));
	loginfo.devicePort = 8000;
	memcpy(loginfo.deviceAddress,"192.168.1.64", DEVICE_ADDRESS_LEN);
	memcpy(loginfo.username, "admin", USERNAME_LEN);
	memcpy(loginfo.password, "oeasy808", PASSWORD_LEN);

	OEASY_LOGIN_RESULTINFO resultInfo;
	memset(&resultInfo, 0 ,sizeof(OEASY_LOGIN_RESULTINFO));
	int ret = Oeasy_IPC_Login(handle,&loginfo, &resultInfo);
	if (ret >=0 )
	{
		Oeasy_IPC_SetLiveDataCB(handle, (LIVEDATACALLBACK)videoDataCallBack, NULL);

		IPCLiveHandle livehandle = Oeasy_IPC_StartLive(handle, MAIN_STREAM);
		while (1)
		{
			SLEEP(2000);
		}
		Oeasy_IPC_StopLive(handle, livehandle);
		Oeasy_IPC_Logout(handle);
	}

	//OEASY_API int stdcall Oeasy_IPC_StopLive(OEASYHANDLE handle, IPCLiveHandle livehandle);
	Oeasy_IPC_Destroy(handle);

	Oeasy_ReleaseHandle(handle);
	Oeasy_DeinitAll();
	return 0;
}
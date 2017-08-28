#include <iostream>
#include "OeasyMidSdk.h"
#include "OeasyMidDefines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h> 
#define SLEEP(X) Sleep(X)
#else
#include <unistd.h> 
#define SLEEP(X) sleep(X)
#endif

using namespace OeasyMidIPC;
using namespace OeasyMidIntercom;
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

	
	int ret2 = Oeasy_IPC_Create(handle, DAHUA_IPC);
        cout<<"create ret = "<<ret2<<endl;
	OEASY_LOGINFO loginfo;
	memset(&loginfo, 0 ,sizeof(OEASY_LOGINFO));
	loginfo.devicePort = 37777;
	memcpy(loginfo.deviceAddress,"192.168.1.108", DEVICE_ADDRESS_LEN);
	memcpy(loginfo.username, "admin", USERNAME_LEN);
	memcpy(loginfo.password, "oeasy123456", PASSWORD_LEN);

	OEASY_LOGIN_RESULTINFO resultInfo;
	memset(&resultInfo, 0 ,sizeof(OEASY_LOGIN_RESULTINFO));
	int ret = Oeasy_IPC_Login(handle,&loginfo, &resultInfo);
	cout<<"login ret = "<<ret<<endl;

	unsigned char *picBuffer = (unsigned char*)malloc(512*1024);
	memset(picBuffer, 0, 512*1024);
	unsigned long retSize = 0;

	if (ret >=0 )
	{
		//Oeasy_IPC_SetLiveDataCB(handle, (LIVEDATACALLBACK)videoDataCallBack, NULL);

		IPCLiveHandle livehandle = Oeasy_IPC_StartLive(handle, MAIN_STREAM);
		cout<<"start live result = "<<livehandle<<endl;
		Oeasy_IPC_SetLiveDataCB(handle, (LIVEDATACALLBACK)videoDataCallBack, NULL);
		int count = 5;
		while (count--)
		{
			SLEEP(1000);
		}
		int ret = Oeasy_IPC_CaptureImage( handle,picBuffer, 1024*512, &retSize);
		if (ret > 0)
		{
			FILE *fp;
			fp = fopen("d:/abc.jpg","wb");
			fwrite(picBuffer, retSize, 1, fp);
			fflush(fp);
			fclose(fp);
		}

		Oeasy_IPC_StopLive(handle, livehandle);
		Oeasy_IPC_Logout(handle);
	}
	free(picBuffer);
	Oeasy_IPC_Destroy(handle);
	

	//Oeasy_Intercom_Create(handle, OEASY_INTERCOM);

	//Oeasy_Intercom_startServer(handle, 12308);
	//while (1)
	//{
	//	SLEEP(2000);
	//}
	//Oeasy_Intercom_stopServer(handle);
	//Oeasy_Intercom_Destroy(handle);

	Oeasy_ReleaseHandle(handle);
	Oeasy_DeinitAll();
	return 0;
}

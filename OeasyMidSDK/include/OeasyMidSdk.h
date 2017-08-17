#ifndef OEASY_MID_SDK
#define OEASY_MID_SDK

#include "OeasyMidDefines.h"
#include <iostream>
using namespace std;

#define OEASYDLL_EXPORT


#define OEASYHANDLE		unsigned int

#ifdef _WIN32
#define stdcall _stdcall

#ifdef OEASYDLL_EXPORT
#define OEASY_API  __declspec(dllexport)
#else
#define OEASY_API  __declspec(dllimport)
#endif

#else
#define stdcall
#define CALLBACK  // __cdecl和__stdcall有差异，windows下会有问题
#define OEASY_API
#endif


#ifdef __cplusplus
extern "C" {
#endif
//global API
OEASY_API int stdcall Oeasy_InitAll();

OEASY_API void stdcall Oeasy_DeinitAll();

OEASY_API void stdcall Oeasy_GetOeasyMidVersion(char* version);

//instance API

OEASY_API OEASYHANDLE stdcall Oeasy_CreateHandle();

OEASY_API void stdcall Oeasy_ReleaseHandle(OEASYHANDLE handle);

namespace OeasyMidIPC{

#define IPCLiveHandle long

//callback
typedef void (*LIVEDATACALLBACK) (long lPlayHandle,  unsigned char *pBuffer, unsigned long bufferSize, void *pUSer);

//api
OEASY_API int stdcall Oeasy_IPC_Create(OEASYHANDLE handle, CAMERATYPE cameraType);

OEASY_API int stdcall Oeasy_IPC_Destroy(OEASYHANDLE handle);

OEASY_API int stdcall Oeasy_IPC_Login(OEASYHANDLE handle, OEASY_LOGINFO* loginfo, OEASY_LOGIN_RESULTINFO* resultInfo);

OEASY_API int stdcall Oeasy_IPC_Logout(OEASYHANDLE handle);

OEASY_API void stdcall Oeasy_IPC_SetLiveDataCB(OEASYHANDLE handle, LIVEDATACALLBACK videoDataCB, void *pUser);

OEASY_API IPCLiveHandle stdcall Oeasy_IPC_StartLive(OEASYHANDLE handle, STREAMTYPE streamtype, bool bstartsms = false, char* mediaserverurl = "");

OEASY_API int stdcall Oeasy_IPC_StopLive(OEASYHANDLE handle, IPCLiveHandle livehandle);

OEASY_API int stdcall Oeasy_IPC_CaptureImage(OEASYHANDLE handle, char *picBuffer, unsigned long bufferSize,  unsigned long* sizeReturned);
}

namespace OeasyMidIntercom{

OEASY_API int stdcall Oeasy_Intercom_Create(OEASYHANDLE handle, INTERCOMTYPE Type);

OEASY_API void stdcall Oeasy_Intercom_Destroy(OEASYHANDLE handle);

OEASY_API void stdcall Oeasy_Intercom_startServer(OEASYHANDLE handle, unsigned short serverPort);

OEASY_API void stdcall  Oeasy_Intercom_stopServer(OEASYHANDLE handle);
}

namespace OeasyMidAlarmHost{


}

#ifdef __cplusplus
}
#endif

#endif //OEASY_MID_SDK

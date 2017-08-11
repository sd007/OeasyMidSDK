#include "OeasyMidSdk.h"
#include "devicefactory.h"
#include "BaseType.h"
#include "log4cpp.h"
#include "OeasyMidDefines.h"
#include "HandleManager.h"
//hikvison
#include "HCNetSDK.h"

using namespace std;

#define OEASYMID_VERSION "0.0.1"

#ifdef _WIN32
BOOL APIENTRY DllMain( HANDLE hModule, 
	DWORD  ul_reason_for_call, 
	LPVOID lpReserved
	)
{
	OEASYLOG_I("DllMain");

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
#else
#include <signal.h>
void sigPipe(int sig){}
#endif

OEASY_API void Oeasy_GetOeasyMidVersion( char* version )
{
	OEASYLOG_I("Oeasy_getOeasyMidVersion");
	OEASY_ASSERT(version,,)
	memcpy(version, OEASYMID_VERSION, sizeof(OEASYMID_VERSION));
}

OEASY_API int stdcall Oeasy_InitAll()
{
	OEASYLOG_I("Oeasy_InitAll");
#ifdef _USE_IPC
	NET_DVR_Init();
#endif
	
	return 0;
}

OEASY_API void stdcall Oeasy_DeinitAll()
{
#ifdef _USE_IPC
	NET_DVR_Cleanup();
#endif
	OEASYLOG_I("Oeasy_DeinitAll");
	logInstance.destory();
}

OEASY_API OEASYHANDLE stdcall Oeasy_CreateInstance()
{
	OEASYLOG_I("Oeasy_createInstance");
	OEASYHANDLE handle = OEASY_HANDLE_INVALIDE;
	CApiImpl* pCore = CHandleManager::CreateApiManager(handle);
	OEASY_ASSERT (pCore, {}, handle);
	return handle;
}

OEASY_API void stdcall Oeasy_ReleaseHandle( OEASYHANDLE handle)
{
	OEASYLOG_I("Oeasy_releaseHandle");
	CHandleManager::DeleteApiManager(handle);
}


#ifdef _USE_IPC

OEASY_API int stdcall OeasyMidIPC::Oeasy_IPC_Create(OEASYHANDLE handle, CAMERATYPE cameraType)
{
	OEASYLOG_I("Oeasy_IPC_Init handle = %d", handle);
	CApiImpl* pCore = CHandleManager::GetApiManager(handle);
	OEASY_ASSERT (pCore, {}, handle);
	pCore->IPC_Create(cameraType);
	return 0;
}

OEASY_API int  stdcall OeasyMidIPC::Oeasy_IPC_Destroy(OEASYHANDLE handle)
{
	OEASYLOG_I("Oeasy_IPC_Deinit handle = %d", handle);
	CApiImpl* pCore = CHandleManager::GetApiManager(handle);
	OEASY_ASSERT (pCore, {}, handle);
	pCore->IPC_Destroy();
	return 0;
}

OEASY_API int stdcall OeasyMidIPC::Oeasy_IPC_Login( OEASYHANDLE handle, OEASY_LOGINFO* loginfo, OEASY_LOGIN_RESULTINFO* resultInfo )
{
	OEASY_ASSERT(loginfo && resultInfo,,-1)
	OEASYLOG_I("Oeasy_IPC_Login handle = %d", handle);
	CApiImpl* pCore = CHandleManager::GetApiManager(handle);
	OEASY_ASSERT (pCore, {}, handle);
	pCore->IPC_Login(loginfo, resultInfo);
	return 0;
}

OEASY_API int stdcall OeasyMidIPC::Oeasy_IPC_Logout( OEASYHANDLE handle )
{
	OEASYLOG_I("Oeasy_IPC_Logout handle = %d", handle);
	CApiImpl* pCore = CHandleManager::GetApiManager(handle);
	OEASY_ASSERT (pCore, {}, handle);
	pCore->IPC_LoginOut();
	return 0;
}

OEASY_API IPCLiveHandle stdcall OeasyMidIPC::Oeasy_IPC_StartLive( OEASYHANDLE handle, STREAMTYPE streamtype, bool bstartsms /*= false*/, char* mediaserverurl /*= ""*/ )
{
	OEASYLOG_I("Oeasy_IPC_StartLive handle = %d", handle);
	CApiImpl* pCore = CHandleManager::GetApiManager(handle);
	OEASY_ASSERT (pCore, {}, handle);
	pCore->IPC_StartLive(streamtype, bstartsms, mediaserverurl);
	return 0;
}

OEASY_API int stdcall OeasyMidIPC::Oeasy_IPC_StopLive( OEASYHANDLE handle, IPCLiveHandle livehandle )
{
	OEASYLOG_I("Oeasy_IPC_StopLive handle = %d ,livehandle = %d", handle, livehandle);
	CApiImpl* pCore = CHandleManager::GetApiManager(handle);
	OEASY_ASSERT (pCore, {}, handle);
	pCore->IPC_StopLive(livehandle);
	return 0;
}

OEASY_API void stdcall OeasyMidIPC::Oeasy_IPC_SetLiveDataCB( OEASYHANDLE handle, LIVEDATACALLBACK videoDataCB, void *pUser )
{
	OEASYLOG_I("Oeasy_IPC_SetLiveDataCB handle = %d", handle);
	CApiImpl* pCore = CHandleManager::GetApiManager(handle);
	pCore->IPC_SetLiveDataCB(videoDataCB,pUser);
}

#endif
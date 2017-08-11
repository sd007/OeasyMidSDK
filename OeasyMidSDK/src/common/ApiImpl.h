#ifndef __OEASYMID_COMMON_APIIMPL__
#define __OEASYMID_COMMON_APIIMPL__

#include "OeasyMidDefines.h"
#include "OeasyMidSdk.h"
#include "cameradefine.h"
#include "intercomdefine.h"
#include "alarmhostdefine.h"
#include "BaseType.h"
#include "devicefactory.h"
#include "idevice.h"
//typedef std::vector<NVR_QCMSG>	VQC_MSG;
using namespace OeasyMidIPC;

class CHandleManager;
class CApiImpl
{
	friend class CHandleManager;
public:
	CApiImpl(void);
	~CApiImpl(void);
	DEVICETYPE getDeviceType();
#ifdef _USE_IPC

	int IPC_Create(CAMERATYPE type);
	CAMERATYPE IPC_GetCameraType();
	void IPC_Destroy();
	OEASY_S32 IPC_Login(OEASY_LOGINFO* loginfo, OEASY_LOGIN_RESULTINFO* resultInfo);
	OEASY_S32 IPC_LoginOut();
	void IPC_SetLiveDataCB(LIVEDATACALLBACK videoDataCB, void *pUser );
	OEASY_DWORD IPC_StartLive(STREAMTYPE streamtype, bool bstartsms = false, char* mediaserverurl = "");
	OEASY_S32 IPC_StopLive(OEASY_DWORD livehandle);

#endif
	OEASY_S32 GetEvent(OEASY_CHAR *msg);


protected:
	OEASY_U32 Ref();
	OEASY_U32 UnRef();
	OEASY_U32 GetRef();
	OEASY_U32 ARef();
	OEASY_U32 UnARef();
	OEASY_U32 GetARef();

	//static OEASY_U32 OnNotify(NETNOTIFY msg, OEASY_CHAR* data, OEASY_S32 len, void* pUserData);
	static OEASY_U32 OnEvent(OEASYLOG_EVENTMSG event, OEASY_CHAR* data, OEASY_S32 len, void* pUserData);
private:

	IDevice *m_curDevice;
	DeviceFactory *m_deviceFactory;
	int m_deviceModel;
	DEVICETYPE m_deviceType;
	OEASY_U32 m_iRef;
	OEASY_U32 m_iARef;

	MUTEX m_lock;


};
#endif //__OEASYMID_COMMON_APIIMPL__




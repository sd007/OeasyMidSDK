#include "ApiImpl.h"
#include "cameradefine.h"
#include "intercomdefine.h"
#include "alarmhostdefine.h"
#include "log4cpp.h"

CApiImpl::CApiImpl(void)
	:m_deviceModel(NO_DEVICE),
	m_deviceType(NO_DEVICE),
	m_curDevice(NULL)
{
	m_iRef = 0;
	m_iARef = 0;
	INIT_MUTEX(&m_lock);
	m_deviceFactory = new DeviceFactory();
}


CApiImpl::~CApiImpl(void)
{
	IPC_Destroy();
	OEASY_SAFACE_DELETE(m_deviceFactory);
	OEASY_SAFACE_DELETE(m_curDevice);
	RELEASE_MUTEX(&m_lock);
}

OEASY_U32 CApiImpl::Ref()
{
	m_iRef ++;
	return m_iRef;
}

OEASY_U32 CApiImpl::UnRef()
{
	if (m_iRef > 0) {
		m_iRef--;
	}
	return m_iRef;
}

OEASY_U32 CApiImpl::GetRef()
{
	return m_iRef;
}

OEASY_U32 CApiImpl::ARef()
{
	m_iARef ++;
	return m_iARef;
}

OEASY_U32 CApiImpl::UnARef()
{
	if (m_iARef > 0) {
		m_iARef--;
	}
	return m_iARef;
}

OEASY_U32 CApiImpl::GetARef()
{
	return m_iARef;
}

OEASY_U32  CApiImpl::OnEvent(OEASYLOG_EVENTMSG event, OEASY_CHAR* data, OEASY_S32 len, void* pUserData)
{
	//CApiImpl *pThis = (CApiImpl *)pUserData;
	//pThis->m_msgdata->message = event;
	//pThis->m_msgdata->dataSize = len;
	//memcpy(pThis->m_msgdata->Data, data, len);
	//pThis->eventWriter->putFrame((char*)pThis->m_msgdata, len+sizeof(NVR_QCMSG), SHM_ACCESS_WRITE_NONBLOCK, 1);
	//switch(event)
	//{
	
	//}
	return 0;
}

OEASY_S32 CApiImpl::GetEvent(OEASY_CHAR *msg)
{
	//int key = 0;
	//if (eventReader->getFrame((FOS_CHAR*)msg, 128*1024, &key))
	//{
	//	NVR_QCMSG *nvrMsg = (NVR_QCMSG*)msg;
	//	switch(nvrMsg->message)
	//	{
	//	case NVR_NEWIP_INFO:
	//		if (m_pStaticIPToDHCP != NULL)
	//		{
	//			FY_NET_STOP_SEARCH(m_pStaticIPToDHCP);
	//			m_pStaticIPToDHCP = NULL;
	//		}
	//		break;
	//	case NVR_INIT_INFO:
	//		memcpy(nvrMsg->Data,&m_lgInitInfo,sizeof(FsNvrMsg::InitInfo));
	//		break;
	//	case NVR_INIT_INFO_COMPATIBLE:
	//		memcpy(nvrMsg->Data,&m_lgInitInfoCompatible,sizeof(FsNvrMsg::InitInfo_Compatible));
	//		break;
	//	case NVR_LOGIN_INFO:
	//		memcpy(nvrMsg->Data,&m_lgResp,sizeof(FsNvrMsg::LoginResp));
	//		break;
	//	}
	//	return NVRCMD_OK;
	//}
	//else
	//{
	//	return NVRCMD_NODATA;
	//}
	return 0;
}

int CApiImpl::IPC_Create(CAMERATYPE type)
{
	m_deviceType = CAMERA;
	m_deviceModel = type;

	m_curDevice = m_deviceFactory->createDevice(m_deviceType, m_deviceModel);
	if (NULL == m_curDevice)
	{
		return -1;
	}
	return 0;
}

void CApiImpl::IPC_Destroy()
{
	OEASY_SAFACE_DELETE(m_curDevice)
}


OEASY_S32 CApiImpl::IPC_Login( OEASY_LOGINFO* loginfo, OEASY_LOGIN_RESULTINFO* resultInfo )
{
	int camerahandle = m_curDevice->loginCamera(*(_OEASY_LOGINFO*)loginfo, *(_OEASY_LOGIN_RESULTINFO*)resultInfo);
	if (camerahandle < 0)
	{
		OEASYLOG_I("IPC_Login error!, ip = %d, port = %d, ret = %d", loginfo->deviceAddress , loginfo->devicePort, camerahandle);
		return -1;
	}
	OEASYLOG_I("IPC_Login success, handle = %d", camerahandle);
	return camerahandle;
}

DEVICETYPE CApiImpl::getDeviceType()
{
	return m_deviceType;
}

CAMERATYPE CApiImpl::IPC_GetCameraType()
{
	return (CAMERATYPE)m_deviceModel;
}

OeasyMid::OEASY_S32 CApiImpl::IPC_LoginOut()
{
	return m_curDevice->logoutCamera();
}

OeasyMid::OEASY_DWORD CApiImpl::IPC_StartLive(STREAMTYPE streamtype, bool bstartsms /*= false*/, char* mediaserverurl /*= ""*/ )
{
	return m_curDevice->openVideo((_STREAMTYPE)streamtype, (OEASY_BOOL)bstartsms, mediaserverurl);
}

OeasyMid::OEASY_S32 CApiImpl::IPC_StopLive( OEASY_DWORD livehandle )
{
	return m_curDevice->closeVideo(livehandle);
}

void CApiImpl::IPC_SetLiveDataCB( LIVEDATACALLBACK videoDataCB, void *pUser )
{
	return m_curDevice->setLiveDataCB(videoDataCB, pUser);
}

OeasyMid::OEASY_S32 CApiImpl::IPC_CaptureImage( OEASY_U8 *picBuffer, OEASY_DWORD bufferSize, OEASY_DWORD* sizeReturned )
{
	return m_curDevice->captureImage(picBuffer, bufferSize, sizeReturned);
}

void CApiImpl::Intercom_startServer( OEASY_U16 serverport )
{
	m_curDevice->Intercom_startServer(serverport);
}

void CApiImpl::Intercom_stopServer()
{
	m_curDevice->Intercom_stopServer();
}

int CApiImpl::Intercom_Create( INTERCOMTYPE type )
{
	m_deviceType = INTERCOM;
	m_deviceModel = type;

	m_curDevice = m_deviceFactory->createDevice(m_deviceType, m_deviceModel);
	if (NULL == m_curDevice)
	{
		return -1;
	}
	return 0;
}

void CApiImpl::Intercom_Destroy()
{
	OEASY_SAFACE_DELETE(m_curDevice)
}



	



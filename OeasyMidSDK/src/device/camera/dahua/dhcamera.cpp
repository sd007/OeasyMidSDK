#include "dhcamera.h"
#include "log4cpp.h"
#include "Tick.h"
#include "OeasyMidDefines.h"
using namespace OeasyMid_DH;

#define  SNAPBUFFER_SZIE 2*1024*1024

_CAMERATYPE DHCamera::getCameraType()
{
	return _DAHUA_IPC;
}


OeasyMid_DH::DHCamera::DHCamera()
:m_cameraID(-1),
m_liveplayID(-1),
m_bmainstartSMS(OEASY_FALSE),
m_bsubstartSMS(OEASY_FALSE),
m_snapCmdSerial(0)
{
	m_snapBuffer = (OEASY_U8 *)malloc(SNAPBUFFER_SZIE);
	m_snapSize = 0;
	INIT_MUTEX(&m_snapMutex);
	CLIENT_SetSnapRevCallBack((fSnapRev)SnapDataRev, (LDWORD)this);
}

OeasyMid_DH::DHCamera::~DHCamera()
{
	free(m_snapBuffer);
	RELEASE_MUTEX(&m_snapMutex);
}

void CALLBACK OeasyMid_DH::DHCamera::realDataCallback( LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG param, LDWORD dwUser )
{
	DHCamera *curCamera = (DHCamera*)dwUser;
	if (curCamera == NULL)
	{
		OEASYLOG_E("DHCamera::realDataCallback error!");
		return;
	}
	switch(dwDataType)
	{
		case 0: //原始音视频混合数据
			if(dwBufSize > 0)
			{

				if (curCamera->m_bmainstartSMS)  //推主码流
				{
				}else if(curCamera->m_bsubstartSMS){ //推子码流

				}else if(curCamera->m_liveDataCB){ //不推流
					(*curCamera->m_liveDataCB)((long)lRealHandle, (unsigned char*)pBuffer, (unsigned long)dwBufSize, curCamera->m_pUserData);
				}
			}
			break;
		case 1: //标准视频数据
			if(dwBufSize > 0)
			{
				
				if (curCamera->m_bmainstartSMS)  //推主码流
				{
				}else if(curCamera->m_bsubstartSMS){ //推子码流

				}else if(curCamera->m_liveDataCB){ //不推流
					(*curCamera->m_liveDataCB)((long)lRealHandle, (unsigned char*)pBuffer, (unsigned long)dwBufSize, curCamera->m_pUserData);
				}
			}
			break;
		case 2: //yuv数据
			break;
		case 3: //pcm数据
			break;
		case 4: //原始音频数据
			break;
	}
}

void CALLBACK OeasyMid_DH::DHCamera::MsgCallBack( LONG lCommand, LLONG lLoginID, char *pBuf, DWORD dwBufLen, char *pchDVRIP, LONG nDVRPort, LDWORD dwUser )
{
	DHCamera *curCamera = (DHCamera*)dwUser;
	if (curCamera == NULL)
	{
		OEASYLOG_E("DHCamera::MsgCallBack error!");
		return;
	}
	//OEASYLOG_I("DHCamera::MsgCallBack command = %d", lCommand);
	switch(lCommand)
	{
	case DH_EVENT_FACE_DETECTION: //人脸检测事件
		{
			if (curCamera->m_alarmMsgCB)
			{
				(*curCamera->m_alarmMsgCB)((ALARMTYPE)_FACEDETECT_ALARM, lLoginID, pBuf, (unsigned long)dwBufLen, pchDVRIP,nDVRPort, curCamera->m_pAlarmUserData);
			}
		}
		break;
	case DH_EVENT_MOTIONDETECT: //移动侦测事件
		{
			if (curCamera->m_alarmMsgCB)
			{
				(*curCamera->m_alarmMsgCB)((ALARMTYPE)_MOVEDETECT_ALARM, lLoginID, pBuf, (unsigned long)dwBufLen, pchDVRIP,nDVRPort, curCamera->m_pAlarmUserData);
			}
		}
		break;
	case DH_ALARM_ALARM_EX:			//0x2101	//External alarm 
		{
			//(*curCamera->m_alarmMsgCB)((ALARMTYPE)_FACEDETECT_ALARM, lLoginID, pBuf, (unsigned long)dwBufLen, pchDVRIP,nDVRPort, curCamera->m_pAlarmUserData);
		}
		break;
	case DH_MOTION_ALARM_EX:		//	0x2102	//Motion detection alarm 
		{
			if (curCamera->m_alarmMsgCB)
			{
				(*curCamera->m_alarmMsgCB)((ALARMTYPE)_MOVEDETECT_ALARM, lLoginID, pBuf, (unsigned long)dwBufLen, pchDVRIP,nDVRPort, curCamera->m_pAlarmUserData);
			}		
		}
		break;
	case DH_VIDEOLOST_ALARM_EX:		//0x2103	//Video loss alarm 
	case DH_SHELTER_ALARM_EX:		//	0x2104	//Camera masking alarm 
	case DH_SOUND_DETECT_ALARM_EX:	//0x2105	//Audio detection alarm 
		break;
	default:
		break;
	}
}


CameraId OeasyMid_DH::DHCamera::loginCamera( _OEASY_LOGINFO loginfo, _OEASY_LOGIN_RESULTINFO resultInfo )
{
	//NET_PARAM netparm = {0};
	//netparm.nConnectTime = 2000;
	////超时时间
	//CLIENT_SetNetworkParam(&netparm);

	NET_DEVICEINFO_Ex deviceInfo ={0};
	unsigned long lLogin;
	int errcode = 0;
	lLogin = CLIENT_LoginEx2(loginfo.deviceAddress,loginfo.devicePort, loginfo.username,loginfo.password, EM_LOGIN_SPEC_CAP_TCP, NULL, &deviceInfo, &errcode);
	m_cameraID = lLogin;
	if (m_cameraID == 0)
	{
		OEASYLOG_E("DH loginCamera error, code = %d", errcode);
		m_cameraID = -1;
	}
	return m_cameraID;
}

OeasyMid::OEASY_S32 OeasyMid_DH::DHCamera::logoutCamera()
{
	BOOL ret = CLIENT_Logout(m_cameraID);
	if (ret < 0)
	{
		OEASYLOG_E("DH logoutCamera error, m_cameraID = %d", m_cameraID);
	}
	return ret;
}

void OeasyMid_DH::DHCamera::setLiveDataCB( LIVEDATACALLBACK videoDataCB, void *pUser )
{
	m_liveDataCB = videoDataCB;
	m_pUserData = pUser;
	BOOL ret = CLIENT_SetRealDataCallBackEx( m_liveplayID, (fRealDataCallBackEx)DHCamera::realDataCallback, (LDWORD)this, RAW_DATA);
	return;
}

LiveplayId OeasyMid_DH::DHCamera::openVideo( _STREAMTYPE streamtype, OEASY_BOOL bstartsms /*= OEASY_FALSE*/, OEASY_CHAR* mediaserverurl /*= ""*/ )
{
	if (_MAIN_STREAM == streamtype && bstartsms)
	{
		m_bmainstartSMS = OEASY_TRUE;
	}else if(_SUB_STREAM == streamtype && bstartsms){
		m_bsubstartSMS = OEASY_TRUE;
	}

	DH_RealPlayType realtype = (streamtype == _MAIN_STREAM) ? DH_RType_Realplay:DH_RType_Realplay_1;
	int nChannelID = 0;
	LLONG ret = CLIENT_RealPlayEx(m_cameraID, nChannelID, NULL, realtype);
	if (ret == 0)
	{
		OEASYLOG_E("DH openVideo failed! ret = %d, m_cameraID = %d",ret, m_cameraID);
		m_liveplayID = -1;
	}
	m_liveplayID = ret;
	return ret;
}

OeasyMid::OEASY_BOOL OeasyMid_DH::DHCamera::closeVideo( LiveplayId liveid )
{
	OEASY_BOOL ret = OEASY_TRUE;
	if (liveid > 0)
	{
		ret = (OEASY_BOOL)CLIENT_StopRealPlayEx(liveid);
	}
	return ret;
}

void CALLBACK OeasyMid_DH::DHCamera::SnapDataRev( LLONG lLoginID,BYTE *pBuf,UINT RevLen,UINT EncodeType, DWORD CmdSerial, LDWORD dwUser )
{
	DHCamera *curCamera = (DHCamera*)dwUser;
	if (curCamera == NULL || curCamera->m_snapCmdSerial !=  CmdSerial)
	{
		OEASYLOG_E("DHCamera::SnapDataRev error!");
		return;
	}
	if (RevLen > 0 )
	{
		OEASYLOG_I("DHCamera::SnapDataRev RevLen = %d", RevLen);
		LOCK_MUTEX(&curCamera->m_snapMutex);
		if (SNAPBUFFER_SZIE > RevLen)
		{
			memcpy(curCamera->m_snapBuffer, pBuf, RevLen);
			curCamera->m_snapSize = RevLen;
			OEASYLOG_I("DHCamera::SnapDataRev OK");
		}
		UNLOCK_MUTEX(&curCamera->m_snapMutex);
	}
}


OeasyMid::OEASY_S32 OeasyMid_DH::DHCamera::captureImage( OEASY_U8*picBuffer, OEASY_DWORD bufferSize, OEASY_DWORD* sizeReturned )
{
	int timeout = 1000; //10s
	//CLIENT_SetSnapRevCallBack((fSnapRev)SnapDataRev, (LDWORD)this);
	OEASY_U64 enterTime = CTick::GetTickCount();
	SNAP_PARAMS pram;
	pram.Channel = 0;
	pram.Quality = 3;
	pram.mode = 0;
	m_snapCmdSerial++;
	pram.CmdSerial = m_snapCmdSerial;
	BOOL ret = CLIENT_SnapPictureEx(m_cameraID, &pram);
	OEASYLOG_W("DHCamera::CLIENT_SnapPictureEx ret = %d", ret);
	while(CTick::GetTickCount() - enterTime < timeout)
	{
		if (m_snapSize > 0) //geted image
		{
			if (bufferSize >= m_snapSize)
			{
				memcpy(picBuffer, m_snapBuffer, m_snapSize);
			}
			*sizeReturned = m_snapSize;
			break;
		}
	}
	if (m_snapSize < 0) //noimage
	{
		*sizeReturned = m_snapSize;
		return -1; //tiemout and no image
	}
	memset(m_snapBuffer, 0, SNAPBUFFER_SZIE);
	m_snapSize = 0;
	return 1;
}

OeasyMid::OEASY_S32 OeasyMid_DH::DHCamera::setAlarmParam( _ALARMSETTING *param )
{
	return 0;
}

OeasyMid::OEASY_S32 OeasyMid_DH::DHCamera::startAlarm( ALARMMESGCALLBACK alarmMsgCB, void *pUser )
{

	m_alarmMsgCB = alarmMsgCB;
	m_pAlarmUserData = pUser;
	CLIENT_StartListenEx(m_cameraID);
	CLIENT_SetDVRMessCallBack((fMessCallBack)MsgCallBack ,LDWORD(this));
	return 0;
}

OeasyMid::OEASY_S32 OeasyMid_DH::DHCamera::stopAlarm()
{
	CLIENT_StopListen(m_cameraID);
	return 0;
}



#include "hkcamera.h"
#include "HCNetSDK.h"
#include "log4cpp.h"
using namespace OeasyMid_HK;


HKCamera::HKCamera()
	:m_cameraID(-1),
	m_liveplayHandle(-1),
	m_bmainstartSMS(OEASY_FALSE),
	m_bsubstartSMS(OEASY_FALSE)
{
}

HKCamera::~HKCamera()
{

}
/**************************CALLBACK***********************/
void HKCamera::exceptionCB( DWORD dwType, LONG lUserID, LONG lHandle, void *pUser )
{
	char tempbuf[256] = {0};
	HKCamera* currentCamera = (HKCamera*)pUser;
	switch(dwType) 
	{
	case EXCEPTION_RECONNECT:    //预览时重连
		OEASYLOG_W("----------reconnect--------%d\n", time(NULL));
		break;
	default:
		break;
	}
}

void HKCamera::videoDataCallback( LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void* pUser )
{
	HKCamera *currentCamera = (HKCamera*)pUser;
	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD:
		break;
	case NET_DVR_STREAMDATA:
		if (dwBufSize > 0)
		{
			(*currentCamera->m_liveDataCB)((long)lPlayHandle, (unsigned char*)pBuffer, (unsigned long)dwBufSize, currentCamera->m_pUserData);
			/*QByteArray *m_frame;

			if (lRealHandle == tmp->m_lRealPlayHandle)
			{
				m_frame = &tmp->m_frame_main;
			}
			else if (lRealHandle == tmp->m_lRealPlayHandle1)
			{
				m_frame = &tmp->m_frame_second;
			}
			else
			{
				return ;
			}

			QByteArray frame((const char*)pBuffer, dwBufSize);
			if (frame.size() >= 4 && frame.at(0) == '\0' && frame.at(1) == '\0' && frame.at(2) == '\1' && frame.at(3) == (char)0xba)
			{
				if (m_frame->size() > 0)
				{
					int totalLen;

					totalLen = GetH246FromPs(m_frame->data(), m_frame->size());
					if (totalLen == 0)
					{
						*m_frame = frame;
						return;
					}
					*m_frame = m_frame->mid(0, totalLen);

					if (lRealHandle == tmp->m_lRealPlayHandle)
					{
						emit tmp->packetReady(*m_frame);
					}
					else if (lRealHandle == tmp->m_lRealPlayHandle1)
					{
						emit tmp->packetReady1(*m_frame);
					}
				}
				m_frame->clear();
			}
			*m_frame += frame;*/
		}
		break;
	case NET_DVR_STD_VIDEODATA:
		OEASYLOG_I("NET_DVR_STD_VIDEODATA size = %d", dwBufSize);
		break;
	default:
		break;
	}
}
/********************************************************/

_CAMERATYPE HKCamera::getCameraType()
{
	return _HIKVISION_IPC;
}

CameraId HKCamera::loginCamera( _OEASY_LOGINFO loginfo, _OEASY_LOGIN_RESULTINFO resultInfo )
{
	OEASYLOG_I("HKCamera::loginCamera");
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	//登录参数，包括设备地址、登录用户、密码等
	NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
	struLoginInfo.bUseAsynLogin = 0; //使用同步登录方式
	strcpy_s(struLoginInfo.sDeviceAddress, loginfo.deviceAddress); //设备IP地址
	struLoginInfo.wPort = loginfo.devicePort; //设备服务端口
	strcpy_s(struLoginInfo.sUserName, loginfo.username); //设备登录用户名
	strcpy_s(struLoginInfo.sPassword, loginfo.password); //设备登录密码
	//设备信息, 输出参数
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

	m_cameraID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
	if (m_cameraID < 0)
	{
		OEASY_DWORD errcode = NET_DVR_GetLastError();
		if (NET_DVR_PASSWORD_ERROR == errcode)
		{
			OEASYLOG_E("HKCamera::Login failed, username or password error!");
		}else
		{
			OEASYLOG_E("HKCamera::Login failed, error code: %d\n", errcode);
		}
		return m_cameraID;
	}
	memcpy(&resultInfo.Deviceinfo, &struDeviceInfoV40.struDeviceV30, sizeof(NET_DVR_DEVICEINFO_V30));
	NET_DVR_SetExceptionCallBack_V30(0, NULL, (ExceptionCallBack)HKCamera::exceptionCB, this);
	OEASYLOG_I("HKCamera::loginCamera success, m_cameraID = %d", m_cameraID);
	return m_cameraID;
}

OEASY_S32 HKCamera::logoutCamera()
{
	OEASY_ASSERT(m_cameraID,,OEASY_FALSE);
	OEASYLOG_I("HKCamera::loginCamera m_cameraID = %d", m_cameraID);
	return (OEASY_S32)NET_DVR_Logout(m_cameraID);
}

LiveplayHandle HKCamera::openVideo(_STREAMTYPE streamtype, OEASY_BOOL bstartsms /*= OEASY_FALSE*/, OEASY_CHAR* mediaserverurl /*= "" */)
{
	OEASY_ASSERT(m_cameraID,,OEASY_FALSE);
	OEASYLOG_I("HKCamera::openVideo m_cameraID = %d, streamtype = %d, bstartsms = %d", m_cameraID, streamtype, bstartsms);
	//启动预览并设置回调数据流
	NET_DVR_PREVIEWINFO struPlayInfo;
	memset(&struPlayInfo, 0, sizeof(struPlayInfo));
	struPlayInfo.hPlayWnd = 0; //需要SDK 解码时句柄设为有效值，仅取流不解码时可设为空
	struPlayInfo.lChannel = 1; //hikivison IPC 为1
	struPlayInfo.dwStreamType = streamtype;
	struPlayInfo.dwLinkMode = 0; //0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
	struPlayInfo.bBlocked = 1; //0- 非阻塞取流，1- 阻塞取流 若设为不阻塞，表示发起与设备的连接就认为连接成功，如果发生码流接收失败、播放失败等情况以预览异常的方式通知上层。在循环播放的时候可以减短停顿的时间，与NET_DVR_RealPlay处理一致。若设为阻塞，表示直到播放操作完成才返回成功与否。
	if (_MAIN_STREAM == streamtype && bstartsms)
	{
		m_bmainstartSMS = OEASY_TRUE;
	}else if(_SUB_STREAM == streamtype && bstartsms){
		m_bsubstartSMS = OEASY_TRUE;
	}
	m_liveplayHandle = NET_DVR_RealPlay_V40(m_cameraID, &struPlayInfo, (REALDATACALLBACK)HKCamera::videoDataCallback, this);
	return m_liveplayHandle;
}

OEASY_BOOL HKCamera::closeVideo(LiveplayHandle livehandle)
{
	OEASY_ASSERT(m_cameraID,,OEASY_FALSE);
	OEASYLOG_I("HKCamera::closeVideo m_cameraID = %d", m_cameraID);
	return (OEASY_BOOL)NET_DVR_StopRealPlay(livehandle);
}


void OeasyMid_HK::HKCamera::setLiveDataCB( LIVEDATACALLBACK videoDataCB, void *pUser )
{
	m_liveDataCB = videoDataCB;
	m_pUserData = pUser;
}

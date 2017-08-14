#ifndef HK_CAMERA_H
#define HK_CAMERA_H

#include "cameradefine.h"
#include "icamera.h"
#include <iostream>
#include<map>
#include<string>
#include "OeasyMidSdk.h"
using namespace OeasyMidIPC;

using namespace std;
using namespace OeasyMid;

namespace OeasyMid_HK{

	class HKCamera : public ICamera
	{
	public:
		HKCamera();
		virtual ~HKCamera();
		virtual _CAMERATYPE getCameraType();
		virtual CameraId loginCamera(_OEASY_LOGINFO loginfo, _OEASY_LOGIN_RESULTINFO resultInfo);
		virtual OEASY_S32 logoutCamera();
		virtual void setLiveDataCB( LIVEDATACALLBACK videoDataCB, void *pUser );
		virtual LiveplayId openVideo(_STREAMTYPE streamtype, OEASY_BOOL bstartsms = OEASY_FALSE, OEASY_CHAR* mediaserverurl = "");
		virtual OEASY_BOOL closeVideo(LiveplayId liveid);
		//callback 
		static void exceptionCB(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
		static void CALLBACK realDataCallback(LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void* pUser);

	public:
		CameraId m_cameraID;
		LiveplayId m_liveplayID;
		OEASY_BOOL m_bmainstartSMS; //主码流是否上传到流服务器
		OEASY_BOOL m_bsubstartSMS; //子码流是否上传到流服务器
		LIVEDATACALLBACK m_liveDataCB;
		void *m_pUserData; 
	};
}

#endif //HK_CAMERA_H
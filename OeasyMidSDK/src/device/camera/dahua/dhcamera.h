#ifndef DH_CAMERA_H
#define DH_CAMERA_H

#include "cameradefine.h"
#include "icamera.h"
#include "avglobal.h"
#include "dhconfigsdk.h"
#include "dhnetsdk.h"

namespace OeasyMid_DH{

	typedef enum{
		RAW_DATA = 0x00000001,
		MPEG4_H264 = 0x00000002,
		YUV_DATA = 0x00000004,
		ALL_DATA = 0x0000001f
	};

	class DHCamera : public ICamera
	{
	public:
		DHCamera();
		virtual ~DHCamera();

		virtual _CAMERATYPE getCameraType();
		virtual CameraId loginCamera(_OEASY_LOGINFO loginfo, _OEASY_LOGIN_RESULTINFO resultInfo);
		virtual OEASY_S32 logoutCamera();
		virtual void setLiveDataCB( LIVEDATACALLBACK videoDataCB, void *pUser );
		virtual LiveplayId openVideo(_STREAMTYPE streamtype, OEASY_BOOL bstartsms = OEASY_FALSE, OEASY_CHAR* mediaserverurl = "");
		virtual OEASY_BOOL closeVideo(LiveplayId liveid);
		virtual OEASY_S32 captureImage( OEASY_U8 *picBuffer, OEASY_DWORD bufferSize, OEASY_DWORD* sizeReturned );
		//callback 
		static void CALLBACK realDataCallback(LLONG  lRealHandle, DWORD  dwDataType, BYTE  *pBuffer, DWORD  dwBufSize,LONG param, LDWORD dwUser);
		static void CALLBACK SnapDataRev(LLONG  lLoginID,BYTE *pBuf,UINT RevLen,UINT EncodeType, DWORD CmdSerial, LDWORD dwUser);
	public:
		OEASY_U32 m_snapCmdSerial;
		OEASY_U8 *m_snapBuffer;
		OEASY_U32 m_snapSize;
		MUTEX m_snapMutex;
	private:
		CameraId m_cameraID;
		LiveplayId m_liveplayID;
		OEASY_BOOL m_bmainstartSMS; //主码流是否上传到流服务器
		OEASY_BOOL m_bsubstartSMS; //子码流是否上传到流服务器
		LIVEDATACALLBACK m_liveDataCB;
		void *m_pUserData; 
		
	};
}

#endif //DH_CAMERA_H
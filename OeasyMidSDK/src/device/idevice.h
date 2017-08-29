#ifndef _DEVICE_IDEVICE__H
#define _DEVICE_IDEVICE__H
#include "cameradefine.h"
#include "intercomdefine.h"
#include "alarmhostdefine.h"
#include "OeasyMidSdk.h"
using namespace OeasyMidIPC;

typedef long CameraId;
typedef long LiveplayId;

class IDevice
{
public:
	/****************************camera method  begin*********************************************/
	virtual _CAMERATYPE getCameraType() = 0;
	virtual CameraId loginCamera(_OEASY_LOGINFO loginfo, _OEASY_LOGIN_RESULTINFO resultInfo) = 0;
	virtual OEASY_S32 logoutCamera() = 0;
	virtual void setLiveDataCB( LIVEDATACALLBACK videoDataCB, void *pUser ) = 0;
	virtual LiveplayId openVideo(_STREAMTYPE streamtype, OEASY_BOOL bstartsms = OEASY_FALSE, OEASY_CHAR* mediaserverurl = "") = 0;
	virtual OEASY_BOOL closeVideo(LiveplayId handle) = 0;
	virtual OEASY_S32 captureImage( OEASY_U8 *picBuffer, OEASY_DWORD bufferSize, OEASY_DWORD* sizeReturned ) = 0;
	virtual OEASY_S32 setAlarmParam( _ALARMSETTING *param ) = 0;
	virtual OEASY_S32 startAlarm(ALARMMESGCALLBACK alarmMsgCB, void *pUser) = 0;
	virtual OEASY_S32 stopAlarm() = 0;

	//unused
	virtual OEASY_BOOL release() = 0;
	/****************************camera method  end**********************************************/

	/****************************Intercom method begin******************************************/
	virtual void Intercom_startServer( OEASY_U16 serverport ) = 0;
	virtual void Intercom_stopServer() = 0;
	/****************************Intercom method  end*******************************************/
};

#endif //_DEVICE_IDEVICE__H
#ifndef ICAMERA_H
#define ICAMERA_H
#include "cameradefine.h"
#include "idevice.h"
#include "OeasyMidSdk.h"
using namespace OeasyMidIPC;

class ICamera : public IDevice
{
public:
	virtual _CAMERATYPE getCameraType() = 0;
	virtual CameraId loginCamera(_OEASY_LOGINFO loginfo, _OEASY_LOGIN_RESULTINFO resultInfo) = 0;
	virtual OEASY_S32 logoutCamera() = 0;

	virtual LiveplayId openVideo(_STREAMTYPE streamtype, OEASY_BOOL bstartsms = OEASY_FALSE, OEASY_CHAR* mediaserverurl = "") = 0;
	virtual OEASY_BOOL closeVideo(LiveplayId handle) = 0;

	virtual void setLiveDataCB( LIVEDATACALLBACK videoDataCB, void *pUser ) = 0;
	virtual OEASY_S32 captureImage( OEASY_U8 *picBuffer, OEASY_DWORD bufferSize, OEASY_DWORD* sizeReturned ) = 0;

	//unuse method for Camera
	virtual OEASY_BOOL release(){return OEASY_TRUE;}
	virtual void Intercom_startServer( OEASY_U16 serverport ){}
	virtual void Intercom_stopServer(){}
};

#endif //ICAMERA_H
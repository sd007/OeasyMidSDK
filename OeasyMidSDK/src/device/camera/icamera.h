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

	virtual LiveplayHandle openVideo(_STREAMTYPE streamtype, OEASY_BOOL bstartsms = OEASY_FALSE, OEASY_CHAR* mediaserverurl = "") = 0;
	virtual OEASY_BOOL closeVideo(LiveplayHandle handle) = 0;

	virtual void setLiveDataCB( LIVEDATACALLBACK videoDataCB, void *pUser ) = 0;
	//unuse method for Camera
	virtual OEASY_BOOL release(){return OEASY_TRUE;}

};

#endif //ICAMERA_H
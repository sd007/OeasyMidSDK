#ifndef ICAMERA_H
#define ICAMERA_H
#include "cameradefine.h"

class ICamera
{
public:
	virtual CAMERATYPE getCameraType() = 0;

};

#endif //ICAMERA_H
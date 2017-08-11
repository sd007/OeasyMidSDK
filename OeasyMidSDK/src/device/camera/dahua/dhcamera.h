#ifndef DH_CAMERA_H
#define DH_CAMERA_H

#include "cameradefine.h"
#include "icamera.h"

namespace OeasyMid_DH{
	class DHCamera : public ICamera
	{
	public:
		DHCamera();
		virtual ~DHCamera();

		virtual _CAMERATYPE getCameraType();
	};
}

#endif //DH_CAMERA_H
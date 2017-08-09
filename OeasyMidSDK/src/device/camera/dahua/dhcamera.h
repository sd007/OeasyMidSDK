#include "cameradefine.h"
#include "icamera.h"

namespace OeasyMid_DH{
	class DHCamera : public ICamera
	{
	public:
		DHCamera();
		virtual ~DHCamera();

		virtual CAMERATYPE getCameraType();
	};
}

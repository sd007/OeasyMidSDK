#include "cameradefine.h"
#include "icamera.h"

namespace OeasyMid_HK{
	class HKCamera : public ICamera
	{
	public:
		HKCamera();
		virtual ~HKCamera();
		virtual CAMERATYPE getCameraType();
	};
}

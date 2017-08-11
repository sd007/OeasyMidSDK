#ifndef __OEASYMID_PLATFORM_AUTOLOCK__
#define __OEASYMID_PLATFORM_AUTOLOCK__
#include "BaseType.h"
namespace OeasyMid
{
	class CAutoLock
	{
	public:
		CAutoLock(MUTEX& mutex);
		~CAutoLock();
	protected:
		MUTEX&	mMutex;
	};
};

#endif //__OEASYMID_PLATFORM_AUTOLOCK__



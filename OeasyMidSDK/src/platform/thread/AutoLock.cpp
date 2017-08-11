#include "AutoLock.h"

namespace OeasyMid
{
	CAutoLock::CAutoLock(MUTEX& mutex)
		: mMutex(mutex)
	{
		LOCK_MUTEX(&mutex);
	}

	CAutoLock::~CAutoLock()
	{
		UNLOCK_MUTEX(&mMutex);
	}
};



#ifndef HK_ALARMHOST_H
#define HK_ALARMHOST_H
#include "ialarmhost.h"
#include "alarmhostdefine.h"
#include "BaseType.h"
#include <iostream>

using namespace OeasyMid;


namespace OeasyMid_HK{

	typedef void (CALLBACK* ExceptionCallBack)(DWORD dwType, LONG lUserID, LONG lHandle, void *pUsser);
	class HKAlarmHost : public IAlarmHost
	{
	public:
		HKAlarmHost();
		virtual ~HKAlarmHost();
		virtual _ALARMHOSTTYPE getAlarmHostType();

	};
}

#endif //HK_ALARMHOST_H
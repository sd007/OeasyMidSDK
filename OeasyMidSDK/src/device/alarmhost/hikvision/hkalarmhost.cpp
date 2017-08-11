#include "hkalarmhost.h"
using namespace OeasyMid_HK;

HKAlarmHost::HKAlarmHost()
{

}

HKAlarmHost::~HKAlarmHost()
{

}

_ALARMHOSTTYPE OeasyMid_HK::HKAlarmHost::getAlarmHostType()
{
	return _HIKVISION_ALARMHOST;
}
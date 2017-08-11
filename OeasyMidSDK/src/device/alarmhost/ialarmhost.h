#ifndef IALARMHOST_H
#define IALARMHOST_H
#include "alarmhostdefine.h"
#include "idevice.h"

class IAlarmHost :public IDevice
{
public:
	virtual _ALARMHOSTTYPE getAlarmHostType() = 0;

};

#endif //IALARMHOST_H
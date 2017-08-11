#ifndef ALARMHOST_DEFINE_H
#define ALARMHOST_DEFINE_H
#include "BaseType.h"
using namespace OeasyMid;

#pragma pack(push,1)


//报警主机
typedef enum{
	_HIKVISION_ALARMHOST, //海康威视
	_OTHER_ALARMHOST
}_ALARMHOSTTYPE;


 #pragma pack(pop) 
#endif
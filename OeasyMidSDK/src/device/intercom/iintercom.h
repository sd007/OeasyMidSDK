#ifndef IINTERCOM_H
#define IINTERCOM_H
#include "intercomdefine.h"
#include "idevice.h"

class IItercom :public IDevice
{
public:
	virtual _INTERCOMTYPE getIntercomType() = 0;
		
};


#endif //IINTERCOM_H
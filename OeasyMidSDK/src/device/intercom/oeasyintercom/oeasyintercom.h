#ifndef OEASYLOG_INTERCOM_H
#define OEASYLOG_INTERCOM_H
#include "intercomdefine.h"
#include "iintercom.h"

class OeasyIntercom : public IItercom
{
public:
	OeasyIntercom();
	virtual ~OeasyIntercom();

	virtual _INTERCOMTYPE getIntercomType();
};

#endif //OEASYLOG_INTERCOM_H
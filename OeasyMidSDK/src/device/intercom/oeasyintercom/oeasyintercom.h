#include "intercomdefine.h"
#include "iintercom.h"

class OeasyIntercom : public IItercom
{
public:
	OeasyIntercom();
	virtual ~OeasyIntercom();

	virtual INTERCOMTYPE getIntercomType();
};


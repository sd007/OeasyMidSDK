#ifndef OEASY_MID_SDK
#define OEASY_MID_SDK

#include <iostream>
using namespace std;

#define OEASYDLL_EXPORT

#ifdef _WIN32
#define stdcall _stdcall
#endif

#ifdef OEASYDLL_EXPORT
#define OEASY_API  __declspec(dllexport)
#else
#define OEASY_API  __declspec(dllimport)
#endif

namespace OeasyMid{
	OEASY_API int stdcall Oeasy_init();

	OEASY_API void stdcall Oeasy_deinit();
}


namespace OeasyMidIPC{


OEASY_API void stdcall Oeasy_connectIPC();


}

namespace OeasyMidIntercom{


}

#endif //OEASY_MID_SDK

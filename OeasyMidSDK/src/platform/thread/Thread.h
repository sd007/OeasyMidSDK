
#ifndef __OEASYMID_PLATFORM_THREAD__
#define __OEASYMID_PLATFORM_THREAD__
#include "BaseType.h"
#include "log4cpp.h"
namespace OeasyMid
{
	typedef enum{
		THREADSTATE_NONE,
		THREADSTATE_CREATE,
		THREADSTATE_RUN,
		THREADSTATE_QUIT,
	}THREAD_STATE;
	class CThread
	{
	public:
		CThread();
		~CThread();
	public:
		OEASY_S32 CreateThread(OEASY_S32 isDetached, OEASY_S32 isSopeInSystem, OEASY_S32 stackSize, THREADFUNC func, OEASY_VOID *context);
		OEASY_VOID ExitThead(OEASY_DWORD dwMilliseconds, OEASY_S32 code);
		OEASY_BOOL IsThreadRun();
	protected:
		OEASY_S32	 waitForSingleObject(OEASY_DWORD dwMilliseconds);
		OEASY_BOOL closeHandle();
		OEASY_BOOL terminateThread(OEASY_S32 code);
		OEASY_THREADMETHOD_DEC(threadRun, lpParam);
	protected:
		OEASY_THREAD			m_hThread;
		OEASY_THREADID		m_hThreadID;
		THREAD_STATE		m_eRelease;
		THREADFUNC			m_pThreadFunc;
		OEASY_VOID			*m_pContext;
	};
};

#endif //__OEASYMID_PLATFORM_THREAD__



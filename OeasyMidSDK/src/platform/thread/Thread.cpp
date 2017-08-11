#include "Thread.h"

namespace OeasyMid
{
	CThread::CThread()
#ifndef _WP8
		: m_hThread(0)
#else
		: m_hThread()
#endif
		, m_hThreadID(0)
		, m_eRelease(THREADSTATE_NONE)
		, m_pContext(NULL)
		, m_pThreadFunc(NULL)
	{
	}

	CThread::~CThread()
	{
	}

	OEASY_S32 CThread::CreateThread(OEASY_S32 isDetached, OEASY_S32 isScopeInSystem, OEASY_S32 stackSize, THREADFUNC func, OEASY_VOID *context)
	{
		m_pContext = context;
		m_pThreadFunc = func;
		if (m_pThreadFunc == NULL)
		{
			OEASYLOG_E("CreateThread failed: m_pThreadFunc is NULL!!");
			return OEASY_FALSE;
		}
#ifndef _WIN32
		int ret = 0;
		pthread_attr_t attr;
		ret = pthread_attr_init( &attr);
		if( ret != 0 )
		{
			//DEBUG_API_ERROR(COMMON_LIB, "pthread_attr_init()");
		}
		if( isDetached )
		{
			ret = pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED);
			if( ret != 0 )
			{
				//DEBUG_API_ERROR(COMMON_LIB, "pthread_attr_setdetachstate()");
			}
		}
		if( isScopeInSystem )
		{
			ret = pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );
			if( ret != 0 )
			{
				//DEBUG_API_ERROR(COMMON_LIB, "pthread_attr_setscope()");
			}
		}
		/*
		ret = pthread_attr_setstacksize( &attr, stackSize );
		if( ret != 0 )
		{
			//DEBUG_API_ERROR(COMMON_LIB, "pthread_attr_setstacksize()");
		}
		*/
		m_eRelease = THREADSTATE_CREATE;
		if ( pthread_create( &m_hThread, &attr, &threadRun, this) != 0 )
		{
			pthread_attr_destroy( &attr);
		}
#else

#ifdef _WP8
		m_eRelease = THREADSTATE_CREATE;
		OEASY_THREAD t(threadRun, this);
		m_hThread.swap(t);
		m_hThread.detach();
#else
		m_eRelease = THREADSTATE_CREATE;
		m_hThread = ::CreateThread(NULL, NULL, threadRun, this, NULL, &m_hThreadID);
#endif
#endif
		SLEEP(10);//释放对cpu的占用
		return 0;
	}

	OEASY_VOID CThread::ExitThead(OEASY_DWORD dwMilliseconds, OEASY_S32 code)
	{
		if (waitForSingleObject(dwMilliseconds) == WAIT_TIMEOUT)
		{
			OEASYLOG_E("ExitThead failed! The error is WAIT_TIMEOUT!");
			if (IsThreadRun())
			{
				terminateThread(code);
			}
			closeHandle();
		}
		m_eRelease = THREADSTATE_NONE;
	}

	OEASY_S32	 CThread::waitForSingleObject(OEASY_DWORD dwMilliseconds)
	{
#ifdef _WIN32
#ifndef _WP8
		return ::WaitForSingleObject(m_hThread, dwMilliseconds);
#else
		while (m_eRelease != THREADSTATE_NONE)
		{
			if (m_eRelease == THREADSTATE_QUIT)
			{
				return 0;
			}
			SLEEP(10);
		}
		return 0;
#endif
#else
		while (m_eRelease != THREADSTATE_NONE)
		{
			if (m_eRelease == THREADSTATE_QUIT)
			{
				return 0;
			}
			SLEEP(10);
		}
		return 0;
#endif
	}

	OEASY_BOOL CThread::closeHandle()
	{
#ifdef _WIN32
#ifndef _WP8
		CloseHandle(m_hThread);
		m_hThread = NULL;
		return OEASY_TRUE;
#else
		return OEASY_TRUE;
#endif
#else
		m_hThread = NULL;
		return OEASY_TRUE;
#endif		
	}

	OEASY_BOOL CThread::terminateThread(OEASY_S32 code)
	{
#ifdef _WIN32
#ifndef _WP8
		return (OEASY_BOOL)::TerminateThread(m_hThread, 0);
#else
		return OEASY_TRUE;
#endif
#else
		return OEASY_TRUE;
#endif	
	}

	OEASY_BOOL CThread::IsThreadRun()
	{
		return (OEASY_BOOL)(m_eRelease != THREADSTATE_NONE && m_eRelease != THREADSTATE_QUIT);
	}

	OEASY_THREADMETHOD(CThread::threadRun, lpParam)
	{
		CThread* pThis = static_cast<CThread*>(lpParam);
		pThis->m_eRelease = THREADSTATE_RUN;
		pThis->m_pThreadFunc(pThis->m_pContext);
		pThis->m_eRelease = THREADSTATE_QUIT;
		THREAD_RETURN(0);
	}
}



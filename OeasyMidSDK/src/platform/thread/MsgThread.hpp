#ifndef __OEASYMID_PLATFORM_MSGTHREAD__
#define __OEASYMID_PLATFORM_MSGTHREAD__
#include "BaseType.h"
#include "Thread.h"
#include "AutoLock.h"
namespace OeasyMid{
	template<typename T, typename V>
	class CMsgThread
	{
	public:
		CMsgThread();
		~CMsgThread();
	public:
		OEASY_S32 CreateThread(OEASY_S32 isDetached, OEASY_S32 isScopeInSystem, OEASY_S32 stackSize, THREADFUNC func, OEASY_VOID *context);
		void DeInitThread();
		OEASY_VOID ExitThead(int timeout, int exitcode);
		OEASY_VOID ClearMsg();
		OEASY_VOID PostMsg(T& msg);
		OEASY_BOOL PeekMsg(T& msg);
		OEASY_VOID PeekEndMsg(T& msg);
		OEASY_BOOL IsThreadRun();
	private:
		V       	    m_Msg;
		MUTEX			m_Mutex;
		CThread			m_hThread;
	};

	template<typename T, typename V>
	CMsgThread<T, V>::CMsgThread()
	{
		INIT_MUTEX(&m_Mutex);
	}

	template<typename T, typename V>
	CMsgThread<T, V>::~CMsgThread()
	{
		RELEASE_MUTEX(&m_Mutex);
	}

	template<typename T, typename V>
	OEASY_BOOL CMsgThread<T, V>::IsThreadRun()
	{
		return m_hThread.IsThreadRun();
	}

	template<typename T, typename V>
	void CMsgThread<T, V>::DeInitThread()
	{
		return;
	}

	template<typename T, typename V>
	OEASY_S32 CMsgThread<T, V>::CreateThread(OEASY_S32 isDetached, OEASY_S32 isScopeInSystem, OEASY_S32 stackSize, THREADFUNC func, OEASY_VOID *context)
	{
		CAutoLock lock(m_Mutex);
		//m_Msg.clear();
		return m_hThread.CreateThread(isDetached, isScopeInSystem, stackSize, func, context);
	}

	template<typename T, typename V>
	OEASY_VOID CMsgThread<T, V>::ExitThead(int timeout, int exitcode)
	{
		m_hThread.ExitThead(timeout, exitcode);
	}

	template<typename T, typename V>
	OEASY_VOID CMsgThread<T, V>::PostMsg(T& msg)
	{
		CAutoLock lock(m_Mutex);
		m_Msg.push_back(msg);
	}

	template<typename T, typename V>
	OEASY_VOID CMsgThread<T, V>::ClearMsg()
	{
		CAutoLock lock(m_Mutex);
		m_Msg.clear();
	}

	template<typename T, typename V>
	OEASY_BOOL CMsgThread<T, V>::PeekMsg(T& msg)
	{
		CAutoLock lock(m_Mutex);
		unsigned long size = m_Msg.size();
		if (size > 0)
		{
			msg = m_Msg[0];
			m_Msg.erase(m_Msg.begin());
			return OEASY_TRUE;
		}
		return OEASY_FALSE;
	}


	template<typename T, typename V>
	OEASY_VOID CMsgThread<T, V>::PeekEndMsg(T& msg)
	{
		typename V::iterator it;
		CAutoLock lock(m_Mutex);
		for(it = m_Msg.begin(); it < m_Msg.end(); )
		{
			if (msg == *it)
			{
				msg = *it;
				it = m_Msg.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}
#endif






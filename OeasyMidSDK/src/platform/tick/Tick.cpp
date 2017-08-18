#include "Tick.h"

namespace OeasyMid
{
	OEASY_U64 CTick::s_tickCount = 0;
	CTick* CTick::s_pTick = OEASY_NULL;
	CTick::CTick()
		: m_bRun(OEASY_TRUE)
	{
		
	}

	CTick::~CTick()
	{
		
	}

	CTick* CTick::GetInstance()
	{
		if (s_pTick == NULL)
		{
			s_pTick = new CTick();
			if (!s_pTick->init())
			{
				delete s_pTick;
				s_pTick = NULL;
			}
		}
		return s_pTick;
	}

	OEASY_VOID CTick::ReleaseInstance()
	{
		if (s_pTick)
		{
			s_pTick->deInit();
			delete s_pTick;
			s_pTick = NULL;
		}
	}

	OEASY_VOID  CTick::deInit()
	{
		m_bRun = OEASY_FALSE;
		OEASYLOG_I("CTick::deInit()\n");
		m_hThread.ExitThead(500, 0);
	}

	OEASY_BOOL CTick::init()
	{
		if ( 0 != m_hThread.CreateThread(1, 1, 20 * 1024, &threadRun, this) )
		{
			OEASYLOG_E("CTick::init failed! m_hThread.CreateThread failed!");
			return OEASY_FALSE;
		}
		return OEASY_TRUE;
	}

	OEASY_U64 CTick::GetTickCount()
	{
		return s_tickCount;
	}

	void CTick::Delay(int ms)
	{
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = ms*1000;
		//select(0, NULL, NULL, NULL, &timeout);
		SLEEP(10);
	}

	OEASY_THREADMETHOD(CTick::threadRun, lpParam)
	{
		CTick* pThis = static_cast<CTick*>(lpParam);
		while (pThis->m_bRun)
		{
			pThis->Delay(10);
			++s_tickCount;
		}
		OEASYLOG_I("tick exit!!!");
		THREAD_RETURN(0);
	}
}





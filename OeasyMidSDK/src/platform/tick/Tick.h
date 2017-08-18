#ifndef __OEASY_PLATFORM_TICK__
#define __OEASY_PLATFORM_TICK__
#include "BaseType.h"
#include "Thread.h"
#include "log4cpp.h"

namespace OeasyMid{
class CTick
{
	private:
		CTick();
		~CTick();
	public:
		//GetInstance 与 ReleaseInstance 是非线程安全函数
		static CTick* GetInstance();
		static OEASY_VOID ReleaseInstance();
		
		static OEASY_U64 GetTickCount(); //单位 10ms
		static void Delay(int ms);	   //单位ms
	protected:
		OEASY_BOOL init();
		OEASY_VOID deInit();
		OEASY_THREADMETHOD_DEC(threadRun, lpParam);
	protected:
		static OEASY_U64		s_tickCount;//单位 10ms
		static CTick		*s_pTick;
		CThread		m_hThread;
		OEASY_BOOL	m_bRun;
	};

};
#endif //__OEASY_PLATFORM_TICK__


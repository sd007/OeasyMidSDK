#ifndef __OEASYMID_COMMON_HANDLEMANAGER__
#define __OEASYMID_COMMON_HANDLEMANAGER__
#include "BaseType.h"
#include "AutoLock.h"
#include "ApiImpl.h"

class CHandleManager
{
public:
	CHandleManager();
	~CHandleManager();
public:
	//非线程安全
	static void ReleaseInstance();
	static OEASY_HANDLE_STATE CheckState(OEASY_U32 handle, int *usrRight);
	static CApiImpl* GetApiManager(OEASY_U32& handle);
	static void ReleaseApiManager(OEASY_U32 handle);
	static CApiImpl* CreateApiManager(OEASY_U32& handle);
	static void DeleteApiManager(OEASY_U32 handle);
private:
	std::map<OEASY_U32, CApiImpl*>	m_hApi;

	static  OEASY_BOOL m_bInit;
	static	OEASY_U32	m_sIndexHandle;
	MUTEX	m_mMutex;
};

#endif //__OEASYMID_COMMON_HANDLEMANAGER__

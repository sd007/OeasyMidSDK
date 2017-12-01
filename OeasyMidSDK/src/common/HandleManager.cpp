#include "HandleManager.h"
#include "oeasylog.h"


static CHandleManager gHandleManager;
OEASY_U32 CHandleManager::m_sIndexHandle = 0;
OEASY_BOOL CHandleManager::m_bInit = OEASY_FALSE;
CHandleManager::CHandleManager()
{
	INIT_MUTEX(&m_mMutex);
}

CHandleManager::~CHandleManager()
{
	RELEASE_MUTEX(&m_mMutex);
}

void CHandleManager::ReleaseInstance()
{
	while(OEASY_TRUE)
	{
		CAutoLock lock(gHandleManager.m_mMutex);
		std::map<OEASY_U32, CApiImpl*>::iterator it =  gHandleManager.m_hApi.begin();
		for (; it != gHandleManager.m_hApi.end();)
		{
			CApiImpl* pApi = (CApiImpl*)it->second;
			if (pApi->GetRef() == 0 && pApi->GetARef() == 0)
			{
				gHandleManager.m_hApi.erase(it++);
				switch(pApi->getDeviceType())
				{
				case CAMERA:
					pApi->IPC_Destroy();
					break;
				default:
					OEASYLOG_I("not  destroy!");
					break;
				}
				delete pApi;
			}
		}
		if (gHandleManager.m_hApi.empty())
		{
			break;
		}
		SLEEP(10);
	}
}


CApiImpl* CHandleManager::GetApiManager(OEASY_U32& handle)
{
	CAutoLock lock(gHandleManager.m_mMutex);
	if (handle <= m_sIndexHandle && handle > 0)
	{
		std::map<OEASY_U32, CApiImpl*>::iterator it = gHandleManager.m_hApi.find(handle);
		if (it != gHandleManager.m_hApi.end())
		{
			if(it->second->GetARef() == 0) return NULL;

			it->second->Ref();
			return (CApiImpl*)(it->second);
		}
	}
	return NULL;
}

CApiImpl* CHandleManager::CreateApiManager(OEASY_U32& handle)
{
	CAutoLock lock(gHandleManager.m_mMutex);
	handle = ++m_sIndexHandle;
	CApiImpl* pApi = new CApiImpl;
	gHandleManager.m_hApi[m_sIndexHandle] = pApi;
	pApi->ARef();
	return pApi;
}

void CHandleManager::DeleteApiManager(OEASY_U32 handle)
{
	CAutoLock lock(gHandleManager.m_mMutex);
	std::map<OEASY_U32, CApiImpl*>::iterator it = gHandleManager.m_hApi.find(handle);
	if (it != gHandleManager.m_hApi.end())
	{
		CApiImpl* pApi = (CApiImpl*)it->second;
		if ( pApi->UnARef() == 0 && pApi->GetRef() == 0)
		{
			gHandleManager.m_hApi.erase(it);
			switch(pApi->getDeviceType())
			{
			case CAMERA:
				pApi->IPC_Destroy();
				break;
			default:
				OEASYLOG_I("not  destroy!");
				break;
			}
			delete pApi;
			pApi = NULL;
		}else if (pApi->GetARef() == 0){
			switch(pApi->getDeviceType())
			{
			case CAMERA:
				pApi->IPC_Destroy();
				break;
			default:
				OEASYLOG_I("not  destroy!");
				break;
			}
		}
	}
}

void CHandleManager::ReleaseApiManager(OEASY_U32 handle)
{
	CAutoLock lock(gHandleManager.m_mMutex);
	std::map<OEASY_U32, CApiImpl*>::iterator it = gHandleManager.m_hApi.find(handle);
	if (it != gHandleManager.m_hApi.end())
	{
		CApiImpl* pApi = (CApiImpl*)it->second;
		if ( pApi->UnRef() == 0 && pApi->GetARef() == 0)
		{
			gHandleManager.m_hApi.erase(it);
			switch(pApi->getDeviceType())
			{
			case CAMERA:
				pApi->IPC_Destroy();
				break;
			default:
				OEASYLOG_I("not  destroy!");
				break;
			}
			delete pApi;
			pApi = NULL;
		}			
	}
}



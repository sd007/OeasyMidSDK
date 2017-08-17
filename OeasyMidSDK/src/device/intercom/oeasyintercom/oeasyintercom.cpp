#include "oeasyintercom.h"
#include "log4cpp.h"

OeasyIntercom::OeasyIntercom():m_serverPort(0)
{
	m_serverSock = new OeasySocket();
	m_bdataThreadRun = OEASY_FALSE;
	for(int i =0 ;i < Max_Count_Intercom; i++)
	{
		m_bdataSockUse[i] = OEASY_FALSE;
	}
}

OeasyIntercom::~OeasyIntercom()
{
	if (m_serverSock)
	{
		m_serverSock->CloseSocket();
		delete m_serverSock;
		m_serverSock = NULL;
	}
}

_INTERCOMTYPE OeasyIntercom::getIntercomType()
{
	return _OEASYLOG_INTERCOM;
}


OEASY_THREADMETHOD(OeasyIntercom::ThreadServer, param)
{
	OeasyIntercom* pThis = (OeasyIntercom*)param;
	pThis->ThreadServerWork();
	return 0;
}

OEASY_THREADMETHOD(OeasyIntercom::ThreadData, param)
{
	OeasyIntercom* pThis = (OeasyIntercom*)param;
	pThis->ThreadDataWork();
	return 0;
}

void OeasyIntercom::Intercom_startServer( OEASY_U16 serverport )
{
	m_serverPort = serverport;
	m_hServerThread.CreateThread(1, 1, 1024 * 1024, ThreadServer, this);
}

void OeasyIntercom::Intercom_stopServer()
{
	TMSG msga;
	msga.message = WM_QUIT;
	m_hServerThread.PostMsg(msga);
	m_hDataThread.PostMsg(msga);
}


void* OeasyIntercom::ThreadServerWork()
{
	TMSG msg;
	OEASY_DWORD heart = 0;
	char recvBuffer[512] = {0};
Reconnect:
	int serverSockfd = m_serverSock->CreateSocket(AF_INET, SOCK_STREAM);
	OEASYLOG_I("ThreadServerWork  will bind port: %d", m_serverPort);
	int ret = m_serverSock->Bind(m_serverPort);
	if (ret < 0)
	{
		OEASYLOG_E("ThreadServerWork bind %d error!");
		while(m_hServerThread.PeekMsg(msg))
		{
			switch(msg.message)
			{
			case WM_QUIT:
				goto QUIT;
			default:
				break;
			}
		}
		SLEEP(5000);
		goto Reconnect;
	}
	m_serverSock->Listen();
	//服务端尽可能使用SO_REUSEADDR,在绑定之前尽可能调用setsockopt来设置SO_REUSEADDR套接字选项。该选项可以使得server不必等待TIME_WAIT状态消失就可以重启服务器
	int on = 1;  
	m_serverSock->Setsockopt(m_serverSock->Getsocket(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	m_serverSock->SetNonBlock(true);

	while(1)
	{
		while(m_hServerThread.PeekMsg(msg))
		{
			switch(msg.message)
			{
			case WM_QUIT:
				goto QUIT;
			default:
				break;
			}
		}

		fd_set sockSet;
		FD_ZERO(&sockSet);
		FD_SET(m_serverSock->Getsocket(), &sockSet);
		struct timeval socktimeout;
		socktimeout.tv_sec=0;            
		socktimeout.tv_usec=50000;
		while(OeasySocket::Select(m_serverSock->Getsocket()+1, &sockSet, NULL, NULL, &socktimeout) > 0){
			if (FD_ISSET(m_serverSock->Getsocket(), &sockSet))
			{
				OEASY_BOOL bgetSock= OEASY_FALSE;
				for (int i =0; i< Max_Count_Intercom; i++)
				{
					bool bok = m_serverSock->Accept(&m_dataSock[i]);
					if (bok)
					{	
						m_bdataSockUse[i] = OEASY_TRUE;
						bgetSock = OEASY_TRUE;
						OEASYLOG_I("ThreadServerWork: m_serverSock Accept datesock fd = %d,", m_dataSock[i].Getsocket());
						m_dataSock[i].SetNonBlock();
						//请求intercom MAC地址
					}
				}
				if (!bgetSock)
				{
					OEASYLOG_W("sorry, can not get thread, no resourse for new connection!");
					break;
				}
				if (!m_bdataThreadRun)
				{
					//启动收发数据的线程
					m_hDataThread.CreateThread(1, 1, 2*1024 * 1024, ThreadData, this);	
					OEASYLOG_I("ThreadServerWork: will start data thread!");
				}	
				break;
			}
		}
		Sleep(50);
	}
	OEASYLOG_W("ThreadServerWork will Quit!");
	m_serverSock->CloseSocket();
	return NULL;
QUIT:
	OEASYLOG_W("ThreadServerWork goto Quit!");
	m_serverSock->CloseSocket();
	return NULL;
}

#define RECV_SIZE 1024
void* OeasyIntercom::ThreadDataWork()
{
	TMSG msg;
	OEASY_BOOL bReconnet = OEASY_FALSE;
	OEASY_CHAR recvBuffer[RECV_SIZE] = {0};
	while(1)
	{
		while(m_hDataThread.PeekMsg(msg))
		{
			switch(msg.message)
			{
			case WM_QUIT:
				goto QUIT;
			default:
				break;
			}
		}
		int maxSocketfd = 0;
		fd_set sockSet;
		FD_ZERO(&sockSet);
		for(int i =0 ;i < Max_Count_Intercom && m_bdataSockUse[i]; i++)
		{
			FD_SET(m_dataSock[i].Getsocket(), &sockSet);
			maxSocketfd = (maxSocketfd > m_dataSock[i].Getsocket())? maxSocketfd : (maxSocketfd = m_dataSock[i].Getsocket());
		}
		struct timeval socktimeout;
		socktimeout.tv_sec=0;            
		socktimeout.tv_usec=50000;
		while(OeasySocket::Select(maxSocketfd+1, &sockSet, NULL, NULL, &socktimeout) > 0)
		{
			for (int i =0 ;i < Max_Count_Intercom && m_bdataSockUse[i]; i++)
			{
				if(FD_ISSET( m_dataSock[i].Getsocket(), &sockSet))
				{
					int ret = SOCK_RECV(m_dataSock[i].Getsocket(), (char*)recvBuffer, RECV_SIZE, 0);
					if (ret > 0)
					{
						
					}
				}
			}
		}	
	}
	OEASYLOG_I("ThreadDataWork will QUIT!");
	m_bdataThreadRun = OEASY_FALSE;
	return NULL;
QUIT:
	m_bdataThreadRun = OEASY_FALSE;
	OEASYLOG_I("ThreadDataWork get QUIT!");
	return NULL;
}

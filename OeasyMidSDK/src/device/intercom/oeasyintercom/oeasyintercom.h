#ifndef OEASYLOG_INTERCOM_H
#define OEASYLOG_INTERCOM_H
#include "intercomdefine.h"
#include "iintercom.h"
#include "MsgThread.hpp"
#include "OeasySocket.h"

const int Max_Count_Intercom =  64; //一个端口可监听的最大数，决定开线程的最大个数

typedef struct tagTMSG
{
	int message;
	long wParam;
	long lParam;
	bool operator == (struct tagTMSG msg) const
	{
		return msg.message == message;
	}
}TMSG, *PTMSG;

typedef std::vector<TMSG>	VT_TMSG;

class OeasyIntercom : public IItercom
{
public:
	OeasyIntercom();
	virtual ~OeasyIntercom();

	virtual _INTERCOMTYPE getIntercomType();

	virtual void Intercom_startServer( OEASY_U16 serverport );

	virtual void Intercom_stopServer();

	virtual void* ThreadServerWork();
	OEASY_THREADMETHOD_DEC(ThreadServer, param);

	virtual void* ThreadDataWork();
	OEASY_THREADMETHOD_DEC(ThreadData, param);

private:
	CMsgThread<TMSG, VT_TMSG>	m_hServerThread;
	CMsgThread<TMSG, VT_TMSG>	m_hDataThread;
	OEASY_U16 m_serverPort;
	OeasySocket *m_serverSock;
	OeasySocket m_dataSock[Max_Count_Intercom];
	OEASY_BOOL m_bdataSockUse[Max_Count_Intercom];
	OEASY_BOOL m_bdataThreadRun;
};

#endif //OEASYLOG_INTERCOM_H
#ifndef OEASY_SOKET
#define OEASY_SOKET

#include "BaseType.h"

class OeasySocket{
public:
	OeasySocket();
	~OeasySocket();
	int Getsocket();
	int CreateSocket(int af, int type, int protocol = 0);
	int Bind(unsigned short port);
	int Listen(int backlog = 128);
	static int Setsockopt(int sockfd, int level, int optname,  const void *optval, int optlen); 
	bool CloseSocket(int socket = 0);

	bool Connect(char* ip, unsigned short port);
	//acceptSocket为accept返回的数据socket
	bool Accept(OeasySocket* acceptSocket);

	static int Writen(int sockfd, char* buf, int len);
	static int Readn(int sockfd, char* buf, int len);

	int recv(void *buf, size_t n, int flags, struct sockaddr_in* addr);

	static int Select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, struct timeval* timeout);
	bool SetNonBlock(bool nonBlock = true);
private:
	void setinnerSocket(int socket);
private:
	int m_socket;
	short m_bindPort;
};

#endif


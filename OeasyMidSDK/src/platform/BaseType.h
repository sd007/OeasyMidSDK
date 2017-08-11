#ifndef __OEASYMIDSDK_PLATFORM_BASETYPE__
#define __OEASYMIDSDK_PLATFORM_BASETYPE__

//#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
//#endif

#ifdef _WIN32
#include <windows.h>
//#include <ws2def.h>
#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

#ifndef _WP8
#include <IPTypes.h>
#include <IPHlpApi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#else
#include <thread>
#include <mutex>
#endif

#else
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#ifdef ANDROID //Android NDK下的包含路径不一样
#include <fcntl.h>
#else
#include <sys/fcntl.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#endif
#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <string.h>
#ifndef WM_QUIT
#define WM_QUIT		0x0012

#endif

namespace OeasyMid
{
	/*基本类型*/
	typedef unsigned char           OEASY_U8;
	typedef unsigned short          OEASY_U16;
	typedef unsigned int            OEASY_U32;

	typedef signed char             OEASY_S8;
	typedef short                   OEASY_S16;
	typedef int                     OEASY_S32;
	typedef char                    OEASY_CHAR;
	
	typedef unsigned long			OEASY_DWORD;
	typedef long					OEASY_SDWORD;
	typedef void					OEASY_VOID;

	typedef enum {
		OEASY_FALSE = 0,
		OEASY_TRUE  = 1,
	} OEASY_BOOL;

#ifndef _M_IX86
	typedef unsigned long long		OEASY_U64;
	typedef long long				OEASY_S64;
#else
	typedef unsigned __int64		OEASY_U64;
	typedef __int64					OEASY_S64;
#endif

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#ifndef NULL
#define NULL			0L
#endif
#define OEASY_NULL		0L
#define OEASY_SUCCESS		0
#define OEASY_FAILURE		(-1)
	
#define OEASY_SAFACE_FREE(obj)	if(obj){free(obj);obj=NULL;}
#define OEASY_SAFACE_DELETE(obj)	if(obj){delete obj;obj=NULL;}
#define OEASY_SAFACE_DELETE_ARRAY(obj)	if(obj){delete []obj;obj=NULL;}

#define OEASY_RETURN_VOID
#define _OEASY_USEASSERT	0
#define _OEASY_USE_ASSERTASIF	1
#if _OEASY_USEASSERT
#define OEASY_ASSERT(a, exec, ret)	assert(a)
#elif _OEASY_USE_ASSERTASIF
#define OEASY_ASSERT(a, exec, ret)	if(!(a))\
									{\
										exec;\
										return ret;\
									} 
#else
#define OEASY_ASSERT(a, exec, ret)
#endif

#ifdef _WIN32
#ifndef _WP8

	/*线程相关*/
	typedef OEASY_DWORD ( __stdcall *THREADFUNC )(void* lpThreadParameter);
	typedef HANDLE				OEASY_THREAD;
	typedef DWORD				OEASY_THREADID;
#define THREAD_RETURN(x)		return (x)
#define OEASY_THREADMETHOD(func, lpParam)	DWORD WINAPI func(void* lpParam)
#define OEASY_THREADMETHOD_DEC(func, lpParam)	static DWORD WINAPI func(void* lpParam)
#define SLEEP(t)				Sleep(t)

	/*锁相关*/
	typedef CRITICAL_SECTION		MUTEX;
#define INIT_MUTEX(x)				InitializeCriticalSection(x)
#define RELEASE_MUTEX(x)			DeleteCriticalSection(x)
#define TRYLOCK_MUTEX(x)			(!TryEnterCriticalSection(x))
#define LOCK_MUTEX(x)				EnterCriticalSection(x)
#define UNLOCK_MUTEX(x)				LeaveCriticalSection(x)
#else

	/*线程相关*/
	typedef OEASY_DWORD ( __stdcall *THREADFUNC )(void* lpThreadParameter);
	typedef std::thread				OEASY_THREAD;
	typedef DWORD					OEASY_THREADID;
#define THREAD_RETURN(x)		return (x)
#define OEASY_THREADMETHOD(func, lpParam)	DWORD WINAPI func(void* lpParam)
#define OEASY_THREADMETHOD_DEC(func, lpParam)	static DWORD WINAPI func(void* lpParam)
#define SLEEP(t)				std::this_thread::sleep_for(std::chrono::milliseconds(10));

/*锁相关*/
	typedef std::mutex		MUTEX;
#define INIT_MUTEX(x)				
#define RELEASE_MUTEX(x)			
#define TRYLOCK_MUTEX(x)			(*x).try_lock()
#define LOCK_MUTEX(x)				(*x).lock()
#define UNLOCK_MUTEX(x)				(*x).unlock()
#endif

/*文件相关*/
#define OEASY_FILE									FILE
#define OEASY_FILE_OPEN(name, mode)					fopen(name, mode)
#define OEASY_FILE_CLOSE(fp)							fclose(fp)
#define OEASY_FILE_WRITE(buf, size, n, fp)			fwrite(buf, size, n, fp)
#define OEASY_FILE_READ(buf, size, n, fp)				fread(buf, size, n, fp)
#define OEASY_FILE_SEEK(fp, offset, origin)			fseek(fp, offset, origin)
#define OEASY_FILE_TELL(fp)							ftell(fp)
#define OEASY_FILE_GETERR()							GetLastError();

/*socket相关*/
	typedef	SOCKET									OEASY_SOCKET;
	typedef	int										OEASY_SOCKLEN;
//socket 初始化与反初始化,整个SDK只调用一次
#define SOCK_INIT		WSADATA data;\
							WSAStartup(MAKEWORD(2, 2), &data);
#define SOCK_DEINIT		WSACleanup();
#define SOCK_IOCTL(x, y, z)						::ioctlsocket(x, y, z)
#define SOCK_CLOSE(x)							::closesocket(x)
#define SOCK_CREATE(af, type, protocol)			::socket(af, type, protocol)
#define SOCK_LISTEN(s, backlog)					::listen(s, backlog)
#define SOCK_BIND(s, addr, namelen)				::bind(s, addr, namelen)
#define SOCK_ACCEPT(s, addr, addrlen)			::accept(s, addr, addrlen)
#define SOCK_CONNECT(s, name, namelen)			::connect(s, name, namelen)
#define SOCK_SEND(s, buf, len, flags)			::send(s, buf, len, flags)
#define SOCK_RECV(s, buf, len, flags)			::recv(s, buf, len, flags)
#define SOCK_SENDTO(s, buf, len, flags, to, tolen)		::sendto(s, buf, len, flags, to, tolen)
#define SOCK_RECVFROM(s, buf, len, flags, from, fromlen)		::recvfrom(s, buf, len, flags, from, fromlen)
#define SOCK_SELECT(nfds, readfds, writefds, exceptfds, timeout)	::select(nfds, readfds, writefds, exceptfds, timeout)
#define SOCK_SETOPT(s, level, optname, optval, optlen)	::setsockopt(s, level, optname, optval, optlen)
#define SOCK_GETOPT(s, level, optname, optval, optlen)	::getsockopt(s, level, optname, optval, optlen)
#define OEASY_HTONL(hostlong)		::htonl(hostlong)
#define OEASY_NTOHL(hostlong)		::ntohl(netlong)
#define OEASY_HTONS(hostshort)	::htons(hostshort)
#define OEASY_NTOHS(hostshort)	::ntohs(netshort)
#define OEASYLOG_INET_ADDR(cp)		inet_addr(cp)
#define OEASYLOG_INADDR_NONE			INADDR_NONE
#else	/* linux or mac */
#define stricmp strcasecmp
/*线程相关*/
	typedef void* (*THREADFUNC)(void*);
	typedef pthread_t			OEASY_THREAD;
	typedef void*				OEASY_THREADID;
#define	THREAD_FUNC_TO			void *(*)(void *)
#define WAIT_TIMEOUT			0
#define THREAD_RETURN(x)		return NULL
#define THREADMETHOD(func, lpParam)	void* func(void* lpParam)
#define THREADMETHOD_DEC(func, lpParam)	static void* func(void* lpParam)
#define SLEEP(t)				usleep(t*1000)

/*锁相关*/
	typedef pthread_mutex_t			MUTEX;
#define INIT_MUTEX(x)				pthread_mutex_init(x, NULL)
#define RELEASE_MUTEX(x)			pthread_mutex_destroy(x)
#define TRYLOCK_MUTEX(x)			pthread_mutex_trylock(x)
#define LOCK_MUTEX(x)				pthread_mutex_lock(x)
#define UNLOCK_MUTEX(x)				pthread_mutex_unlock(x)

/*文件相关*/
#define OEASY_FILE									FILE
#define OEASY_FILE_OPEN(name, mode)					fopen(name, mode)
#define OEASY_FILE_CLOSE(fp)							fclose(fp)
#define OEASY_FILE_WRITE(buf, size, n, fp)			fwrite(buf, size, n, fp)
#define OEASY_FILE_READ(buf, size, n, fp)				fread(buf, size, n, fp)
#define OEASY_FILE_SEEK(fp, offset, origin)			fseek(fp, offset, origin)
#define OEASY_FILE_TELL(fp)							ftell(fp)
#define OEASY_FILE_GETERR()							errno;

/*socket相关*/
	typedef	int										OEASY_SOCKET;
	typedef	socklen_t								OEASY_SOCKLEN;
#define SOCK_INIT
#define SOCK_DEINIT
#define SOCK_IOCTL(x, y, z)						::fcntl((x), (y), (z))
#define SOCK_CLOSE(x)							::close((x))
#define SOCK_CREATE(af, type, protocol)			::socket(af, type, protocol)
#define SOCK_LISTEN(s, backlog)					::listen(s, backlog)
#define SOCK_BIND(s, addr, namelen)				::bind(s, addr, namelen)
#define SOCK_ACCEPT(s, addr, addrlen)			::accept(s, addr, addrlen)
#define SOCK_CONNECT(s, name, namelen)			::connect(s, name, namelen)
#define SOCK_SEND(s, buf, len, flags)			::send(s, buf, len, flags)
#define SOCK_RECV(s, buf, len, flags)			::recv(s, buf, len, flags)
#define SOCK_SENDTO(s, buf, len, flags, to, tolen)		::sendto(s, buf, len, flags, to, tolen)
#define SOCK_RECVFROM(s, buf, len, flags, from, fromlen)				::recvfrom(s, buf, len, flags, from, fromlen)
#define SOCK_SELECT(nfds, readfds, writefds, exceptfds, timeout)	::select(nfds, readfds, writefds, exceptfds, timeout)
#define SOCK_SETOPT(s, level, optname, optval, optlen)	::setsockopt(s, level, optname, optval, optlen)
#define SOCK_GETOPT(s, level, optname, optval, optlen)	::getsockopt(s, level, optname, optval, optlen)
#define OEASY_HTONL(hostlong)		::htonl(hostlong)
#define OEASY_NTOHL(hostlong)		::ntohl(netlong)
#define OEASY_HTONS(hostshort)	::htons(hostshort)
#define OEASY_NTOHS(hostshort)	::ntohs(netshort)
#define OEASYLOG_INET_ADDR(cp)		inet_addr(cp)
#define OEASYLOG_INADDR_NONE			INADDR_NONE
#endif
};
#endif



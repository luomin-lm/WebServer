#ifndef _CLM_SOCK_TCP_H
#define _CLM_SOCK_TCP_H


/********************* Windows与Linux兼容 *******************************/
#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

#define socklen_t	int
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define BOOL				int
#define SOCKET				int
#define TRUE				1
#define FALSE				0
#define INVALID_SOCKET		-1
#define VOID				void
#define UINT				unsigned int
#define SOCKET_ERROR		-1
#define TCHAR				char
#define TEXT(str)			str
#endif

class CTcpSock
{
public:
	CTcpSock();
	~CTcpSock();

#ifdef _WIN32
	// 初始化，安装
	static BOOL SockInit(int v1, int v2);
	// 卸载
	static BOOL SockUninit();
#endif

public:
	int Write(const SOCKET& clnt_sock, const void* buffer, const int& bufSize);
	int Read(const SOCKET& clnt_sock, void* buffer, const int& maxBufSize);
	void Close(const SOCKET& sock);
	BOOL Create(SOCKET& sock);
};






#endif


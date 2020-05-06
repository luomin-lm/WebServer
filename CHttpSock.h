#ifndef _CLM_SOCK_HTTP_H
#define _CLM_SOCK_HTTP_H

#include "CTcpSock.h"

typedef struct __InetInfo{
	SOCKET m_socket;
	sockaddr_in sin;
}inetInfo, *pInetInfo;

class CHttpSock : public CTcpSock
{
public:
	CHttpSock(const int& port = 8080);

public:
	inetInfo Wait();

private:
	SOCKET m_socket;
};





#endif


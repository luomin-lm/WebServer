#include "CTcpSock.h"

CTcpSock::CTcpSock()
{
}

CTcpSock::~CTcpSock()
{
}

#ifdef _WIN32
BOOL CTcpSock::SockInit(int v1, int v2)
{
	WSAData wd;
	if (0 != WSAStartup(MAKEWORD(v1, v2), &wd)){
		return FALSE;
	}

	return TRUE;
}

BOOL CTcpSock::SockUninit()
{
	return WSACleanup();
}
#endif

int CTcpSock::Write(const SOCKET& clnt_sock, const void* buffer, const int& bufSize)
{
	int nread = 0;
#ifdef _WIN32
	nread = send(clnt_sock, (char*)buffer, bufSize, 0);
#else
	nread = write(clnt_sock, (char*)buffer, bufSize);
#endif
	return nread;
}

int CTcpSock::Read(const SOCKET& clnt_sock, void* buffer, const int& maxBufSize)
{
	int nread = 0;
#ifdef _WIN32
	nread = recv(clnt_sock, (char*)buffer, maxBufSize, 0);
#else
	nread = read(clnt_sock, (char*)buffer, maxBufSize);
#endif
	return nread;
}

void CTcpSock::Close(const SOCKET& sock)
{
#ifdef _WIN32
	closesocket(sock);
#else
	close(sock);
#endif
}

BOOL CTcpSock::Create(SOCKET& sock)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock){
		return FALSE;
	}

	return TRUE;
}









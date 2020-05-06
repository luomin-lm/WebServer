#include "CHttpSock.h"

CHttpSock::CHttpSock(const int& port)
{
	if (FALSE == Create(m_socket)){
		throw "Error reason: create socket fail!";
	}

#ifndef _WIN32
	int opt = 1;
	// 设置套接字为程序终止立马释放端口资源
	setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

	struct sockaddr_in sin = { 0 };
	sin.sin_family = PF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

	if (SOCKET_ERROR == bind(m_socket, (struct sockaddr*)&sin, sizeof(sin))){
		throw "Error reason: bind port fail!";
	}
	if (SOCKET_ERROR == listen(m_socket, 5)){
		throw "Error reason: bind port fail!";
	}
}

inetInfo CHttpSock::Wait()
{
	inetInfo info;
	socklen_t len = sizeof(info.sin);

	while (1){
		info.m_socket = accept(m_socket, (struct sockaddr*)&info.sin, &len);
		if (INVALID_SOCKET != info.m_socket){
			return info;
		}
	}

	return info;
}



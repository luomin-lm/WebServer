#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CHttpHandle.h"

#define DEFAULT_PORT	8080

CHttpSock* Init(const int& port)
{
	CHttpSock* pSock;
#ifdef _WIN32
	CHttpSock::SockInit(2, 2);
#endif

	try{
		pSock = new CHttpSock(port);
	}
	catch (const char* str){
		printf("%s\r\n", str);
		system("pause");
		exit(-1);
	}
	return pSock;
}
void UnInit(CHttpSock *pSock)
{
	delete pSock;
#ifdef _WIN32
	pSock->SockUninit();
#endif
}

int main(int argc, char* argv[])
{
	CHttpSock *pSock = NULL;
	CHttpHandle *pHdl = NULL;
    int port = DEFAULT_PORT;
    
    if(argc == 2){
        port = atoi(argv[1]);
    }
	pSock = Init(port);
	printf("\r\n\t\tWebServer Init Success , Wait Connection ....\r\n\r\n");
	while (1){
		pHdl = new CHttpHandle(pSock->Wait());
		pHdl->RunHandleThread();
	}
	
	UnInit(pSock);
	return 0;
}








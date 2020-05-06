#include "CHttpHandle.h"
#include <stdio.h>

// 使用正则表达式
#ifdef _WIN32
#include <regex>
#else
#include <regex.h>
#endif

CHttpHandle::CHttpHandle(const inetInfo& info)
{
	m_info = info;
	memset(m_printStr, 0, 256);
}

#ifdef _WIN32
DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
	CHttpHandle* pHttpHandle = (CHttpHandle*)lpParam;
	pHttpHandle->Handle();
	delete pHttpHandle;

	return 0;
}
#endif

void* MyThreadFunc(void* arg)
{
	CHttpHandle* pHttpHandle = (CHttpHandle*)arg;
	pHttpHandle->Handle();
	delete pHttpHandle;

	return NULL;
}

void CHttpHandle::RunHandleThread()
{
	memset(m_buff, 0, BUF_SIZE);
	int len = Read(m_buff, BUF_SIZE - 1);
	if (FALSE == ExecRegex(m_buff, "^[A-z]{3,7} [/|A-z|0-9|.]{1,} HTTP/1.[1|0]\r\n([A-z|-]{1,}: [^\r]{1,}\r\n){1,}[^/r]{1,}")){
		return ;
	}

#ifdef _WIN32
	CreateThread(NULL, 0, ThreadFunc, this, 0, NULL);
#else
	pthread_t id;
	pthread_create(&id, NULL, MyThreadFunc, this);
	pthread_detach(id);
#endif
}

void CHttpHandle::Handle()
{
	char mothed[10] = { 0 };
	char url[256] = { 0 };
	int i, len, j;

	len = strlen(m_buff);
	
	for (i = 0; i < 9; i++){
		if (m_buff[i] == ' '){
			break;
		}
		mothed[i] = m_buff[i];
	}
	++i;
	for (j = 0; j < 256; j++){
		if (m_buff[i + j] == ' '){
			break;
		}
		url[j] = m_buff[i + j];
	}
		
#ifdef PRINT_MOTHED
	#ifdef PRINT_URL
		sprintf(m_printStr ,"%s\t%s\t\t", mothed, url);
	#else
		sprintf(m_printStr ,"%s\t\t", mothed);
	#endif
#else
	#ifdef PRINT_URL
		sprintf(m_printStr ,"%s\t\t", url);
	#endif
#endif
	
	this->Depatch(mothed, url, len);
}

void CHttpHandle::GetRequestResult(const char* filePath, char* result)
{
	char hz[32] = { 0 };
	int len = strlen(filePath), i = 0;

	while (filePath[--len] != '.');
	while (filePath[++len] != '\0'){
		hz[i++] = filePath[len];
	}

	if (0 == strcmp("jpg", hz)){
		strcpy(result, "image\\jpeg");
	}
	else if (0 == strcmp("png", hz)){
		strcpy(result, "image\\png");
	}
	else if (0 == strcmp("gif", hz)){
		strcpy(result, "image\\gif");
	}
	else if (0 == strcmp("html", hz)){
		strcpy(result, "text/html");
	}
	else{
		strcpy(result, "text/plain");
	}
}

void CHttpHandle::MethodGet(const char* url)
{
	char filePath[256] = { 0 };
	sprintf(filePath, "./%s", url);

	SendFile(filePath);
}
void CHttpHandle::MethodPost(const char* url, const char* postMsgStr)
{
	ns_dsa::CLMMap<std::string, std::string> postMsg;
	postStr2Map(postMsg, postMsgStr);
	MethodGet(url);
}
void CHttpHandle::MethodHead(const char* url)
{

}
void CHttpHandle::MethodPut(const char* url)
{

}
void CHttpHandle::MethodDelete(const char* url)
{

}
void CHttpHandle::MethodTrace(const char* url)
{

}
void CHttpHandle::MethodConnect(const char* url)
{

}
void CHttpHandle::MethodOptions(const char* url)
{

}
void CHttpHandle::MethodError(const char* url)
{

}

void CHttpHandle::Depatch(const char* method, char* url, const int& len)
{
	char data[1024] = { 0 };
	if (0 == strcmp(method, "GET")){
		this->MethodGet(url);
	}
	else if (0 == strcmp(method, "POST")){
		ExecRegex(m_buff, "\r\n\r\n[^\r]{1,}", data);
		this->MethodPost(url, data + 4);
	}
	else if (0 == strcmp(method, "CONNECT")){
		this->MethodConnect(url);
	}
	else if (0 == strcmp(method, "DELETE")){
		this->MethodDelete(url);
	}
	else if (0 == strcmp(method, "HEAD")){
		this->MethodHead(url);
	}
	else if (0 == strcmp(method, "OPTIONS")){
		this->MethodOptions(url);
	}
	else if (0 == strcmp(method, "PUT")){
		this->MethodPut(url);
	}
	else if (0 == strcmp(method, "TRACE")){
		this->MethodTrace(url);
	}
	else{
		this->MethodError(url);
	}
}

int CHttpHandle::Write(const void* buffer, const int& bufSize)
{
	return m_sock.Write(m_info.m_socket, buffer, bufSize);
}

int CHttpHandle::Read(void* buffer, const int& maxBufSize)
{
	return m_sock.Read(m_info.m_socket, buffer, maxBufSize);
}

void CHttpHandle::Close()
{
	m_sock.Close(m_info.m_socket);
}

CHttpHandle::~CHttpHandle()
{
	Close();
	printf("%s\n", m_printStr);
#ifdef PRINT_DATA
	printf("%s\n\n", m_buff);
#endif
}

#ifdef _WIN32
BOOL CHttpHandle::ExecRegex(const char* src, const char *patternStr, char *result)
{
	int i = 0, j = 0;
	std::string s = src;
	std::smatch res;
	std::regex pattern(patternStr);// 正则字段

	//迭代器声明
	std::string::const_iterator iterStart = s.begin();
	std::string::const_iterator iterEnd = s.end();
	std::string temp;
	if (regex_search(iterStart, iterEnd, res, pattern)){
		temp = res[0];  // 获取到信息
	}
	else{
		return FALSE;
	}
	if (result != NULL){
		strcpy_s(result, BUF_SIZE, temp.c_str());
	}
	return TRUE;
}
#else
BOOL CHttpHandle::ExecRegex(const char* src, const char *patternStr, char *result)
{
	int len;
	regmatch_t pmatch;
	regex_t reg;

	regcomp(&reg, patternStr, REG_EXTENDED);	//编译正则表达式
	int status = regexec(&reg, src, 1, &pmatch, 0);
	/* 匹配正则表达式，注意regexec()函数一次只能匹配一个，不能连续匹配，网上很多示例并没有说明这一点 */
	if (status == REG_NOMATCH){
		return 0;
	}
	else if (pmatch.rm_so != -1){
		len = pmatch.rm_eo - pmatch.rm_so;  // 匹配长度
		if (NULL != result){
			strncpy(result, src + pmatch.rm_so, len);
		}
		return TRUE;
	}

	return FALSE;
}
#endif

void CHttpHandle::postStr2Map(ns_dsa::CLMMap<std::string, std::string>& map, const char* postStr)
{
	char name[36], value[36];
	int i = 0, j = 0;

	while (postStr[i] != '\0'){
		j = 0;
		memset(name, 0, 36);
		memset(value, 0, 36);

		for (j = 0; j < 35; j++){
			if (postStr[i] == '=' || postStr[i] == '&' || postStr[i] == '\0'){
				if (postStr[i] != '\0'){
					++i;
				}
				break;
			}
			name[j] = postStr[i++];
		}
		for (j = 0; j < 35; j++){
			if (postStr[i] == '=' || postStr[i] == '&' || postStr[i] == '\0'){
				if (postStr[i] != '\0'){
					++i;
				}
				break;
			}
			value[j] = postStr[i++];
		}
		map.PushFront(name, value);
	}
}

int CHttpHandle::SendFile(const std::string& filePath)
{
	FILE *fp = NULL;
	int nread;
	char str[2048] = { 0 };

	if (NULL == (fp = fopen(filePath.c_str(), "rb"))){
#ifdef PRINT_RESULT
		strcat(m_printStr, "404 Not File");
#endif
		sprintf(str, HTTP_HEADER_ERROR, 404, "Not found file");
		Write(str, strlen(str));
		sprintf(str, HTTP_HTML_DATA_DEMO, "Error", "404 , Not found file");
		Write(str, strlen(str));

		return 0;
	}
	else{
#ifdef PRINT_RESULT
		strcat(m_printStr, "200 OK");
#endif

		Write(HTTP_HEADER_SUCCESS, strlen(HTTP_HEADER_SUCCESS));
		// 发送文件
		nread = fread(str, 1, 512, fp);
		while (nread == 512){
			Write(str, 512);
			nread = fread(str, 1, 512, fp);
		}
		Write(str, nread);
		fclose(fp);
	}
	return 1;
}


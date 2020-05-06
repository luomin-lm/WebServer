#ifndef _CLM_HTTP_HANDLE_H
#define _CLM_HTTP_HANDLE_H

#include "CHttpSock.h"
#include "map.h"
#include <string>
#define BUF_SIZE	1024

#define HTTP_HEADER_SUCCESS		"HTTP/1.0 200 OK\r\nServer : nginx\r\nContent-Type: text/html;charset:utf-8;\r\nCache-Control: max-age=0\r\n\r\n"
#define HTTP_HEADER_SUC_FT		"HTTP/1.0 200 OK\r\nServer : nginx\r\nContent-Type: %s\r\nCache-Control: max-age=0\r\n\r\n"
#define HTTP_HEADER_ERROR		"HTTP/1.0 %d %s\r\nServer : nginx\r\nContent-Type: text/html\r\nCache-Control: max-age=0\r\n\r\n"
#define HTTP_HTML_ERROR			"<!DOCTYPE html><html><head><title>Error</title><meta charset=utf-8>\
<style>body{background-color:#%x;}</style></head><body>\
<br><br><br><br><br><h1><p align=center>error code is < %d > , and error reason is < %s ></p></h1>\
</body></html>"
#define HTTP_HTML_DATA			"<!DOCTYPE html><html><head><title>%s</title><meta charset=utf-8>\
<style>body{background-color:#987654;}</style></head><body>\
<br><br><br><br><br><h1><p align=center>%s < %s > </p></h1>\
</body></html>"
#define HTTP_HTML_DATA_DEMO		"<!DOCTYPE html><html><head><title>%s</title><meta charset=utf-8>\
<style>body{background-color:#987654;}</style></head><body>\
<br><br><br><br><br> %s \
</body></html>"
#define PRINT_STRING_SIZE		512
#define HTTP_HTML_LIST_UL		"<ul>%s</ul>"
#define HTTP_HTML_LIST_LI		"<li>%s</li>"

#define PRINT_MOTHED	// 打印请求方法
#define PRINT_URL		// 打印请求URL
#define PRINT_RESULT	// 打印请求结果
#undef  PRINT_DATA		// 打印请求的所有数据

class CHttpHandle
{
public:
	CHttpHandle(const inetInfo& info);
	~CHttpHandle();

public:
	// 运行处理线程
	void RunHandleThread();
	// 处理函数
	void Handle();
	// 根据方法分类
	virtual void Depatch(const char* method, char* url, const int& len);
	// 执行正则表达式
	BOOL ExecRegex(const char* src, const char *patternStr, char *result = NULL);

public:
	// 处理不同方法的处理虚函数组
	// 获取资源
	virtual void MethodGet(const char* url);
	// 附加有数据传输
	virtual void MethodPost(const char* url, const char* postMsgStr);
	// 获取响应报头
	virtual void MethodHead(const char* url);
	// 请求服务器存储某个资源
	virtual void MethodPut(const char* url);
	// 请求服务器删除某个资源
	virtual void MethodDelete(const char* url);
	// 请求服务器回传某个资源
	virtual void MethodTrace(const char* url);
	// 未使用
	virtual void MethodConnect(const char* url);
	// 查询
	virtual void MethodOptions(const char* url);
	// 不是以上的方法
	virtual void MethodError(const char* url);

protected:
	int Write(const void* buffer, const int& bufSize);
	int Read(void* buffer, const int& maxBufSize);
	void Close();
	void GetRequestResult(const char* filePath, char* result);
	void postStr2Map(ns_dsa::CLMMap<std::string, std::string>& result, const char* postStr);
	int SendFile(const std::string& filePath);

private:
	inetInfo m_info; // 存储请求方地址
	CTcpSock m_sock; // 网络操作对象
	char m_buff[BUF_SIZE]; // 存储请求数据包
	char m_printStr[256];
};




#endif


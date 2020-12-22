#pragma once
#include <fstream>
#include <winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

class ClientSocket
{
private:
	SOCKET sock;
public:
	ClientSocket(void);
	static int Startup();       // 初始化
	int Create();               //创建
	int CreateUDP();               //创建UDP套接字
	int Bind(int port);  //绑定
	int Connect(const char *ip, const char*port);    //请求连接
	int Send(const char *file, int len);         //发送数据
	int Recive(char *filedata, int len);                   //接收数据
	int Recvfrom(char *filedata, int len);
	void Close();       //关闭连接
	static void Cleanup();     //释放资源
};
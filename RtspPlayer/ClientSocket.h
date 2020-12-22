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
	static int Startup();       // ��ʼ��
	int Create();               //����
	int CreateUDP();               //����UDP�׽���
	int Bind(int port);  //��
	int Connect(const char *ip, const char*port);    //��������
	int Send(const char *file, int len);         //��������
	int Recive(char *filedata, int len);                   //��������
	int Recvfrom(char *filedata, int len);
	void Close();       //�ر�����
	static void Cleanup();     //�ͷ���Դ
};
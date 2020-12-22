#include "stdafx.h"
#include "ClientSocket.h"

ClientSocket::ClientSocket(void)
{
	sock = INVALID_SOCKET;
}

int ClientSocket::Startup()
{
	WORD wVersionRequested = MAKEWORD(2, 2); //winsock2.2
	WSADATA wsaData;
	WSAStartup(wVersionRequested, &wsaData);
	return 0;
}

int ClientSocket::Create()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);     //协议族：面向网络的IPv4,sock类型：面向连接的TCP流套接字,协议特定类型：default
	if (sock == INVALID_SOCKET) {
		return -1;
	}
	return 0;
}

int ClientSocket::CreateUDP()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);     //协议族：面向网络的IPv4,sock类型：UDP套接字,协议特定类型：default
	if (sock == INVALID_SOCKET) {
		return -1;
	}
	return 0;
}


int ClientSocket::Bind(int port)
{
	struct sockaddr_in CAddr;
	CAddr.sin_family = AF_INET;
	CAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	CAddr.sin_port = htons(port);      //指定端口
	if (bind(sock, (struct sockaddr *) &CAddr, sizeof(CAddr)) == SOCKET_ERROR) {
		return -1;
	}
	return 0;
}

int ClientSocket::Connect(const char *ip, const char*port)
{
	struct sockaddr_in SAddr;
	SAddr.sin_family = AF_INET;
	struct in_addr server_addr;
	server_addr.s_addr = inet_addr(ip);
	SAddr.sin_addr = server_addr;
	SAddr.sin_port = htons(atoi(port));
	if (connect(sock, (struct sockaddr *) &SAddr, sizeof(SAddr)) == SOCKET_ERROR) {
		return -1;
	}
	return 0;
}

int ClientSocket::Send(const char *file, int len)
{
	int slen = send(sock, file, len, 0);
	if (slen <0) {
		return -1;
	}
	return slen;
}


int ClientSocket::Recive(char *filedata, int len)
{
	int rlen = recv(sock, filedata, len, 0);
	if (rlen <0) {
		return -1;
	}
	return rlen;
}

int ClientSocket::Recvfrom(char *filedata, int len)
{
	struct sockaddr_in SAddr;
	socklen_t addrlen = sizeof(SAddr);
	int rlen = recvfrom(sock, filedata, len, 0, (struct sockaddr *) &SAddr, &addrlen);
	return rlen;
}

void ClientSocket::Close()
{
	closesocket(sock);
}

void ClientSocket::Cleanup()
{
	WSACleanup();
}
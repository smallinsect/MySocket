#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[]) {

	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);
	//创建套接字
	SOCKET sskt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in saddr = { 0 };
	saddr.sin_family = AF_INET;//IPV4协议
	saddr.sin_port = htons(8080);//8080端口
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);//本机IP
	//将套接字和端口、ip绑定
	bind( sskt, (sockaddr *)&saddr, sizeof(saddr));
	
	sockaddr_in caddr = {0};//连接的客户端套接字信息
	int caddrLen = sizeof(caddr);
	char buf[1024];
	while (true) {
		//接受客户端发来的信息
		recvfrom(sskt, buf, 1024, 0, (sockaddr *)&caddr, &caddrLen);
		//打印客户端的ip和端口
		printf("[client] ip:%s port:%d\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));
		//打印客户端发来的信息
		printf("[client] %s\n", buf);
		//将客户端发来的信息发回给客户端
		sendto(sskt, buf, strlen(buf) + 1, 0, (sockaddr *)&caddr, caddrLen);
	}

	closesocket(sskt);

	WSACleanup();
	system("pause");
	return 0;
}
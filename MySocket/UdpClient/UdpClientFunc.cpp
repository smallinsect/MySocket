
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "UdpClientFunc.h"

int function01() {
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);
	//创建套接字
	SOCKET cskt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//需要连接的服务端信息
	sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;//需要连接的服务端的IPV4协议
	saddr.sin_port = htons(8080);//需要连接的服务端的8080端口
	saddr.sin_addr.s_addr = inet_addr("192.168.3.18");//需要连接的服务端的IP

	char buf[1024];
	while (true) {
		printf("[client] >>");
		scanf("%s", buf);
		//向服务端发送数据
		sendto( cskt, buf, strlen(buf)+1, 0, (sockaddr *)&saddr, sizeof(saddr));
		//接受服务端的数据
		recvfrom( cskt, buf, 1024, 0, NULL, NULL);
		printf("[server] %s\n", buf);
	}

	closesocket(cskt);
	WSACleanup();
	system("pause");
}

int function02() {

	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);
	//创建套接字
	SOCKET cskt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (cskt == INVALID_SOCKET) {
		printf("[client] socket error ...\n");
		system("pause");
		exit(1);
	}
	printf("[client] socket success ...\n");
	//客户端信息
	sockaddr_in caddr = { 0 };
	caddr.sin_family = AF_INET;//客户端的IPV4协议
	caddr.sin_port = htons(8787);//客户端打开的8080端口
	caddr.sin_addr.s_addr = inet_addr("0.0.0.0");//客户端的IP，本机

	int ret = bind(cskt, (sockaddr *)&caddr, sizeof(caddr));
	if (ret == SOCKET_ERROR) {
		printf("[client] bind error ...\n");
		system("puase");
		exit(1);
	}
	printf("[client] bind success ...\n");


	while (true) {
		char buf[1024] = { 0 };
		//接受服务端的数据
		ret = recvfrom(cskt, buf, 1024, 0, NULL, NULL);
		if (ret == SOCKET_ERROR) {
			printf("[client] recvfrom error ...\n");
			system("puase");
			exit(1);
		}
		printf("[client] recvfrom success ...\n");
		printf("[server] msg:%s\n", buf);
	}

	closesocket(cskt);
	WSACleanup();
	system("pause");
	return 0;
}

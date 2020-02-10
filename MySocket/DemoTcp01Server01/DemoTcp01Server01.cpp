#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Windows.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

int func(int argc, char* argv[]);

int main(int argc, char* argv[]) {

	func(argc, argv);

	system("pause");
	return 0;
}

int func(int argc, char* argv[]) {
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		printf("[server] WSAStartup error ...\n");
		return -1;
	}
	printf("[server] WSAStartup success ...\n");

	//创建套接字
	SOCKET skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == INVALID_SOCKET) {
		printf("[server] socket error ...\n");
		return -1;
	}
	printf("[server] socket success ...\n");
	//服务器信息
	sockaddr_in addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(8000);
	addrServ.sin_addr.s_addr = inet_addr("0.0.0.0");

	//套接字绑定端口和ip
	if (bind(skt, (sockaddr*)&addrServ, sizeof(addrServ)) == SOCKET_ERROR) {
		printf("[server] bind error ...\n");
		return -1;
	}
	printf("[server] bind success ...\n");

	//开始监听套接字
	if (listen(skt, SOMAXCONN) == SOCKET_ERROR) {
		printf("[server] listen error ...\n");
		return -1;
	}
	printf("[server] listen success ...\n");

	//客户端信息
	sockaddr_in caddr = { 0 };
	int caddrlen = sizeof(sockaddr_in);
	SOCKET cskt = accept(skt, (sockaddr*)&caddr, &caddrlen);//接受客户端连接
	if (cskt == SOCKET_ERROR) {
		printf("[server] accept error ...\n");
		return -1;
	}
	printf("[server] accept success ...\n");
	//打印连接的客户端信息
	printf("[client] ip:%s port:%d connect ...\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

	while (true) {
		char buf[1024] = "爱白菜的小昆虫服务器收到消息了";
		//先发送数据给客户端
		if (send(cskt, buf, strlen(buf) + 1, 0) == SOCKET_ERROR) {//向客户端发送消息
			printf("[server] send error ...\n");
			return -1;
		}
		printf("[server] send success ...\n");
		//接受客户端的数据
		if (recv(cskt, buf, sizeof(buf), 0) <= 0) {//接受客户端发来的消息
			printf("[server] recv error ...\n");
			return -1;
		}
		printf("[server] recv msg:%s\n", buf);
	}

	closesocket(skt);
	WSACleanup();
	return 0;
}
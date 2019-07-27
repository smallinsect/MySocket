//////////////////////////////////////////////////////////////////////////
//
// 服务器端程序
//
//////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[]) {
	puts("hello world");

	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//服务器创建套接字
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR) {
		puts("socket error ...");
		system("pause");
		exit(-1);
	}
	puts("socket success ...");
	//服务器绑定IP和端口
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8080);
	saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (bind(serverSocket, (const sockaddr *)&saddr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		puts("bind error ...");
		system("pause");
		exit(-1);
	}
	puts("bind success ...");

	//服务器开始监听
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		puts("listen error ...");
		system("pause");
		exit(-1);
	}
	puts("listen success ...");

	sockaddr_in clientAddr = {0};
	int clientAddrLen = sizeof(sockaddr_in);
	SOCKET clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrLen);
	if (clientSocket == INVALID_SOCKET) {
		puts("accept error ...");
		system("pause");
		exit(-1);
	}
	puts("accept success ...");


	char buf[1024];
	while (true) {
		memset(buf, 0, sizeof(buf));
		if (recv(clientSocket, buf, 1024, 0) == SOCKET_ERROR) {
			puts("recv error ...");
			system("pause");
			exit(-1);
		}
		printf("%s", buf);
	}

	closesocket(serverSocket);

	WSACleanup();

	system("pause");
	return 0;
}
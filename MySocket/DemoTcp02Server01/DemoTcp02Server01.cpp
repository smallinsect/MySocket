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


	FD_SET fds;
	FD_ZERO(&fds);
	FD_SET(skt, &fds);

	timeval tv = {1, 0};
	while (true) {
		fd_set readfds = fds;

		if (select(0, &readfds, NULL, NULL, &tv) <= 0) {
			continue;
		}

		for (u_int i = 0; i < readfds.fd_count; i++) {
			SOCKET tskt = readfds.fd_array[i];
			//服务器socket响应，有新的客户端连接请求
			if (tskt == skt) {
				SOCKET sskt = accept(tskt, NULL, NULL);
				if (sskt == INVALID_SOCKET) {//客户端连接失败
					printf("[server] accept error ...\n");
					continue;
				}
				//客户端连接成功
				printf("[server] accept success ...\n");
				//将新连接的客户端添加到集合中
				FD_SET(sskt, &fds);
			}
			else {//接受客户端的数据请求
				char buf[1024] = {0};
				//接受客户端发来的数据
				int len = recv(tskt, buf, 1024, 0);
				if (len <= 0) {
					printf("[server] recv error ...\n");
					//将客户端套接字从集合中删除
					FD_CLR(tskt, &fds);
					//关闭客户端套接字
					closesocket(tskt);
					continue;
				}
				printf("socket:%d %s\n", tskt, buf);
				char resbuf[1024] = "服务器收到消息";
				//向客户端发送数据
				send(tskt, resbuf, strlen(resbuf), 0);
			}
		}
	}

	//关闭服务器套接字
	closesocket(skt);
	WSACleanup();
}

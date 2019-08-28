//
//广播服务器
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
	if (sskt == INVALID_SOCKET) {
		printf("[server] socket error ...\n");
		system("pause");
		exit(1);
	}
	printf("[server] socket sucess ...\n");

	//服务端端信息
	sockaddr_in saddr = { 0 };
	saddr.sin_family = AF_INET;//IPV4协议
	saddr.sin_port = htons(8080);//8080端口
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);//本机IP
	//将套接字和端口、ip绑定
	int ret = bind(sskt, (sockaddr *)&saddr, sizeof(saddr));
	if (ret == SOCKET_ERROR) {
		printf("[server] bind error ...\n");
		system("pause");
		exit(1);
	}
	printf("[server] bind sucess ...\n");

	//接受广播客户端的信息
	sockaddr_in caddr = {0};
	caddr.sin_family = AF_INET;//客户端协议IPV4
	caddr.sin_port = htons(8787);//客户端端口8787
	caddr.sin_addr.s_addr = inet_addr("192.168.3.255");//广播IP地址

	//设置广播权限
	int flag = 1;
	setsockopt(sskt, SOL_SOCKET, SO_BROADCAST, (const char *)&flag, sizeof(flag));

	int num = 0;
	while (true) {
		char buf[1024] = {0};
		sprintf(buf, "hello, upd == %d", num++);
		if (num > 20) {
			printf("[server] sendto end ...\n");
			break;
		}
		//将客户端发来的信息发回给客户端
		sendto(sskt, buf, strlen(buf) + 1, 0, (sockaddr *)&caddr, sizeof(caddr));
		printf("[server] msg:%s\n", buf);
		Sleep(700);
	}

	closesocket(sskt);

	WSACleanup();
	system("pause");
	return 0;
}
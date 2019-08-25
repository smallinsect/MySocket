//socket简单的服务器代码
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <Windows.h>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[]) {

	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//创建服务器套接字
	SOCKET sktSer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addrSer = { 0 };
	addrSer.sin_family = AF_INET;//服务器的IPV4协议
	addrSer.sin_port = htons(8080);//服务器的端口
	addrSer.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器的IP

	//将套接字和端口、ip绑定
	int i = bind(sktSer, (sockaddr *)&addrSer, sizeof(addrSer));

	//开始监听套接字
	listen(sktSer, 5);

	//接受客户端套接字
	SOCKET sktCli = accept(sktSer, NULL, NULL);
	char buf[1024];
	while (true) {
		recv(sktCli, buf, sizeof(buf), 0);//接受客户端发送的数据
		printf("[client] %s\n", buf);

		send(sktCli, buf, strlen(buf) + 1, 0);//向客户端发送数据
	}

	WSACleanup();
	return 0;
}
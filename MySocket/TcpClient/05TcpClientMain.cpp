//socket简单的客户端代码
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

	//创建客户端套接字
	SOCKET sktCli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addrSer = {0};
	addrSer.sin_family = AF_INET;//要接的服务器的IPV4协议
	addrSer.sin_port = htons(8080);//要连接的服务器的端口
	addrSer.sin_addr.s_addr = inet_addr("127.0.0.1");//要连接的服务器的IP
	//连接服务器
	connect(sktCli, (sockaddr *)&addrSer, sizeof(addrSer));

	char buf[1024];
	while (true) {
		printf(">>");
		scanf("%s", buf);
		send(sktCli, buf, strlen(buf) + 1, 0);//向服务器发送数据

		recv(sktCli, buf, sizeof(buf), 0);//接受服务器发送的数据
		printf("[server] %s\n", buf);
	}

	WSACleanup();
	return 0;
}
//socket客户端代码 将代码完善
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

void printMenu();

int main(int argc, char *argv[]) {

	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//创建客户端套接字
	SOCKET sktCli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sktCli == INVALID_SOCKET) {
		printf("[client] socket error ...\n");
		exit(1);
	}
	printf("[client] socket success ...\n");

	//需要连接的服务器信息
	sockaddr_in addrSer = { 0 };
	addrSer.sin_family = AF_INET;//要接的服务器的IPV4协议
	addrSer.sin_port = htons(8080);//要连接的服务器的端口
	addrSer.sin_addr.s_addr = inet_addr("192.168.3.58");//要连接的服务器的IP
	//连接服务器
	if (connect(sktCli, (sockaddr *)&addrSer, sizeof(addrSer)) == SOCKET_ERROR) {
		printf("[client] connect error ...\n");
		exit(1);
	}
	printf("[client] connect success ...\n");

	char buf[1024];
	int n;
	while (true) {
		printMenu();
		printf(">>");
		int ret = scanf("%d", &n);
		if (ret < 1) {//非法输入
			break;
		}
		if (n == 0) {//退出程序
			printf("[client] quit ...\n");
			break;
		}
		printf("input message>>");
		scanf("%s", buf);
		//向服务器发送数据
		if (send(sktCli, buf, strlen(buf) + 1, 0) == SOCKET_ERROR) {
			printf("[client] send error ...\n");
			exit(1);
		}
		printf("[client] send success ...\n");

		//接受服务器发送的数据
		int len = recv(sktCli, buf, sizeof(buf), 0);
		if (len == -1) {//接受错误
			printf("[client] recv error ...\n");
			exit(1);
		}
		if (len == 0) {//服务器退出
			printf("[server] quit ...\n");
			break;
		}
		printf("[server] %s\n", buf);//打印服务器的信息
	}

	closesocket(sktCli);//关闭套接字
	WSACleanup();
	return 0;
}

void printMenu() {
	printf("*************************\n");
	printf("*1.send message         *\n");
	printf("*0.quit process         *\n");
	printf("*************************\n");
}

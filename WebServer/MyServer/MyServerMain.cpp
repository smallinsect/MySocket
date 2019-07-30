//////////////////////////////////////////////////////////////////////////
//
// 服务器端程序
//
//////////////////////////////////////////////////////////////////////////

#include "MyWeb.h"

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
	sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8080);
	//saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//不设置ip，bind自动绑定本机ip地址。
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

	while (true) {
		sockaddr_in clientAddr = { 0 };
		int clientAddrLen = sizeof(sockaddr_in);
		SOCKET clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrLen);
		if (clientSocket == INVALID_SOCKET) {
			puts("accept error ...");
			system("pause");
			exit(-1);
		}
		puts("accept success ...");
		printf("client ip: %s\n", inet_ntoa(clientAddr.sin_addr));
		printf("client port:%d\n", clientAddr.sin_port);

		while (true) {
			//接受浏览器请求
			if (Request(clientSocket) != 0) {
				break;
			}
			//响应浏览器请求
			Response(clientSocket);
		}
		closesocket(clientSocket);
		printf("client quit ...\n");
	}

	closesocket(serverSocket);

	WSACleanup();

	system("pause");
	return 0;
}
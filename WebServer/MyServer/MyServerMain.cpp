//////////////////////////////////////////////////////////////////////////
//
// 服务器端程序
//
//////////////////////////////////////////////////////////////////////////

#include "MyWeb.h"

int main(int argc, char *argv[]) {

	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//服务器创建套接字
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR) {
		puts("[sever] socket error ...");
		system("pause");
		exit(-1);
	}
	puts("[sever] socket success ...");
	//服务器绑定IP和端口
	sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8080);
	//saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//不设置ip，bind自动绑定本机ip地址。
	if (bind(serverSocket, (const sockaddr *)&saddr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		puts("[sever] bind error ...");
		system("pause");
		exit(-1);
	}
	puts("[sever] bind success ...");

	//服务器开始监听
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		puts("[sever] listen error ...");
		system("pause");
		exit(-1);
	}
	puts("[sever] listen success ...");

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(serverSocket, &fds);

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1000;
	while (true) {
		fd_set fdsTemp = fds;
		if (select(0, &fdsTemp, NULL, NULL, &tv) <= 0) {
			continue;
		}
		for (u_int i = 0; i < fdsTemp.fd_count; ++i) {
			if (fdsTemp.fd_array[i] == serverSocket) {
				sockaddr_in clientAddr = { 0 };
				int clientAddrLen = sizeof(sockaddr_in);
				SOCKET clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrLen);
				if (clientSocket == INVALID_SOCKET) {
					puts("[sever] accept error ...");
					system("pause");
					exit(-1);
				}
				printf("[client] %s:%d accept success ...\n", inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port);

				FD_SET(clientSocket, &fds);

				continue;
			}
			//接受浏览器请求
			if (Request(fdsTemp.fd_array[i]) > 0) {
				//响应浏览器请求
				Response(fdsTemp.fd_array[i]);
			}
			else {
				//浏览器退出
				SOCKET clientSocket = fdsTemp.fd_array[i];
				FD_CLR(fdsTemp.fd_array[i], &fds);
				closesocket(clientSocket);
			}
		}
	}

	closesocket(serverSocket);

	WSACleanup();

	system("pause");
	return 0;
}

#include "MyServerWeb.h"


int main(int argc, char *argv[]) {

	WSADATA wd;
	int iRes = WSAStartup(MAKEWORD(2,2), &wd);
	if (iRes != 0) {
		printf("[server] WSAStartup error %d ...\n", iRes);
		system("pause");
		return -1;
	}

	addrinfo hints, *result = NULL;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;

	iRes = getaddrinfo(NULL, "8080", &hints, &result);//获取本机ip
	if (iRes != 0) {
		printf("[server] getaddrinfo error %d ...\n", iRes);
		system("pause");
		return -1;
	}

	//创建套接字
	SOCKET sockServer = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sockServer == INVALID_SOCKET) {
		printf("[server] socket error ...\n");
		system("pause");
		return -1;
	}
	printf("[server] socket success ...\n");

	//套接字绑定协议、端口、IP地址
	if (bind(sockServer, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR) {
		printf("[server] bind error ...\n");
		system("pause");
		return -1;
	}
	printf("[server] bind success ...\n");

	freeaddrinfo(result);

	//开始监听
	if (listen(sockServer, SOMAXCONN) == SOCKET_ERROR) {
		cout << "[server] listen error ...\n";
		system("pause");
		return -1;
	}
	cout << "[server] listen success ...\n";

	fd_set fdsSockets;
	FD_ZERO(&fdsSockets);
	FD_SET(sockServer, &fdsSockets);

	timeval tv = { 0, 3000 };
	while (true) {
		fd_set fdsTemp = fdsSockets;

		int iret = select(0, &fdsTemp, NULL, NULL, &tv);
		if (iret == 0) {
			continue;
		}
		if (iret < 0) {
			cout << "[server] select error ...\n";
			break;
		}
		for (u_int i = 0; i < fdsTemp.fd_count; ++i) {
			if (sockServer == fdsTemp.fd_array[i]) {//服务器接收请求
				sockaddr_in saddrClient = {0};
				int saddrClientLen = sizeof(sockaddr_in);
				SOCKET sockClient = accept(sockServer, (sockaddr *)&saddrClient, &saddrClientLen);
				if (sockClient == INVALID_SOCKET) {
					cout << "[server] accept error ...\n";
					continue;
				}

				FD_SET(sockClient, &fdsSockets);
				continue;
			}
			if (request(fdsTemp.fd_array[i]) > 0) {//浏览器请求
				response(fdsTemp.fd_array[i]);//服务器响应
			}
			else {//清理socket
				SOCKET sockTemp = fdsTemp.fd_array[i];

				FD_CLR(sockTemp, &fdsSockets);
				closesocket(sockTemp);
			}
		}
	}

	FD_ZERO(&fdsSockets);
	closesocket(sockServer);

	system("pause");
	return 0;
}

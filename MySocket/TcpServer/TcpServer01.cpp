#include "TcpServer01.h"


void TcpServer01::init(const char *IP, u_short port) {
	if (skt != INVALID_SOCKET) {
		printf("[server] socket start listen ...\n");
		return;
	}
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		printf("[server] WSAStartup error ...\n");
		return ;
	}
	printf("[server] WSAStartup success ...\n");

	//创建套接字
	SOCKET skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == INVALID_SOCKET) {
		printf("[server] socket error ...\n");
		return ;
	}
	printf("[server] socket success ...\n");
	//服务器信息
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(IP);

	//套接字绑定端口和ip
	if (bind(skt, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("[server] bind error ...\n");
		return ;
	}
	printf("[server] bind success ...\n");

	//开始监听套接字
	if (listen(skt, SOMAXCONN) == SOCKET_ERROR) {
		printf("[server] listen error ...\n");
		return ;
	}
	printf("[server] listen success ...\n");
}
void TcpServer01::onRun() {
	fd_set fds;
	FD_ZERO(&fds);//清空集合
	FD_SET(skt, &fds);//将服务器socket放入集合

	timeval tv = { 0, 300 };
	while (true) {
		fd_set readfds = fds;//初始化监听的集合
		int iret = select(0, &readfds, NULL, NULL, &tv);
		if (iret < 0) {
			printf("[server] select error ...\n");
			break;
		}
		if (iret == 0) {//集合中套接字没有响应的
			continue;
		}
		for (u_int i = 0; i < readfds.fd_count; ++i) {
			SOCKET sktt = readfds.fd_array[i];
			if (sktt == skt) {//有新的客户端请求连接
				sockaddr_in addrCli = { 0 };
				int addrlenCli = sizeof(addrCli);
				SOCKET sktCli = accept(sktt, (sockaddr *)&addrCli, &addrlenCli);
				if (sktCli == INVALID_SOCKET) {
					printf("[server] accept error ...\n");
					continue;
				}
				printf("socket:%d ip:%s port:%d join ...\n", sktCli, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
				FD_SET(sktCli, &fds);//将新连接的客户端放入集合
				continue;
			}
			//客户端响应请求
			char buf[1024] = { 0 };
			iret = recv(sktt, buf, sizeof(buf), 0);
			if (iret == SOCKET_ERROR) {//接受错误
				printf("[server] recv error ...\n");
				FD_CLR(sktt, &fds);//将客户端在集合中清除
				closesocket(sktt);//关闭客户端socket
				continue;
			}
			if (iret == 0) {//客户端退出
				printf("[client] exit ...\n");
				FD_CLR(sktt, &fds);//将客户端在集合中清除
				closesocket(sktt);//关闭客户端socket
				continue;
			}
			printf("[client] msg: %s\n", buf);
			if (strcmp(buf, "getName") == 0) {
				sprintf(buf, "%s", "爱白菜的小昆虫.");
			}
			else if (strcmp(buf, "getAge") == 0) {
				sprintf(buf, "%s", "100000.");
			}
			else {
				sprintf(buf, "%s", "????.");
			}
			send(sktt, buf, strlen(buf) + 1, 0);
		}
	}

	printf("[server] exit ...\n");
	FD_CLR(skt, &fds);//清除集合中的服务器socket
}
bool TcpServer01::isRun() {
	return skt != INVALID_SOCKET;
}

void TcpServer01::readData() {
}
void TcpServer01::writeData() {
}

void TcpServer01::closeSocket(){
	if (skt != INVALID_SOCKET) {
		closesocket(skt);
		WSACleanup();
		skt = INVALID_SOCKET;
	}
}


TcpServer01::TcpServer01(){
	skt = INVALID_SOCKET;
	memset(&addr, 0, sizeof(addr));
}

TcpServer01::~TcpServer01(){
	if (skt != INVALID_SOCKET) {
		closeSocket();
	}
}

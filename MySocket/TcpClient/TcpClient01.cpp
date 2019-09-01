#include "TcpClient01.h"
#include "define.h"


void TcpClient01::init() {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		printf("[client] WSAStartup error ...\n");
		return ;
	}
	printf("[client] WSAStartup success ...\n");

	//创建套接字
	skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == SOCKET_ERROR) {
		printf("[client] socket error ...\n");
		return;
	}
	printf("[client] socket success ...\n");
}

void TcpClient01::connectServer(const char *IP, u_short port) {
	if (skt == INVALID_SOCKET) {
		init();
	}
	//服务端信息
	sockaddr_in addrServ = { 0 };
	addrServ.sin_family = AF_INET;//IPV4协议
	addrServ.sin_port = htons(port);//端口
	addrServ.sin_addr.s_addr = inet_addr(IP);//IP地址

	//连接服务器
	if (connect(skt, (sockaddr *)&addrServ, sizeof(addrServ)) == SOCKET_ERROR) {
		printf("[client] connect error ...\n");
		return ;
	}
	printf("[client] connect success ...\n");
}

void TcpClient01::onRun() {
	while (true) {
		printf("*************************\n");
		printf("*getName                *\n");
		printf("*getAge                 *\n");
		printf("*exit                   *\n");
		printf("*************************\n");
		char szCmd[64] = { 0 };
		scanf("%s", szCmd);
		if (strcmp(szCmd, "exit") == 0) {
			printf("[client] exit ...\n");
			break;
		}
		send(skt, szCmd, strlen(szCmd) + 1, 0);

		char szRecv[1024] = { 0 };
		int ret = recv(skt, szRecv, sizeof(szRecv), 0);
		if (ret <= 0) {
			printf("[client] server exit ...\n");
			break;
		}
		printf("[server] msg: %s\n", szRecv);
	}
}
bool TcpClient01::isRun() {
	return skt != INVALID_SOCKET;
}
void TcpClient01::readData() {
	char szRecv[1024];
	int ret = recv(skt, szRecv, sizeof(szRecv), 0);
	if (ret <= 0) {
		printf("[client] server exit ...\n");
		return ;
	}
	DataPackage *p = (DataPackage*)szRecv;
	printf("[server] msg: age=%d name=%s\n", p->iAge, p->szName);
}
void TcpClient01::writeData() {
	char szCmd[1024];
	int iret = send(skt, szCmd, strlen(szCmd) + 1, 0);
	if (iret == SOCKET_ERROR) {
		printf("[client] send error ...\n");
		return;
	}
	printf("[client] send success ...\n");
}

void TcpClient01::closeSocket() {
	if (skt != INVALID_SOCKET) {
		closesocket(skt);
		WSACleanup();
		skt = INVALID_SOCKET;
	}
}

TcpClient01::TcpClient01(){
	skt = INVALID_SOCKET;
}


TcpClient01::~TcpClient01(){
	if (skt != INVALID_SOCKET) {
		closeSocket();
	}
}


#include "define.h"
#include "TcpClient01.h"
#include "TcpClientFunc.h"

//ip地址 端口
SOCKET init(const char *IP, u_short port) {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		printf("[client] WSAStartup error ...\n");
		return INVALID_SOCKET;
	}
	printf("[client] WSAStartup success ...\n");

	//创建套接字
	SOCKET skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == SOCKET_ERROR) {
		printf("[client] socket error ...\n");
		return INVALID_SOCKET;
	}
	printf("[client] socket success ...\n");

	//服务端信息
	sockaddr_in addrServ = {0};
	addrServ.sin_family = AF_INET;//IPV4协议
	addrServ.sin_port = htons(port);//端口
	addrServ.sin_addr.s_addr = inet_addr(IP);//IP地址

	//连接服务器
	if (connect(skt, (sockaddr *)&addrServ, sizeof(addrServ)) == SOCKET_ERROR) {
		printf("[client] connect error ...\n");
		return -1;
	}
	printf("[client] connect success ...\n");
	return skt;
}

void destroy(SOCKET skt) {
	closesocket(skt);
	WSACleanup();
}

//socket简单的客户端
int function01() {
	SOCKET sktCli = init("0.0.0.0", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}

	char buf[1024] = {0};
	if (recv(sktCli, buf, sizeof(buf), 0) == SOCKET_ERROR) {
		printf("[client] recv error ...\n");
		return -1;
	}
	printf("[client] recv success ...\n");
	printf("[server] msg: %s\n", buf);

	printf("input>>");
	scanf("%s", buf);
	send(sktCli, buf, strlen(buf) + 1, 0);

	destroy(sktCli);
	return 0;
}

//结构化的网络消息
int function02() {
	SOCKET sktCli = init("192.168.3.18", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}

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
		send(sktCli, szCmd, strlen(szCmd) + 1, 0);

		char szRecv[1024] = { 0 };
		int ret = recv(sktCli, szRecv, sizeof(szRecv), 0);
		if (ret <= 0) {
			printf("[client] server exit ...\n");
			break;
		}
		DataPackage *p = (DataPackage*)szRecv;
		printf("[server] msg: age=%d name=%s\n", p->iAge, p->szName);
	}

	destroy(sktCli);
	return 0;
}


//客户端完善结构化消息
int function03() {
	SOCKET sktCli = init("192.168.3.18", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}

	while (true) {
		char szCmd[64] = { 0 };
		scanf("%s", szCmd);
		if (strcmp(szCmd, "exit") == 0) {
			printf("[client] exit ...\n");
			break;
		}

		DataHeader dh = { 0 };
		if (strcmp(szCmd, "login") == 0) {
			Login login;
			strcpy(login.szName, "爱白菜的小昆虫登录");
			strcpy(login.szPass, "我不知道密码是啥");
			send(sktCli, (const char *)&login, sizeof(login), 0);
			LoginResult result;
			recv(sktCli, (char *)&result, sizeof(result), 0);
			printf("[server] cmd=%d dataLength=%d iResult=%d\n", result.cmd, result.dataLength, result.iResult);
		}
		else if (strcmp(szCmd, "logout") == 0) {
			Logout logout;
			strcpy(logout.szName, "爱白菜的小昆虫退出");
			send(sktCli, (const char *)&logout, sizeof(logout), 0);
			LogoutResult result;
			recv(sktCli, (char *)&result, sizeof(result), 0);
			printf("[server] cmd=%d dataLength=%d iResult=%d\n", result.cmd, result.dataLength, result.iResult);
		}
		else {
		}
	}

	destroy(sktCli);
	return 0;
}

//客户端写数据
DWORD WINAPI ThreadFunc04(LPVOID params) {
	SOCKET skt = (SOCKET)params;
	int ret;
	char buf[1024];
	while (true) {
		printf("*************************\n");
		printf("*getName                *\n");
		printf("*getAge                 *\n");
		printf("*exit                   *\n");
		printf("*************************\n");
		scanf("%s", buf);
		if (strcmp(buf, "exit") == 0) {
			break;
		}
		ret = send(skt, buf, strlen(buf)+1, 0);
		if (ret == SOCKET_ERROR) {
			printf("[client] send error ...\n");
			return -1;
		}
		printf("[client] %s send success ...\n", buf);
		ret = recv(skt, buf, sizeof(buf), 0);
		if (ret == SOCKET_ERROR) {
			printf("[client] recv error ...\n");
			return -1;
		}
		if (ret == 0) {
			printf("[client] server exit ...\n");
			return -1;
		}
		printf("[server] msg: %s\n", buf);
	}
	return 0;
}

//socket客户端 使用线程接受和发送消息
int function04() {
	SOCKET sktCli = init("192.168.3.18", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}
	
	HANDLE hThread;
	DWORD exitCode;
	DWORD threadId;

	hThread = CreateThread(NULL, 0, ThreadFunc04, (LPVOID)sktCli, 0, &threadId);
	while (true) {
		GetExitCodeThread(hThread, &exitCode);//获取执行码

		if (exitCode != STILL_ACTIVE) {//线程运行完
			break;
		}
		Sleep(1000);
	}
	CloseHandle(hThread);

	printf("ReadThreadFunc04 return %d\n", exitCode);
	printf("[client] run end ...\n");

	destroy(sktCli);
	return 0;
}

//socket简单的客户端代码
int function05() {
	SOCKET sktCli = init("192.168.3.18", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}

	char buf[1024];
	while (true) {
		printf("*************************\n");
		printf("*getName                *\n");
		printf("*getAge                 *\n");
		printf("*exit                   *\n");
		printf("*************************\n");
		scanf("%s", buf);
		if (strcmp(buf, "exit") == 0) {
			break;
		}
		int iret = send(sktCli, buf, strlen(buf) + 1, 0);
		if (iret == SOCKET_ERROR) {
			printf("[client] send error ...\n");
			return -1;
		}
		printf("[client] %s send success ...\n", buf);
		iret = recv(sktCli, buf, sizeof(buf), 0);
		if (iret == SOCKET_ERROR) {
			printf("[client] recv error ...\n");
			return -1;
		}
		if (iret == 0) {
			printf("[client] server exit ...\n");
			return -1;
		}
		printf("[server] msg: %s\n", buf);
	}

	destroy(sktCli);
	return 0;
}

//socket客户端代码 客户端发送命令 接受服务器发送的消息
int function06() {
	SOCKET sktCli = init("192.168.3.18", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}

	while (true) {
		//printMenu();
		char szCmd[64] = {0};
		scanf("%s", szCmd);
		if (strcmp(szCmd, "exit") == 0) {
			printf("[client] exit ...\n");
			break;
		}
		send(sktCli, szCmd, strlen(szCmd)+1, 0);

		char szRecv[1024] = {0};
		int ret = recv(sktCli, szRecv, sizeof(szRecv), 0);
		if (ret <= 0) {
			printf("[client] server exit ...\n");
			break;
		}
		printf("[server] msg:%s\n", szRecv);
	}

	destroy(sktCli);
	return 0;
}

//将客户端封装成类 使用
int function07() {
	TcpClient01 client;
	client.connectServer("192.168.3.18", 8080);
	client.onRun();

	return 0;
}

int function08() {
	return 0;
}
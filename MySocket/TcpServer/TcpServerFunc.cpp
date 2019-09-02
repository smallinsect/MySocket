#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
using namespace std;

#include "TcpServerFunc.h"

//初始化
SOCKET init(const char *IP, u_short port) {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		printf("[server] WSAStartup error ...\n");
		return -1;
	}
	printf("[server] WSAStartup success ...\n");

	//创建套接字
	SOCKET skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == INVALID_SOCKET) {
		printf("[server] socket error ...\n");
		return -1;
	}
	printf("[server] socket success ...\n");
	//服务器信息
	sockaddr_in addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(port);
	addrServ.sin_addr.s_addr = inet_addr(IP);

	//套接字绑定端口和ip
	if (bind(skt, (sockaddr *)&addrServ, sizeof(addrServ)) == SOCKET_ERROR) {
		printf("[server] bind error ...\n");
		return -1;
	}
	printf("[server] bind success ...\n");

	//开始监听套接字
	if (listen(skt, SOMAXCONN) == SOCKET_ERROR) {
		printf("[server] listen error ...\n");
		return -1;
	}
	printf("[server] listen success ...\n");

	return skt;
}

void destroy(SOCKET skt) {
	closesocket(skt);
	WSACleanup();
}

//socket简单的服务端
int function01() {
	SOCKET sktServ = init("0.0.0.0", 8080);
	if (sktServ == INVALID_SOCKET) {
		return -1;
	}

	sockaddr_in caddr;
	int caddrlen = sizeof(sockaddr_in);
	SOCKET _cSocket = accept(sktServ, (sockaddr *)& caddr, &caddrlen);//接受客户端连接
	if (_cSocket == SOCKET_ERROR) {
		printf("[server] accept error ...\n");
		return -1;
	}
	printf("[server] accept success ...\n");
	//打印连接的客户端信息
	printf("[client] ip:%s port:%d\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

	char buf[1024] = "爱白菜的小昆虫服务器收到消息了";
	send(_cSocket, buf, strlen(buf)+1, 0);//向客户端发送消息

	recv(_cSocket, buf, sizeof(buf), 0);//接受客户端发来的消息
	printf("[client] msg:%s\n", buf);
	
	printf("[server] exit ...\n");
	destroy(sktServ);
	return 0;
}

struct DataPackage {
	int iAge;
	char szName[32];
};
//socket服务端传输结构体
int function02() {
	SOCKET sktServ = init("0.0.0.0", 8080);
	if (sktServ == INVALID_SOCKET) {
		return -1;
	}

	SOCKET sktCli = accept(sktServ, NULL, NULL);
	while (true) {
		char szCmd[64] = { 0 };
		int ret = recv(sktCli, szCmd, sizeof(szCmd), 0);
		if (ret == SOCKET_ERROR) {
			printf("[server] recv error ...\n");
			break;
		}
		if (ret == 0) {
			printf("[client] exit ...\n");
			break;
		}
		printf("[client] %s\n", szCmd);
		char szMsg[1024];
		if (strcmp(szCmd, "getInfo") == 0) {
			DataPackage dp = { 1000, "爱白菜的小昆虫" };
			send(sktCli, (const char *)&dp, sizeof(dp), 0);
		}
		else {
			sprintf(szMsg, "%s", "???.");
			send(sktCli, szMsg, strlen(szMsg) + 1, 0);
		}
	}

	destroy(sktServ);
	return 0;
}
//交互命令
enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};
//消息头
struct DataHeader {
	int cmd;//消息指令
	int dataLength;//消息长度
};
//登录消息
struct Login : public DataHeader {
	Login() {
		cmd = CMD_LOGIN;
		dataLength = sizeof(Login);
	}
	char szName[64];//用户名
	char szPass[64];//密码
};
//登录返回消息
struct LoginResult : public DataHeader {
	LoginResult() {
		cmd = CMD_LOGIN_RESULT;
		dataLength = sizeof(LoginResult);
		iResult = 1;
	}
	int iResult;
};
//登出消息
struct Logout : public DataHeader {
	Logout() {
		cmd = CMD_LOGOUT;
		dataLength = sizeof(Logout);
	}
	char szName[64];
};
//登出返回消息
struct LogoutResult : public DataHeader {
	LogoutResult() {
		cmd = CMD_LOGOUT_RESULT;
		dataLength = sizeof(LogoutResult);
		iResult = 2;
	}
	int iResult;
};
//服务端完善结构化消息
int function03() {
	SOCKET sktServ = init("0.0.0.0", 8080);
	if (sktServ == INVALID_SOCKET) {
		return -1;
	}

	SOCKET sktCli = accept(sktServ, NULL, NULL);
	while (true) {
		DataHeader header;
		int ret = recv(sktCli, (char *)&header, sizeof(header), 0);
		if (ret == SOCKET_ERROR) {
			printf("[server] recv error ...\n");
			break;
		}
		if (ret == 0) {
			printf("[client] exit ...\n");
			break;
		}
		printf("[client] cmd=%d datalength=%d\n", header.cmd, header.dataLength);

		switch (header.cmd) {
		case CMD_LOGIN: {
			Login login;
			//客户端发送login结构体，但是服务端前面接受了DataHeader，所以这里只需要接受剩下的消息
			recv(sktCli, (char *)&login + sizeof(DataHeader), sizeof(login) - sizeof(DataHeader), 0);
			printf("[client] name=%s pass=%s\n", login.szName, login.szPass);

			LoginResult result;
			send(sktCli, (const char *)&result, sizeof(result), 0);
			break;
		}
		case CMD_LOGOUT: {
			Logout logout;
			//客户端发送login结构体，但是服务端前面接受了DataHeader，所以这里只需要接受剩下的消息
			recv(sktCli, (char *)&logout + sizeof(DataHeader), sizeof(logout) - sizeof(DataHeader), 0);
			printf("[client] name=%s\n", logout.szName);

			LogoutResult result;
			send(sktCli, (const char *)&result, sizeof(result), 0);
			break;
		}
		default:
			break;
		}
	}

	destroy(sktServ);
	return 0;
}

//用线程处理一个客户端的收发信息
DWORD WINAPI ThreadFunc04(LPVOID params) {
	SOCKET skt = (SOCKET)params;
	sockaddr_in addr = {0};
	int addrLen = sizeof(addr);
	getpeername(skt, (sockaddr *)&addr, &addrLen);//获取套接字的信息

	char buf[1024];
	int ret;
	while (true) {
		ret = recv(skt, buf, sizeof(buf), 0);
		if (ret == SOCKET_ERROR) {
			printf("[server] recv socket:%d ip:%s port:%d error ...\n", skt, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
			break;
		}
		if (ret == 0) {
			printf("[client] socket:%d ip:%s port:%d exit ...\n", skt, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
			break;
		}
		printf("[client] socket:%d ip:%s port:%d\n", skt, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
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

		send(skt, buf, strlen(buf)+1, 0);
	}

	return 0;
}

//服务端 使用线程连接多个客户端
int function04() {
	SOCKET sktServ = init("0.0.0.0", 8080);
	if (sktServ == INVALID_SOCKET) {
		return -1;
	}

	DWORD threadId;
	SOCKET sktCli = INVALID_SOCKET;
	sockaddr_in addr = { 0 };
	int addrLen = sizeof(addr);
	while (true) {
		sktCli = accept(sktServ, (sockaddr *)&addr, &addrLen);
		if (sktCli == INVALID_SOCKET) {
			printf("[client] accpet error ...\n");
			return -1;
		}
		printf("[client] socket:%d ip:%s port:%d accept ...\n", sktCli, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

		//创建线程
		CreateThread(NULL, 0, ThreadFunc04, (LPVOID)sktCli, 0, &threadId);
	}

	destroy(sktServ);
	return 0;
}

//socket 使用select模型 连接多个客户端
int function05() {
	SOCKET sktServ = init("0.0.0.0", 8080);
	if (sktServ == INVALID_SOCKET) {
		return -1;
	}

	fd_set fds;
	FD_ZERO(&fds);//清空集合
	FD_SET(sktServ, &fds);//将服务器socket放入集合

	timeval tv = {0, 300};
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
			SOCKET skt = readfds.fd_array[i];
			if (skt == sktServ) {//有新的客户端请求连接
				sockaddr_in addr = {0};
				int addrlen = sizeof(addr);
				SOCKET sktCli = accept(skt, (sockaddr *)&addr, &addrlen);
				if (sktCli == INVALID_SOCKET) {
					printf("[server] accept error ...\n");
					continue;
				}
				printf("socket:%d ip:%s port:%d join ...\n", sktCli, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
				FD_SET(sktCli, &fds);//将新连接的客户端放入集合
				continue;
			}
			//客户端响应请求
			char buf[1024] = {0};
			iret = recv(skt, buf, sizeof(buf), 0);
			if (iret == SOCKET_ERROR) {//接受错误
				printf("[server] recv error ...\n");
				FD_CLR(skt, &fds);//将客户端在集合中清除
				closesocket(skt);//关闭客户端socket
				continue;
			}
			if (iret == 0) {//客户端退出
				printf("[client] exit ...\n");
				FD_CLR(skt, &fds);//将客户端在集合中清除
				closesocket(skt);//关闭客户端socket
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
			send(skt, buf, strlen(buf) + 1, 0);
		}
	}

	printf("[server] exit ...\n");
	FD_CLR(sktServ, &fds);//清除集合中的服务器socket
	destroy(sktServ);
	return 0;
}

//接受客户端发送的命令 发送消息给客户端
int function06() {
	SOCKET sktServ = init("0.0.0.0", 8080);
	if (sktServ == INVALID_SOCKET) {
		return -1;
	}

	SOCKET sktCli = accept(sktServ, NULL, NULL);
	while (true) {
		char szCmd[64] = {0};
		int ret = recv(sktCli, szCmd, sizeof(szCmd), 0);
		if (ret == SOCKET_ERROR) {
			printf("[server] recv error ...\n");
			break;
		}
		if (ret == 0) {
			printf("[client] exit ...\n");
			break;
		}
		printf("[client] %s\n", szCmd);
		char szMsg[1024];
		if (strcmp(szCmd, "getName") == 0) {
			sprintf(szMsg, "%s", "爱白菜的小昆虫.");
		}
		else if (strcmp(szCmd, "getAge") == 0) {
			sprintf(szMsg, "%s", "1000.");
		}
		else {
			sprintf(szMsg, "%s", "???.");
		}
		send(sktCli, szMsg, strlen(szMsg)+1, 0);
	}

	destroy(sktServ);
	return 0;
}

int function07() {
	SOCKET sktServ = init("0.0.0.0", 8080);
	if (sktServ == INVALID_SOCKET) {
		return -1;
	}

	SOCKET sktCli = accept(sktServ, NULL, NULL);
	while (true) {
		char szCmd[64] = { 0 };
		int ret = recv(sktCli, szCmd, sizeof(szCmd), 0);
		if (ret == SOCKET_ERROR) {
			printf("[server] recv error ...\n");
			break;
		}
		if (ret == 0) {
			printf("[client] exit ...\n");
			break;
		}
		printf("[client] %s\n", szCmd);
		char szMsg[1024];
		if (strcmp(szCmd, "getName") == 0) {
			sprintf(szMsg, "%s", "爱白菜的小昆虫.");
		}
		else if (strcmp(szCmd, "getAge") == 0) {
			sprintf(szMsg, "%s", "1000.");
		}
		else {
			sprintf(szMsg, "%s", "???.");
		}
		send(sktCli, szMsg, strlen(szMsg) + 1, 0);
	}

	destroy(sktServ);
	return 0;
}


int function08() {
	return 0;
}
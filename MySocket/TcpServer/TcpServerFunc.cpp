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
		cout << "WSAStartup error ..." << endl;
		return -1;
	}
	cout << "WSAStartup success ..." << endl;

	//创建套接字
	SOCKET skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == INVALID_SOCKET) {
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;
	//服务器信息
	sockaddr_in addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(port);
	addrServ.sin_addr.s_addr = inet_addr(IP);

	//套接字绑定端口和ip
	if (bind(skt, (sockaddr *)&addrServ, sizeof(addrServ)) == SOCKET_ERROR) {
		cout << "bind error ..." << endl;
		return -1;
	}
	cout << "bind success ..." << endl;

	//开始监听套接字
	if (listen(skt, SOMAXCONN) == SOCKET_ERROR) {
		cout << "listen error ..." << endl;
		return -1;
	}
	cout << "listen success ..." << endl;

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

	while (true) {
		sockaddr_in caddr;
		int caddrlen = sizeof(sockaddr_in);
		SOCKET _cSocket = accept(sktServ, (sockaddr *)& caddr, &caddrlen);
		if (_cSocket == SOCKET_ERROR) {
			cout << "accept error ..." << endl;
			return -1;
		}
		cout << "accept success ..." << endl;

		cout << "client ip : " << inet_ntoa(caddr.sin_addr) << endl;
		cout << "client port : " << caddr.sin_port << endl;
		cout << "client size : " << sizeof(caddr) << endl;

		char buf[1024] = "爱白菜的小昆虫服务器收到消息了";
		send(_cSocket, buf, strlen(buf), 0);

		memset(buf, 0, sizeof(buf));
		recv(_cSocket, buf, sizeof(buf), 0);
		cout << "recv client msg : " << buf << endl;
	}

	cout << "server exit ..." << endl;

	destroy(sktServ);
	return 0;
}

//socket服务端传输结构体
int function02() {
	SOCKET sktServ = init("0.0.0.0", 8080);
	if (sktServ == INVALID_SOCKET) {
		return -1;
	}

	//while (true) {
	//	SOCKADDR_IN caddr;
	//	int caddrlen = sizeof(SOCKADDR_IN);
	//	SOCKET _cSocket = accept(sktServ, (sockaddr *)& caddr, &caddrlen);
	//	if (_cSocket == SOCKET_ERROR) {
	//		cout << "accept error ..." << endl;
	//		return -1;
	//	}
	//	cout << "accept success ..." << endl;

	//	cout << "client ip : " << inet_ntoa(caddr.sin_addr) << endl;
	//	cout << "client port : " << ntohs(caddr.sin_port) << endl;
	//	cout << "client size : " << sizeof(caddr) << endl;

	//	char buf[1024] = "爱白菜的小昆虫服务器收到消息了";
	//	send(_cSocket, buf, strlen(buf), 0);

	//	//memset(buf, 0, sizeof(buf));
	//	struct Message msg;
	//	if (recv(_cSocket, (char *)&msg, sizeof(msg), 0) == SOCKET_ERROR) {
	//		cout << "recv error ..." << endl;
	//		break;
	//	}
	//	cout << "recv success ..." << endl;
	//	cout << "recv client msg iCmd : " << msg.iCmd << endl;
	//	cout << "recv client msg szMsg : " << msg.szMsg << endl;
	//}

	cout << "server exit ..." << endl;

	destroy(sktServ);
	return 0;
}
int function03() {
	SOCKET sktServ = init("0.0.0.0", 8080);
	if (sktServ == INVALID_SOCKET) {
		return -1;
	}

	while (true) {
		sockaddr_in caddr;
		int caddrlen = sizeof(sockaddr_in);
		SOCKET _cSocket = accept(sktServ, (sockaddr *)& caddr, &caddrlen);
		if (_cSocket == SOCKET_ERROR) {
			cout << "accept error ..." << endl;
			return -1;
		}
		cout << "accept success ..." << endl;

		cout << "client ip : " << inet_ntoa(caddr.sin_addr) << endl;
		cout << "client port : " << ntohs(caddr.sin_port) << endl;
		cout << "client size : " << sizeof(caddr) << endl;

		char buf[1024] = "爱白菜的小昆虫服务器收到消息了";
		send(_cSocket, buf, strlen(buf), 0);
		while (true) {
			memset(buf, 0, sizeof(buf));
			if (recv(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
				cout << "recv error ..." << endl;
				break;
			}
			cout << "recv success ..." << endl;
			cout << "client iCmd : " << buf << endl;
			if (strcmp(buf, "SignIn") == 0) {
				char msgBuf[] = "登录成功";
				if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
					cout << "send error ..." << endl;
					return -1;
				}
				cout << "send success ..." << endl;
			}
			else if (strcmp(buf, "getName") == 0) {
				char msgBuf[] = "小昆虫";
				if (send(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
					cout << "send error ..." << endl;
					return -1;
				}
			}
			else if (strcmp(buf, "getAge") == 0) {
				char msgBuf[] = "10000岁";
				if (send(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
					cout << "send error ..." << endl;
					return -1;
				}
			}
			else if (strcmp(buf, "SignOut") == 0) {
				char msgBuf[] = "退出登录";
				if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
					cout << "send error ..." << endl;
					return -1;
				}
				cout << "send success ..." << endl;
			}
			else if (strcmp(buf, "Quit") == 0) {
				char msgBuf[] = "退出程序";
				if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
					cout << "send error ..." << endl;
					return -1;
				}
				cout << "send success ..." << endl;
				cout << "client quit ..." << endl;
				break;
			}
			else {
				cout << "iCmd error ..." << endl;
				break;
			}
		}
	}
	cout << "server exit ..." << endl;

	destroy(sktServ);
	return 0;
}

//服务端与客户端的数据交互 用结构体
int function04() {
	SOCKET sktServ = init("0.0.0.0", 8080);
	if (sktServ == INVALID_SOCKET) {
		return -1;
	}
	
	//while (true) {
	//	sockaddr_in caddr;
	//	int caddrlen = sizeof(sockaddr_in);
	//	SOCKET _cSocket = accept(sktServ, (sockaddr*)& caddr, &caddrlen);
	//	if (_cSocket == SOCKET_ERROR) {
	//		cout << "accept error ..." << endl;
	//		return -1;
	//	}
	//	cout << "accept success ..." << endl;
	//
	//	cout << "client ip : " << inet_ntoa(caddr.sin_addr) << endl;
	//	cout << "client port : " << ntohs(caddr.sin_port) << endl;
	//	cout << "client size : " << sizeof(caddr) << endl;
	//
	//	char buf[1024] = "爱白菜的小昆虫服务器收到消息了";
	//	send(_cSocket, buf, strlen(buf), 0);
	//	while (true) {
	//		DataHeader dh = {};
	//
	//		if (recv(_cSocket, (char *)&dh, sizeof(DataHeader), 0) == SOCKET_ERROR) {
	//			cout << "recv error ..." << endl;
	//			break;
	//		}
	//		cout << "recv success ..." << endl;
	//
	//		switch (dh.iCmd) {
	//		case CMD_SIGN_IN:{
	//			SignIn si = {};
	//			recv(_cSocket, (char *)&si, sizeof(SignIn), 0);
	//			SignResult sr = {1};
	//			send(_cSocket, (const char *)&sr, sizeof(SignResult), 0);
	//			break;
	//		}
	//		case CMD_SIGN_OUT: {
	//			SignOut so = {};
	//			recv(_cSocket, (char*)& so, sizeof(SignOut), 0);
	//			SignResult sr = { 2 };
	//			send(_cSocket, (const char*)& sr, sizeof(SignResult), 0);
	//			break;
	//		}
	//		default: {
	//			dh.iCmd = CMD_ERROR;
	//			send(_cSocket, (const char *)&dh, sizeof(DataHeader), 0);
	//
	//			break;
	//		}
	//		}
	//
	//	}
	//}
	
	cout << "server exit ..." << endl;
	destroy(sktServ);
	return 0;
}

int function05() {
	SOCKET sktServ = init("0.0.0.0", 8080);
	if (sktServ == INVALID_SOCKET) {
		return -1;
	}

	while (true) {
		SOCKADDR_IN caddr;
		int caddrlen = sizeof(SOCKADDR_IN);
		SOCKET _cSocket = accept(sktServ, (PSOCKADDR)& caddr, &caddrlen);
		if (_cSocket == SOCKET_ERROR) {
			cout << "accept error ..." << endl;
			return -1;
		}
		cout << "accept success ..." << endl;

		cout << "client ip : " << inet_ntoa(caddr.sin_addr) << endl;
		cout << "client port : " << ntohs(caddr.sin_port) << endl;
		cout << "client size : " << sizeof(caddr) << endl;

		char buf[1024] = "爱白菜的小昆虫服务器收到消息了";
		send(_cSocket, buf, strlen(buf), 0);

		memset(buf, 0, sizeof(buf));
		recv(_cSocket, buf, sizeof(buf), 0);
		cout << "recv client msg : " << buf << endl;
	}

	cout << "server exit ..." << endl;

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

struct DataPackage {
	int age;
	char name[32];
};

//结构化的网络消息
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
		if (strcmp(szCmd, "getInfo") == 0) {
			DataPackage dp = {1000, "爱白菜的小昆虫"};
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
enum CMD{
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
struct Login : public DataHeader{
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
int function08() {
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
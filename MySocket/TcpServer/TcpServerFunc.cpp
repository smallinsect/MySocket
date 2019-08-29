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

struct Message {
	int iCmd;//指令
	char szMsg[1024];//消息
};
enum CMD {
	CMD_ERROR,
	CMD_SIGN_IN,
	CMD_SIGN_OUT,
};
//消息头
struct DataHeader {
	int iCmd;//命令
	int iDataLength;//数据长度
};
//登录
struct SignIn {
	char szUserName[32];
	char szPassWord[32];
};
//登出
struct SignOut {
	int iResult;
};

//返回值
struct SignResult {
	int iResult;
};

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

	while (true) {
		SOCKADDR_IN caddr;
		int caddrlen = sizeof(SOCKADDR_IN);
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

		//memset(buf, 0, sizeof(buf));
		struct Message msg;
		if (recv(_cSocket, (char *)&msg, sizeof(msg), 0) == SOCKET_ERROR) {
			cout << "recv error ..." << endl;
			break;
		}
		cout << "recv success ..." << endl;
		cout << "recv client msg iCmd : " << msg.iCmd << endl;
		cout << "recv client msg szMsg : " << msg.szMsg << endl;
	}

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
	
	while (true) {
		sockaddr_in caddr;
		int caddrlen = sizeof(sockaddr_in);
		SOCKET _cSocket = accept(sktServ, (sockaddr*)& caddr, &caddrlen);
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
			DataHeader dh = {};
	
			if (recv(_cSocket, (char *)&dh, sizeof(DataHeader), 0) == SOCKET_ERROR) {
				cout << "recv error ..." << endl;
				break;
			}
			cout << "recv success ..." << endl;
	
			switch (dh.iCmd) {
			case CMD_SIGN_IN:{
				SignIn si = {};
				recv(_cSocket, (char *)&si, sizeof(SignIn), 0);
				SignResult sr = {1};
				send(_cSocket, (const char *)&sr, sizeof(SignResult), 0);
				break;
			}
			case CMD_SIGN_OUT: {
				SignOut so = {};
				recv(_cSocket, (char*)& so, sizeof(SignOut), 0);
				SignResult sr = { 2 };
				send(_cSocket, (const char*)& sr, sizeof(SignResult), 0);
				break;
			}
			default: {
				dh.iCmd = CMD_ERROR;
				send(_cSocket, (const char *)&dh, sizeof(DataHeader), 0);
	
				break;
			}
			}
	
		}
	}
	
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
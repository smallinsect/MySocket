
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

#include "TcpClientFunc.h"

//ip地址 端口
SOCKET init(const char *IP, u_short port) {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		cout << "WSAStartup error ..." << endl;
		return INVALID_SOCKET;
	}
	cout << "WSAStartup success ..." << endl;

	//创建套接字
	SOCKET skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == SOCKET_ERROR) {
		cout << "socket error ..." << endl;
		return INVALID_SOCKET;
	}
	cout << "socket success ..." << endl;

	//服务端信息
	sockaddr_in addrServ;
	addrServ.sin_family = AF_INET;//IPV4协议
	addrServ.sin_port = htons(port);//端口
	addrServ.sin_addr.s_addr = inet_addr(IP);//IP地址

	//连接服务器
	if (connect(skt, (sockaddr *)&addrServ, sizeof(addrServ)) == SOCKET_ERROR) {
		cout << "connect error ..." << endl;
		return -1;
	}
	cout << "connect success ..." << endl;

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

	char buf[1024];
	memset(buf, 0, sizeof(buf));
	if (recv(sktCli, buf, sizeof(buf), 0) == SOCKET_ERROR) {
		cout << "recv error ..." << endl;
		return -1;
	}
	cout << "recv success ..." << endl;
	
	cout << "server msg : " << buf << endl;

	cout << "input msg >>";
	cin >> buf;
	send(sktCli, buf, strlen(buf), 0);

	destroy(sktCli);
	return 0;
}

struct Message {
	int iCmd;//指令
	char szMsg[1024];//消息
};
//socket客户端传输结构体
int function02() {
	SOCKET sktCli = init("0.0.0.0", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}

	char buf[1024];
	memset(buf, 0, sizeof(buf));
	if (recv(sktCli, buf, sizeof(buf), 0) == SOCKET_ERROR) {
		cout << "recv error ..." << endl;
		return -1;
	}
	cout << "recv success ..." << endl;
	
	cout << "server msg : " << buf << endl;

	cout << "input msg >>";
	struct Message msg = {2333, "爱白菜的小昆虫是客户端"};
	if (send(sktCli, (const char*)& msg, sizeof(msg), 0) == SOCKET_ERROR) {
		cout << "send error ..." << endl;
		return -1;
	}
	cout << "send success ..." << endl;

	destroy(sktCli);
	return 0;
}

//socket客户端 客户端与服务端的数据交互
int function03() {
	SOCKET sktCli = init("0.0.0.0", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}

	char buf[1024];
	memset(buf, 0, sizeof(buf));
	if (recv(sktCli, buf, sizeof(buf), 0) == SOCKET_ERROR) {
		cout << "recv error ..." << endl;
		return -1;
	}
	cout << "recv success ..." << endl;
	
	cout << "server msg : " << buf << endl;

	while (true) {
		cout << "input cmd >>";
		cin >> buf;
		char msgBuf[1024] = "";
		if (strcmp(buf, "SignIn") == 0) {
			if (send(sktCli, buf, sizeof(buf), 0) == SOCKET_ERROR) {
				cout << "send error ..." << endl;
				return -1;
			}
			cout << "send success ..." << endl;
			if (recv(sktCli, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
				cout << "recv error ..." << endl;
			}
			cout << "recv success ..." << endl;
			cout << "server : " << msgBuf << endl;
		} else if (strcmp(buf, "getName") == 0) {
			if (send(sktCli, buf, sizeof(buf), 0) == SOCKET_ERROR) {
				cout << "send error ..." << endl;
				return -1;
			}
			if (recv(sktCli, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
				cout << "recv error ..." << endl;
			}
			cout << "recv success ..." << endl;
			cout << "server : " << msgBuf << endl;
		} else if (strcmp(buf, "getAge") == 0) {
			if (send(sktCli, buf, sizeof(buf), 0) == SOCKET_ERROR) {
				cout << "send error ..." << endl;
				return -1;
			}
			if (recv(sktCli, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
				cout << "recv error ..." << endl;
			}
			cout << "recv success ..." << endl;
			cout << "server : " << msgBuf << endl;
		} else if (strcmp(buf, "SignOut") == 0) {
			if (send(sktCli, buf, sizeof(buf), 0) == SOCKET_ERROR) {
				cout << "send error ..." << endl;
				return -1;
			}
			cout << "send success ..." << endl;
			if (recv(sktCli, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
				cout << "recv error ..." << endl;
			}
			cout << "recv success ..." << endl;
			cout << "server : " << msgBuf << endl;
		} else if (strcmp(buf, "Quit") == 0) {
			if (send(sktCli, buf, sizeof(buf), 0) == SOCKET_ERROR) {
				cout << "send error ..." << endl;
				return -1;
			}
			cout << "send success ..." << endl;
			if (recv(sktCli, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
				cout << "recv error ..." << endl;
			}
			cout << "recv success ..." << endl;
			cout << "server : " << msgBuf << endl;
			break;
		} else {
			cout << "iCmd error ..." << endl;
		}
	}

	destroy(sktCli);
	return 0;
}

//socket客户端 客户端与服务端的数据交互 用结构体
int function04() {
	SOCKET sktCli = init("0.0.0.0", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}
	
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	if (recv(sktCli, buf, sizeof(buf), 0) == SOCKET_ERROR) {
		cout << "recv error ..." << endl;
		return -1;
	}
	cout << "recv success ..." << endl;
	cout << "server msg : " << buf << endl;
	
	while (true) {
		cout << "input cmd >>";
		cin >> buf;
		char msgBuf[1024] = "";
		if (send(sktCli, buf, sizeof(buf), 0) == SOCKET_ERROR) {
			cout << "send error ..." << endl;
			return -1;
		}
		cout << "send success ..." << endl;
		if (recv(sktCli, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
			cout << "recv error ..." << endl;
		}
		cout << "recv success ..." << endl;
		cout << "server : " << msgBuf << endl;
	}

	destroy(sktCli);
	return 0;
}

//socket简单的客户端代码
int function05() {
	SOCKET sktCli = init("192.168.3.58", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}

	char buf[1024];
	while (true) {
		printf(">>");
		scanf("%s", buf);
		send(sktCli, buf, strlen(buf) + 1, 0);//向服务器发送数据

		recv(sktCli, buf, sizeof(buf), 0);//接受服务器发送的数据
		printf("[server] %s\n", buf);
	}

	destroy(sktCli);
	return 0;
}

void printMenu() {
	printf("*************************\n");
	printf("*getName                *\n");
	printf("*getAge                 *\n");
	printf("*exit                   *\n");
	printf("*************************\n");
}

//socket客户端代码 客户端发送命令 接受服务器发送的消息
int function06() {
	SOCKET sktCli = init("192.168.3.18", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}

	while (true) {
		printMenu();
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

struct DataPackage {
	int age;
	char name[32];
};
//结构化的网络消息
int function07() {
	SOCKET sktCli = init("192.168.3.18", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}

	while (true) {
		printMenu();
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
		printf("[server] msg: age=%d name=%s\n", p->age, p->name);
	}

	destroy(sktCli);
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
//客户端完善结构化消息
int function08() {
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

		DataHeader dh = {0};
		if (strcmp(szCmd, "login") == 0) {
			Login login;
			strcpy(login.szName, "爱白菜的小昆虫登录");
			strcpy(login.szPass, "我不知道密码是啥");
			send(sktCli, (const char *)&login, sizeof(login), 0);
			LoginResult result;
			recv(sktCli, (char *)&result, sizeof(result), 0);
			printf("[server] cmd=%d dataLength=%d iResult=%d\n", result.cmd, result.dataLength, result.iResult);
		}
		else if(strcmp(szCmd, "logout") == 0){
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
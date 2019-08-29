
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

//socket简单的客户端
int function01() {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		cout << "WSAStartup error ..." << endl;
		return -1;
	}
	cout << "WSAStartup success ..." << endl;

	SOCKET _cSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_cSocket == SOCKET_ERROR) {
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;

	sockaddr_in caddr;
	caddr.sin_family = AF_INET;
	caddr.sin_port = htons(8000);
	caddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (connect(_cSocket, (sockaddr *)&caddr, sizeof(caddr)) == SOCKET_ERROR) {
		cout << "connect error ..." << endl;
		return -1;
	}
	cout << "connect success ..." << endl;

	char buf[1024];
	memset(buf, 0, sizeof(buf));
	if (recv(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
		cout << "recv error ..." << endl;
		return -1;
	}
	cout << "recv success ..." << endl;
	
	cout << "server msg : " << buf << endl;

	cout << "input msg >>";
	cin >> buf;
	send(_cSocket, buf, strlen(buf), 0);

	WSACleanup();
	system("pause");
	return 0;
}

struct Message {
	int iCmd;//指令
	char szMsg[1024];//消息
};
//socket客户端传输结构体
int function02() {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		cout << "WSAStartup error ..." << endl;
		return -1;
	}
	cout << "WSAStartup success ..." << endl;

	SOCKET _cSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_cSocket == SOCKET_ERROR) {
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;

	sockaddr_in caddr;
	caddr.sin_family = AF_INET;
	caddr.sin_port = htons(8000);
	caddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (connect(_cSocket, (sockaddr *)&caddr, sizeof(caddr)) == SOCKET_ERROR) {
		cout << "connect error ..." << endl;
		return -1;
	}
	cout << "connect success ..." << endl;

	char buf[1024];
	memset(buf, 0, sizeof(buf));
	if (recv(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
		cout << "recv error ..." << endl;
		return -1;
	}
	cout << "recv success ..." << endl;
	
	cout << "server msg : " << buf << endl;

	cout << "input msg >>";
	//cin >> buf;
	struct Message msg = {2333, "爱白菜的小昆虫是客户端"};
	if (send(_cSocket, (const char*)& msg, sizeof(msg), 0) == SOCKET_ERROR) {
		cout << "send error ..." << endl;
		return -1;
	}
	cout << "send success ..." << endl;

	WSACleanup();
	system("pause");
	return 0;
}


//socket客户端 客户端与服务端的数据交互
int function03() {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		cout << "WSAStartup error ..." << endl;
		return -1;
	}
	cout << "WSAStartup success ..." << endl;

	SOCKET _cSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_cSocket == SOCKET_ERROR) {
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;

	sockaddr_in caddr;
	caddr.sin_family = AF_INET;
	caddr.sin_port = htons(8000);
	caddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (connect(_cSocket, (sockaddr *)&caddr, sizeof(caddr)) == SOCKET_ERROR) {
		cout << "connect error ..." << endl;
		return -1;
	}
	cout << "connect success ..." << endl;

	char buf[1024];
	memset(buf, 0, sizeof(buf));
	if (recv(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
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
			if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
				cout << "send error ..." << endl;
				return -1;
			}
			cout << "send success ..." << endl;
			if (recv(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
				cout << "recv error ..." << endl;
			}
			cout << "recv success ..." << endl;
			cout << "server : " << msgBuf << endl;
		} else if (strcmp(buf, "getName") == 0) {
			if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
				cout << "send error ..." << endl;
				return -1;
			}
			if (recv(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
				cout << "recv error ..." << endl;
			}
			cout << "recv success ..." << endl;
			cout << "server : " << msgBuf << endl;
		} else if (strcmp(buf, "getAge") == 0) {
			if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
				cout << "send error ..." << endl;
				return -1;
			}
			if (recv(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
				cout << "recv error ..." << endl;
			}
			cout << "recv success ..." << endl;
			cout << "server : " << msgBuf << endl;
		} else if (strcmp(buf, "SignOut") == 0) {
			if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
				cout << "send error ..." << endl;
				return -1;
			}
			cout << "send success ..." << endl;
			if (recv(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
				cout << "recv error ..." << endl;
			}
			cout << "recv success ..." << endl;
			cout << "server : " << msgBuf << endl;
		} else if (strcmp(buf, "Quit") == 0) {
			if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
				cout << "send error ..." << endl;
				return -1;
			}
			cout << "send success ..." << endl;
			if (recv(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
				cout << "recv error ..." << endl;
			}
			cout << "recv success ..." << endl;
			cout << "server : " << msgBuf << endl;
			break;
		} else {
			cout << "iCmd error ..." << endl;
		}
	}


	WSACleanup();
	system("pause");
	return 0;
}

//socket客户端 客户端与服务端的数据交互 用结构体
int function04() {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		cout << "WSAStartup error ..." << endl;
		return -1;
	}
	cout << "WSAStartup success ..." << endl;
	
	SOCKET _cSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_cSocket == SOCKET_ERROR) {
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;
	
	sockaddr_in caddr;
	caddr.sin_family = AF_INET;
	caddr.sin_port = htons(8000);
	caddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	
	if (connect(_cSocket, (sockaddr*)& caddr, sizeof(caddr)) == SOCKET_ERROR) {
		cout << "connect error ..." << endl;
		return -1;
	}
	cout << "connect success ..." << endl;
	
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	if (recv(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
		cout << "recv error ..." << endl;
		return -1;
	}
	cout << "recv success ..." << endl;
	cout << "server msg : " << buf << endl;
	
	while (true) {
		cout << "input cmd >>";
		cin >> buf;
		char msgBuf[1024] = "";
		if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
			cout << "send error ..." << endl;
			return -1;
		}
		cout << "send success ..." << endl;
		if (recv(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
			cout << "recv error ..." << endl;
		}
		cout << "recv success ..." << endl;
		cout << "server : " << msgBuf << endl;
	}
	
	WSACleanup();
	system("pause");
}

//socket简单的客户端代码
int function05() {
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//创建客户端套接字
	SOCKET sktCli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addrSer = {0};
	addrSer.sin_family = AF_INET;//要接的服务器的IPV4协议
	addrSer.sin_port = htons(8080);//要连接的服务器的端口
	addrSer.sin_addr.s_addr = inet_addr("192.168.3.58");//要连接的服务器的IP
	//连接服务器
	connect(sktCli, (sockaddr *)&addrSer, sizeof(addrSer));

	char buf[1024];
	while (true) {
		printf(">>");
		scanf("%s", buf);
		send(sktCli, buf, strlen(buf) + 1, 0);//向服务器发送数据

		recv(sktCli, buf, sizeof(buf), 0);//接受服务器发送的数据
		printf("[server] %s\n", buf);
	}

	WSACleanup();
	return 0;
}

void printMenu() {
	printf("*************************\n");
	printf("*1.send message         *\n");
	printf("*0.quit process         *\n");
	printf("*************************\n");
}

//socket客户端代码 将代码完善
int function06() {
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//创建客户端套接字
	SOCKET sktCli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sktCli == INVALID_SOCKET) {
		printf("[client] socket error ...\n");
		exit(1);
	}
	printf("[client] socket success ...\n");

	//需要连接的服务器信息
	sockaddr_in addrSer = { 0 };
	addrSer.sin_family = AF_INET;//要接的服务器的IPV4协议
	addrSer.sin_port = htons(8080);//要连接的服务器的端口
	addrSer.sin_addr.s_addr = inet_addr("192.168.3.58");//要连接的服务器的IP
	//连接服务器
	if (connect(sktCli, (sockaddr *)&addrSer, sizeof(addrSer)) == SOCKET_ERROR) {
		printf("[client] connect error ...\n");
		exit(1);
	}
	printf("[client] connect success ...\n");

	char buf[1024];
	int n;
	while (true) {
		printMenu();
		printf(">>");
		int ret = scanf("%d", &n);
		if (ret < 1) {//非法输入
			break;
		}
		if (n == 0) {//退出程序
			printf("[client] quit ...\n");
			break;
		}
		printf("input message>>");
		scanf("%s", buf);
		//向服务器发送数据
		if (send(sktCli, buf, strlen(buf) + 1, 0) == SOCKET_ERROR) {
			printf("[client] send error ...\n");
			exit(1);
		}
		printf("[client] send success ...\n");

		//接受服务器发送的数据
		int len = recv(sktCli, buf, sizeof(buf), 0);
		if (len == -1) {//接受错误
			printf("[client] recv error ...\n");
			exit(1);
		}
		if (len == 0) {//服务器退出
			printf("[server] quit ...\n");
			break;
		}
		printf("[server] %s\n", buf);//打印服务器的信息
	}

	closesocket(sktCli);//关闭套接字
	WSACleanup();
	return 0;
}
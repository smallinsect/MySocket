
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

//ip��ַ �˿�
SOCKET init(const char *IP, u_short port) {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		cout << "WSAStartup error ..." << endl;
		return INVALID_SOCKET;
	}
	cout << "WSAStartup success ..." << endl;

	//�����׽���
	SOCKET skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == SOCKET_ERROR) {
		cout << "socket error ..." << endl;
		return INVALID_SOCKET;
	}
	cout << "socket success ..." << endl;

	//�������Ϣ
	sockaddr_in addrServ;
	addrServ.sin_family = AF_INET;//IPV4Э��
	addrServ.sin_port = htons(port);//�˿�
	addrServ.sin_addr.s_addr = inet_addr(IP);//IP��ַ

	//���ӷ�����
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

//socket�򵥵Ŀͻ���
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
	int iCmd;//ָ��
	char szMsg[1024];//��Ϣ
};
//socket�ͻ��˴���ṹ��
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
	struct Message msg = {2333, "���ײ˵�С�����ǿͻ���"};
	if (send(sktCli, (const char*)& msg, sizeof(msg), 0) == SOCKET_ERROR) {
		cout << "send error ..." << endl;
		return -1;
	}
	cout << "send success ..." << endl;

	destroy(sktCli);
	return 0;
}

//socket�ͻ��� �ͻ��������˵����ݽ���
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

//socket�ͻ��� �ͻ��������˵����ݽ��� �ýṹ��
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

//socket�򵥵Ŀͻ��˴���
int function05() {
	SOCKET sktCli = init("192.168.3.58", 8080);
	if (sktCli == INVALID_SOCKET) {
		return -1;
	}

	char buf[1024];
	while (true) {
		printf(">>");
		scanf("%s", buf);
		send(sktCli, buf, strlen(buf) + 1, 0);//���������������

		recv(sktCli, buf, sizeof(buf), 0);//���ܷ��������͵�����
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

//socket�ͻ��˴��� �ͻ��˷������� ���ܷ��������͵���Ϣ
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
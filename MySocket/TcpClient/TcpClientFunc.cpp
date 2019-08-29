
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

//socket�򵥵Ŀͻ���
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
	int iCmd;//ָ��
	char szMsg[1024];//��Ϣ
};
//socket�ͻ��˴���ṹ��
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
	struct Message msg = {2333, "���ײ˵�С�����ǿͻ���"};
	if (send(_cSocket, (const char*)& msg, sizeof(msg), 0) == SOCKET_ERROR) {
		cout << "send error ..." << endl;
		return -1;
	}
	cout << "send success ..." << endl;

	WSACleanup();
	system("pause");
	return 0;
}


//socket�ͻ��� �ͻ��������˵����ݽ���
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

//socket�ͻ��� �ͻ��������˵����ݽ��� �ýṹ��
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

//socket�򵥵Ŀͻ��˴���
int function05() {
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//�����ͻ����׽���
	SOCKET sktCli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addrSer = {0};
	addrSer.sin_family = AF_INET;//Ҫ�ӵķ�������IPV4Э��
	addrSer.sin_port = htons(8080);//Ҫ���ӵķ������Ķ˿�
	addrSer.sin_addr.s_addr = inet_addr("192.168.3.58");//Ҫ���ӵķ�������IP
	//���ӷ�����
	connect(sktCli, (sockaddr *)&addrSer, sizeof(addrSer));

	char buf[1024];
	while (true) {
		printf(">>");
		scanf("%s", buf);
		send(sktCli, buf, strlen(buf) + 1, 0);//���������������

		recv(sktCli, buf, sizeof(buf), 0);//���ܷ��������͵�����
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

//socket�ͻ��˴��� ����������
int function06() {
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//�����ͻ����׽���
	SOCKET sktCli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sktCli == INVALID_SOCKET) {
		printf("[client] socket error ...\n");
		exit(1);
	}
	printf("[client] socket success ...\n");

	//��Ҫ���ӵķ�������Ϣ
	sockaddr_in addrSer = { 0 };
	addrSer.sin_family = AF_INET;//Ҫ�ӵķ�������IPV4Э��
	addrSer.sin_port = htons(8080);//Ҫ���ӵķ������Ķ˿�
	addrSer.sin_addr.s_addr = inet_addr("192.168.3.58");//Ҫ���ӵķ�������IP
	//���ӷ�����
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
		if (ret < 1) {//�Ƿ�����
			break;
		}
		if (n == 0) {//�˳�����
			printf("[client] quit ...\n");
			break;
		}
		printf("input message>>");
		scanf("%s", buf);
		//���������������
		if (send(sktCli, buf, strlen(buf) + 1, 0) == SOCKET_ERROR) {
			printf("[client] send error ...\n");
			exit(1);
		}
		printf("[client] send success ...\n");

		//���ܷ��������͵�����
		int len = recv(sktCli, buf, sizeof(buf), 0);
		if (len == -1) {//���ܴ���
			printf("[client] recv error ...\n");
			exit(1);
		}
		if (len == 0) {//�������˳�
			printf("[server] quit ...\n");
			break;
		}
		printf("[server] %s\n", buf);//��ӡ����������Ϣ
	}

	closesocket(sktCli);//�ر��׽���
	WSACleanup();
	return 0;
}
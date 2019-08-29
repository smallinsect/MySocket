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
	int iCmd;//ָ��
	char szMsg[1024];//��Ϣ
};
enum CMD {
	CMD_ERROR,
	CMD_SIGN_IN,
	CMD_SIGN_OUT,
};
//��Ϣͷ
struct DataHeader {
	int iCmd;//����
	int iDataLength;//���ݳ���
};
//��¼
struct SignIn {
	char szUserName[32];
	char szPassWord[32];
};
//�ǳ�
struct SignOut {
	int iResult;
};

//����ֵ
struct SignResult {
	int iResult;
};

//socket�򵥵ķ����
int function01() {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		cout << "WSAStartup error ..." << endl;
		return -1;
	}
	cout << "WSAStartup success ..." << endl;

	SOCKET _sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sSocket == SOCKET_ERROR) {
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;

	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (bind(_sSocket, (sockaddr *)&saddr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		cout << "bind error ..." << endl;
		return -1;
	}
	cout << "bind success ..." << endl;

	if (listen(_sSocket, 5) == SOCKET_ERROR) {
		cout << "listen error ..." << endl;
		return -1;
	}
	cout << "listen success ..." << endl;

	while (true) {
		sockaddr_in caddr;
		int caddrlen = sizeof(sockaddr_in);
		SOCKET _cSocket = accept(_sSocket, (sockaddr *)& caddr, &caddrlen);
		if (_cSocket == SOCKET_ERROR) {
			cout << "accept error ..." << endl;
			return -1;
		}
		cout << "accept success ..." << endl;

		cout << "client ip : " << inet_ntoa(caddr.sin_addr) << endl;
		cout << "client port : " << caddr.sin_port << endl;
		cout << "client size : " << sizeof(caddr) << endl;

		char buf[1024] = "���ײ˵�С����������յ���Ϣ��";
		send(_cSocket, buf, strlen(buf), 0);

		memset(buf, 0, sizeof(buf));
		recv(_cSocket, buf, sizeof(buf), 0);
		cout << "recv client msg : " << buf << endl;
	}


	cout << "server exit ..." << endl;
	closesocket(_sSocket);
	WSACleanup();

	system("pause");
	return 0;
}

//socket����˴���ṹ��
int function02() {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		cout << "WSAStartup error ..." << endl;
		return -1;
	}
	cout << "WSAStartup success ..." << endl;

	SOCKET _sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sSocket == SOCKET_ERROR) {
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;

	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (bind(_sSocket, (sockaddr *)&saddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		cout << "bind error ..." << endl;
		return -1;
	}
	cout << "bind success ..." << endl;

	if (listen(_sSocket, 5) == SOCKET_ERROR) {
		cout << "listen error ..." << endl;
		return -1;
	}
	cout << "listen success ..." << endl;

	while (true) {
		SOCKADDR_IN caddr;
		int caddrlen = sizeof(SOCKADDR_IN);
		SOCKET _cSocket = accept(_sSocket, (sockaddr *)& caddr, &caddrlen);
		if (_cSocket == SOCKET_ERROR) {
			cout << "accept error ..." << endl;
			return -1;
		}
		cout << "accept success ..." << endl;

		cout << "client ip : " << inet_ntoa(caddr.sin_addr) << endl;
		cout << "client port : " << caddr.sin_port << endl;
		cout << "client size : " << sizeof(caddr) << endl;

		char buf[1024] = "���ײ˵�С����������յ���Ϣ��";
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
	closesocket(_sSocket);
	WSACleanup();

	system("pause");
	return 0;
}
int function03() {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		cout << "WSAStartup error ..." << endl;
		return -1;
	}
	cout << "WSAStartup success ..." << endl;

	SOCKET _sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sSocket == SOCKET_ERROR) {
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;

	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (bind(_sSocket, (sockaddr *)&saddr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		cout << "bind error ..." << endl;
		return -1;
	}
	cout << "bind success ..." << endl;

	if (listen(_sSocket, 5) == SOCKET_ERROR) {
		cout << "listen error ..." << endl;
		return -1;
	}
	cout << "listen success ..." << endl;

	while (true) {
		sockaddr_in caddr;
		int caddrlen = sizeof(sockaddr_in);
		SOCKET _cSocket = accept(_sSocket, (sockaddr *)& caddr, &caddrlen);
		if (_cSocket == SOCKET_ERROR) {
			cout << "accept error ..." << endl;
			return -1;
		}
		cout << "accept success ..." << endl;

		cout << "client ip : " << inet_ntoa(caddr.sin_addr) << endl;
		cout << "client port : " << caddr.sin_port << endl;
		cout << "client size : " << sizeof(caddr) << endl;

		char buf[1024] = "���ײ˵�С����������յ���Ϣ��";
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
				char msgBuf[] = "��¼�ɹ�";
				if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
					cout << "send error ..." << endl;
					return -1;
				}
				cout << "send success ..." << endl;
			}
			else if (strcmp(buf, "getName") == 0) {
				char msgBuf[] = "С����";
				if (send(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
					cout << "send error ..." << endl;
					return -1;
				}
			}
			else if (strcmp(buf, "getAge") == 0) {
				char msgBuf[] = "10000��";
				if (send(_cSocket, msgBuf, sizeof(msgBuf), 0) == SOCKET_ERROR) {
					cout << "send error ..." << endl;
					return -1;
				}
			}
			else if (strcmp(buf, "SignOut") == 0) {
				char msgBuf[] = "�˳���¼";
				if (send(_cSocket, buf, sizeof(buf), 0) == SOCKET_ERROR) {
					cout << "send error ..." << endl;
					return -1;
				}
				cout << "send success ..." << endl;
			}
			else if (strcmp(buf, "Quit") == 0) {
				char msgBuf[] = "�˳�����";
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
	closesocket(_sSocket);
	WSACleanup();

	system("pause");
}

//�������ͻ��˵����ݽ��� �ýṹ��
int function04() {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		cout << "WSAStartup error ..." << endl;
		return -1;
	}
	cout << "WSAStartup success ..." << endl;
	
	SOCKET _sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sSocket == SOCKET_ERROR) {
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;
	
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (bind(_sSocket, (sockaddr*)& saddr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		cout << "bind error ..." << endl;
		return -1;
	}
	cout << "bind success ..." << endl;
	
	if (listen(_sSocket, 5) == SOCKET_ERROR) {
		cout << "listen error ..." << endl;
		return -1;
	}
	cout << "listen success ..." << endl;
	
	while (true) {
		sockaddr_in caddr;
		int caddrlen = sizeof(sockaddr_in);
		SOCKET _cSocket = accept(_sSocket, (sockaddr*)& caddr, &caddrlen);
		if (_cSocket == SOCKET_ERROR) {
			cout << "accept error ..." << endl;
			return -1;
		}
		cout << "accept success ..." << endl;
	
		cout << "client ip : " << inet_ntoa(caddr.sin_addr) << endl;
		cout << "client port : " << caddr.sin_port << endl;
		cout << "client size : " << sizeof(caddr) << endl;
	
		char buf[1024] = "���ײ˵�С����������յ���Ϣ��";
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
	closesocket(_sSocket);
	WSACleanup();
	
	system("pause");
	return 0;
}

int function05() {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		cout << "WSAStartup error ..." << endl;
		return -1;
	}
	cout << "WSAStartup success ..." << endl;

	SOCKET _sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sSocket == SOCKET_ERROR) {
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;

	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (bind(_sSocket, (PSOCKADDR)& saddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		cout << "bind error ..." << endl;
		return -1;
	}
	cout << "bind success ..." << endl;

	if (listen(_sSocket, 5) == SOCKET_ERROR) {
		cout << "listen error ..." << endl;
		return -1;
	}
	cout << "listen success ..." << endl;

	while (true) {
		SOCKADDR_IN caddr;
		int caddrlen = sizeof(SOCKADDR_IN);
		SOCKET _cSocket = accept(_sSocket, (PSOCKADDR)& caddr, &caddrlen);
		if (_cSocket == SOCKET_ERROR) {
			cout << "accept error ..." << endl;
			return -1;
		}
		cout << "accept success ..." << endl;

		cout << "client ip : " << inet_ntoa(caddr.sin_addr) << endl;
		cout << "client port : " << caddr.sin_port << endl;
		cout << "client size : " << sizeof(caddr) << endl;

		char buf[1024] = "���ײ˵�С����������յ���Ϣ��";
		send(_cSocket, buf, strlen(buf), 0);

		memset(buf, 0, sizeof(buf));
		recv(_cSocket, buf, sizeof(buf), 0);
		cout << "recv client msg : " << buf << endl;
	}


	cout << "server exit ..." << endl;
	closesocket(_sSocket);
	WSACleanup();

	system("pause");
	return 0;
}
int function06() {
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//�����������׽���
	SOCKET sktSer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addrSer = { 0 };
	addrSer.sin_family = AF_INET;//��������IPV4Э��
	addrSer.sin_port = htons(8080);//�������Ķ˿�
	addrSer.sin_addr.s_addr = inet_addr("127.0.0.1");//��������IP

	//���׽��ֺͶ˿ڡ�ip��
	int i = bind(sktSer, (sockaddr *)&addrSer, sizeof(addrSer));

	//��ʼ�����׽���
	listen(sktSer, 5);

	//���ܿͻ����׽���
	SOCKET sktCli = accept(sktSer, NULL, NULL);
	char buf[1024];
	while (true) {
		recv(sktCli, buf, sizeof(buf), 0);//���ܿͻ��˷��͵�����
		printf("[client] %s\n", buf);

		send(sktCli, buf, strlen(buf) + 1, 0);//��ͻ��˷�������
	}

	WSACleanup();
}
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

//��ʼ��
SOCKET init(const char *IP, u_short port) {
	WSADATA wd;
	int ret = WSAStartup(MAKEWORD(2, 2), &wd);
	if (ret != 0) {
		cout << "WSAStartup error ..." << endl;
		return -1;
	}
	cout << "WSAStartup success ..." << endl;

	//�����׽���
	SOCKET skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == INVALID_SOCKET) {
		cout << "socket error ..." << endl;
		return -1;
	}
	cout << "socket success ..." << endl;
	//��������Ϣ
	sockaddr_in addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(port);
	addrServ.sin_addr.s_addr = inet_addr(IP);

	//�׽��ְ󶨶˿ں�ip
	if (bind(skt, (sockaddr *)&addrServ, sizeof(addrServ)) == SOCKET_ERROR) {
		cout << "bind error ..." << endl;
		return -1;
	}
	cout << "bind success ..." << endl;

	//��ʼ�����׽���
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

//socket�򵥵ķ����
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

		char buf[1024] = "���ײ˵�С����������յ���Ϣ��";
		send(_cSocket, buf, strlen(buf), 0);

		memset(buf, 0, sizeof(buf));
		recv(_cSocket, buf, sizeof(buf), 0);
		cout << "recv client msg : " << buf << endl;
	}

	cout << "server exit ..." << endl;

	destroy(sktServ);
	return 0;
}

//socket����˴���ṹ��
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

	//	char buf[1024] = "���ײ˵�С����������յ���Ϣ��";
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

	destroy(sktServ);
	return 0;
}

//�������ͻ��˵����ݽ��� �ýṹ��
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
	//	char buf[1024] = "���ײ˵�С����������յ���Ϣ��";
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

		char buf[1024] = "���ײ˵�С����������յ���Ϣ��";
		send(_cSocket, buf, strlen(buf), 0);

		memset(buf, 0, sizeof(buf));
		recv(_cSocket, buf, sizeof(buf), 0);
		cout << "recv client msg : " << buf << endl;
	}

	cout << "server exit ..." << endl;

	destroy(sktServ);
	return 0;
}

//���ܿͻ��˷��͵����� ������Ϣ���ͻ���
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
			sprintf(szMsg, "%s", "���ײ˵�С����.");
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

//�ṹ����������Ϣ
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
			DataPackage dp = {1000, "���ײ˵�С����"};
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

//��������
enum CMD{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};
//��Ϣͷ
struct DataHeader {
	int cmd;//��Ϣָ��
	int dataLength;//��Ϣ����
};
//��¼��Ϣ
struct Login : public DataHeader{
	Login() {
		cmd = CMD_LOGIN;
		dataLength = sizeof(Login);
	}
	char szName[64];//�û���
	char szPass[64];//����
};
//��¼������Ϣ
struct LoginResult : public DataHeader {
	LoginResult() {
		cmd = CMD_LOGIN_RESULT;
		dataLength = sizeof(LoginResult);
		iResult = 1;
	}
	int iResult;
};
//�ǳ���Ϣ
struct Logout : public DataHeader {
	Logout() {
		cmd = CMD_LOGOUT;
		dataLength = sizeof(Logout);
	}
	char szName[64];
};
//�ǳ�������Ϣ
struct LogoutResult : public DataHeader {
	LogoutResult() {
		cmd = CMD_LOGOUT_RESULT;
		dataLength = sizeof(LogoutResult);
		iResult = 2;
	}
	int iResult;
};
//��������ƽṹ����Ϣ
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
			//�ͻ��˷���login�ṹ�壬���Ƿ����ǰ�������DataHeader����������ֻ��Ҫ����ʣ�µ���Ϣ
			recv(sktCli, (char *)&login + sizeof(DataHeader), sizeof(login) - sizeof(DataHeader), 0);
			printf("[client] name=%s pass=%s\n", login.szName, login.szPass);

			LoginResult result;
			send(sktCli, (const char *)&result, sizeof(result), 0);
			break;
		}
		case CMD_LOGOUT: {
			Logout logout;
			//�ͻ��˷���login�ṹ�壬���Ƿ����ǰ�������DataHeader����������ֻ��Ҫ����ʣ�µ���Ϣ
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
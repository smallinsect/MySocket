
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
	sockaddr_in addrServ = {0};
	addrServ.sin_family = AF_INET;//IPV4Э��
	addrServ.sin_port = htons(port);//�˿�
	addrServ.sin_addr.s_addr = inet_addr(IP);//IP��ַ

	//���ӷ�����
	if (connect(skt, (sockaddr *)&addrServ, sizeof(addrServ)) == SOCKET_ERROR) {
		printf("%d\n", GetLastError());
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


struct DataPackage {
	int iAge;
	char szName[32];
};
//�ṹ����������Ϣ
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

//��������
enum CMD {
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
struct Login : public DataHeader {
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
//�ͻ������ƽṹ����Ϣ
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
			strcpy(login.szName, "���ײ˵�С�����¼");
			strcpy(login.szPass, "�Ҳ�֪��������ɶ");
			send(sktCli, (const char *)&login, sizeof(login), 0);
			LoginResult result;
			recv(sktCli, (char *)&result, sizeof(result), 0);
			printf("[server] cmd=%d dataLength=%d iResult=%d\n", result.cmd, result.dataLength, result.iResult);
		}
		else if (strcmp(szCmd, "logout") == 0) {
			Logout logout;
			strcpy(logout.szName, "���ײ˵�С�����˳�");
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

//�ͻ���д����
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

//socket�ͻ��� ʹ���߳̽��ܺͷ�����Ϣ
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
		GetExitCodeThread(hThread, &exitCode);//��ȡִ����

		if (exitCode != STILL_ACTIVE) {//�߳�������
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

//socket�򵥵Ŀͻ��˴���
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

//socket�ͻ��˴��� �ͻ��˷������� ���ܷ��������͵���Ϣ
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

int function07() {
	return 0;
}

int function08() {
	return 0;
}
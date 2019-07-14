/*************************************************************
**socket�����
**
**�������ͻ��˵����ݽ��� �ýṹ��
**
**
**
**************************************************************/

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#include <iostream>
using namespace std;

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
int main() {

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
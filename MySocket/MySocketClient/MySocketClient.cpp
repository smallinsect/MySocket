#include "MySocketClient.h"



int MySocketClient::_connect(const char* ip, u_short port) {
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		cout << "WSAStartup error ..." << GetLastError() << endl;
		return -1;
	}
	cout << "WSAStartup success ..." << endl;

	m_skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_skt == INVALID_SOCKET) {
		cout << "[client] socket error ..." << GetLastError() << endl;
		return -1;
	}
	cout << "[client] socket success ..." << endl;

	sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);

	if (connect(m_skt, (sockaddr*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		cout << "[client] connect error ..." << GetLastError() << endl;
		return -1;
	}
	cout << "[client] connect success ..." << endl;

	m_bRun = true;
	m_threadid = _beginthread(_workthread, 0, this);
	return 0;
}

void MySocketClient::_stop() {
	m_bRun = false;
	closesocket(m_skt);
	WSACleanup();
}

void MySocketClient::_workthread(void* lpParam) {
	MySocketClient* client = (MySocketClient*)lpParam;

	while (client->m_bRun) {
		char cmdBuf[1024] = "";
		cout << ">>";
		cin >> cmdBuf;
		if (strcmp(cmdBuf, "exit") == 0) {
			cout << "exit client ..." << endl;
			break;
		}
		else if (strcmp(cmdBuf, "login") == 0) {
			Login login;
			strcpy(login.userName, "°®°×²Ë");
			strcpy(login.passWord, "Ð¡À¥³æ");
			send(client->m_skt, (const char *)&login, sizeof(Login), 0);

			LoginResult loginRet;
			recv(client->m_skt, (char *)&loginRet, sizeof(LoginResult), 0);

			cout << "LoginResult: " << loginRet.result << endl;
		}
		else if (strcmp(cmdBuf, "logout") == 0) {
			Logout logout;
			strcpy(logout.userName, "°®°×²ËµÄÐ¡À¥³æ");
			send(client->m_skt, (const char *)&logout, sizeof(Logout), 0);

			LogoutResult logoutRet;
			recv(client->m_skt, (char *)&logoutRet, sizeof(LoginResult), 0);

			cout << "LogoutResult: " << logoutRet.result << endl;
		}
		else {
			cout << "cmd error" << endl;
		}
	}
	cout << "thread " << client->m_threadid << " end ..." << endl;
}


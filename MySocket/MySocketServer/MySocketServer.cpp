#include "MySocketServer.h"

int MySocketServer::_start(const char* ip, u_short port) {
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		cout << "[server] WSAStartup error ..." << GetLastError() << endl;
		return -1;
	}
	cout << "[server] WSAStartup success ..." << GetLastError() << endl;

	m_skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_skt == INVALID_SOCKET) {
		cout << "[server] socket error ..." << endl;
		return -1;
	}
	cout << "[server] socket success ..." << endl;

	sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);

	if (bind(m_skt, (sockaddr*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		cout << "[server] bind error ..." << endl;
		return -1;
	}
	cout << "[server] bind success ..." << endl;

	if (listen(m_skt, SOMAXCONN) == SOCKET_ERROR) {
		cout << "[server] listen error ..." << endl;
		return -1;
	}
	cout << "[server] listen success ..." << endl;

	FD_ZERO(&m_fds);
	FD_SET(m_skt, &m_fds);

	m_bRun = true;
	m_threadid = _beginthread(_workthread, 0, this);
	return 0;
}

void MySocketServer::_stop() {
	m_bRun = false;
	closesocket(m_skt);
	WSACleanup();
}

void MySocketServer::_workthread(void* lpParam) {
	MySocketServer* server = (MySocketServer*)lpParam;

	timeval tv = {0, 1000};
	while (server->m_bRun) {
		fd_set readfds = server->m_fds;

		if (select(0, &readfds, NULL, NULL, &tv) <= 0) {
			continue;
		}
		for (u_int i = 0; i < readfds.fd_count; ++i) {
			SOCKET tskt = readfds.fd_array[i];
			if (tskt == server->m_skt) {//有新的客户端连接
				SOCKET cskt = accept(tskt, NULL, NULL);
				if (cskt == INVALID_SOCKET) {
					cout << "[server] accept error ..." << GetLastError() << endl;
				}
				cout << "[server] accopet success ..." << endl;

				FD_SET(cskt, &server->m_fds);
				continue;
			}
			if (server->_dispatch(tskt) != 0) {
				FD_CLR(tskt, &server->m_fds);
				closesocket(tskt);
			}
		}
	}
	cout << "thread " << server->m_threadid << " end ..." << endl;
}

int MySocketServer::_dispatch(SOCKET skt) {
	DataHeader header = {};
	int nLen = recv(skt, (char *)&header, sizeof(DataHeader), 0);
	if (nLen == SOCKET_ERROR) {
		cout << "recv error ..." << endl;
		return -1;
	}
	cout << "recv success ..." << endl;
	cout << "client cmd : " << header.cmd << endl;
	switch (header.cmd) {
	case CMD_LOGIN:
	{
		Login login;
		recv(skt, (char *)&login + sizeof(DataHeader), sizeof(Login) - sizeof(DataHeader), 0);
		cout << "recv cmd : " << login.cmd << endl;
		cout << "recv len : " << login.dataLength << endl;
		cout << "userName : " << login.userName << endl;
		cout << "passWord : " << login.passWord << endl;
		LoginResult ret;
		send(skt, (const char *)&ret, sizeof(LoginResult), 0);
		break;
	}
	case CMD_LOGOUT:
	{
		Logout Logout;
		recv(skt, (char *)&Logout + sizeof(DataHeader), sizeof(Logout) - sizeof(DataHeader), 0);
		cout << "recv cmd : " << Logout.cmd << endl;
		cout << "recv len : " << Logout.dataLength << endl;
		cout << "userName : " << Logout.userName << endl;
		LogoutResult ret;
		send(skt, (const char *)&ret, sizeof(LogoutResult), 0);
		break;
	}
	default:
	{
		header.cmd = CMD_ERROR;
		header.dataLength = 0;
		send(skt, (char *)&header, sizeof(DataHeader), 0);
		break;
	}
	}

	return 0;
}
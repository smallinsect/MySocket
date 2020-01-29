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
	//m_threadid = _beginthread(_workthread, 0, this);
	return 0;
}

void MySocketServer::_stop() {
	m_bRun = false;
	_close();
	WSACleanup();
}

void MySocketServer::_workthread(void* lpParam) {
	MySocketServer* server = (MySocketServer*)lpParam;

	timeval tv = {0, 1000};
	while (server->m_bRun) {
		fd_set readfds;
		FD_ZERO(&readfds);

		FD_SET(server->m_skt, &readfds);
		SOCKET maxSocket = server->m_skt;
		for (int i = (int)server->m_vSockets.size() - 1; i >= 0; --i) {
			FD_SET(server->m_vSockets[i]->getSocket(), &readfds);
			if (maxSocket < server->m_vSockets[i]->getSocket()) {
				maxSocket = server->m_vSockets[i]->getSocket();
			}
		}

		if (select((int)(maxSocket+1), &readfds, NULL, NULL, &tv) <= 0) {
			continue;
		}

		if (FD_ISSET(server->m_skt, &readfds)) {
			FD_CLR(server->m_skt, &readfds);
			server->_accept();
		}

		for (int i = (int)server->m_vSockets.size() - 1; i >= 0; --i) {
			SOCKET skt = server->m_vSockets[i]->getSocket();
			if (FD_ISSET(skt, &readfds)) {
				if (server->_recvdata(server->m_vSockets[i]) == -1) {
					auto iter = server->m_vSockets.begin() + i;
					if (iter != server->m_vSockets.end()) {
						delete server->m_vSockets[i];
						server->m_vSockets.erase(iter);
					}
				}
			}
		}
	}
	cout << "thread " << server->m_threadid << " end ..." << endl;
}

void MySocketServer::_work() {
	timeval tv = { 0, 1000 };
	while (m_bRun) {
		fd_set readfds;
		FD_ZERO(&readfds);

		FD_SET(m_skt, &readfds);
		SOCKET maxSocket = m_skt;
		for (int i = (int)m_vSockets.size() - 1; i >= 0; --i) {
			FD_SET(m_vSockets[i]->getSocket(), &readfds);
			if (maxSocket < m_vSockets[i]->getSocket()) {
				maxSocket = m_vSockets[i]->getSocket();
			}
		}

		if (select((int)maxSocket + 1, &readfds, NULL, NULL, &tv) <= 0) {
			continue;
		}

		if (FD_ISSET(m_skt, &readfds)) {
			FD_CLR(m_skt, &readfds);
			_accept();
		}

		for (int i = (int)m_vSockets.size() - 1; i >= 0; --i) {
			SOCKET skt = m_vSockets[i]->getSocket();
			if (FD_ISSET(skt, &readfds)) {
				if (_recvdata(m_vSockets[i]) == -1) {
					auto iter = m_vSockets.begin() + i;
					if (iter != m_vSockets.end()) {
						delete m_vSockets[i];
						m_vSockets.erase(iter);
					}
				}
			}
		}
	}
	cout << "thread " << m_threadid << " end ..." << endl;
}

SOCKET MySocketServer::_accept() {
	sockaddr_in addr = {0};
	int addrlen = sizeof(addr);
	SOCKET skt = accept(m_skt, (sockaddr*)&addr, &addrlen);
	if (skt == INVALID_SOCKET) {
		printf("socket=<%lld>错误，接收到无效客户端SOCKET...\n", skt);
	}
	else {
		NewUserJoin userJoin;
		_senddataToAll(&userJoin);
		printf("socket=<%lld>新客户端加入：socket=%lld,IP=%s,port=%d\n",
			skt, skt, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
		
		m_vSockets.push_back(new MySockets(skt));
	}
	return skt;
}

void MySocketServer::_close() {
	if (m_skt != INVALID_SOCKET) {
		for (int i = (int)m_vSockets.size() - 1; i >= 0; --i) {
			closesocket(m_vSockets[i]->getSocket());
			delete m_vSockets[i];
		}
		closesocket(m_skt);
	}
}

int MySocketServer::_dispatch(SOCKET skt) {
	
	DataHeader header;
	int nLen = recv(skt, (char *)&header, sizeof(DataHeader), 0);
	if (nLen == SOCKET_ERROR) {
		cout << "recv error ..." << endl;
		return -1;
	}
	cout << "recv success ..." << endl;
	cout << "client cmd : " << header.cmd << endl;

	OnNetMsg(skt, &header);

	return 0;
}

int MySocketServer::_recvdata(MySockets* pSockets) {
	int nLen = recv(pSockets->getSocket(), m_szRecv, RECV_BUF_SIZE, 0);
	if (nLen <= 0) {
		cout << "recv error ..." << endl;
		return -1;
	}
	cout << "recv success ..." << endl;

	memcpy(pSockets->getMsgBuf()+ pSockets->getLastPos(), m_szRecv, nLen);
	pSockets->setLastPos(pSockets->getLastPos()+ nLen);
	//判断当前消息缓冲区的数据长度大于消息头DataHeader长度
	while (pSockets->getLastPos() >= sizeof(DataHeader)) {
		//这是就可以知道当前消息的长度
		DataHeader* header = (DataHeader*)pSockets->getMsgBuf();
		//判断消息缓冲区的数据长度大于消息长度
		if (pSockets->getLastPos() >= header->dataLength) {
			//消息缓冲区剩余未处理数据的长度
			int nSize = pSockets->getLastPos() - header->dataLength;
			//处理网络消息
			OnNetMsg(pSockets->getSocket(), header);
			//将消息缓冲区剩余未处理数据前移
			memcpy(pSockets->getMsgBuf(), pSockets->getMsgBuf()+header->dataLength, nSize);
			//消息缓冲区的数据尾部位置前移
			pSockets->setLastPos(nSize);
		}
		else {
			break;
		}
	}
	return 0;
}

int MySocketServer::_senddata(SOCKET skt, DataHeader* header) {
	if (_isRun() && header) {
		return send(skt, (const char*)header, header->dataLength, 0);
	}
	return SOCKET_ERROR;
}

void MySocketServer::_senddataToAll(DataHeader* header) {
	for (int i = (int)m_vSockets.size() - 1; i >= 0; --i) {
		_senddata(m_vSockets[i]->getSocket(), header);
	}
}

bool MySocketServer::_isRun() {
	return m_bRun;
}

void MySocketServer::OnNetMsg(SOCKET skt, DataHeader* header) {
	switch (header->cmd) {
	case CMD_LOGIN:
	{
		Login* login = (Login*)header;
		printf("收到客户端<socket=%lld>请求：CMD_LOGIN，数据长度：%d，userName=%s password=%s\n",
			skt, login->dataLength, login->userName, login->passWord);

		LoginResult ret;
		_senddata(skt, &ret);
		break;
	}
	case CMD_LOGOUT:
	{		
		Logout* logout = (Logout*)header;
		printf("收到客户端<socket=%lld>请求：CMD_LOGIN，数据长度：%d，userName=%s\n",
			skt, logout->dataLength, logout->userName);
		LogoutResult ret;
		_senddata(skt, &ret);
		break;
	}
	case CMD_DATA:
	{
		Data* data = (Data*)header;
		printf("收到客户端<socket=%lld>请求：CMD_DATA，数据长度：%d\n",
			skt, data->dataLength);
		DataResult ret;
		_senddata(skt, &ret);
		break;
	}
	default:
	{
		DataHeader ret;
		_senddata(skt, &ret);
		break;
	}
	}

}
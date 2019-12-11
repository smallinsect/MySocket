#include "MySocketClient.h"



int MySocketClient::_connect(const char* ip, u_short port) {
	WSADATA wd;
	if (::WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
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
	m_lastPos = 0;
	//m_threadid = _beginthread(_workthread, 0, this);
	m_threadid = _beginthread(_recvthread, 0, this);
	return 0;
}

void MySocketClient::_stop() {
	m_bRun = false;
	closesocket(m_skt);
	::WSACleanup();
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
			strcpy(login.userName, "���ײ�");
			strcpy(login.passWord, "С����");
			send(client->m_skt, (const char *)&login, sizeof(Login), 0);

			LoginResult loginRet;
			recv(client->m_skt, (char *)&loginRet, sizeof(LoginResult), 0);

			cout << "LoginResult: " << loginRet.result << endl;
		}
		else if (strcmp(cmdBuf, "logout") == 0) {
			Logout logout;
			strcpy(logout.userName, "���ײ˵�С����");
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

void MySocketClient::_recvthread(void* lpParam) {
	MySocketClient* client = (MySocketClient*)lpParam;

	while (client->m_bRun) {
		if (client->_recvdata() < 0) {
			break;
		}
	}
	cout << "thread " << client->m_threadid << " end ..." << endl;
}

//�������� ����ճ�� ��ְ�
int MySocketClient::_recvdata() {

	int nLen = recv(m_skt, m_szRecv, RECV_BUF_SIZE, 0);
	cout << "nLen=" << nLen << endl;
	if (nLen <= 0) {
		return -1;
	}
	//���յ������ݿ�����������
	memcpy(m_szMsgBuf + m_lastPos, m_szRecv, nLen);
	//��Ϣ������������β��λ�ƺ���
	m_lastPos += nLen;
	//���������ݳ��ȴ���DataHeader�����Ǿ�ȥȡ����
	while (m_lastPos >= sizeof(DataHeader)) {
		DataHeader* header = (DataHeader*)m_szMsgBuf;
		//�ж���Ϣ�����������ݳ��ȴ�����Ϣ����
		if (m_lastPos >= header->dataLength) {
			//ʣ��δ������Ϣ���������ݵĳ���
			int nSize = m_lastPos - header->dataLength;
			//����������Ϣ
			OnNetMsg(header);
			//����Ϣ������ʣ��δ�������Ϣǰ��
			memcpy(m_szMsgBuf, m_szMsgBuf+header->dataLength, nSize);
			//��Ϣ����������β��λ��ǰ��
			m_lastPos = nSize;
		}
		else {
			//ʣ����Ϣ����
			break;
		}
	}
	return 0;
}

int MySocketClient::_senddata() {
	//Login login;
	//strcpy(login.userName, "���ײ�");
	//strcpy(login.passWord, "С����");
	//send(m_skt, (const char *)&login, sizeof(Login), 0);
	Data data;
	memset(data.szData, 'a', sizeof(data.szData));
	send(m_skt, (const char *)&data, sizeof(data), 0);
	return 0;
}

bool MySocketClient::_isRun() {
	return m_bRun;
}

void MySocketClient::OnNetMsg(DataHeader* header) {
	switch (header->cmd) {
	case CMD_LOGIN_RESULT:
	{
		LoginResult* login = (LoginResult*)header;
		cout << "recv cmd : CMD_LOGIN_RESULT, dataLength=" << login->dataLength << endl;
		break;
	}
	case CMD_LOGOUT_RESULT:
	{
		LogoutResult* logout = (LogoutResult*)header;
		cout << "<socket=" << m_skt << "> recv cmd : CMD_LOGOUT_RESULT, dataLength=" << logout->dataLength << endl;
		break;
	}
	case CMD_DATA_RESULT:
	{
		DataResult* data = (DataResult*)header;
		cout << "<socket=" << m_skt << "> recv cmd : CMD_DATA_RESULT, dataLength=" << data->dataLength << endl;
		break;
	}
	case CMD_NEW_USER_JOIN:
	{
		NewUserJoin* userJoin = (NewUserJoin*)header;
		cout << "<socket=" << m_skt << "> recv cmd : CMD_LOGOUT_RESULT, dataLength=" << userJoin->dataLength << endl;
		break;
	}
	case CMD_ERROR:
	{
		cout << "<socket=" << m_skt << "> recv cmd : CMD_ERROR, dataLength=" << header->dataLength << endl;
		break;
	}
	default:
	{
		cout << "<socket=" << m_skt << "> �յ�δ������Ϣ��dataLength=" << header->dataLength << endl;
		break;
	}
	}
}
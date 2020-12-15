

#include "MyServer.h"
#include "MyRequest.h"


int MyServer::Start(const char* ip, u_short port) {//��ʼ������
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		cout << "[server] WSAStartup error ..." << GetLastError() << endl;
		return -1;
	}
	cout << "[server] WSAStartup success ..." << endl;

	//����һ���׽��� IPV4 ������ TCPЭ��
	m_skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_skt == INVALID_SOCKET) {
		cout << "[server] socket error ..." << GetLastError() << endl;
		return -1;
	}
	cout << "[server] socket success ..." << endl;

	//IPV4Э�� ip �˿�
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;//IPV4
	addr.sin_addr.s_addr = inet_addr(ip);//�Զ��󶨱���IP��ַ
	addr.sin_port = htons(port);//�˿�8000

	//���׽���
	if (bind(m_skt, (const sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		cout << "[server] bind error ..." << GetLastError() << endl;
		return -1;
	}
	cout << "[server] bind success ..." << endl;

	//�����׽���
	if (listen(m_skt, SOMAXCONN) == SOCKET_ERROR) {
		cout << "[server] listen error ..." << GetLastError() << endl;
		return -1;
	}
	cout << "[server] listen success ..." << endl;

	FD_ZERO(&m_fds);//��ʼ��
	FD_SET(m_skt,&m_fds);//����socket���뼯��

	m_bExit = false;
	// �����߳�
	threadid = _beginthread(workthread, 0, this);

	cout << "[server] WebServer running ..." << endl;
	return 0;
}

void MyServer::Stop() {//ֹͣ����
	m_bExit = true;
	closesocket(m_skt);//�ر��׽���
	WSACleanup();//��������
}

bool MyServer::dispatch(SOCKET skt, char* buf, int length) {//�ַ���Ϣ
	MyRequest request;
	request.setParam(buf, strlen(buf) + 1);

	char responseBuf[MAX_SIZE_BUF] = { 0 };
	int bufLen = sprintf(responseBuf,"HTTP/1.1 200 OK\r\nContent-Type: %s; charset=gb2312\r\n", request.getContentType().c_str());
	//Content-Type: image/png
	FILE* fp = fopen(request.file.c_str(), "rb");
	if (fp == NULL) {//��ȡ�ļ�ʧ��
		char page[] = "<html><head><title>���ײ˵�С�������</title></head><body style='color:green;background:#eee;'>���ײ˵�С����404</body></html>";

		bufLen += sprintf(responseBuf + bufLen, "Content-Length: %d\r\n\r\n", strlen(page));

		send(skt, responseBuf, bufLen, 0);
		send(skt, page, strlen(page), 0);
		return true;
	}

	fseek(fp, 0, SEEK_END);//�ļ�ָ�������ļ�ĩβ��ƫ����0
	long contentLength = ftell(fp);//��ȡ�ļ��������ֽ���
	fseek(fp, 0, SEEK_SET);//�ļ�ָ�������ļ�ͷ��ƫ����0

	bufLen += sprintf(responseBuf + bufLen, "Content-Length: %d\r\n\r\n", contentLength);
	send(skt, responseBuf, bufLen, 0);

	size_t fl;
	while ((fl = fread(responseBuf, sizeof(char), MAX_SIZE_BUF, fp)) > 0) {
		send(skt, responseBuf, fl, 0);
	}

	fclose(fp);
	return true;
}

void MyServer::workthread(void* lpParam) {
	MyServer* webserver = (MyServer*)lpParam;
	timeval tv = { 0, 1000 };

	char buf[MAX_SIZE_BUF];
	while (!webserver->m_bExit) {
		fd_set readfds = webserver->m_fds;
		if (select(0, &readfds, NULL, NULL, &tv) <= 0) {
			continue;
		}
		for (u_int i = 0; i < readfds.fd_count; ++i) {
			SOCKET tskt = readfds.fd_array[i];
			if (tskt == webserver->m_skt) {//���µĿͻ�����������
				SOCKET cskt = accept(webserver->m_skt, NULL, NULL);
				if (cskt == INVALID_SOCKET) {
					cout << "[server] accept error ..." << GetLastError() << endl;
					return;
				}
				cout << "[server] accept success ..." << endl;
				FD_SET(cskt, &webserver->m_fds);
				continue;
			}
			memset(buf, 0, sizeof(buf));
			int irecv = recv(tskt, buf, sizeof(buf), 0);//����������������Ϣ
			if (irecv > 0) {
				// ������ܵ���Ϣ
				webserver->dispatch(tskt, buf, strlen(buf));
			}
			// ���ͻ����׽��֣��Ӽ�����ɾ����
			FD_CLR(tskt, &webserver->m_fds);
			//irecv=0 ��ʾ�ͻ����˳� irecv=-1 ��ʾ�ͻ����쳣�˳� 
			closesocket(tskt); // �رտͻ����׽���
		}
	}
	cout << "threadid = "<< webserver->threadid  << "end ..." << endl;
}

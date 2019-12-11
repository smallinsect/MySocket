
#ifndef __MYSOCKETCLIENT_H__
#define __MYSOCKETCLIENT_H__

#include "../MySocketInclude/common.h"


class MySocketClient{
public:
	int _connect(const char* ip = "127.0.0.1", u_short port = 8000);
	int _recvdata();
	int _senddata();
	bool _isRun();
	void _stop();
	

	void OnNetMsg(DataHeader* header);
	static void _workthread(void* lpParam);
	static void _recvthread(void* lpParam);
private:

	bool m_bRun;//���У�true��ֹͣ��false
	uintptr_t m_threadid;//�߳�id

	SOCKET m_skt;

	char m_szRecv[RECV_BUF_SIZE];//���ܻ�����
	char m_szMsgBuf[RECV_BUF_SIZE];//�ڶ������� ��Ϣ������
	int m_lastPos;
};



#endif
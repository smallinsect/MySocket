
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

	bool m_bRun;//运行：true，停止：false
	uintptr_t m_threadid;//线程id

	SOCKET m_skt;

	char m_szRecv[RECV_BUF_SIZE];//接受缓冲区
	char m_szMsgBuf[RECV_BUF_SIZE];//第二缓冲区 消息缓冲区
	int m_lastPos;
};



#endif
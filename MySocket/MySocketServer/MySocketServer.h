

#ifndef __MYSOCKETSERVER_H__
#define __MYSOCKETSERVER_H__

#include "../MySocketInclude/common.h"

class MySockets {
public:
	MySockets(SOCKET skt = INVALID_SOCKET) {
		m_skt = skt;
		memset(m_szMsgBuf, 0, sizeof(m_szMsgBuf));
		m_lastPos = 0;
	}

	SOCKET getSocket() {
		return m_skt;
	}
	char* getMsgBuf() {
		return m_szMsgBuf;
	}
	int getLastPos() {
		return m_lastPos;
	}
	void setLastPos(int lastPos) {
		m_lastPos = lastPos;
	}
private:
	SOCKET m_skt;
	//第二缓冲区 消息缓冲区
	char m_szMsgBuf[RECV_BUF_SIZE];
	//消息缓冲区的数据尾部位置
	int m_lastPos;
};

class MySocketServer{
public:
	int _start(const char* ip = "127.0.0.1", u_short port = 8000);
	int _recvdata(MySockets* pSockets);
	int _senddata(SOCKET skt, DataHeader* header);
	bool _isRun();
	void _stop();
	SOCKET _accept();
	void _close();
	void _senddataToAll(DataHeader* header);
	void _work();

	void OnNetMsg(SOCKET skt, DataHeader* header);
private:
	static void _workthread(void* lpParam);

	int _dispatch(SOCKET skt);

	bool m_bRun;//运行：true，停止：false
	uintptr_t m_threadid;//线程id

	fd_set m_fds;
	SOCKET m_skt;

	char m_szRecv[RECV_BUF_SIZE];
	vector<MySockets*> m_vSockets;
};

#endif


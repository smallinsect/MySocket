
#ifndef __MYSOCKETCLIENT_H__
#define __MYSOCKETCLIENT_H__

#include "../MySocketInclude/common.h"

class MySocketClient{
public:
	int _connect(const char* ip = "127.0.0.1", u_short port = 8000);
	void _stop();
private:
	static void _workthread(void* lpParam);

	bool m_bRun;//���У�true��ֹͣ��false
	uintptr_t m_threadid;//�߳�id

	SOCKET m_skt;
};



#endif
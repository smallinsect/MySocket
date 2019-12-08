
#ifndef __MYSOCKETCLIENT_H__
#define __MYSOCKETCLIENT_H__

#include "../MySocketInclude/common.h"

class MySocketClient{
public:
	int _connect(const char* ip = "127.0.0.1", u_short port = 8000);
	void _stop();
private:
	static void _workthread(void* lpParam);

	bool m_bRun;//运行：true，停止：false
	uintptr_t m_threadid;//线程id

	SOCKET m_skt;
};



#endif
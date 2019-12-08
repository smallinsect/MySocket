

#ifndef __MYSOCKETSERVER_H__
#define __MYSOCKETSERVER_H__

#include "../MySocketInclude/common.h"

class MySocketServer{
public:
	int _start(const char* ip = "127.0.0.1", u_short port = 8000);
	void _stop();
private:
	static void _workthread(void* lpParam);

	int _dispatch(SOCKET skt);

	bool m_bRun;//运行：true，停止：false
	uintptr_t m_threadid;//线程id

	fd_set m_fds;
	SOCKET m_skt;
};

#endif


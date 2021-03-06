#ifndef __MyServer_H__
#define __MyServer_H__

#include "common.h"

class MyServer {
public:
	int Start(const char* ip, u_short port);//开始服务器
	void Stop();//停止服务

private:
	bool dispatch(SOCKET skt, char* buf, int length);//分发消息
	static void workthread(void* lpParam);

	uintptr_t threadid;//线程ID
	bool m_bExit;//true 线程退出 false 线程执行

	fd_set m_fds;//socket集合
	SOCKET m_skt;//服务器socket
};


#endif // !__MyServer_H__
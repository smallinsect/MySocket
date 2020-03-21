#ifndef __MyServer_H__
#define __MyServer_H__

#include "common.h"

class MyServer {
public:
	int Start(const char* ip, u_short port);//��ʼ������
	void Stop();//ֹͣ����

private:
	bool dispatch(SOCKET skt, char* buf, int length);//�ַ���Ϣ
	static void workthread(void* lpParam);

	uintptr_t threadid;//�߳�ID
	bool m_bExit;//true �߳��˳� false �߳�ִ��

	fd_set m_fds;//socket����
	SOCKET m_skt;//������socket
};


#endif // !__MyServer_H__
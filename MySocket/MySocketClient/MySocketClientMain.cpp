/***************************************************
网络报文的数据结构格式
***************************************************/


#include "MySocketClient.h"



bool g_bRun;
void _workdthread(void* lpParam) {
	MySocketClient* pclient = new MySocketClient();
	pclient->_connect("127.0.0.1", 8000);
	while (g_bRun) {
		if (!pclient->_isRun()) {
			break;
		}
		pclient->_senddata();
		//pclient->_recvdata();
	}
	pclient->_stop();
	delete pclient;
}

int main(){

	int cnt = 10;// FD_SETSIZE;
	uintptr_t threadid[10];
	g_bRun = true;
	for (int i = 0; i < cnt; ++i) {
		threadid[i] = _beginthread(_workdthread, 0, NULL);
	}
	while (true) {
		if (!_kbhit() && _getch() == VK_SPACE) {
			g_bRun = false;
			break;
		}
		//Sleep(1000);
	}


	system("pause");
	return 0;
}
/***************************************************
���籨�ĵ����ݽṹ��ʽ
***************************************************/


#include "MySocketServer.h"

int main(){

	MySocketServer server;
	server._start("0.0.0.0", 8000);

	while (true) {
		Sleep(1000);
	}

	system("pause");
	return 0;
}
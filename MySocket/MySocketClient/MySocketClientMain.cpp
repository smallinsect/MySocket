/***************************************************
���籨�ĵ����ݽṹ��ʽ
***************************************************/


#include "MySocketClient.h"

int main(){

	MySocketClient client;
	client._connect("127.0.0.1", 8000);
	while (true) {
		Sleep(1000);
	}

	system("pause");
	return 0;
}


#include "MyServer.h"


int main(int argc, char* argv[]) {
	MyServer server;
	server.Start("0.0.0.0", 8000);
	while (true) {
		printf("���ո��˳�����...\n");
		if (_kbhit()) {//�������Ƿ������� ������1 ������0
			if (_getch() == VK_SPACE) {
				server.Stop();
				break;
			}
		}
		Sleep(1000);
	}
	printf("server stop ...\n");
	system("pause");
	return 0;
}
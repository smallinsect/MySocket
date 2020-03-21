

#include "MyServer.h"


int main(int argc, char* argv[]) {
	MyServer server;
	server.Start("0.0.0.0", 8000);
	while (true) {
		printf("按空格，退出服务...\n");
		if (_kbhit()) {//检测键盘是否有输入 有输入1 无输入0
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
//socket�ͻ��˴��� ����������
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

void printMenu();

int main(int argc, char *argv[]) {

	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//�����ͻ����׽���
	SOCKET sktCli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sktCli == INVALID_SOCKET) {
		printf("[client] socket error ...\n");
		exit(1);
	}
	printf("[client] socket success ...\n");

	//��Ҫ���ӵķ�������Ϣ
	sockaddr_in addrSer = { 0 };
	addrSer.sin_family = AF_INET;//Ҫ�ӵķ�������IPV4Э��
	addrSer.sin_port = htons(8080);//Ҫ���ӵķ������Ķ˿�
	addrSer.sin_addr.s_addr = inet_addr("192.168.3.58");//Ҫ���ӵķ�������IP
	//���ӷ�����
	if (connect(sktCli, (sockaddr *)&addrSer, sizeof(addrSer)) == SOCKET_ERROR) {
		printf("[client] connect error ...\n");
		exit(1);
	}
	printf("[client] connect success ...\n");

	char buf[1024];
	int n;
	while (true) {
		printMenu();
		printf(">>");
		int ret = scanf("%d", &n);
		if (ret < 1) {//�Ƿ�����
			break;
		}
		if (n == 0) {//�˳�����
			printf("[client] quit ...\n");
			break;
		}
		printf("input message>>");
		scanf("%s", buf);
		//���������������
		if (send(sktCli, buf, strlen(buf) + 1, 0) == SOCKET_ERROR) {
			printf("[client] send error ...\n");
			exit(1);
		}
		printf("[client] send success ...\n");

		//���ܷ��������͵�����
		int len = recv(sktCli, buf, sizeof(buf), 0);
		if (len == -1) {//���ܴ���
			printf("[client] recv error ...\n");
			exit(1);
		}
		if (len == 0) {//�������˳�
			printf("[server] quit ...\n");
			break;
		}
		printf("[server] %s\n", buf);//��ӡ����������Ϣ
	}

	closesocket(sktCli);//�ر��׽���
	WSACleanup();
	return 0;
}

void printMenu() {
	printf("*************************\n");
	printf("*1.send message         *\n");
	printf("*0.quit process         *\n");
	printf("*************************\n");
}

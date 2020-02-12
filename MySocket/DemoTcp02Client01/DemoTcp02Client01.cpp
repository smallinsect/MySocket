#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Windows.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

int func(int argc, char* argv[]);

int main(int argc, char* argv[]) {

	func(argc, argv);

	system("pause");
	return 0;
}

int func(int argc, char* argv[]) {
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		printf("[client] WSAStartup error ...\n");
		system("pause");
		return -1;
	}
	printf("[client] WSAStartup success ...\n");

	//�����׽���
	SOCKET skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == SOCKET_ERROR) {
		printf("[client] socket error ...\n");
		return -1;
	}
	printf("[client] socket success ...\n");

	//�������Ϣ
	sockaddr_in addrServ = { 0 };
	addrServ.sin_family = AF_INET;//IPV4Э��
	addrServ.sin_port = htons(8000);//�˿�
	addrServ.sin_addr.s_addr = inet_addr("127.0.0.1");//IP��ַ

	//���ӷ�����
	if (connect(skt, (sockaddr*)&addrServ, sizeof(addrServ)) == SOCKET_ERROR) {
		printf("[client] connect error ...\n");
		return -1;
	}
	printf("[client] connect success ...\n");

	while (true) {
		char buf[1024] = { 0 };
		printf(">>");
		scanf("%s", buf);
		//�����˷�������
		if (send(skt, buf, strlen(buf) + 1, 0) == SOCKET_ERROR) {
			printf("[client] send error ...\n");
			return -1;
		}
		printf("[client] send success ...\n");

		//�Ƚ��ܷ��������͵�����
		if (recv(skt, buf, sizeof(buf), 0) == SOCKET_ERROR) {
			printf("[client] recv error ...\n");
			return -1;
		}
		printf("[client] recv success ...\n");
		printf("[client] recv msg: %s\n", buf);

	}

	closesocket(skt);
	WSACleanup();
	return 0;
}
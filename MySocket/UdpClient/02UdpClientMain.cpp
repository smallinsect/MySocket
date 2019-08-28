//�㲥�ͻ��� ��������
//
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[]) {

	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);
	//�����׽���
	SOCKET cskt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (cskt == INVALID_SOCKET) {
		printf("[client] socket error ...\n");
		system("pause");
		exit(1);
	}
	printf("[client] socket success ...\n");
	//�ͻ�����Ϣ
	sockaddr_in caddr = { 0 };
	caddr.sin_family = AF_INET;//�ͻ��˵�IPV4Э��
	caddr.sin_port = htons(8787);//�ͻ��˴򿪵�8080�˿�
	caddr.sin_addr.s_addr = inet_addr("0.0.0.0");//�ͻ��˵�IP������

	int ret = bind(cskt, (sockaddr *)&caddr, sizeof(caddr));
	if (ret == SOCKET_ERROR) {
		printf("[client] bind error ...\n");
		system("puase");
		exit(1);
	}
	printf("[client] bind success ...\n");


	while (true) {
		char buf[1024] = {0};
		//���ܷ���˵�����
		ret = recvfrom(cskt, buf, 1024, 0, NULL, NULL);
		if (ret == SOCKET_ERROR) {
			printf("[client] recvfrom error ...\n");
			system("puase");
			exit(1);
		}
		printf("[client] recvfrom success ...\n");
		printf("[server] msg:%s\n", buf);
	}

	closesocket(cskt);
	WSACleanup();
	system("pause");
	return 0;
}
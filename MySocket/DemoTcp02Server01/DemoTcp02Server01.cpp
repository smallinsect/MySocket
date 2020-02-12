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
		printf("[server] WSAStartup error ...\n");
		return -1;
	}
	printf("[server] WSAStartup success ...\n");

	//�����׽���
	SOCKET skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == INVALID_SOCKET) {
		printf("[server] socket error ...\n");
		return -1;
	}
	printf("[server] socket success ...\n");
	//��������Ϣ
	sockaddr_in addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(8000);
	addrServ.sin_addr.s_addr = inet_addr("0.0.0.0");

	//�׽��ְ󶨶˿ں�ip
	if (bind(skt, (sockaddr*)&addrServ, sizeof(addrServ)) == SOCKET_ERROR) {
		printf("[server] bind error ...\n");
		return -1;
	}
	printf("[server] bind success ...\n");

	//��ʼ�����׽���
	if (listen(skt, SOMAXCONN) == SOCKET_ERROR) {
		printf("[server] listen error ...\n");
		return -1;
	}
	printf("[server] listen success ...\n");


	FD_SET fds;
	FD_ZERO(&fds);
	FD_SET(skt, &fds);

	timeval tv = {1, 0};
	while (true) {
		fd_set readfds = fds;

		if (select(0, &readfds, NULL, NULL, &tv) <= 0) {
			continue;
		}

		for (u_int i = 0; i < readfds.fd_count; i++) {
			SOCKET tskt = readfds.fd_array[i];
			//������socket��Ӧ�����µĿͻ�����������
			if (tskt == skt) {
				SOCKET sskt = accept(tskt, NULL, NULL);
				if (sskt == INVALID_SOCKET) {//�ͻ�������ʧ��
					printf("[server] accept error ...\n");
					continue;
				}
				//�ͻ������ӳɹ�
				printf("[server] accept success ...\n");
				//�������ӵĿͻ�����ӵ�������
				FD_SET(sskt, &fds);
			}
			else {//���ܿͻ��˵���������
				char buf[1024] = {0};
				//���ܿͻ��˷���������
				int len = recv(tskt, buf, 1024, 0);
				if (len <= 0) {
					printf("[server] recv error ...\n");
					//���ͻ����׽��ִӼ�����ɾ��
					FD_CLR(tskt, &fds);
					//�رտͻ����׽���
					closesocket(tskt);
					continue;
				}
				printf("socket:%d %s\n", tskt, buf);
				char resbuf[1024] = "�������յ���Ϣ";
				//��ͻ��˷�������
				send(tskt, resbuf, strlen(resbuf), 0);
			}
		}
	}

	//�رշ������׽���
	closesocket(skt);
	WSACleanup();
}

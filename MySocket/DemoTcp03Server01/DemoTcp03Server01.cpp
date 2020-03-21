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
DWORD WINAPI ThreadDo(PVOID pvParam);

struct server {
	SOCKET skt;
	sockaddr addr;
};

int serLen;
server sers[100];

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
	printf("[server] listen ip:%s port:%d success ...\n", inet_ntoa(addrServ.sin_addr), ntohs(addrServ.sin_port));


	while (true) {
		//�ͻ�����Ϣ
		sockaddr_in caddr = { 0 };
		int caddrlen = sizeof(sockaddr_in);
		SOCKET cskt = accept(skt, (sockaddr*)&caddr, &caddrlen);//���ܿͻ�������
		if (cskt == SOCKET_ERROR) {
			printf("[server] accept error ...\n");
			return -1;
		}
		printf("[server] accept success ...\n");
		//��ӡ���ӵĿͻ�����Ϣ
		printf("[client] ip:%s port:%d connect ...\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

		DWORD threadId;
		//�������ӵ��û�����һ���߳�
		HANDLE hThread1 = CreateThread(NULL, 0, ThreadDo, (LPVOID)cskt, 0, &threadId);
	}

	closesocket(skt);
	WSACleanup();
	return 0;
}

DWORD WINAPI ThreadDo(PVOID pvParam) {
	SOCKET skt = (SOCKET)pvParam;
	while (true) {
		char buf[1024] = "���ײ˵�С����������յ���Ϣ��";
		//�ȷ������ݸ��ͻ���
		if (send(skt, buf, strlen(buf) + 1, 0) == SOCKET_ERROR) {//��ͻ��˷�����Ϣ
			printf("[server] send error ...\n");
			return -1;
		}
		printf("[server] send success ...\n");
		//���ܿͻ��˵�����
		if (recv(skt, buf, sizeof(buf), 0) <= 0) {//���ܿͻ��˷�������Ϣ
			printf("[server] recv error ...\n");
			return -1;
		}
		printf("[server] recv msg:%s\n", buf);
	}
	return 0;
}
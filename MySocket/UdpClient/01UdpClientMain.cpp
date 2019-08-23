
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
	//��Ҫ���ӵķ������Ϣ
	sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;//��Ҫ���ӵķ���˵�IPV4Э��
	saddr.sin_port = htons(8080);//��Ҫ���ӵķ���˵�8080�˿�
	saddr.sin_addr.s_addr = inet_addr("192.168.3.18");//��Ҫ���ӵķ���˵�IP

	char buf[1024];
	while (true) {
		printf("[client] >>");
		scanf("%s", buf);
		//�����˷�������
		sendto( cskt, buf, strlen(buf)+1, 0, (sockaddr *)&saddr, sizeof(saddr));
		//���ܷ���˵�����
		recvfrom( cskt, buf, 1024, 0, NULL, NULL);
		printf("[server] %s\n", buf);
	}

	closesocket(cskt);
	WSACleanup();
	system("pause");
	return 0;
}
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
	SOCKET sskt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in saddr = { 0 };
	saddr.sin_family = AF_INET;//IPV4Э��
	saddr.sin_port = htons(8080);//8080�˿�
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);//����IP
	//���׽��ֺͶ˿ڡ�ip��
	bind( sskt, (sockaddr *)&saddr, sizeof(saddr));
	
	sockaddr_in caddr = {0};//���ӵĿͻ����׽�����Ϣ
	int caddrLen = sizeof(caddr);
	char buf[1024];
	while (true) {
		//���ܿͻ��˷�������Ϣ
		recvfrom(sskt, buf, 1024, 0, (sockaddr *)&caddr, &caddrLen);
		//��ӡ�ͻ��˵�ip�Ͷ˿�
		printf("[client] ip:%s port:%d\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));
		//��ӡ�ͻ��˷�������Ϣ
		printf("[client] %s\n", buf);
		//���ͻ��˷�������Ϣ���ظ��ͻ���
		sendto(sskt, buf, strlen(buf) + 1, 0, (sockaddr *)&caddr, caddrLen);
	}

	closesocket(sskt);

	WSACleanup();
	system("pause");
	return 0;
}
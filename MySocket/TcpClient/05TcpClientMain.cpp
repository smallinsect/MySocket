//socket�򵥵Ŀͻ��˴���
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <Windows.h>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[]) {

	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	//�����ͻ����׽���
	SOCKET sktCli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addrSer = {0};
	addrSer.sin_family = AF_INET;//Ҫ�ӵķ�������IPV4Э��
	addrSer.sin_port = htons(8080);//Ҫ���ӵķ������Ķ˿�
	addrSer.sin_addr.s_addr = inet_addr("127.0.0.1");//Ҫ���ӵķ�������IP
	//���ӷ�����
	connect(sktCli, (sockaddr *)&addrSer, sizeof(addrSer));

	char buf[1024];
	while (true) {
		printf(">>");
		scanf("%s", buf);
		send(sktCli, buf, strlen(buf) + 1, 0);//���������������

		recv(sktCli, buf, sizeof(buf), 0);//���ܷ��������͵�����
		printf("[server] %s\n", buf);
	}

	WSACleanup();
	return 0;
}
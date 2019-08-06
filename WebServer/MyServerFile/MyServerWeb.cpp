
#include "MyServerWeb.h"


int request(SOCKET s) {
	char buf[1024] = "";
	int iret = recv(s, buf, sizeof(buf), 0);
	if (iret == SOCKET_ERROR) {
		printf("[client] recv error ...\n");
		return -1;
	}
	if (iret == 0) {
		printf("[client] quit ...\n");
		return 0;
	}
	printf("len=%d\n", strlen(buf));
	printf("******************request header message st******************");
	printf("%s", buf);
	printf("******************request header message en******************");


	return 1;
}

void response(SOCKET s) {
}

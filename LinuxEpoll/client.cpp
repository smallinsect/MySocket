#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SOCKET int

#include "config.h"

int main(int argc, char *argv[]){
	char szIp[32];
	char szPort[8];
	readData(szIp, szPort);

	SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addr={};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(szPort));
	addr.sin_addr.s_addr = inet_addr(szIp);

	if(connect(sClient, (sockaddr *)&addr, sizeof(addr)) == -1){
		printf("connect error ...\n");
		return -1;
	}
	printf("connect success ...\n");
	char buf[1024];
	int iRes;
	while(true){
		printf("read<<");
		iRes = read(sClient, buf, sizeof(buf));
		if(iRes < 1){
			printf("read error %d ...\n", iRes);
			break;
		}
		printf("%s\n", buf);
	
		printf("write>>");
		scanf("%s", buf);
		write(sClient, buf, strlen(buf)+1);
	}


	close(sClient);
	return 0;
}

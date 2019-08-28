//
//广播客户端代码
//
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char *argv[]){
	//创建套接字
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(fd == -1){
		printf("[client] socket error ...\n");
		exit(1);
	}
	printf("[client] socket success ...\n");

	sockaddr_in client = {0};
	client.sin_family = AF_INET;
	client.sin_port = htons(8787);
	client.sin_addr.s_addr = inet_addr("0.0.0.0");
	//inet_pton(AF_INET, "0.0.0.0", &client.sin_addr.s_addr);

	int ret = bind(fd, (sockaddr *)&client, sizeof(client));
	if(ret == -1){
		printf("[client] bind error ...\n");
		exit(1);
	}
	printf("[client] bind success ...\n");

	while(true){
		char buf[1024] = {0};
		ret = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
		if(ret == -1){
			printf("[client] recvfrom error ....\n");
			break;
		}
		if(ret == 0){
			printf("[client] server quit ...\n");
			break;
		}
		printf("[client] recvfrom success ...\n");
		printf("[server] msg:%s\n", buf);
	}

	close(fd);
	return 0;
}

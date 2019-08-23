
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "code.h"

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("./a.out port\n");
		exit(-1);
	}
	int port = atoi(argv[1]);
	int sskt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	//saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//绑定套接字和IP地址、端口
	if(bind(sskt, (sockaddr *)&saddr, sizeof(saddr)) == -1){
		printf("[server] bind error ...\n");
		return -1;
	}
	printf("[server] bind success ...\n");

	//监听套接字
	if(listen(sskt, SOMAXCONN) == -1){
		printf("[server] listen error ...\n");
		return -1;
	}
	printf("[server] listen success ...\n");

	//接受客户端的连接请求
	int cskt = accept(sskt, NULL, NULL);
	char buf[1024];
	char bufG[1024];
	while(true){
		printf("[server] >>");
		scanf("%s", buf);
		utog(buf, strlen(buf), bufG, 1024);
		send(cskt, bufG, strlen(bufG)+1, 0);
		
		int irecv = recv(cskt, bufG, 1024, 0);
		if(irecv == -1){
			printf("[server] recv error ...\n");
			break;
		}
		if(irecv == 0){
			printf("[server] client quit ...\n");
			break;
		}
		gtou(bufG, strlen(bufG), buf, 1024);
		printf("%s", buf);
	}
	close(sskt);
	return 0;
}

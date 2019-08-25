

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("./a.out port");
		return -1;
	}
	
	int sskt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sskt == -1){
		printf("[server] socket error ...\n");
		return -1;
	}
	printf("[server] socket success ...\n");

	sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[1]));
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(sskt, (sockaddr *)&saddr, sizeof(saddr)) == -1){
		printf("[server] bind error ...\n");
		return -1;
	}
	printf("[server] bind success ...\n");
	printf("[server] ip:%s\n", inet_ntoa(saddr.sin_addr));
	printf("[server] port:%d\n", ntohs(saddr.sin_port));

	int iret;
	sockaddr_in caddr = {0};
	socklen_t caddrLen = sizeof(caddr);
	char buf[1024];
	while(true){
		iret = recvfrom(sskt, buf, 1024, 0, (sockaddr *)&caddr, &caddrLen);
		if(iret == -1){
			printf("[server] recvfrom error ...\n");
			break;
		}
		printf("[client] ip:%s\n", inet_ntoa(caddr.sin_addr));
		printf("[client] port:%d\n", ntohs(caddr.sin_port));
		printf("[client] %s\n", buf);
		
		sendto(sskt, buf, strlen(buf)+1, 0, (sockaddr*)&caddr, caddrLen);
	}
	close(sskt);
	return 0;
}


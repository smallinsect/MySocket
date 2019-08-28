//
//广播服务器代码
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
		printf("[sever] socket error ...\n");
		exit(1);
	}
	printf("[server] socket success ...\n");

	//绑定server的ip和端口
	sockaddr_in serv = {0};
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8080);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = bind(fd, (sockaddr *)&serv, sizeof(serv));
	if(ret == -1){
		printf("[server] bind error ...\n");
		exit(1);
	}
	printf("[server] bind success ...\n");

	//初始化客户端地址信息
	sockaddr_in client = {0};
	client.sin_family = AF_INET;
	client.sin_port = htons(8787);//客户端绑定的端口
	//使用广播地址给客户端发送数据
	client.sin_addr.s_addr = inet_addr("192.168.3.255");
	//inet_pton(AF_INET, "192.168.3.255", &client.sin_add.s_addr);
	
	//给服务器开放广播权限
	int flag = 1;
	setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));

	//通信信
	while(true){
		//一直给客户端发送数据
		static int num = 0;
		if(num > 20){
			printf("[server] sendto end ...\n");
			break;
		}
		char buf[1024] = {0};
		sprintf(buf, "hello, udp == %d", num++);
		sendto(fd, buf, strlen(buf)+1, 0, (sockaddr *)&client, sizeof(client));
		printf("[server] send buff %s\n", buf);

		sleep(1);
	}
	close(fd);

	return 0;
}

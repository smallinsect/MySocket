//
//组播服务器代码
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <net/if.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char *argv[]){
	//创建套接字
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(fd == -1){
		printf("[server] socket error ...\n");
		exit(1);
	}
	printf("[server] socket success ...\n");

	//服务端信息
	sockaddr_in serv = {0};
	serv.sin_family = AF_INET;//IPV4协议
	serv.sin_port = htons(8080);//端口8080
	//serv.sin_addr.s_addr = inet_addr("0.0.0.0");
	inet_pton(AF_INET, "0.0.0.0", &serv.sin_addr.s_addr);

	//绑定
	int ret = bind(fd, (sockaddr *)&serv, sizeof(serv));
	if(ret == -1){
		printf("[server] bind error ...\n");
		exit(1);
	}
	printf("[server] bind success ...\n");

	//客户端信息
	sockaddr_in client = {0};
	client.sin_family = AF_INET;//IPV4协议
	client.sin_port = htons(8787);//端口8787
	//使用组播地址给客户端发数据
	inet_pton(AF_INET, "239.0.0.10", &client.sin_addr.s_addr);

	//给服务器开放组播权限
	ip_mreqn flag = {0};
	inet_pton(AF_INET, "239.0.0.10", &flag.imr_multiaddr.s_addr);//组播地址
	inet_pton(AF_INET, "0.0.0.0", &flag.imr_address.s_addr);//本地IP
	flag.imr_ifindex = if_nametoindex("enp2s0");//获取本机网卡
	setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &flag, sizeof(flag));

	int num = 0;
	//循环发送消息
	while(true){
		if(num > 20){
			printf("[server] sendto msg end ...\n");
			break;
		}
		char buf[1024] = {0};
		sprintf(buf, "hello, multicast udp==%d", num++);

		sendto(fd, buf, strlen(buf)+1, 0, (sockaddr *)&client, sizeof(client));
		printf("[server] msg:%s\n", buf);

		sleep(1);
	}

	close(fd);
	return 0;
}

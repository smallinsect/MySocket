//
//组播客户端代码
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
		printf("[client] socket error ...\n");
		exit(1);
	}
	printf("[client] socket success ...\n");

	//客户端信息
	sockaddr_in client = {0};
	client.sin_family = AF_INET;//IPV4协议
	client.sin_port = htons(8787);//端口8787
	inet_pton(AF_INET, "0.0.0.0", &client.sin_addr.s_addr);//本机ip地址

	int ret = bind(fd, (sockaddr *)&client, sizeof(client));
	if(ret == -1){
		printf("[client] bind error ...\n");
		exit(1);
	}
	printf("[client] bind success ...\n");

	//加入到组播地址
	ip_mreqn fl = {0};
	inet_pton(AF_INET, "239.0.0.10", &fl.imr_multiaddr.s_addr);//组播地址
	inet_pton(AF_INET, "0.0.0.0", &fl.imr_address.s_addr);//本机IP地址
	fl.imr_ifindex = if_nametoindex("enp2s0");//本机网卡
	setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &fl, sizeof(fl));

	while(true){
		char buf[1024] = {0};
		ret = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
		if(ret == -1) {
			printf("[client] recvfrom error ...\n");
			exit(1);
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

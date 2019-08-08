#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){

	int sser = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addrSer;
	memset(&addrSer, 0, sizeof(addrSer));
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(8080);
//	addrSer.sin_addr.s_addr = inet_addr("127.0.0.1");
//
	bind(sser, (struct sockaddr *)&addrSer, sizeof(addrSer));

	listen(sser, 20);

	struct sockaddr_in addrCli;
	memset(&addrSer, 0, sizeof(addrSer));
	socklen_t addrLen = sizeof(addrCli);
	int scli = accept(sser, (struct sockaddr *)&addrCli, &addrLen);

	char buf[1024];
	while(true){
		printf(">>");
		scanf("%s", buf);
		write(scli, buf, strlen(buf)+1);

		read(scli, buf, 1024);
		printf("[client]: %s\n", buf);
	}

	close(sser);
	close(scli);
	//启动epoll模型
	puts("爱白菜的小昆虫");
	return 0;
}

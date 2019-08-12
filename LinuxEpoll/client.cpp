#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SOCKET int

int main(int argc, char *argv[]){
	SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addr={};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = inet_addr("192.168.3.18");

	connect(sClient, (sockaddr *)&addr, sizeof(addr));

	char buf[1024];
	read(sClient, buf, sizeof(buf));
	printf("%s\n", buf);

	while(true){
		scanf("%s", buf);
		write(sClient, buf, strlen(buf)+1);
	}


	close(sClient);
	return 0;
}

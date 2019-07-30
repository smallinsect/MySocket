

#include "MyWeb.h"

#define PATHWEB "./webui"


char type[16];
char reso[64];
char vers[32];

int Request(SOCKET s) {
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	int len = recv(s, buf, 1024, 0);
	if (len == SOCKET_ERROR) {
		printf("recv error ...\n");
		return -1;
	}
	if (len == 0) {
		printf("client quit ...\n");
		return -1;
	}
	printf("recv success ...\n");
	printf("%s", buf);

	memset(type, 0, sizeof(type));
	memset(reso, 0, sizeof(reso));
	memset(vers, 0, sizeof(vers));
	sscanf(buf, "%s %s %s", type, reso, vers);
	return 0;
}

void Response(SOCKET s) {
	char buf[1024] = {0};
	int bufLen = sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=gb2312\r\n");

	char file[1024] = {0};
	sprintf(file, "%s%s", PATHWEB, reso);
	printf("请求文件路径：%s\n", file);

	FILE *f = fopen(file, "rb");
	if (f == NULL) {
		static char defPage[] = "<html><b><center>404 not find!</center></b></html>";
		printf("读取文件失败\n");
		bufLen += sprintf(buf + bufLen, "Content-Length: %d\r\n\r\n%s", strlen(defPage), defPage);
		send(s, buf, strlen(buf), 0);
		printf("响应消息：\n%s", buf);
		return;
	}
	fseek(f, 0, SEEK_END);
	int contentLength = ftell(f);
	bufLen += sprintf(buf + bufLen, "Content-Length: %d\r\n\r\n", contentLength);
	send(s, buf, strlen(buf), 0);
	printf("响应消息：\n%s", buf);
	fseek(f, 0, SEEK_SET);

	size_t len;
	while ((len = fread(buf, sizeof(char), sizeof(buf), f)) > 0) {
		printf("%s", buf);
		if (send(s, buf, len, 0) == SOCKET_ERROR) {
			printf("send error ...\n");
			break;
		}
		printf("send success ...\n");
	}

	fclose(f);
}
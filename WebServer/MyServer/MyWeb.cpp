

#include "MyWeb.h"

#define PATHWEB "./webui"


char method[16];
char URI[64];
char protocol[32];

char fileType[32];

int Request(SOCKET s) {
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	int len = recv(s, buf, 1024, 0);
	if (len == SOCKET_ERROR) {
		printf("[server] recv error ...\n");
		return len;
	}
	if (len == 0) {
		printf("[server] client quit ...\n");
		return len;
	}
	printf("[server] recv success ...\n");
	printf("......................request message begin......................\n");
	printf("%s", buf);
	printf("......................request message end........................\n");

	memset(method, 0, sizeof(method));
	memset(URI, 0, sizeof(URI));
	memset(protocol, 0, sizeof(protocol));
	sscanf(buf, "%s %s %s", method, URI, protocol);
	printf("request message method=%s, URI=%s, protocol=%s\n", method, URI, protocol);

	memset(fileType, 0, sizeof(fileType));
	sscanf(URI, "%[^.]%[^ ?]", fileType, fileType);
	printf("request message fileTpye=%s\n", fileType);
	return len;
}

void Response(SOCKET s) {
	char buf[1024] = {0};
	int bufLen = sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: %s; charset=gb2312\r\n", getContentType(fileType).c_str());
	char file[1024] = {0};
	sprintf(file, "%s%s", PATHWEB, URI);
	printf("请求文件路径：%s\n", file);

	FILE *f = fopen(file, "rb");
	//打开文件失败
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

std::string getContentType(const std::string &fileType) {
	std::string contentType = "";

	if (fileType == ".htm") {
		contentType = "text/html";
	}
	else if (fileType == ".html") {
		contentType = "text/html";
	}
	else if (fileType == ".css") {
		contentType = "text/css";
	}
	else if (fileType == ".js") {
		contentType = "application/x-javascript";
	}
	else if (fileType == ".xml") {
		contentType = "text/xml";
	}
	else if (fileType == ".txt") {
		contentType = "text/plain";
	}
	else if (fileType == ".bmp") {
		contentType = "application/x-bmp";
	}
	else if (fileType == ".jpg") {
		contentType = "application/x-jpg";
	}
	else if (fileType == ".png") {
		contentType = "application/x-png";
	}	
	else if (fileType == ".gif") {
		contentType = "image/gif";
	}
	else if (fileType == ".ico") {
		contentType = "image/x-icon";
	}
	return contentType;
}
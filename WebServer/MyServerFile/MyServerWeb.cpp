
#include "MyServerWeb.h"

#define PATHWEB "./webui"

char method[8];
char URL[1024];
char protoVer[16];
char fileType[16];

int request(SOCKET s) {
	char buf[1024] = "";
	int iret = recv(s, buf, sizeof(buf), 0);
	if (iret == SOCKET_ERROR) {
		printf("[client] recv error ...\n");
		return -1;
	}
	if (iret == 0) {
		printf("[client] quit ...\n");
		return 0;
	}
	printf("******************request header message st******************\n");
	printf("%s", buf);
	printf("******************request header message en******************\n");

	sscanf(buf, "%[^ ] %[^ ] %[^ ]", method, URL, protoVer);
	sscanf(buf, "%[^.]%[^ ]", fileType, fileType);
	printf("fileType=%s\n", fileType);

	return 1;
}

void response(SOCKET s) {
	char buf[1024] = { 0 };
	int bufLen = sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: %s; charset=gb2312\r\n", getContentType(fileType).c_str());
	char file[1024] = { 0 };
	sprintf(file, "%s%s", PATHWEB, URL);
	printf("请求文件路径：%s\n", file);

	printf("......................response message header begin......................\n");
	FILE *f = fopen(file, "rb");
	//打开文件失败
	if (f == NULL) {
		static char defPage[] = "<html><b><center>404 not find!</center></b></html>";
		printf("读取文件失败\n");
		bufLen += sprintf(buf + bufLen, "Content-Length: %d\r\n\r\n%s", strlen(defPage), defPage);
		send(s, buf, strlen(buf), 0);
		printf("[server] response message ...\n%s", buf);
		return;
	}

	fseek(f, 0, SEEK_END);
	int contentLength = ftell(f);
	bufLen += sprintf(buf + bufLen, "Content-Length: %d\r\n\r\n", contentLength);
	send(s, buf, strlen(buf), 0);
	printf("%s", buf);
	printf("......................response message header end.........................\n");
	fseek(f, 0, SEEK_SET);

	printf("......................response message content begin......................\n");
	size_t len;
	memset(buf, 0, sizeof(buf));
	while ((len = fread(buf, sizeof(char), sizeof(buf), f)) > 0) {
		printf("%s", buf);
		if (send(s, buf, len, 0) == SOCKET_ERROR) {
			printf("\n[server] send error ...\n");
			break;
		}
		memset(buf, 0, sizeof(buf));
	}
	printf("......................response message content end........................\n");

	fclose(f);
}

string getContentType(const string &fileType) {
	string contentType = "";

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
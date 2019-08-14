
#include "MyServerWeb.h"

#define PATHWEB "./webui"
#define PATHFILE TEXT("E:/smallinsect")

#define MAXBUF 8096

char method[32];
char URL[1024];
char protoVer[16];
char fileType[1024];
const char *matching = ".";

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

	sscanf(buf, "%[^ ] %[^ ] %[^ \r\n]", method, URL, protoVer);
	char szTemp[1024];
	strcpy(szTemp, URL);
	char *p = strtok(szTemp, matching);
	while (p) {
		strcpy(fileType, p);
		p = strtok(NULL, matching);
	}

	printf("fileType=%s\n", fileType);

	return 1;
}

void response(SOCKET s) {
	char resHeadBuf[1024] = { 0 };
	int bufLen;//
	char szContent[MAXBUF];//发送内容
	int contentLength = 0;//内容长度
	TCHAR szFilePath[MAX_PATH] = PATHFILE;//默认路径
	if (fileType[0] == '/') {//文件路径
		strcpy(fileType, ".html");
		bufLen = sprintf(resHeadBuf, "HTTP/1.1 200 OK\r\nContent-Type: %s; charset=gb2312\r\n", getContentType(fileType).c_str());

		contentLength = sprintf(szContent, "<html><head><title>路径下的文件</title></head><body><table>");
		size_t len = strlen(URL);
		while (len > 1 && URL[len-1] == '/') {//去掉末尾/
			URL[len - 1] = '\0';
			--len;
		}
		if (len == 1) {//默认目录
			URL[0] = '\0';
		}
		if (strstr(URL, "/.")) {//当前目录
			URL[len - 2] = '\0';
		}
		if (strstr(URL, "/..")) {//上级目录
			len = len - 3;//去掉/..
			while (len > 1 && URL[len-1] != '/') {//找到上一级目录/
				--len;
			}
			URL[len - 2] = '\0';
		}
		lstrcat(szFilePath, URL);
		lstrcat(szFilePath, "/*");
		WIN32_FIND_DATA findFileData;
		HANDLE hListFile = FindFirstFile(szFilePath, &findFileData);
		if (hListFile == INVALID_HANDLE_VALUE) {
			printf("FindFirstFile error %d...\n", GetLastError());
		}
		while (true) {
			contentLength += sprintf(szContent + contentLength, "<tr><td><a href='%s/%s'>%s</a></td></tr>", URL, findFileData.cFileName, findFileData.cFileName);
			if (!FindNextFile(hListFile, &findFileData)) {
				break;
			}
		}
		contentLength += sprintf(szContent + contentLength, "</table></body></html>");

		bufLen += sprintf(resHeadBuf + bufLen, "Content-Length: %d\r\n\r\n", contentLength);
		send(s, resHeadBuf, strlen(resHeadBuf), 0);
		send(s, szContent, contentLength, 0);
	}
	else {//文件
		size_t len = strlen(fileType);
		fileType[len+1] = '\0';
		while (len > 0) {
			fileType[len] = fileType[len-1];
			--len;
		}
		fileType[0] = '.';
		printf("fileType=%s\n", fileType);
		bufLen = sprintf(resHeadBuf, "HTTP/1.1 200 OK\r\nContent-Type: %s; charset=gb2312\r\n", getContentType(fileType).c_str());

		strcat(szFilePath, URL);
		FILE *f = fopen(szFilePath, "r");
		if (f == NULL) {
			contentLength = sprintf(szContent, "<html><head><title>提示</title><meta http-equiv='Content-Type' content='text/html; charset = gb2312' /></head><body>404</body></html>");
			bufLen += sprintf(resHeadBuf + bufLen, "Content-Length: %d\r\n\r\n", contentLength);

			send(s, resHeadBuf, strlen(resHeadBuf), 0);
			send(s, szContent, contentLength, 0);
			return;
		}
		contentLength = fread(szContent, 1, MAXBUF, f);
		bufLen += sprintf(resHeadBuf + bufLen, "Content-Length: %d\r\n\r\n", contentLength);

		send(s, resHeadBuf, strlen(resHeadBuf), 0);
		send(s, szContent, contentLength, 0);
	}

}

string getContentType(const string &fileType) {
	string contentType = "";

	if (fileType == ".htm" || fileType == ".html") {
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
	else if (fileType == ".pdf") {
		contentType = "application/pdf";
	}
	else if (fileType == ".ico") {
		contentType = "image/x-icon";
	}
	return contentType;
}
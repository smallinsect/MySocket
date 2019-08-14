
#include "MyServerWeb.h"

#define PATHWEB "./webui"

#define MAXBUF 8096

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
	char resHeadBuf[1024] = { 0 };
	int bufLen = sprintf(resHeadBuf, "HTTP/1.1 200 OK\r\nContent-Type: %s; charset=gb2312\r\n", getContentType(fileType).c_str());
	char file[1024] = { 0 };
	sprintf(file, "%s%s", PATHWEB, URL);
	printf("请求文件路径：%s\n", file);

	char szContent[MAXBUF];
	int contentLength = 0;
	contentLength = sprintf(szContent, "<html><head><title>路径下的文件</title></head><body><table>");
	TCHAR szPath[MAX_PATH] = TEXT("E:/smallinsect");
	TCHAR szFilePath[MAX_PATH];
	lstrcpy(szFilePath, szPath);
	lstrcat(szFilePath, "/*");
	WIN32_FIND_DATA findFileData;
	HANDLE hListFile = FindFirstFile(szFilePath, &findFileData);
	if (hListFile == INVALID_HANDLE_VALUE) {
		printf("FindFirstFile error %d...\n", GetLastError());
	}
	while (true) {
		contentLength += sprintf(szContent + contentLength, "<tr><td><a href='./%s'>%s</a></td></tr>", findFileData.cFileName, findFileData.cFileName);
		if (!FindNextFile(hListFile, &findFileData)) {
			break;
		}
	}
	contentLength += sprintf(szContent + contentLength, "</table></body></html>");

	bufLen += sprintf(resHeadBuf + bufLen, "Content-Length: %d\r\n\r\n", contentLength);
	send(s, resHeadBuf, strlen(resHeadBuf), 0);
	send(s, szContent, contentLength, 0);

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


#include "MyServer.h"
#include "MyRequest.h"


int MyServer::Start(const char* ip, u_short port) {//开始服务器
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		cout << "[server] WSAStartup error ..." << GetLastError() << endl;
		return -1;
	}
	cout << "[server] WSAStartup success ..." << endl;

	//创建一个套接字 IPV4 流类型 TCP协议
	m_skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_skt == INVALID_SOCKET) {
		cout << "[server] socket error ..." << GetLastError() << endl;
		return -1;
	}
	cout << "[server] socket success ..." << endl;

	//IPV4协议 ip 端口
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;//IPV4
	addr.sin_addr.s_addr = inet_addr(ip);//自动绑定本机IP地址
	addr.sin_port = htons(port);//端口8000

	//绑定套接字
	if (bind(m_skt, (const sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		cout << "[server] bind error ..." << GetLastError() << endl;
		return -1;
	}
	cout << "[server] bind success ..." << endl;

	//监听套接字
	if (listen(m_skt, SOMAXCONN) == SOCKET_ERROR) {
		cout << "[server] listen error ..." << GetLastError() << endl;
		return -1;
	}
	cout << "[server] listen success ..." << endl;

	FD_ZERO(&m_fds);//初始化
	FD_SET(m_skt,&m_fds);//服务socket放入集合

	m_bExit = false;
	// 创建线程
	threadid = _beginthread(workthread, 0, this);

	cout << "[server] WebServer running ..." << endl;
	return 0;
}

void MyServer::Stop() {//停止服务
	m_bExit = true;
	closesocket(m_skt);//关闭套接字
	WSACleanup();//清理数据
}

bool MyServer::dispatch(SOCKET skt, char* buf, int length) {//分发消息
	MyRequest request;
	request.setParam(buf, strlen(buf) + 1);

	char responseBuf[MAX_SIZE_BUF] = { 0 };
	int bufLen = sprintf(responseBuf,"HTTP/1.1 200 OK\r\nContent-Type: %s; charset=gb2312\r\n", request.getContentType().c_str());
	//Content-Type: image/png
	FILE* fp = fopen(request.file.c_str(), "rb");
	if (fp == NULL) {//读取文件失败
		char page[] = "<html><head><title>爱白菜的小昆虫标题</title></head><body style='color:green;background:#eee;'>爱白菜的小昆虫404</body></html>";

		bufLen += sprintf(responseBuf + bufLen, "Content-Length: %d\r\n\r\n", strlen(page));

		send(skt, responseBuf, bufLen, 0);
		send(skt, page, strlen(page), 0);
		return true;
	}

	fseek(fp, 0, SEEK_END);//文件指针移至文件末尾，偏移量0
	long contentLength = ftell(fp);//获取文件内容总字节数
	fseek(fp, 0, SEEK_SET);//文件指针移至文件头，偏移量0

	bufLen += sprintf(responseBuf + bufLen, "Content-Length: %d\r\n\r\n", contentLength);
	send(skt, responseBuf, bufLen, 0);

	size_t fl;
	while ((fl = fread(responseBuf, sizeof(char), MAX_SIZE_BUF, fp)) > 0) {
		send(skt, responseBuf, fl, 0);
	}

	fclose(fp);
	return true;
}

void MyServer::workthread(void* lpParam) {
	MyServer* webserver = (MyServer*)lpParam;
	timeval tv = { 0, 1000 };

	char buf[MAX_SIZE_BUF];
	while (!webserver->m_bExit) {
		fd_set readfds = webserver->m_fds;
		if (select(0, &readfds, NULL, NULL, &tv) <= 0) {
			continue;
		}
		for (u_int i = 0; i < readfds.fd_count; ++i) {
			SOCKET tskt = readfds.fd_array[i];
			if (tskt == webserver->m_skt) {//有新的客户端请求连接
				SOCKET cskt = accept(webserver->m_skt, NULL, NULL);
				if (cskt == INVALID_SOCKET) {
					cout << "[server] accept error ..." << GetLastError() << endl;
					return;
				}
				cout << "[server] accept success ..." << endl;
				FD_SET(cskt, &webserver->m_fds);
				continue;
			}
			memset(buf, 0, sizeof(buf));
			int irecv = recv(tskt, buf, sizeof(buf), 0);//接受浏览器请求的消息
			if (irecv > 0) {
				// 处理接受的消息
				webserver->dispatch(tskt, buf, strlen(buf));
			}
			// 将客户端套接字，从集合中删除。
			FD_CLR(tskt, &webserver->m_fds);
			//irecv=0 表示客户端退出 irecv=-1 表示客户端异常退出 
			closesocket(tskt); // 关闭客户端套接字
		}
	}
	cout << "threadid = "<< webserver->threadid  << "end ..." << endl;
}

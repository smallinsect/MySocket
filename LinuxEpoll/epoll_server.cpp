#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("./a.out port\n");
		exit(-1);
	}
	//创建套接字
	int sskt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[1]));
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);//监听本机所有的IP

	//绑定套接字和IP地址、端口
	if(bind(sskt, (sockaddr *)&saddr, sizeof(saddr)) == -1){
		printf("[server] bind error ...\n");
		return -1;
	}
	printf("[server] bind success ...\n");

	//监听套接字
	if(listen(sskt, SOMAXCONN) == -1){
		printf("[server] listen error ...\n");
		return -1;
	}
	printf("[server] listen success ...\n");
	
	//epoll树根节点
	int epfd = epoll_create(3000);
	//初始化epoll树 监听的sskt挂到树上
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = sskt;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sskt, &ev);
	
	//存储发什么变化的fd对应信息
	struct epoll_event all[3000];
	int allLen = sizeof(all)/sizeof(all[0]);
	while(true){
		//使用epoll通知内核文件IO检测 委托内核检测事件
		int ret = epoll_wait(epfd, all, allLen, -1);
		//根据ret遍历all数组
		for(int i = 0; i < ret; ++i){
			int fd = all[i].data.fd;
			//有新的连接
			if(fd == sskt){
				struct sockaddr_in addrCli = {0};
				socklen_t addrLenCli = sizeof(addrCli);
				//接受连接请求-accept不阻塞
				int cskt = accept(sskt, (sockaddr *)&addrCli, &addrLenCli);
				if(cskt == -1){
					printf("[server] accept error ...\n");
					return -1;
				}
				//打印客户端信息
				printf("client ip:%s port:%d\n", inet_ntoa(addrCli.sin_addr), ntohs(addrCli.sin_port));
				//将信得到的cskt挂到树上
				struct epoll_event temp;
				temp.events = EPOLLIN;
				temp.data.fd = cskt;
				epoll_ctl(epfd, EPOLL_CTL_ADD, cskt, &temp);
			}else{
				//已经连接的客户端有数据发送过来
				//之处理客户端发来的数据
				if(!all[i].events & EPOLLIN){
					continue;
				}
				char buf[1024];
				//接受客户端的信息
				int len = recv(fd, buf, 1024, 0);
				if(len == -1){//接受数据错误
					printf("[server] recv error ...\n");
					return -1;
				}else if(len == 0){//客户端断开连接
					printf("[client] quit ...\n");
					//检测的fd从树上删除
					ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
					if(ret == -1){
						printf("[server] epoll_ctl -del error ...");
						return -1;
					}
					close(fd);
				}else{//接受数据正常
					printf("[client] %s\n", buf);
					//把客户端发来的数据返回给用户
					send(fd, buf, strlen(buf), 0);
				}
			}

		}
	}

	close(sskt);
	return 0;
}

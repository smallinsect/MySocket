# TCP协议RST：RST介绍、什么时候发送RST包

RST：（Reset the connection）

## 一、RST介绍

RST标示复位、用来异常的关闭连接。
1. 发送RST包关闭连接时，不必等缓冲区的包都发出去，直接就丢弃缓冲区中的包，发送RST。
2. 而接收端收到RST包后，也不必发送ACK包来确认。

## 二、什么时候发送RST包

1.  建立连接的SYN到达某端口，但是该端口上没有正在 监听的服务。
2. TCP收到了一个根本不存在的连接上的分节。
3. 请求超时。 使用setsockopt的SO_RCVTIMEO选项设置recv的超时时间。接收数据超时时，会发送RST包。

## 三、尝试手动发送RST包

1. 使用shutdown、close关闭套接字，发送的是FIN，不是RST。
2. 套接字关闭前，使用sleep。对运行的程序Ctrl+C，会发送FIN，不是RST。
3. 套接字关闭前，执行return、exit(0)、exit(1)，会发送FIN、不是RST。 
以上几种方法，都不能发送RST包。 发送RST包，需要自己伪造数据包进行发送。
测试代码：http://blog.csdn.net/guowenyan001/article/details/11742621

## 参考资料：

RST攻击：http://baike.baidu.com/view/1044719.htm
几种连接中出现RST的情况：http://my.oschina.net/costaxu/blog/127394

http://russelltao.iteye.com/blog/1405349

http://blog.csdn.net/youkuxiaobin/article/details/6917880
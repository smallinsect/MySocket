# MySocket
用于学习socket编程，记录的代码

网络报文的数据结构格式

网络数据报文的格式定义

select模型处理多个客户端


#MyServer
一个简单的web服务器

#MyServerFile
用于获取本地文件路径和文件，发送给浏览器的服务器。

#Web消息请求和响应的规则
https://www.runoob.com/http/http-header-fields.html



## Linux的Tcp修改缓存

内核设置的套接字缓存

```
/proc/sys/net/core/rmem_default，net.core.rmem_default，套接字接收缓存默认值 (bit)
/proc/sys/net/core/wmem_default，net.core.wmem_default，套接字发送缓存默认值 (bit)
/proc/sys/net/core/rmem_max，net.core.rmem_max，套接字接收缓存最大值 (bit)
/proc/sys/net/core/wmem_max，net.core.wmem_max，发送缓存最大值 (bit)
tcp缓存
/proc/sys/net/ipv4/tcp_rmem：net.ipv4.tcp_rmem，接收缓存设置，依次代表最小值、默认值和最大值(bit)
4096    87380    4194304
/proc/sys/net/ipv4/tcp_wmem：net.ipv4.tcp_wmem，发送缓存设置，依次代表最小值、默认值和最大值(bit)
/proc/sys/net/ipv4/tcp_mem:
94500000    915000000    927000000
对应net.ipv4.tcp_mem，tcp整体缓存设置，对所有tcp内存使用状况的控制，单位是页，依次代表TCP整体内存的无压力值、压力模式开启阀值、最大使用值，用于控制新缓存的分配是否成功
tcp或者udp的设置会覆盖内核设置。其中只有tcp_mem是用于tcp整体内存的控制，其他都是针对单个连接的。
```
修改配置

```
sysctl -w net.core.rmem_max=8388608
...
sysctl -w net.ipv4.tcp_mem='8388608 8388608 8388608'
```

估算TCP缓存大小

```
以tcp接收缓存为例（实际上发送窗口=对方的接收窗口），tcp接收缓存有2部分组成：接收窗口及应用缓存，应用缓存用于应用的延时读及一些调度信息。linux使用net.ipv4.tcp_adv_win_scale（对应文件/proc/sys/net/ipv4/tcp_adv_win_scale）指出应用缓存的比例。
if tcp_adv_win_scale > 0: 应用缓存 = buffer / (2^tcp_adv_win_scale)，tcp_adv_win_scale默认值为2，表示缓存的四分之一用于应用缓存，可用接收窗口占四分之三。
if tcp_adv_win_scale <= 0: 应用缓存 = buffer - buffer/2^(-tcp_adv_win_scale)，即接收窗口=buffer/2^(-tcp_adv_win_scale)，如果tcp_adv_win_scale=-2，接收窗口占接收缓存的四分之一。
那如果能估算出接收窗口就能算出套接字缓存的大小。如何算接收窗口呢？
BDP(bandwidth-delay product，带宽时延积) = bandwith(bits/sec) * delay(sec)，代表网络传输能力，为了充分利用网络，最大接收窗口应该等于BDP。delay = RTT/2。
receive_win = bandwith * RTT / 2
buffer = rec_win/(3/4) (上面知道tcp_adv_win_scale=2时表示接收窗口占buffer的3/4
以我们的机房为例，同机房的带宽为30Gbit/s，两台机器ping可获得RTT大概为0.1ms，那BDP=(30Gb/1000) * 0.1 / 2 = 1.5Mb，buffer = 1.5Mb * 4 / 3 = 2Mb
```

TCP缓存的综合考虑

```
如第三节我们真的能设置tcp最大缓存为2Mb吗？通常一台机器会部署多个服务，一个服务内部也往往会建立多个tcp连接。但系统内存是有限的，如果有4000个连接，满负荷工作，达到最大窗口。那么tcp整体消耗内存=4000 * 2Mb = 1GB。
并发连接越多，默认套接字缓存越大，则tcp占用内存越大。当套接字缓存和系统内存一定时，会影响并发连接数。对于高并发连接场景，系统资源不足，缩小缓存限制；并发连接少时，可以适当放大缓存限制。
linux自身引入了自动调整接收缓存的功能，来使吞吐量最大，(缓存最大值还是受限于tcp_rmem[2])。配置项如下。
net.ipv4.tcp_moderate_rcvbuf = 1 (/proc/sys/net/ipv4/tcp_moderate_rcvbuf)
```

**系统TCP连接内存大小限制**

- TCP的每一个连接请求，读写都需要占用系统内存资源，可根据系统配置，对TCP连接数，内存大小，限制调优。

------

**查看系统内存资源**

```
记录内存 详情：cat /proc/meminfo
命令：free -m
```

------

**TCP连接内存限制 内核调优**

**修改文件：/etc/sysctl.conf**

**生效命令：sysctl -p /etc/sysctl.conf**

**1、限制TCP总连接内存使用**

```
# 内核分给TCP的内存大小范围，单位为page。
# 第一个数字表示，当 tcp 使用的 page 少于 196608 时，kernel 不对其进行任何的干预
# 第二个数字表示，当 tcp 使用了超过 262144 的 pages 时，kernel 会进入 “memory pressure” 压力模式
# 第三个数字表示，当 tcp 使用的 pages 超过 393216 时就会报：Out of socket memory。
net.ipv4.tcp_mem = 196608 262144 393216
```

注：最大范围可根据系统内存大小进行设置。
注：内核分配给TCP连接的内存，单位是Page，1 Page = 4096 Bytes。
注：查看分配给TCP连接得内存：getconf PAGESIZE

**2、限制TCP单连接读写内存使用**

```
# TCP连接读缓存大小。最小内存 缺省内存 最大内存（单位为：Byte字节）
net.ipv4.tcp_rmem = 4096 4096 1073741824

# TCP连接写缓存大小。最小内存 缺省内存 最大内存（单位为：Byte字节）
net.ipv4.tcp_wmem = 4096 4096 1073741824
```

注:4.0G TCP内存能容纳的连接数读写相加： 4000M/8 = 500K = 50万并发
注:TCP请求读写缓存可跟据程序平均请求资源大小设置。

**如何永久性修改TCP缓冲区的大小：在 \**/etc/sysctl.conf\** 修改如下内容：**

```
net.ipv4.tcp_mem = 6177504 8236672 16777216
net.ipv4.tcp_rmem = 4096 873800 16777216
net.ipv4.tcp_wmem = 4096 873800 16777216
```

**然后执行 \**/sbin/sysctl –p\** 使之生效，修改完后再查看**

```
king@ubuntu:~$ sudo /sbin/sysctl -p
[sudo] king 的密码： 
net.ipv4.tcp_mem = 6177504 8236672 16777216
net.ipv4.tcp_rmem = 4096 873800 16777216
net.ipv4.tcp_wmem = 4096 873800 16777216
```


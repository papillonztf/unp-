#include	"unp.h"//
#include	<time.h>// time();ctime()

int
main(int argc, char **argv)
{
	int					listenfd, connfd;//监听套接字，连接套接字
	struct sockaddr_in	servaddr;//ipv4套接字（源）地址结构
	char				buff[MAXLINE];//4096
	time_t				ticks;//

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);//tcp-ipv4 监听套接字

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        //htonl:主机字节序转为长整型网络字节序.
        //sin_addr.s_addr 为unit32 ，与htonl返回值类型一致
        //INADDR_ANY:0x00000000,表示被动方的任意网络接口。
	servaddr.sin_port        = htons(13);	/* daytime server */

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
        //绑定套接字（源）地址结构至监听套接字
        //以便当listen时，被动方是指定固定ip、端口的接受连接。
        //而主动方，connect时往往是内核指定临时的ip、端口
	Listen(listenfd, LISTENQ);
        //将套接字转为监听套接字，在该套接字经由内核监听主动方连接请求
        //LISTENQ 1028 <unp.h>,即未完成监听队列和已完成监听队列总长<该值

	for ( ; ; ) //循环处理每个客户端连接（为采用多线程，多进程）
    {
		connfd = Accept(listenfd, (SA *) NULL, NULL);
        //Accept：主动方阻塞方式接受一个被对方连接，返回一个连接套接字
        //该连接套接字用于被对方和该指定的主动方通信，可有多个连接套接字
        //第二、三各参数：值-结果传递，分别获取主动方（目的）套接字地址结构
        //和长度。此处，不获取故NULL
        //Accept 是阻塞的：本进程在该accept调用中会被投入睡眠
        //（cpu释放，内存不释放），因为此时，进程一直在等待客户连接到达并
        //被内核接受（完成三次握手）
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s",ctime(&ticks));
        snprintf(buff+strlen(buff),sizeof(buff)-strlen(buff)," Provided By Zi Tengfei\r\n");
        //标准io-snprintf：格式化输出，将小于等于size-1个字符写入buff，
        //并最后自动加一个null在尾部。返回值不包括该结束字符。
        //"%.24s\r\n":不超过24字节长度的字符串，
        Write(connfd, buff, strlen(buff));
        //write系统调用：返回值>0,读到的字节数;=0 已读到文件尾;-1,出错
        //返回 -1,失败磁盘满或进程队列长度限制
        //write不需要循环;而read需要循环，因为内核可能需要时间准备好数据。
        //"/r/n":windows,以\r\n换行，linux以\n换行。
        //此处，被定向至终端的主动方使用，刷新其行缓存。
        //time（）：返回time_t 秒数;
        //ctime():返回字符串，"Wed Jun 30 21:49:08 1993\n"
		Close(connfd);
        //类似close(fd),关闭连接套接字描述符:四次挥手，被对方主动断开连接。
	}
}

/*
describe:使用阻塞io模型，tcp-ipv4，一对一时间获取客户端。 
*/
#include	"unp.h"
//<netinet/in.h>:struct in_addr;
//<sys/socket.h>:(SA)socaddr;socket();connect();
//<string.h>:bzero(),strerro()
//<netinet/in.h>:htos();stoh()
//<arpa/inet.h>:inet_pton(),inet_ntop()
//
//
int
main(int argc, char **argv)//cha**等价于char*argv[],指针数组存放参数。
{
	int					sockfd, n;
        //套接字描述符，是一种文件描述符。
        //因此，可以使用系统调用write、read进行读写操作

	char				recvline[MAXLINE + 1];
        //在此是操作buff，
        //对于，文件iow write、read相当于标准io的进程缓冲区，
        //建议MAXLINE=4096!
                            
	struct sockaddr_in	servaddr;//ipv4套接字地址结构体
        //套接字地址结构体，一般有地址族、结构体长度ip、port等成员。
        //地址族：ip4,ip6,unix域（本地），数据链路，存储。
        //对应：

    //套接字地址结构：可分为套接字目的地址结构和套接字源地址结构
    //主动连接方，往往需要进程初始化套接字目的地址结构。如客户端。
    //被动连接方，往往需要进程初始化套接字源地址结构。如服务端。
	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");
        //非系统调用、库函数调用出错。
        //后续，将提供一系列包裹函数！

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        //套接字：是一种文件描述符，用于通信的一种文件。
        //是应用层和下三层的接口，是用户和内核的接口。
        //socket(),只确定了套接字的协议类型，但未确定ip和port
        //如AF_INET ipv6地址族,SOCKE_STREAM tcp字节流套接字
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));//套接字地址结构初始化为0
	servaddr.sin_family = AF_INET;//指定地址族成员，即指定网络层协议，ipv4
	servaddr.sin_port   = htons(13);	/* daytime server */
        //指定(目的)端口成员，
        //主机字节序转换为网络字节序
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);
        //指定（目的）ip成员，
        //点分十进制ip转化为32位二进制ip
        //inet_pton()适用与ipv4,ipv6,由地址族family区分。

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");
        //系统调用connect：主动方三次握手连接被动方，返回0,成功;-1,失败。
        //需要指定套接字描述符、套接字（目的）地址结构体，结构体大小。
        //对应主动方，内核会给主动方套接字分配port，指定ip。
        //SA，即通用套接字结构sockaddr的别名，历史原因，未能以void×替代。

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
        //系统调用read:文件io,阻塞的！！，读取套接字描述符。
        //返回值 <0失败，=0 文件尾部eof,  >0成功
        //返回值小于要求读取的字节数，对于套接字，是缓冲区未即使缓存所要求的。
        //循环read：内核的缓存机制，使得未能一次读取完。
        //循环结束：对于套接字，连接断开，使得返回值<0，或文件尾，返回值=0;
        //fputs():标准io，以首个null字符终止写入，达到一次写入一行的目的。
        //标准io流定向值标准输出，因此位行缓存，以‘\n’,exit(),缓存满刷新。
        //fput:返回值>0,成功;返回EOF，出错。 
    if (n < 0)
		err_sys("read error");

	exit(0);
}

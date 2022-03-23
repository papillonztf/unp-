
/*
describe:使用阻塞io模型，tcp-ipv6，一对一时间获取客户端。 
*/
#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd, n;
	struct sockaddr_in6	servaddr;//ipv6套接字地址结构体

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ( (sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");//AF_INET6 ipv6地址族

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family = AF_INET6;//
	servaddr.sin6_port   = htons(13);	/* daytime server */
	if (inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0)//ipv6
		err_quit("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

	exit(0);
}

# 介绍:

unpbook学习文件夹:是关于unp(unix网络编程)一书(第三版)的学习,主要包括各章节的例子和修改. 
相关的笔记unpnote 在/git_repo/和flamingo2018/flamingoserver/下)  

```cpp
以编译unp书中例子(./unpv13e/intro/目录下),帮助文档./unpv13e/readme
0. 

cd ./unpv13e

./configure   //make前须调用该脚本进行系统,编译器检测
  
cd lib   
make   
  
cd ../libfree   //可能报错,没关系
make   
    
cd ../libroute  
make //这一步可能会出错，可忽略，只是表示你的系统不支持 4.4BSD，并不影响对该书的学习  
    
cd ../libgai   
make   

// vim lib/unp.h //将 unp.h 中#include "../config.h"修改为#include "config.h"   
sudo cp lib/unp.h /usr/local/include   
sudo cp config.h /usr/local/include  

1.编译daytimetcpsrv.c和daytimetcpcli.c.<一对一tcp阻塞>
    gcc -g daytimetcpsrv.c -o daytimetcpsrv -lunp -static //必须使用-static,否则报未定义.-lunp -static 必须放置在可执行文件之后,否则包未定义.不用-L -I

    实际使用vscode进行编译和调试:
    终端-配置任务-生成tasks.json ,这是编译.
    运行-添加配置-生成launch.json,这是调试.
    两个文件设置请看vscode-打开文件夹 ./unp学习/ -查看.vscode/

2. 如果报错"connection refuse"
service xinetd status    //daytimetcpsrv.c需要xinted服务支持

cd /etc/xinetd.d/  
  
vim daytime  //将  daytime  文件中的两个  disable = yes 修改成  disable = no ，并保存退出，然后重启  xinetd ；

3.如果报"bind error "

sudo netstat -anpt|grep "13"    //所有包括listen&&数字&&pid和进程名&&tcp相关的套接字信息.查看13端口情况.一看被xinetd服务占用.修改源文件的port 
sudo lsof -nP -i:13             //网络相关&&显示端口号而不是名.也是查端口是否占用

4.使用
./daytimetcpserv               
./daytimetcpcli   127.0.0.1        // 客户端主动连接,被动断开

ps -ef|grep "day"                   //获取服务端pid
kill pid                            //正常终止服务端
```

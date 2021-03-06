#include "headH.h"
#define BUFFSIZE 1024
int main()
{
	int fds[2];
	char buf[BUFFSIZE];
	int i,rc,maxfd;
	fd_set inset1,insert2;
	struct timeval tv;

	//create 2 fifos
	if((mkfifo("fifo1",O_CREAT|O_EXCL) < 0) && (errno != EEXIST))
		printf("cannot create fifoserver1\n");
	if((mkfifo("fifo2",O_CREAT|O_EXCL) < 0) && (errno != EEXIST))
		printf("cannot create fifoserver2\n");
	//open fifos
	if((fds[0] = open("fifo1",O_RDWR|O_NONBLOCK,0)) < 0)
		Sys_error("open fifo1 error",1);
	if((fds[1] = open("fifo2",O_RDWR|O_NONBLOCK,0)) < 0)
		Sys_error("open fifo2 error",2);
	if((rc = write(fds[0],"Hello!\n",BUFFSIZE)))
		printf("rc=%d\n",rc);
	lseek(fds[0],0,SEEK_SET);
	maxfd = fds[0] > fds[1] ? fds[0] : fds[1] ;

	//初始化描述符集，并将文件描述符加入到相应的描述符集
	FD_ZERO(&inset1);
	FD_SET(fds[0],&inset1);
	FD_ZERO(&insert2);
	FD_SET(fds[1],&insert2);
	//循环测试文件描述符是否准备就绪，并调用select函数
	while(FD_ISSET(fds[0],&inset1) || FD_ISSET(fds[1],&insert2))
	{
		if(select(maxfd+1,&inset1,&insert2,NULL,NULL) < 0)
			Sys_error("select error",3);
		else{
			if(FD_ISSET(fds[0],&inset1))
			{
				rc = read(fds[0],buf,BUFFSIZE);
				if(rc > 0)
				{
					buf[BUFFSIZE] = '\0';
					printf("read: %s\n",buf);
				}else
					Sys_error("read error",4);
		
		}
	}
	}

	return 0;
}

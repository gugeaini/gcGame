#include<iostream>
#include<zinx.h>
#include<ZinxTCP.h>
#include<sys/types.h>
#include<sys/unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"GameChannel.h"
#include"GameProtocol.h"
#include"GameRole.h"
#include"GameMsg.h"
#include"msg.pb.h"
#include"AOIWrold.h"
#include"TimerChannel.h"
#include"RandomName.h"

using namespace std;
extern RandomName random_Name;

void Deamo()
{
		/*1开进程并杀父*/
	int spid=fork();
	if (spid < 0)
	{
		/*系统异常*/
		exit(-1);
	}
	if (spid > 0)
	{
		exit(0);
	}
	/*监测进程创建成功*/
	/*2当会长*/
	setsid();
	/*3设置工作目录*/

	/*4重定向文件描述符*/
	int nullFd = open("/dev/null", O_RDWR);
	if (nullFd >= 0)
	{
		dup2(nullFd, 0);
		dup2(nullFd, 1);
		dup2(nullFd, 2);
		close(nullFd);
	}
	/*5设置掩码*/
	umask(0);
	/*6核心逻辑*/

	while (1)
	{
		int pid = fork();
		if (pid < 0)
		{
			/*系统异常*/
			exit(-1);
		}
		if (pid > 0)
		{
			/*监控进程等待守护进程正常退出*/
			int ret;
			wait(&ret);
			if (ret == 0)
			{
				exit(0);
			}
		}
		else if(pid == 0)
		{
			break;
		}

	}

	
	

}

int main()
{
	Deamo();

	random_Name.LoadFile();

	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899,new GameTcpFact())));
	ZinxKernel::Zinx_Add_Channel(*(new TimerChannel()));

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}
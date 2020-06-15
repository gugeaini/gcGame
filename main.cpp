#include<iostream>
#include<zinx.h>
#include<ZinxTCP.h>
#include"GameChannel.h"
#include"GameProtocol.h"
#include"GameRole.h"
#include"GameMsg.h"
#include"msg.pb.h"
#include"AOIWrold.h"
using namespace std;



int main()
{



	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899,new GameTcpFact())));

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}
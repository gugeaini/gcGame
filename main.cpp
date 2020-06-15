#include<iostream>
#include<zinx.h>
#include<ZinxTCP.h>
#include"GameChannel.h"
#include"GameProtocol.h"
#include"GameRole.h"
#include"GameMsg.h"
#include"msg.pb.h"
#include<stdio.h>
using namespace std;

/*测试消息类代码*/
void tsetMsg()
{
	auto pologin = new pb::SyncPid();
	pologin->set_pid(1);
	pologin->set_username("tom");

	GameMsg myMsg(GameMsg::MSG_TYPE_SYNCPID_LOGIN, pologin);
	auto str = myMsg.GetSerialize();
	for (auto sign : str)
	{
		
		printf("%02X ", sign);
	}
	puts("");
	
	char buf[] = { 0x08, 0x01, 0x12, 0x03, 0x74, 0x6F, 0x6D };
	str = buf;

	GameMsg myMsg2(GameMsg::MSG_TYPE_SYNCPID_LOGIN, str);
	pb::SyncPid* temp = dynamic_cast<pb::SyncPid*>(myMsg2.poMessage);
	cout << temp->pid() << endl;
	cout << temp->username() << endl;
}

int main()
{
#if 1
	tsetMsg();
#endif // 1


	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899,new GameTcpFact())));

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}
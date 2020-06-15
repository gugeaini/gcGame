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



/*测试 AOI世界地图*/
class myPlayer :
	public Player
{
public:
	int x;
	int y;
	string name;
	myPlayer(int _x,int _y,string _name):x(_x),y(_y),name(_name){}
	// 通过 Player 继承
	virtual int GetX() override
	{
		return x;
	}
	virtual int GetY() override
	{
		return y;
	}
};

int main()
{
	AOIWrold aoi(20, 200, 6, 50, 230, 6);
	myPlayer p1(60, 107, "1");
	myPlayer p2(91, 118, "2");
	myPlayer p3(147, 133, "3");
	aoi.AddPlayer(&p1);
	aoi.AddPlayer(&p2);
	aoi.AddPlayer(&p3);
	for (auto sign : aoi.GetSrdPlayers(&p1))
	{
		cout << dynamic_cast<myPlayer*>(sign)->name << endl;
	}


	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8899,new GameTcpFact())));

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}
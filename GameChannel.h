#pragma once
#include<ZinxTCP.h>
#include"GameProtocol.h"
class GameChannel:
	public ZinxTcpData
{
public:
	GameProtocol* poProtocol = nullptr;	//绑定向上层传输协议对象
	GameChannel(int _fd);
	// 通过 ZinxTcpData 继承
	virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class GameTcpFact :
	public IZinxTcpConnFact
{
public:

	// 通过 IZinxTcpConnFact 继承
	virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;

};
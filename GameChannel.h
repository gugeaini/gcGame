#pragma once
#include<ZinxTCP.h>
#include"GameProtocol.h"
class GameChannel:
	public ZinxTcpData
{
public:
	GameProtocol* poProtocol = nullptr;
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
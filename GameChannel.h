#pragma once
#include<ZinxTCP.h>
class GameChannel:
	public ZinxTcpData
{
public:

	// 通过 ZinxTcpData 继承
	virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class GameTcpFact :
	public IZinxTcpConnFact
{
	GameChannel* poTcp;
public:

	// 通过 IZinxTcpConnFact 继承
	virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;

};
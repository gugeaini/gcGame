#pragma once
#include<ZinxTCP.h>
#include"GameProtocol.h"
class GameChannel:
	public ZinxTcpData
{
public:
	GameProtocol* poProtocol = nullptr;
	GameChannel(int _fd);
	// ͨ�� ZinxTcpData �̳�
	virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class GameTcpFact :
	public IZinxTcpConnFact
{
public:

	// ͨ�� IZinxTcpConnFact �̳�
	virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;

};
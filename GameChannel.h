#pragma once
#include<ZinxTCP.h>
class GameChannel:
	public ZinxTcpData
{
public:

	// ͨ�� ZinxTcpData �̳�
	virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class GameTcpFact :
	public IZinxTcpConnFact
{
	GameChannel* poTcp;
public:

	// ͨ�� IZinxTcpConnFact �̳�
	virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;

};
#include "GameChannel.h"

GameChannel::GameChannel(int _fd):ZinxTcpData(_fd)
{
}

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return poProtocol;
}

ZinxTcpData* GameTcpFact::CreateTcpDataChannel(int _fd)
{
	/*创建tcp通道对象*/
	auto poChannel = new GameChannel(_fd);

	/*创建protoc协议对象*/
	auto poProto = new GameProtocol();

	/*绑定*/
	poChannel->poProtocol = poProto;
	poProto->poChannel = poChannel;
	ZinxKernel::Zinx_Add_Proto(*poProto);
	
	return poChannel;
}

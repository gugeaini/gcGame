#include "GameChannel.h"
#include"GameRole.h"
#include"GameProtocol.h"
GameChannel::~GameChannel()
{
	if (poProtocol != nullptr)
	{
		ZinxKernel::Zinx_Del_Proto(*poProtocol);
		delete poProtocol;
		poProtocol = nullptr;
	}
}
GameChannel::GameChannel(int _fd):ZinxTcpData(_fd)
{
}

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return poProtocol;
}

ZinxTcpData* GameTcpFact::CreateTcpDataChannel(int _fd)
{
	/*创建对象*/
	auto poChannel = new GameChannel(_fd);
	auto poProto = new GameProtocol();
	auto poRole = new GameRole();

	/*绑定*/
	poChannel->poProtocol = poProto;
	poProto->poChannel = poChannel;

	poProto->poRole = poRole;
	poRole->poProtocol = poProto;
	
	/*添加框架*/
	ZinxKernel::Zinx_Add_Proto(*poProto);
	ZinxKernel::Zinx_Add_Role(*poRole);

	return poChannel;
}

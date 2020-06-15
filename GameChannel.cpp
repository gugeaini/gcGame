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
	/*����tcpͨ������*/
	auto poChannel = new GameChannel(_fd);

	/*����protocЭ�����*/
	auto poProto = new GameProtocol();

	/*��*/
	poChannel->poProtocol = poProto;
	
	return poChannel;
}

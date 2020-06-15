#include<iostream>
#include "GameProtocol.h"
#include"GameMsg.h"
#include"GameChannel.h"
#include"msg.pb.h"
using namespace std;

UserData* GameProtocol::raw2request(std::string _szInput)
{
	MuiltMsg* pRet = new MuiltMsg();
	/*数据协议：|长度 4 bye| IP 4 bye | 消息内容 | */
	szCacheData.append(_szInput);

	while (1)
	{
		if (szCacheData.size() < 8)
		{
			/*当前缓存数据头不足 不处理*/
			break;
		}
		int ilen = 0;
		int ip = 0;
		ilen |= szCacheData[0] << 0;
		ilen |= szCacheData[1] << 8;
		ilen |= szCacheData[2] << 16;
		ilen |= szCacheData[3] << 24;
		ip |= szCacheData[4] << 0;
		ip |= szCacheData[5] << 8;
		ip |= szCacheData[6] << 16;
		ip |= szCacheData[7] << 24;

		if (ilen > szCacheData.size()-8)
		{
			/*当前缓存数据内容不完整 不处理*/
			break;
		}

		GameMsg* poMsg = new GameMsg((GameMsg::MSG_TYPE)ip, szCacheData.substr(8, ilen));
		pRet->m_GameMsg_list.push_back(poMsg);
		szCacheData.erase(0, 8 + ilen);
	}

	return pRet;
}

std::string* GameProtocol::response2raw(UserData& _oUserData)
{
	/*数据协议：|长度 4 bye| IP 4 bye | 消息内容 | */
	GET_REF2DATA(GameMsg, outMsg, _oUserData);

	int ilen = 0;
	int ip = 0;
	string str = outMsg.GetSerialize();
	ilen = str.size();
	ip = outMsg.MsgType;

	auto ret = new string();
	ret->push_back((ilen >> 0) & 0xff);
	ret->push_back((ilen >> 8) & 0xff);
	ret->push_back((ilen >> 16) & 0xff);
	ret->push_back((ilen >> 24) & 0xff);
	ret->push_back((ip >> 0) & 0xff);
	ret->push_back((ip >> 8) & 0xff);
	ret->push_back((ip >> 16) & 0xff);
	ret->push_back((ip >> 24) & 0xff);

	ret->append(str);

	return ret;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
	return nullptr;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
	return poChannel;
}

#pragma once
#include<zinx.h>
#include<string>
class GameChannel;
class GameProtocol:
	public Iprotocol
{
	std::string szCacheData;	//tcp传送过来的数据
public:
	GameChannel* poChannel = NULL;	//绑定向下层传输的通道对象

	// 通过 Iprotocol 继承
	virtual UserData* raw2request(std::string _szInput) override;
	virtual std::string* response2raw(UserData& _oUserData) override;
	virtual Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
	virtual Ichannel* GetMsgSender(BytesMsg& _oBytes) override;
};


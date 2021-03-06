#pragma once
#include<zinx.h>
#include<string>
#include<google/protobuf/message.h>
#include<list>
/*单条消息*/
class GameMsg:
	public UserData
{
public:
	enum MSG_TYPE
	{
		MSG_TYPE_SYNCPID_LOGIN=1,		//玩家ID+玩家姓名
		MSG_TYPE_TALK=2,				//聊天内容
		MSG_TYPE_POSITION=3,			//新位置
		MSG_TYPE_BROADCAST=200,		//玩家ID,聊天内容/初始位置/动作/新位置 向外发送的广播请求
		MSG_TYPE_SYNCPID_LOGOFF=201,	//玩家ID和玩家姓名
		MSG_TYPE_SYNCPLAYERS=202		//周围玩家们的位置
	}MsgType;
	google::protobuf::Message* poMessage = nullptr;

public:
	/*根据具体的消息构建对象*/
	GameMsg(MSG_TYPE _type, google::protobuf::Message* _msg);
	/*根据序列化的消息构建对象*/
	GameMsg(MSG_TYPE _type, std::string _outData);

	/*把消息序列化*/
	std::string GetSerialize();

	virtual ~GameMsg();

};

/*多条消息*/
class MuiltMsg :
	public UserData
{
public:
	MuiltMsg();
	virtual ~MuiltMsg();
	std::list<GameMsg*> m_GameMsg_list;
};
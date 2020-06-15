#include "GameMsg.h"
#include"msg.pb.h"

GameMsg::GameMsg(MSG_TYPE _type, google::protobuf::Message* _msg):MsgType(_type),poMessage(_msg)
{
}

GameMsg::GameMsg(MSG_TYPE _type, std::string _outData):MsgType(_type)
{
	switch (_type)
	{
    case GameMsg::MSG_TYPE_SYNCPID_LOGIN:
        poMessage = new pb::SyncPid();
        break;
    case GameMsg::MSG_TYPE_TALK:
        poMessage = new pb::Talk();
        break;
    case GameMsg::MSG_TYPE_POSITION:
        poMessage = new pb::Position();
        break;
    case GameMsg::MSG_TYPE_BROADCAST:
        poMessage = new pb::BroadCast();
        break;
    case GameMsg::MSG_TYPE_SYNCPID_LOGOFF:
        poMessage = new pb::SyncPid();
        break;
    case GameMsg::MSG_TYPE_SYNCPLAYERS:
        poMessage = new pb::SyncPlayers();
        break;
    default:
        break;
	}

    poMessage->ParseFromString(_outData);//反序列化
}
/*str不是堆对象可能会有问题*/
std::string GameMsg::GetSerialize()
{
    std::string str;
    poMessage->SerializeToString(&str);
	return str;
}

GameMsg::~GameMsg()
{
}

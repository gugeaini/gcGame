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

std::string GameMsg::GetSerialize()
{
    std::string str;
    poMessage->SerializeToString(&str);
	return str;
}

GameMsg::~GameMsg()
{
   /*注意不能直接析构poMessage需要强转*/
    if (poMessage != nullptr)
    {    
        delete poMessage;
    }
}

MuiltMsg::MuiltMsg()
{
 
}

MuiltMsg::~MuiltMsg()
{
    for (auto it=m_GameMsg_list.begin();it!=m_GameMsg_list.end();)
    {
        auto temp = *it;
        delete temp;
        it = m_GameMsg_list.erase(it);
    }
 
}

#include "GameRole.h"
#include"GameProtocol.h"
#include"GameChannel.h"
#include"GameMsg.h"
#include"msg.pb.h"
#include<iostream>
#include<algorithm>
#include<random>
#include"TimerChannel.h"
#include"RandomName.h"
using namespace std;

RandomName random_Name;
static AOIWrold* aoiw = new AOIWrold(0, 400, 20, 0, 400, 20);
static default_random_engine random_engine(time(NULL));
class ExitTimer :
	public TimerOutProc
{
public:

	// 通过 TimerOutProc 继承
	virtual void Proc() override
	{
		ZinxKernel::Zinx_Exit();
	}
	virtual int GetTimerSec() override
	{
		return 10;
	}
}et;


void GameRole::ProcChatTalk(std::string _content)
{
	/*发送聊天内容给每一位玩家*/
	for (auto sign : ZinxKernel::Zinx_GetAllRole())
	{
		/*组织聊天消息类*/
		auto* pBr = new pb::BroadCast();
		pBr->set_pid(pid);
		pBr->set_username(szName);
		pBr->set_allocated_content(&_content);
		pBr->set_tp(1);
		/*构造聊天消息类和动态转换发送对象*/
		auto pGameRole=dynamic_cast<GameRole*>(sign);
		auto pGameMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pBr);
		/*发送*/
		ZinxKernel::Zinx_SendOut(*pGameMsg, *(pGameRole->poProtocol));	
	}
}
void GameRole::ProcNewLocation(int _x, int _y, int _z, int _v)
{
	/*	位置移动需要处理旧位置的摘除和新位置的插入以及视野的变化
		对周围出现的新格子进行发送出现消息
		对周围消失的旧格子进行发送消失消息*/
	/* s1旧集合*/
	auto s1 = aoiw->GetSrdPlayers(this);
	aoiw->DelPlayer(this);
	x = _x;
	y = _y;
	z = _z;
	v = _v;
	aoiw->AddPlayer(this);
	/* s2新集合*/
	auto s2 = aoiw->GetSrdPlayers(this);

	for (auto sign : s1)
	{
		if (s2.end() == find(s2.begin(), s2.end(), sign))
		{
			/*视野消失*/
			auto pGameRole = dynamic_cast<GameRole*>(sign);
			if (pGameRole != this)
			{
				ProcVisualDisaAppears(pGameRole);
			}
			
		}
	}
	for (auto sign : s2)
	{
		if (s1.end() == find(s1.begin(), s1.end(), sign))
		{
			/*视野出现*/
			auto pGameRole = dynamic_cast<GameRole*>(sign);
			if (pGameRole != this)
			{
				ProcVisualAppears(pGameRole);
			}
		}
		
	}

	/*对坐标的移动进行发送消息*/
	for (auto sign : aoiw->GetSrdPlayers(this))
	{
		pb::BroadCast* ret = new pb::BroadCast();
		ret->set_pid(pid);
		ret->set_username(szName);
		ret->set_tp(4);
		auto aqPostion = ret->mutable_p();
		aqPostion->set_x(_x);
		aqPostion->set_y(_y);
		aqPostion->set_z(_z);
		aqPostion->set_v(_v);
		auto aqPayer = dynamic_cast<GameRole*>(sign);
		ZinxKernel::Zinx_SendOut(*(new GameMsg(GameMsg::MSG_TYPE_BROADCAST, ret)), *(aqPayer->poProtocol));
	}
	

}
void GameRole::ProcVisualDisaAppears(GameRole* _role)
{
	auto pMsg = _role->CreateLogoffName();
	ZinxKernel::Zinx_SendOut(*pMsg,*poProtocol);
	pMsg = CreateLogoffName();
	ZinxKernel::Zinx_SendOut(*pMsg,*(_role->poProtocol));
}
void GameRole::ProcVisualAppears(GameRole* _role)
{
	auto pMsg = _role->CreateSelfPostion();
	ZinxKernel::Zinx_SendOut(*pMsg, *poProtocol);
	pMsg = CreateSelfPostion();
	ZinxKernel::Zinx_SendOut(*pMsg, *(_role->poProtocol));
}
GameMsg* GameRole::CreateLoginIdName()
{
	pb::SyncPid* pMsg = new pb::SyncPid();
	pMsg->set_pid(pid);
	pMsg->set_username(szName);

	return new GameMsg(GameMsg::MSG_TYPE_SYNCPID_LOGIN, pMsg);
}
GameMsg* GameRole::CreateSrdPlayers()
{
	pb::SyncPlayers* pret = new pb::SyncPlayers();
	
	/*获取周围每一位玩家的信息*/
	for (auto sign : aoiw->GetSrdPlayers(this)) 
	{
		auto pplayer = pret->add_ps();
		auto temp = dynamic_cast<GameRole*>(sign);
		pplayer->set_pid(temp->pid);
		pplayer->set_username(temp->szName);
		auto ppstion = pplayer->mutable_p();
		ppstion->set_x(temp->x);
		ppstion->set_y(temp->y);
		ppstion->set_z(temp->z);
		ppstion->set_v(temp->v);
	}
	

	return new GameMsg(GameMsg::MSG_TYPE_SYNCPLAYERS,pret);
}
GameMsg* GameRole::CreateSelfPostion()
{
	pb::BroadCast* ret = new pb::BroadCast();
	ret->set_pid(pid);
	ret->set_username(szName);
	ret->set_tp(2);
	auto aqPostion = ret->mutable_p();
	aqPostion->set_x(x);
	aqPostion->set_y(y);
	aqPostion->set_z(z);
	aqPostion->set_v(v);

	return new GameMsg(GameMsg::MSG_TYPE_BROADCAST, ret);
}
GameMsg* GameRole::CreateLogoffName()
{
	auto pRet = new pb::SyncPid();
	pRet->set_pid(pid);
	pRet->set_username(szName);

	return new GameMsg(GameMsg::MSG_TYPE_SYNCPID_LOGOFF,pRet);
}


GameRole::GameRole()
{
	szName = random_Name.GetName();
	x = 100 + random_engine() % 50;
	z = 100 + random_engine() % 50;
}
GameRole::~GameRole()
{
	random_Name.AlosName(szName);
}
bool GameRole::Init()
{
	/*摘除定时器任务*/
	if (ZinxKernel::Zinx_GetAllRole().size() <= 2)
	{
		MngTimer::GetMngTimer()->Timer_Del_Proc(&et);
	}
	

	bool bRet = false;
	pid = poProtocol->poChannel->GetFd();
	/*登录成功*/

	/*把自己添加到游戏世界*/
	bRet = aoiw->AddPlayer(this);
	if (bRet)
	{
		/*向自己发送自己的Id 和 姓名 */
		ZinxKernel::Zinx_SendOut(*(CreateLoginIdName()), *poProtocol);

		/*向自己发送周围玩家的位置*/
		ZinxKernel::Zinx_SendOut(*(CreateSrdPlayers()), *poProtocol);

		/*向周围玩家发送自己的位置*/
		for (auto sign : aoiw->GetSrdPlayers(this))
		{
			auto aqPayer = dynamic_cast<GameRole*>(sign);
			ZinxKernel::Zinx_SendOut(*(CreateSelfPostion()), *(aqPayer->poProtocol));
		}

	}

	return bRet;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
	GET_REF2DATA(MuiltMsg, _muiltMsg, _poUserData);
	for (auto _outMsg : _muiltMsg.m_GameMsg_list)
	{
		switch (_outMsg->MsgType)
		{
		case GameMsg::MSG_TYPE_TALK:
			ProcChatTalk(dynamic_cast<pb::Talk*>(_outMsg->poMessage)->content());
			break;
		case GameMsg::MSG_TYPE_POSITION:
		{
			pb::Position* pMsg = dynamic_cast<pb::Position*>(_outMsg->poMessage);
			ProcNewLocation(pMsg->x(), pMsg->y(), pMsg->z(), pMsg->v());
			break;
		}
			
		}
	}

	return nullptr;
}

void GameRole::Fini()
{
	aoiw->DelPlayer(this);
	for (auto sign : aoiw->GetSrdPlayers(this))
	{
		auto aqProtocol = dynamic_cast<GameRole*>(sign)->poProtocol;
		ZinxKernel::Zinx_SendOut(*(CreateLogoffName()),*aqProtocol);
	}

	if (ZinxKernel::Zinx_GetAllRole().size() <= 1)
	{
		/*启动退出定时器*/
		MngTimer::GetMngTimer()->Timer_Add_Proc(&et);
	}
}

int GameRole::GetX()
{
	return (int)x;
}
int GameRole::GetY()
{
	return (int)z;
}

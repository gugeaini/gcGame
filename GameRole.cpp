#include "GameRole.h"
#include"GameProtocol.h"
#include"GameChannel.h"
#include"GameMsg.h"
#include"msg.pb.h"

static AOIWrold* aoiw = new AOIWrold(0, 400, 20, 0, 400, 20);

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
	x = 150;
	z = 150;
	szName = "GUGE";
}

GameRole::~GameRole()
{
}

bool GameRole::Init()
{
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
}

int GameRole::GetX()
{
	return (int)x;
}

int GameRole::GetY()
{
	return (int)z;
}

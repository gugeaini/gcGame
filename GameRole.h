#pragma once
#include<string>
#include<zinx.h>
#include"AOIWrold.h"

class GameMsg;
class GameProtocol;
class GameRole:
	public Irole ,public Player
{
	float x;
	float y;//高
	float z;
	float v;
	
	int pid;
	std::string szName;

	void ProcChatTalk(std::string _content);	/*聊天内容处理*/
	void ProcNewLocation(int _x,int _y,int _z,int _v);	/*移动处理*/
	void ProcVisualDisaAppears(GameRole* _role);		/*处理视野消失*/
	void ProcVisualAppears(GameRole* _role);			/*处理视野出现*/

	GameMsg* CreateLoginIdName();	/*向自己发送自己的Id 和 姓名 */
	GameMsg* CreateSrdPlayers();	/*向自己发送周围玩家的位置*/
	GameMsg* CreateSelfPostion();	/*向周围玩家发送自己的位置*/
	GameMsg* CreateLogoffName();	/*向周围玩家发送自己下线消息*/
public:
	GameProtocol* poProtocol=nullptr;

public:
	GameRole();
	virtual ~GameRole();

	// 通过 Irole 继承
	virtual bool Init() override;
	virtual UserData* ProcMsg(UserData& _poUserData) override;
	virtual void Fini() override;


	// 通过 Player 继承
	virtual int GetX() override;
	virtual int GetY() override;

};


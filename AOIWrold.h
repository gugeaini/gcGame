#pragma once
#include<vector>
#include<list>

/*
网格法AOI

	目的：			获取周围玩家
	模型：			将游戏世界的坐标分割成网格，玩家属于某个网格
	周围：			玩家所属网格周围8个相邻网格内的玩家
	游戏世界矩形：	包含固定数量网格对象的容器
	网格对象：		包含若干玩家的容器
	玩家：			拥有横纵坐标的对象

*/

/*玩家抽象类*/
class Player
{
public:
	virtual int GetX() = 0;
	virtual int GetY() = 0;
};

/*格子类*/
class Grid
{
public:
	std::list<Player*>m_player_list;	//单个网格内的所有玩家
};

/*AOI地图世界类*/
class AOIWrold
{
	int x_begin;
	int x_end;
	int x_count;
	int y_begin;
	int y_end;
	int y_count;

	int x_width;
	int y_width;
public:
	std::vector<Grid>m_grid_vector;		//游戏世界所有网格

public:
	AOIWrold(int _xb, int _xe, int _xc, int _yb, int _ye, int _yc);
	virtual ~AOIWrold();

	/*获取周围玩家*/
	std::list<Player*> GetSrdPlayers(Player* _player);
	bool AddPlayer(Player* _player);
	void DelPlayer(Player* _player);
};


#include "AOIWrold.h"

using namespace std;

AOIWrold::AOIWrold(int _xb, int _xe, int _xc, int _yb, int _ye, int _yc):
	x_begin(_xb),x_end(_xe),x_count(_xc),y_begin(_yb),y_end(_ye),y_count(_yc)
{
	/*网格宽度=（end - begin）/count */
	x_width = (x_end - x_begin) / x_count;
	y_width = (y_end - y_begin) / y_count;

	for (int i = 0; i < x_width * y_width; i++)
	{
		Grid temp;
		m_grid_vector.push_back(temp);
	}
}

AOIWrold::~AOIWrold()
{
}

std::list<Player*> AOIWrold::GetSrdPlayers(Player* _player)
{
	list<Player*> ret;

	/*计算当前玩家所在 横格 和 纵格 */
	int grid_id = (_player->GetX() - x_begin) / x_width + (_player->GetY() - y_begin) / y_width * x_count;
	int x_index = grid_id % x_count;
	int y_index = grid_id / x_count;
	/*判断是否有相邻八个格子*/
	//	1
	if (x_index > 0 && y_index > 0)
	{
		auto& sign = m_grid_vector[grid_id - 1 - x_count].m_player_list;
		ret.insert(ret.end(), sign.begin(), sign.end());
	}
	//	2
	if (y_index > 0)
	{
		auto& sign = m_grid_vector[grid_id - x_count].m_player_list;
		ret.insert(ret.end(), sign.begin(), sign.end());
	}
	//	3
	if (x_index + 1 < x_count && y_index > 0)
	{
		auto& sign = m_grid_vector[grid_id + 1  - x_count].m_player_list;
		ret.insert(ret.end(), sign.begin(), sign.end());
	}
	//	4
	if (x_index > 0)
	{
		auto& sign = m_grid_vector[grid_id - 1].m_player_list;
		ret.insert(ret.end(), sign.begin(), sign.end());
	}
	// 6
	if (x_index + 1 < x_count)
	{
		auto& sign = m_grid_vector[grid_id + 1].m_player_list;
		ret.insert(ret.end(), sign.begin(), sign.end());
	}
	//	7
	if (x_index > 0 && y_index + 1 < y_count)
	{
		auto& sign = m_grid_vector[grid_id - 1 + x_count].m_player_list;
		ret.insert(ret.end(), sign.begin(), sign.end());
	}
	//	8
	if (y_index + 1 < y_count)
	{
		auto& sign = m_grid_vector[grid_id + x_count].m_player_list;
		ret.insert(ret.end(), sign.begin(), sign.end());
	}
	//	9
	if (x_index + 1 < x_count && y_index + 1 < y_count)
	{
		auto& sign = m_grid_vector[grid_id + 1 + x_count].m_player_list;
		ret.insert(ret.end(), sign.begin(), sign.end());
	}
	return ret;
}

bool AOIWrold::AddPlayer(Player* _player)
{
	/*当前格子=(x-xbegin)/xwidth + (y - ybegin)/ywidth*xcount */
	int grid_id = (_player->GetX() - x_begin) / x_width + (_player->GetY() - y_begin) / y_width * x_count;
	m_grid_vector[grid_id].m_player_list.push_back(_player);
	return true;
}

void AOIWrold::DelPlayer(Player* _player)
{
	int grid_id = (_player->GetX() - x_begin) / x_width + (_player->GetY() - y_begin) / y_width * x_count;
	m_grid_vector[grid_id].m_player_list.remove(_player);
}

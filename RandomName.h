#pragma once
#include<vector>
#include<iostream>

using namespace std;
/*名字类*/
class First
{
public:
	string m_First;			//姓
	vector<string>m_szData;	//名字
};

class RandomName
{
	vector<First*> m_Last_First;
public:
	virtual ~RandomName();
	string GetName();	//取名字
	void AlosName(string _name);	//还名字
	void LoadFile();	//导入文件
};


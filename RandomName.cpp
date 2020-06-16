#include "RandomName.h"
#include<random>
#include<fstream>

static default_random_engine random_engine;

RandomName::~RandomName()
{
	for (auto sign : m_Last_First)
	{
		delete sign;
	}
	m_Last_First.erase(m_Last_First.begin(), m_Last_First.end());
}

string RandomName::GetName()
{
	int numFirst = random_engine() % m_Last_First.size();
	int numLast = random_engine() % (m_Last_First[numFirst]->m_szData.size());

	auto first = m_Last_First[numFirst]->m_First;
	auto last = m_Last_First[numFirst]->m_szData[numLast];
	
	/*摘除名字*/
	m_Last_First[numFirst]->m_szData.erase(m_Last_First[numFirst]->m_szData.begin() + numLast);

	/*若该姓的名字为空就摘除*/
	if (m_Last_First[numFirst]->m_szData.size() <= 0)
	{
		delete m_Last_First[numFirst];
		m_Last_First.erase(m_Last_First.begin() + numFirst);

	}

	return last + " " + first;
}

void RandomName::AlosName(string _name)
{
	int posIndex = _name.find(" ", 0);
	auto last = _name.substr(0, posIndex);
	auto first = _name.substr(posIndex + 1, _name.size() - posIndex - 1);
	
	bool falg = false;
	for (auto sign : m_Last_First)
	{
		if (sign->m_First == last)
		{
			sign->m_szData.push_back(first);
			falg = true;
			break;
		}
	}
	if (falg)
	{
		auto pFirst = new First();
		pFirst->m_First = last;
		pFirst->m_szData.push_back(first);
		m_Last_First.push_back(pFirst);
	}
}

void RandomName::LoadFile()
{
	fstream fsLast("random_last.txt");
	fstream fsFirst("random_first.txt");

	vector<string>temp;

	/*先遍历名字*/
	string str;
	while (getline(fsLast, str))
	{
		temp.push_back(str);
	}

	/*遍历所有姓 并添加到容器中*/
	while (getline(fsFirst, str))
	{
		auto pfirst = new First();
		pfirst->m_First = str;
		pfirst->m_szData.assign(temp.begin(), temp.end());
		m_Last_First.push_back(pfirst);
	}
}

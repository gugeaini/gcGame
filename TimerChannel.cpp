#include "TimerChannel.h"
#include<sys/timerfd.h>
#include<iostream>
using namespace std;

MngTimer* MngTimer::poMng = new MngTimer();

TimerChannel::TimerChannel()
{
}

TimerChannel::~TimerChannel()
{
}

bool TimerChannel::Init()
{
	bool bRet = false;
	int ifd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (ifd >= 0)
	{
		struct itimerspec it {
			{1,0},{1,0}
		};
		if (0 == timerfd_settime(ifd, 0, &it, NULL))
		{
			timerFd = ifd;
			bRet = true;
		}
	}

	return bRet;
}

bool TimerChannel::ReadFd(std::string& _input)
{
	bool bRet = false;
	char buf[8] = { 0 };
	if (sizeof(buf) == read(timerFd, buf, sizeof(buf)))
	{
		bRet = true;
		_input.assign(buf, sizeof(buf));
	}
	return bRet;
}

bool TimerChannel::WriteFd(std::string& _output)
{
	return false;
}

void TimerChannel::Fini()
{
	if (timerFd != -1)
	{
		close(timerFd);
		timerFd = -1;
	}
}

int TimerChannel::GetFd()
{
	return timerFd;
}

std::string TimerChannel::GetChannelInfo()
{
	return "timerFd";
}

AZinxHandler* TimerChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return MngTimer::GetMngTimer();
}


MngTimer::MngTimer()
{
	index = 0;
	for (int i = 0; i < 10; i++)
	{
		list<TimerOutProc*>temp;
		m_tack_list.push_back(temp);
	}
}

MngTimer::~MngTimer()
{
}

IZinxMsg* MngTimer::InternelHandle(IZinxMsg& _oInput)
{
	GET_REF2DATA(BytesMsg, outMsg, _oInput);
	unsigned long num = 0;
	outMsg.szData.copy((char*)&num, sizeof(num), 0);
	while (num-- > 0)
	{
		this->index++;
		this->index %= 10;

		list<TimerOutProc*>caChe;

		for (auto it = m_tack_list[this->index].begin(); it != m_tack_list[this->index].end();)
		{
			auto sign = *(it);
			if (sign->iCount <= 0)
			{
				auto temp = *(it);
				caChe.push_back(temp);
				it = m_tack_list[this->index].erase(it);
				Timer_Add_Proc(temp);
			}
			else
			{
				sign->iCount--;
				it++;
			}
		}

		for (auto sign : caChe)
		{
			sign->Proc();
		}
	}

	

	return nullptr;
}

AZinxHandler* MngTimer::GetNextHandler(IZinxMsg& _oNextMsg)
{
	return nullptr;
}

void MngTimer::Timer_Add_Proc(TimerOutProc* _inProc)
{
	int index = (this->index + _inProc->GetTimerSec()) % 10;
	m_tack_list[index].push_front(_inProc);
	_inProc->iCount = _inProc->GetTimerSec() / 10;
}

void MngTimer::Timer_Del_Proc(TimerOutProc* _outProc)
{
	for (auto& sign : m_tack_list)
	{
		for (auto stak : sign)
		{
			if (stak == _outProc)
			{
				sign.remove(_outProc);
				return;
			}
		}
	}
}

MngTimer* MngTimer::GetMngTimer()
{
	return poMng;
}

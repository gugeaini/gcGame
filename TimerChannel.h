#pragma once
#include<zinx.h>
#include<list>
#include<vector>
#include<iostream>
using namespace std;

class TimerOutProc
{
public:
	virtual void Proc() = 0;
	virtual int GetTimerSec() = 0;

	int iCount = -1;
};

class MngTimer :
	public AZinxHandler
{
	vector<list<TimerOutProc*>>m_tack_list;
	static MngTimer* poMng;
	int index;
public:
	MngTimer();
	virtual ~MngTimer();
	// 通过 AZinxHandler 继承
	virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
	virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;

	void Timer_Add_Proc(TimerOutProc* _inProc);
	void Timer_Del_Proc(TimerOutProc* _outProc);
	static MngTimer* GetMngTimer();
};

class TimerChannel:
	public Ichannel
{
	int timerFd = -1;
public:
	TimerChannel();
	virtual ~TimerChannel();

	// 通过 Ichannel 继承
	virtual bool Init() override;

	virtual bool ReadFd(std::string& _input) override;

	virtual bool WriteFd(std::string& _output) override;

	virtual void Fini() override;

	virtual int GetFd() override;

	virtual std::string GetChannelInfo() override;

	virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;

};
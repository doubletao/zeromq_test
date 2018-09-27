#pragma once
#include <thread>
#include <mutex>

class CStdEvent
{
protected:
	bool m_bQiut;//退出标记
	std::mutex m_lockTime;//时间等待锁
	std::condition_variable m_conTime;//时间等待条件锁
public:
	CStdEvent();
	virtual ~CStdEvent();
public:
	void ResetEvent();//重置
	void SetEvent();//注意，这个set只能释放一次信号
	bool Wait(unsigned int nWait = UINT32_MAX);//超时返回false，信号返回true
};

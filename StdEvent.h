#pragma once
#include <thread>
#include <mutex>

class CStdEvent
{
protected:
	bool m_bQiut;//�˳����
	std::mutex m_lockTime;//ʱ��ȴ���
	std::condition_variable m_conTime;//ʱ��ȴ�������
public:
	CStdEvent();
	virtual ~CStdEvent();
public:
	void ResetEvent();//����
	void SetEvent();//ע�⣬���setֻ���ͷ�һ���ź�
	bool Wait(unsigned int nWait = UINT32_MAX);//��ʱ����false���źŷ���true
};

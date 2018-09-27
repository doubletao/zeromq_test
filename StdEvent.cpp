#include "StdEvent.h"

CStdEvent::CStdEvent()
	: m_bQiut(false)
{
}

CStdEvent::~CStdEvent()
{
}

void CStdEvent::ResetEvent()
{
	std::unique_lock<std::mutex> lock(m_lockTime);
	m_bQiut = false;
}

void CStdEvent::SetEvent()
{
	std::unique_lock<std::mutex> lock(m_lockTime);
	m_bQiut = true;
	m_conTime.notify_all();
}

bool CStdEvent::Wait(unsigned int nWait)
{
	std::unique_lock<std::mutex> lock(m_lockTime);
	while (!m_bQiut)
	{
		std::cv_status cvStatus = m_conTime.wait_for(lock, std::chrono::milliseconds(nWait));
		if (std::cv_status::timeout == cvStatus)
		{
			return false;
		}
	}
	return true;
}

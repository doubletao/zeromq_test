
// zeromq_test.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Czeromq_testApp: 
// �йش����ʵ�֣������ zeromq_test.cpp
//

class Czeromq_testApp : public CWinApp
{
public:
	Czeromq_testApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Czeromq_testApp theApp;
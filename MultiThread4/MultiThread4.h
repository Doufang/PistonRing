
// MultiThread4.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMultiThread4App: 
// �йش����ʵ�֣������ MultiThread4.cpp
//

class CMultiThread4App : public CWinApp
{
public:
	CMultiThread4App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMultiThread4App theApp;
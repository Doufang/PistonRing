
// PistonRing.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPistonRingApp: 
// �йش����ʵ�֣������ PistonRing.cpp
//

class CPistonRingApp : public CWinApp
{
public:
	CPistonRingApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPistonRingApp theApp;
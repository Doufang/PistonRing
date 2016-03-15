#pragma once

#include "SerialComm.h"

#define WM_USER_READRELAY_FINISHED WM_USER + 0x300

typedef struct THREADCOMMRELAY
{
	HWND hWnd;					//���ڶ��߳���Ϣ����
	SerialComm* comm;
	bool* running;
	CCriticalSection* pCS;
	UINT16 address;
	UINT16 number;
	bool* back;
	DWORD sleepTime;
}ThreadCommRelay;


extern UINT ThreadCommReadRelay(LPVOID pParam);



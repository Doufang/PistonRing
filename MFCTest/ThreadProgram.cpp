#include "stdafx.h"
#include "ThreadProgram.h"

UINT ThreadCommReadRelay(LPVOID pParam)
{
	ThreadCommRelay* ptp = (ThreadCommRelay*)pParam;
	HWND m_hWnd = ptp->hWnd;
	SerialComm* comm = ptp->comm;
	bool& run = *(ptp->running);
	CCriticalSection* pCS = ptp->pCS;
	UINT16 address = ptp->address;
	UINT16 number = ptp->number;
	bool* back = ptp->back;
	DWORD sleepTime = ptp->sleepTime;
	delete ptp;

	if (!(comm->IsOpen()))
	{
		return 1;
	}

	bool read = false;
	while (run)
	{
		pCS->Lock();
		read = comm->ReadCommRelay(address, number, back);
		pCS->Unlock();

		if (read)
		{
			PostMessage(m_hWnd, WM_USER_READRELAY_FINISHED, 0, 0);
		}

		read = false;
		if (sleepTime < 10)
		{
			sleepTime = 10;
		}
		Sleep(sleepTime);
	}
	return 0;
}

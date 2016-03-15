#pragma once

#include "SerialPort.h"
#include "afxwin.h"
#include <MMSystem.h>
#include <atlimage.h>
#include <Windows.h>
#pragma comment( lib,"winmm.lib" )

class myComm
{
public:
	myComm();
	~myComm();

public:
	CSerialPort mySerialPort;

	void CRC16Fast(BYTE* crcData, int nLength);		//CRCУ��
	void TimeDelay(DWORD timeWantTo);		//��ʱ
	void IntToByte(int intSource, byte* byHigh, byte* byLow);	//

	bool OpenMyComm(HWND pPortOwner, UINT portnr = 1);		//�򿪴���
	void CloseMyComm();		//�رմ���
	bool ReadComm();		//���ڶ�ȡ����
	void SendComm(BYTE* sendData, int sendLength);		//���ڷ�������

	bool ReadCommRelay(int readAddress, int readNumber);		//λ��ָ��
	bool ReadCommRegister(int readAddress, int readNumber);	//�ֶ�ָ��
	bool WriteCommRelay(int writeAddress, bool writeValue);		//λдָ��
	bool WriteCommRegister(int writeAddress, int writeValue);	//��дָ��
};


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

	void CRC16Fast(BYTE* crcData, int nLength);		//CRC校验
	void TimeDelay(DWORD timeWantTo);		//延时
	void IntToByte(int intSource, byte* byHigh, byte* byLow);	//

	bool OpenMyComm(HWND pPortOwner, UINT portnr = 1);		//打开串口
	void CloseMyComm();		//关闭串口
	bool ReadComm();		//串口读取数据
	void SendComm(BYTE* sendData, int sendLength);		//串口发送数据

	bool ReadCommRelay(int readAddress, int readNumber);		//位读指令
	bool ReadCommRegister(int readAddress, int readNumber);	//字读指令
	bool WriteCommRelay(int writeAddress, bool writeValue);		//位写指令
	bool WriteCommRegister(int writeAddress, int writeValue);	//字写指令
};


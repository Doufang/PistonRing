#include "stdafx.h"
#include "myComm.h"


myComm::myComm()
{
}

myComm::~myComm()
{
}


//CRC校验
void myComm::CRC16Fast(BYTE* crcData, int nLength)
{
	//定义一个初始值为 FFFF 的 16 位 CRC 寄存器
	WORD CRC = 0xFFFF;
	BYTE i, j, Tmp = 0;
	//对数据字节循环处理
	for (i = 0; i<nLength; i++)
	{
		CRC ^= crcData[i];		//数据字节和 CRC 寄存器的低 8 位作异或运算， 并把结果在赋到 CRC 寄存器
		//对 CRC 的低 8 位循环处理
		for (j = 0; j<8; j++)
		{
			Tmp = CRC & 0x01;	//获取 CRC 寄存器的最低位数据			
			CRC = CRC >> 1;		//CRC 寄存器右移 1 位（朝最低位）， 同时最高位添零			
			if (Tmp)		//检查移出的最低位是否为 1 。如果为 1 ，则 CRC 寄存器与多项式 A001 异或
			{
				CRC ^= 0xA001;
			}
		}
	}
	//将 CRC 寄存器的高低字节互换
	byte CRCHi = 0x0;
	byte CRCLo = 0x0;
	CRCHi |= CRC;
	CRC >>= 8;
	CRCLo |= CRC;
	//高字节在前，低字节在后
	crcData[nLength] = CRCHi;
	crcData[nLength + 1] = CRCLo;
}

//延时函数
void myComm::TimeDelay(DWORD timeWantTo)
{
	DWORD dwStart = timeGetTime();
	DWORD dwEnd = 0;
	do
	{
		dwEnd = timeGetTime() - dwStart;
	} while (dwEnd < timeWantTo);
}

//整形数转换成字节
void myComm::IntToByte(int intSource, byte* byHigh, byte* byLow)
{
	WORD Address = intSource;
	*byLow = 0;
	*byHigh = 0;
	*byLow |= Address;
	Address >>= 8;
	*byHigh |= Address;
}

//打开串口
bool myComm::OpenMyComm(HWND pPortOwner, UINT portnr)
{
	if (mySerialPort.InitPort(pPortOwner, portnr))
	{
		if (mySerialPort.StartMonitoring())
		{
			return true;
		}
	}
	return false;
}

//关闭串口
void  myComm::CloseMyComm()
{
	mySerialPort.ClosePort();
}

//从串口读取数据
bool myComm::ReadComm()
{
	for (size_t i = 0; i < 20; i++)
	{
		if (mySerialPort.m_ReceiveState)
		{
			break;
		}
		else
		{
			Sleep(10);
		}
	}
	if (mySerialPort.m_ReceiveState)
	{
		BYTE* rxData = mySerialPort.m_ReceiveByte;
		int len = mySerialPort.m_ReceiveNumber;
		byte crcHi = rxData[len - 2];
		byte crcLo = rxData[len - 1];
		CRC16Fast(rxData, len - 2);
		if ((crcHi == rxData[len - 2]) && (crcLo == rxData[len - 1]))
		{
			return true;
		}
	}
	return false;
}

//从串口发送数据
void myComm::SendComm(BYTE* sendData, int sendLength)
{
	mySerialPort.m_ReceiveState = false;
	mySerialPort.m_ReceiveNumber = 0;
	for (size_t i = 0; i < 100; i++)
	{
		mySerialPort.m_ReceiveByte[i] = 0;
	}
	if (mySerialPort.IsOpen())
	{
		mySerialPort.WriteToPort(sendData, sendLength);
	}	
}

//读位数据
bool myComm::ReadCommRelay(int readAddress, int readNumber)
{
	byte byComm[8];
	byComm[0] = 0x1;
	byComm[1] = 0x1;

	IntToByte(readAddress, &byComm[2], &byComm[3]);
	IntToByte(readNumber, &byComm[4], &byComm[5]);

	CRC16Fast(byComm, 6);
	byte *rxData;
	for (size_t i = 0; i < 3; i++)
	{
		SendComm(byComm, 8);
		if (!ReadComm())
		{
			break;
		}
		rxData = mySerialPort.m_ReceiveByte;
		if (rxData[1] == 1)
		{
			return true;
		}
	}
	return false;
}

//读字数据
bool myComm::ReadCommRegister(int readAddress, int readNumber)
{
	byte byComm[8];
	byComm[0] = 0x1;
	byComm[1] = 0x3;

	IntToByte(readAddress, &byComm[2], &byComm[3]);
	IntToByte(readNumber, &byComm[4], &byComm[5]);

	CRC16Fast(byComm, 6);
	byte *rxData;
	for (size_t i = 0; i < 3; i++)
	{
		SendComm(byComm, 8);
		if (!ReadComm())
		{
			break;
		}
		rxData = mySerialPort.m_ReceiveByte;
		if (rxData[1] == 3)
		{
			return true;
		}
	}
	return false;
}

//写位数据
bool myComm::WriteCommRelay(int writeAddress, bool writeValue)
{
	byte byComm[8];
	byComm[0] = 1;
	byComm[1] = 5;

	IntToByte(writeAddress, &byComm[2], &byComm[3]);

	if (writeValue)
	{
		byComm[4] = 0xff;

	}
	else
	{
		byComm[4] = 0x0;
	}
	byComm[5] = 0x0;

	CRC16Fast(byComm, 6);
	byte *rxData;
	for (size_t i = 0; i < 3; i++)
	{
		SendComm(byComm, 8);
		if (!ReadComm())
		{
			break;
		}
		rxData = mySerialPort.m_ReceiveByte;
		if (rxData[1] == 5)
		{
			return true;
		}
	}
	return false;
}

//写字数据
bool myComm::WriteCommRegister(int writeAddress, int writeValue)
{
	byte byComm[8];
	byComm[0] = 1;
	byComm[1] = 6;

	IntToByte(writeAddress, &byComm[2], &byComm[3]);
	IntToByte(writeValue, &byComm[4], &byComm[5]);

	CRC16Fast(byComm, 6);
	byte *rxData;
	for (size_t i = 0; i < 3; i++)
	{
		SendComm(byComm, 8);
		if (!ReadComm())
		{
			break;
		}
		rxData = mySerialPort.m_ReceiveByte;
		if (rxData[1] == 6)
		{
			return true;
		}
	}
	return false;
}

#include "stdafx.h"
#include "SerialComm.h"


static const BYTE aucCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40
};

static const BYTE aucCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
	0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
	0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
	0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
	0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
	0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
	0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
	0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
	0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
	0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
	0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
	0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
	0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
	0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
	0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
	0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
	0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
	0x41, 0x81, 0x80, 0x40
};


SerialComm::SerialComm()
{
}

SerialComm::~SerialComm()
{
}


void SerialComm::CRC16Fast(BYTE* crcData, int nLength)
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

void SerialComm::TblCRC(BYTE* crcData, int nLength)
{
	BYTE ucCRCHi = 0xFF;
	BYTE ucCRCLo = 0xFF;
	DWORD iIndex;

	while (nLength--)
	{
		iIndex = ucCRCLo ^ *(crcData++);
		ucCRCLo = (BYTE)(ucCRCHi ^ aucCRCHi[iIndex]);
		ucCRCHi = aucCRCLo[iIndex];
	}

	//低字节在前，高字节在后
	*crcData = ucCRCLo;
	*(crcData + 1) = ucCRCHi;
}

void SerialComm::ByteToBool(byte* sourceByte, UINT16 number, bool* result)
{
	for (size_t i = 0; i < number; i++)
	{
		*(result + i) = (*(sourceByte + i / 8) >> i % 8) & 1;
	}
}

void SerialComm::ByteToWord(byte* sourceByte, UINT16 number, WORD* result)
{
	for (size_t i = 0; i < number; i++)
	{
		byte* b = sourceByte + 2 * i;
		*(result + i) = MAKEWORD(*(b + 1), *(b));
	}
}

void SerialComm::ByteToDword(byte* sourceByte, UINT16 number, DWORD* result)
{
	for (size_t i = 0; i < number; i++)
	{
		byte* b = sourceByte + 4 * i;
		*(result + i) = MAKELONG(MAKEWORD(*(b + 1), *(b)), MAKEWORD(*(b + 3), *(b + 2)));
	}
}

//串口参数设置
void SerialComm::MySerialSet(short port)
{
	myComm.put_CommPort(port);
	myComm.put_InBufferSize(1024);
	myComm.put_OutBufferSize(1024);
	myComm.put_Settings(L"19200,e,8,1");
	myComm.put_InputMode(1);
	myComm.put_RThreshold(1);
	myComm.put_InputLen(0);
}

//从串口读取数据
bool SerialComm::ReadComm(BYTE* rxData)
{
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len, k;

	variant_inp = myComm.get_Input();
	safearray_inp = variant_inp;
	len = safearray_inp.GetOneDimSize();
	if (len < 5)
	{
		return false;
	}
	for (k = 0; k<len; k++)
	{
		safearray_inp.GetElement(&k, rxData + k);
	}
	byte crcHi = rxData[len - 2];
	byte crcLo = rxData[len - 1];
	TblCRC(rxData, len - 2);
	if ((crcHi == rxData[len - 2]) && (crcLo == rxData[len - 1]))
	{
		return true;
	}
	return false;
}

//从串口发送数据
void SerialComm::SendComm(BYTE* sendData, int sendLength)
{
	CByteArray send;
	send.RemoveAll();
	send.SetSize(sendLength);
	for (size_t i = 0; i<sendLength; i++)
	{
		send.SetAt(i, sendData[i]);
	}
	for (size_t i = 0; i < 105; i++)
	{
		myReceiveDate[i] = 0;
	}
	myComm.put_Output(COleVariant(send));
}


//打开串口
bool SerialComm::OpenMyComm(short port)
{
	//检测串口的状态
	CString sPort;
	sPort.Format(L"COM%d", port);
	HANDLE m_hCom;
	m_hCom = CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	//已经打开、不存在或被占用
	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCom);
		//串口不存在或被占用
		if (!myComm.get_PortOpen())
		{
			return false;
		}
		//串口已经打开，清空缓冲区
		else
		{
			myComm.put_OutBufferCount(0);
			return true;
		}
	}
	//串口可以正常打开
	else
	{
		CloseHandle(m_hCom);
		MySerialSet(port);
		myComm.put_PortOpen(true);
		return true;
	}
}

//关闭串口
void  SerialComm::CloseMyComm()
{
	if (myComm.get_PortOpen())
	{
		myComm.put_PortOpen(false);
	}
}

//读位数据
bool SerialComm::ReadCommRelay(UINT16 address, UINT16 number, bool* back)
{
	if (number > 800)
	{
		number = 800;
	}
	byte byComm[8];
	byComm[0] = 0x1;
	byComm[1] = 0x1;
	byComm[2] = HIBYTE(address);
	byComm[3] = LOBYTE(address);
	byComm[4] = HIBYTE(number);
	byComm[5] = LOBYTE(number);

	TblCRC(byComm, 6);
	for (size_t i = 0; i < 3; i++)
	{
		SendComm(byComm, 8);
		Sleep(50);
		if (!ReadComm(myReceiveDate))
		{
			break;
		}
		if (myReceiveDate[1] == 1)
		{
			ByteToBool(myReceiveDate + 3, number, back);
			return true;
		}
	}
	return false;
}

//读字数据
bool SerialComm::ReadCommWord(UINT16 address, UINT16 number, WORD* back)
{
	if (number > 50)
	{
		number = 50;
	}
	byte byComm[8];
	byComm[0] = 0x1;
	byComm[1] = 0x3;
	byComm[2] = HIBYTE(address);
	byComm[3] = LOBYTE(address);
	byComm[4] = HIBYTE(number);
	byComm[5] = LOBYTE(number);

	TblCRC(byComm, 6);
	for (size_t i = 0; i < 3; i++)
	{
		SendComm(byComm, 8);
		Sleep(50);
		if (!ReadComm(myReceiveDate))
		{
			break;
		}
		if (myReceiveDate[1] == 3)
		{
			ByteToWord(myReceiveDate + 3, number, back);
			return true;
		}
	}
	return false;
}

//读双字数据
bool SerialComm::ReadCommDword(UINT16 address, UINT16 number, DWORD* back)
{
	if (number > 25)
	{
		number = 25;
	}
	byte byComm[8];
	byComm[0] = 0x1;
	byComm[1] = 0x3;
	byComm[2] = HIBYTE(address);
	byComm[3] = LOBYTE(address);
	byComm[4] = HIBYTE(number * 2);
	byComm[5] = LOBYTE(number * 2);

	TblCRC(byComm, 6);
	for (size_t i = 0; i < 3; i++)
	{
		SendComm(byComm, 8);
		Sleep(50);
		if (!ReadComm(myReceiveDate))
		{
			break;
		}
		if (myReceiveDate[1] == 3)
		{
			ByteToDword(myReceiveDate + 3, number, back);
			return true;
		}
	}
	return false;
}

//写位数据
bool SerialComm::WriteCommRelay(UINT16 address, bool value)
{
	byte byComm[8];
	byComm[0] = 1;
	byComm[1] = 5;
	byComm[2] = HIBYTE(address);
	byComm[3] = LOBYTE(address);

	if (value)
	{
		byComm[4] = 0xff;
	}
	else
	{
		byComm[4] = 0x0;
	}
	byComm[5] = 0x0;
	TblCRC(byComm, 6);

	for (size_t i = 0; i < 3; i++)
	{
		SendComm(byComm, 8);
		Sleep(50);
		if (!ReadComm(myReceiveDate))
		{
			break;
		}
		if (myReceiveDate[1] == 5)
		{
			return true;
		}
	}
	return false;
}

//写字数据
bool SerialComm::WriteCommWord(UINT16 address, WORD value)
{
	byte byComm[8];
	byComm[0] = 1;
	byComm[1] = 6;
	byComm[2] = HIBYTE(address);
	byComm[3] = LOBYTE(address);
	byComm[4] = HIBYTE(value);
	byComm[5] = LOBYTE(value);

	TblCRC(byComm, 6);
	byte rxData[8];
	for (size_t i = 0; i < 3; i++)
	{
		SendComm(byComm, 8);
		Sleep(50);
		if (!ReadComm(myReceiveDate))
		{
			break;
		}
		if (myReceiveDate[1] == 6)
		{
			return true;
		}
	}
	return false;
}

//写双字数据
bool SerialComm::WriteCommDword(UINT16 address, DWORD value)
{
	byte byComm[13];
	byComm[0] = 1;
	byComm[1] = 16;
	byComm[2] = HIBYTE(address);
	byComm[3] = LOBYTE(address);
	byComm[4] = 0;
	byComm[5] = 2;
	byComm[6] = 4;
	byComm[7] = HIBYTE(LOWORD(value));
	byComm[8] = LOBYTE(LOWORD(value));
	byComm[9] = HIBYTE(HIWORD(value));
	byComm[10] = LOBYTE(HIWORD(value));

	TblCRC(byComm, 11);
	byte rxData[13];
	for (size_t i = 0; i < 3; i++)
	{
		SendComm(byComm, 13);
		Sleep(50);
		if (!ReadComm(myReceiveDate))
		{
			break;
		}
		if (myReceiveDate[1] == 16)
		{
			return true;
		}
	}
	return false;
}



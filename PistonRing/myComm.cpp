#include "stdafx.h"
#include "myComm.h"


myComm::myComm()
{
}

myComm::~myComm()
{
}


//CRCУ��
void myComm::CRC16Fast(BYTE* crcData, int nLength)
{
	//����һ����ʼֵΪ FFFF �� 16 λ CRC �Ĵ���
	WORD CRC = 0xFFFF;
	BYTE i, j, Tmp = 0;
	//�������ֽ�ѭ������
	for (i = 0; i<nLength; i++)
	{
		CRC ^= crcData[i];		//�����ֽں� CRC �Ĵ����ĵ� 8 λ��������㣬 ���ѽ���ڸ��� CRC �Ĵ���
		//�� CRC �ĵ� 8 λѭ������
		for (j = 0; j<8; j++)
		{
			Tmp = CRC & 0x01;	//��ȡ CRC �Ĵ��������λ����			
			CRC = CRC >> 1;		//CRC �Ĵ������� 1 λ�������λ���� ͬʱ���λ����			
			if (Tmp)		//����Ƴ������λ�Ƿ�Ϊ 1 �����Ϊ 1 ���� CRC �Ĵ��������ʽ A001 ���
			{
				CRC ^= 0xA001;
			}
		}
	}
	//�� CRC �Ĵ����ĸߵ��ֽڻ���
	byte CRCHi = 0x0;
	byte CRCLo = 0x0;
	CRCHi |= CRC;
	CRC >>= 8;
	CRCLo |= CRC;
	//���ֽ���ǰ�����ֽ��ں�
	crcData[nLength] = CRCHi;
	crcData[nLength + 1] = CRCLo;
}

//��ʱ����
void myComm::TimeDelay(DWORD timeWantTo)
{
	DWORD dwStart = timeGetTime();
	DWORD dwEnd = 0;
	do
	{
		dwEnd = timeGetTime() - dwStart;
	} while (dwEnd < timeWantTo);
}

//������ת�����ֽ�
void myComm::IntToByte(int intSource, byte* byHigh, byte* byLow)
{
	WORD Address = intSource;
	*byLow = 0;
	*byHigh = 0;
	*byLow |= Address;
	Address >>= 8;
	*byHigh |= Address;
}

//�򿪴���
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

//�رմ���
void  myComm::CloseMyComm()
{
	mySerialPort.ClosePort();
}

//�Ӵ��ڶ�ȡ����
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

//�Ӵ��ڷ�������
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

//��λ����
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

//��������
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

//дλ����
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

//д������
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

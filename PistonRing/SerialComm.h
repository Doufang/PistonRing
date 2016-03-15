#pragma once

#include "CMSComm.h"
#include "PLCXDM60.h"

class SerialComm
{
public:
	SerialComm();
	~SerialComm();

private:	
	BYTE myReceiveDate[105];

	void CRC16Fast(BYTE* crcData, int nLength);		//CRCУ��
	void TblCRC(BYTE* crcData, int nLength);

	void ByteToBool(byte* sourceByte, UINT16 number, bool* result);
	void ByteToWord(byte* sourceByte, UINT16 number, WORD* result);
	void ByteToDword(byte* sourceByte, UINT16 number, DWORD* result);
	
	void MySerialSet(short port);
	bool ReadComm(BYTE* rxData);		//���ڶ�ȡ����
	void SendComm(BYTE* sendData, int sendLength);		//���ڷ�������

public:
	//���ڱ��������ںʹ��ڿؼ��������ݽ���������Ҫֱ��ʹ�á�
	CMSComm myComm;

	//�򿪴���
	bool OpenMyComm(short port);	

	//�رմ���
	void CloseMyComm();	

	//�жϴ���״̬
	bool IsOpen() { return myComm.get_PortOpen(); };

	//��ȡλ����
	bool ReadCommRelay(UINT16 address, UINT16 number, bool* back);		//λ��ָ��

	//��ȡ������
	bool ReadCommWord(UINT16 address, UINT16 number, WORD* back);	//�ֶ�ָ��

	//��ȡ˫������
	bool ReadCommDword(UINT16 address, UINT16 number, DWORD* back);

	//дλ����
	bool WriteCommRelay(UINT16 address, bool value);		//λдָ��

	//д������
	bool WriteCommWord(UINT16 address, WORD value);	//��дָ��

	//д˫������
	bool WriteCommDword(UINT16 address, DWORD value);
};




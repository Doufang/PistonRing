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

	void CRC16Fast(BYTE* crcData, int nLength);		//CRC校验
	void TblCRC(BYTE* crcData, int nLength);

	void ByteToBool(byte* sourceByte, UINT16 number, bool* result);
	void ByteToWord(byte* sourceByte, UINT16 number, WORD* result);
	void ByteToDword(byte* sourceByte, UINT16 number, DWORD* result);
	
	void MySerialSet(short port);
	bool ReadComm(BYTE* rxData);		//串口读取数据
	void SendComm(BYTE* sendData, int sendLength);		//串口发送数据

public:
	//串口变量，用于和窗口控件进行数据交换，不需要直接使用。
	CMSComm myComm;

	//打开串口
	bool OpenMyComm(short port);	

	//关闭串口
	void CloseMyComm();	

	//判断串口状态
	bool IsOpen() { return myComm.get_PortOpen(); };

	//读取位数据
	bool ReadCommRelay(UINT16 address, UINT16 number, bool* back);		//位读指令

	//读取字数据
	bool ReadCommWord(UINT16 address, UINT16 number, WORD* back);	//字读指令

	//读取双字数据
	bool ReadCommDword(UINT16 address, UINT16 number, DWORD* back);

	//写位数据
	bool WriteCommRelay(UINT16 address, bool value);		//位写指令

	//写字数据
	bool WriteCommWord(UINT16 address, WORD value);	//字写指令

	//写双字数据
	bool WriteCommDword(UINT16 address, DWORD value);
};




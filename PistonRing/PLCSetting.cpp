// PLCSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PistonRing.h"
#include "PLCSetting.h"
#include "afxdialogex.h"


// PLCSetting �Ի���

IMPLEMENT_DYNAMIC(PLCSetting, CDialogEx)

PLCSetting::PLCSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(PLCSetting::IDD, pParent)
	, m_plcHD1252("")
	, m_plcHD1262("")
	, m_plcHD1272("")
{
	m_pParentWnd = pParent;
}

PLCSetting::~PLCSetting()
{
}

void PLCSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_plcHD1252);
	DDX_Text(pDX, IDC_EDIT11, m_plcHD1262);
	DDX_Text(pDX, IDC_EDIT10, m_plcHD1272);
}


BEGIN_MESSAGE_MAP(PLCSetting, CDialogEx)

	ON_EN_CHANGE(IDC_EDIT5, &PLCSetting::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT11, &PLCSetting::OnEnChangeEdit11)
	ON_EN_CHANGE(IDC_EDIT10, &PLCSetting::OnEnChangeEdit10)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON17, &PLCSetting::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &PLCSetting::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON19, &PLCSetting::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON1, &PLCSetting::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &PLCSetting::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &PLCSetting::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &PLCSetting::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &PLCSetting::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &PLCSetting::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &PLCSetting::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &PLCSetting::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &PLCSetting::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &PLCSetting::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &PLCSetting::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &PLCSetting::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON15, &PLCSetting::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &PLCSetting::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON13, &PLCSetting::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &PLCSetting::OnBnClickedButton14)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON20, &PLCSetting::OnBnClickedButton20)
END_MESSAGE_MAP()


// PLCSetting ��Ϣ�������



//����©���Դ����λ��
void PLCSetting::OnEnChangeEdit5()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString m_estr, szTemp;
	GetDlgItem(IDC_EDIT5)->GetWindowTextW(m_estr);
	int nLen = m_estr.GetLength();
	if (!m_estr.IsEmpty())
	{
		szTemp = m_estr.Right(1);
		if ((nLen == 1) && (szTemp == "-"))
		{
			return;
		}	
		BOOL bNum = TRUE;
		CString strtemp = szTemp.SpanIncluding(L"0123456789");
		if (!strtemp.IsEmpty())
			bNum = TRUE;
		else
			bNum = FALSE;
		
		INT nPoint = GetStringNum(m_estr);
		
		if (szTemp == "." || bNum)
		{
			if (nPoint > 0)
			{
				nPoint--;
				m_estr = m_estr.Left(m_estr.GetLength() - 1);
				GetDlgItem(IDC_EDIT5)->SetWindowText(m_estr);
				((CEdit*)GetDlgItem(IDC_EDIT5))->SetSel(-1);
				nLen--;
			}
		}
		else
		{
			m_estr = m_estr.Left(m_estr.GetLength() - 1);
			GetDlgItem(IDC_EDIT5)->SetWindowText(m_estr);
			((CEdit*)GetDlgItem(IDC_EDIT5))->SetSel(-1);
			nLen--;
		}
	}
	
	
}

//����©���������λ��
void PLCSetting::OnEnChangeEdit11()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}

//���ü�϶�������λ��
void PLCSetting::OnEnChangeEdit10()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}


void PLCSetting::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 1:
		//m_plcHD1252 = HD1252;
		//m_plcHD1262 = HD1262;
		//m_plcHD1272 = HD1272;
		//UpdateData(false);
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL PLCSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//m_plcHD1252.Format(L"%d", ToLong32(g_ReadDword[0]));
	//m_plcHD1262.Format(L"%d", ToLong32(g_ReadDword[1]));
	//m_plcHD1272.Format(L"%d", ToLong32(g_ReadDword[2]));
	UpdateData(false);

	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//���е�©���������λ��
void PLCSetting::OnBnClickedButton17()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(422), 1);
	g_CsCommXDM.Unlock();
}

//���е�©���Դ����λ��
void PLCSetting::OnBnClickedButton18()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(420), 1);
	g_CsCommXDM.Unlock();
}

//���е���϶�������λ��
void PLCSetting::OnBnClickedButton19()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������	
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(424), 1);
	g_CsCommXDM.Unlock();
}

//����ַ����м�.�ŵĸ���
int PLCSetting::GetStringNum(CString szVal)
{
	int nNum = 0;
	while (1)
	{
		int nCount = szVal.Find('.');
		if (nCount == -1)
			break;
		szVal = szVal.Right(szVal.GetLength() - nCount - 1);
		nNum++;
	}
	return nNum;

}

//����
void PLCSetting::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������	
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(460), 1);
	g_CsCommXDM.Unlock();
}

//��©��
void PLCSetting::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_AutoRun = false;
	g_Running = true;
	m_pParentWnd->PostMessageW(WM_USER_START_CHECK_LEAKEY, 0, 0);
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(461), 1);
	g_CsCommXDM.Unlock();
	
}

//���϶
void PLCSetting::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_AutoRun = false;
	g_Running = true;
	m_pParentWnd->PostMessageW(WM_USER_START_LOCATION, 0, 0);
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(462), 1);
	g_CsCommXDM.Unlock();
}

//����
void PLCSetting::OnBnClickedButton4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(463), 1);
	g_CsCommXDM.Unlock();
}

//��һ����λ
void PLCSetting::OnBnClickedButton5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(464), 1);
	g_CsCommXDM.Unlock();
}

//���е����Ϻϸ�λ
void PLCSetting::OnBnClickedButton6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(416), 1);
	g_CsCommXDM.Unlock();
}

//���е�����©��λ
void PLCSetting::OnBnClickedButton7()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(417), 1);
	g_CsCommXDM.Unlock();
}

//���е����ϼ�϶λ
void PLCSetting::OnBnClickedButton8()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(418), 1);
	g_CsCommXDM.Unlock();
}

//�������׿�
void PLCSetting::OnBnClickedButton9()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(400), 1);
	g_CsCommXDM.Unlock();
}

//�������׹�
void PLCSetting::OnBnClickedButton10()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(401), 1);
	g_CsCommXDM.Unlock();
}

//ѹ�����׿�
void PLCSetting::OnBnClickedButton11()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(404), 1);
	g_CsCommXDM.Unlock();
}

//ѹ�����׹�
void PLCSetting::OnBnClickedButton12()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(405), 1);
	g_CsCommXDM.Unlock();
}

//�ŷ�����
void PLCSetting::OnBnClickedButton15()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(433), 1);
	g_CsCommXDM.Unlock();
}

//�ŷ��ɿ�
void PLCSetting::OnBnClickedButton16()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(434), 1);
	g_CsCommXDM.Unlock();
}

//�������״�
void PLCSetting::OnBnClickedButton13()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(402), 1);
	g_CsCommXDM.Unlock();
}

//�������׹ر�
void PLCSetting::OnBnClickedButton14()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	g_CsCommXDM.Lock();
	g_CommXDM.WriteCommRelay(PLCM(403), 1);
	g_CsCommXDM.Unlock();
}


BOOL PLCSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	int n;
	if (pMsg->wParam == VK_RETURN)
	{
		CWnd* pWnd = GetFocus();
		int nID;
		if (pWnd)
		{
			nID = pWnd->GetDlgCtrlID();
			UpdateData();
		}
		switch (nID)
		{
		case IDC_EDIT5:			
			//g_CsComm.Lock();
			//g_Serial.WriteCommDword(PLCHD(1252), _ttoi(m_plcHD1252));
			//g_CsComm.Unlock();
			break;
		case IDC_EDIT11:
			//g_CsComm.Lock();
			//g_Serial.WriteCommDword(PLCHD(1262), _ttoi(m_plcHD1262));
			//g_CsComm.Unlock();
			break;
		case IDC_EDIT10:
			//g_CsComm.Lock();
			//g_Serial.WriteCommDword(PLCHD(1272), _ttoi(m_plcHD1272));
			//g_CsComm.Unlock();
			break;
		default:
			break;
		}
		return true;
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void PLCSetting::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}

//С��ת����ʹ��
void PLCSetting::OnBnClickedButton20()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	bool b = false;
	//g_CsComm.Lock();
	//g_Serial.WriteCommRelay(PLCM(99), 1);
	//g_Serial.ReadCommRelay(PLCEY(5), 1, &b);
	//g_CsComm.Unlock();
	if (b)
	{
		GetDlgItem(IDC_BUTTON20)->SetWindowTextW(L"С��ת��բ");
	}
	else
	{
		GetDlgItem(IDC_BUTTON20)->SetWindowTextW(L"С��ת�ͷ�");
	}
}

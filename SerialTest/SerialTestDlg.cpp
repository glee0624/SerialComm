
// SerialTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>	
#include "SerialPort.h"
#include "SerialTest.h"
#include "SerialTestDlg.h"
#include "afxdialogex.h"

#include "ChildDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSerialTestDlg ��ȭ ����




CSerialTestDlg::CSerialTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerialTestDlg::IDD, pParent)
	, m_read(_T(""))
	, m_write(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSerialTestDlg::~CSerialTestDlg()
{	
	m_serial->Close();		
}
void CSerialTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WRITE, m_write);
	DDX_Text(pDX, IDC_EDIT_READ, m_read);
	DDX_Control(pDX, IDC_EDIT_CNTSTATE, m_cnt_state);
}

BEGIN_MESSAGE_MAP(CSerialTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DCB, &CSerialTestDlg::OnBnClickedButtonDcb)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSerialTestDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSerialTestDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSerialTestDlg::OnBnClickedButtonSend)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSerialTestDlg �޽��� ó����

BOOL CSerialTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ���� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	pCursel = new int[5];
	pOpenValue = new int[5];
	m_serial = NULL;	
	m_serial = new CSerialPort();
	ConnectState(false);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);

	pCursel[0] = 2;
	pCursel[1] = 2; 
	pCursel[2] = 0;
	pCursel[3] = 0;
	pCursel[4] = 2;
	SetCombotoDCB();

	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CSerialTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSerialTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSerialTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//void CSerialTestDlg::SetThread()
//{
//	if(pThread == NULL)
//	{
//		pThread = AfxBeginThread(ThreadFunction, m_serial);	
//		if (pThread == NULL)
//		{
//			AfxMessageBox(_T("Error during Set Thread Func"));
//			return;
//		}		
//	}
//}
void CSerialTestDlg::OnBnClickedButtonDcb()
{
	CChildDlg childDlg;
	childDlg.DoModal();
	childDlg.DestroyWindow();
}


void CSerialTestDlg::OnBnClickedButtonOpen()
{
	UpdateData(TRUE);
	
	if(m_serial->IsOpen())
	{
		AfxMessageBox(_T("��Ʈ�� �̹� �����ֽ��ϴ�."));
		return;
	}
	else
	{		
		ConnectPort();			
	}
	
}

void CSerialTestDlg::ConnectPort()
{
	int strPortNum = GetDlgItemInt(IDC_EDIT_COMPORT);
	bOpenFlag = true;
	

	if(strPortNum == NULL)
	{
		AfxMessageBox(_T("��Ʈ�� �Է��ϼ���"));
		return;
	}
	else if(strPortNum != NULL)
	{	
		SetCombotoDCB();
		m_serial->Open(strPortNum, pOpenValue[0], (CSerialPort::Parity)pOpenValue[1], pOpenValue[2],
			(CSerialPort::StopBits)pOpenValue[3], (CSerialPort::FlowControl)pOpenValue[4]);	
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
		ConnectState(true);
	}
}

void CSerialTestDlg::SetCombotoDCB()
{	
	switch(pCursel[0])
	{
	case 0:
		pOpenValue[0] = 19200;
		break;
	case 1:
		pOpenValue[0] = 57600;
		break;
	case 2:
		pOpenValue[0] = 115200;
		break;
	default:
		AfxMessageBox(_T("Baudrate Error"));
	}

	switch(pCursel[1])
	{
	case 0:
		pOpenValue[1] = m_serial->EvenParity;
		break;
	case 1:
		pOpenValue[1] = m_serial->OddParity;
		break;
	case 2:
		pOpenValue[1] = m_serial->NoParity;
		break;
	default:
		AfxMessageBox(_T("Parity Error"));
	}

	switch(pCursel[2])
	{
	case 0:
		pOpenValue[2] = 7;
		break;
	case 1:
		pOpenValue[2] = 8;
		break;
	default:
		AfxMessageBox(_T("DataBit Error"));
	}
	
	switch(pCursel[3])
	{
	case 0:
		pOpenValue[3] = m_serial->OneStopBit;
		break;
	case 1:
		pOpenValue[3] = m_serial->TwoStopBits;
		break;
	default:
		AfxMessageBox(_T("StopBit Error"));
	}
	switch(pCursel[4])
	{
	case 0:
		pOpenValue[4] = m_serial->XonXoffFlowControl;
		break;
	case 1:
		pOpenValue[4] = m_serial->CtsRtsFlowControl;
		break;
	case 2:
		pOpenValue[4] = m_serial->NoFlowControl;
		break;
	default:
		AfxMessageBox(_T("FlowControl Error"));
	}

}

void CSerialTestDlg::OnBnClickedButtonClose()
{
	m_serial->Close();
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	ConnectState(false);
}


void CSerialTestDlg::OnBnClickedButtonSend()
{
	if(m_serial != NULL)
	{
		UpdateData(TRUE);
		char czPacket[512];
		memset(czPacket, NULL, sizeof(czPacket));
		sprintf_s(czPacket, "%s\r", m_write);
		m_serial->Purge(PURGE_TXABORT| PURGE_RXABORT |PURGE_TXCLEAR |PURGE_RXCLEAR );
		m_serial->Write(czPacket, (DWORD)strlen(czPacket));
		Delay(500);
		ReadMessage();
	}
	else
	{
		AfxMessageBox(_T("Please Open the Port"));
	}
}

void CSerialTestDlg::ReadMessage()
{	
	char rPacket[1024];
	memset(rPacket, NULL, sizeof(rPacket));
	m_serial->Read(rPacket, static_cast<DWORD>(strlen(rPacket))); 
	// C++ ���� ĳ���� static_cast�� �����Ϸ����� ���� üũ�� ���� ĳ���� ���׸� ������ ���� �� �ִ�.
	// sizeof() ��ȯ���� size_t�� unsigned int���� �ش��Ѵ�. (DWORD�� unsigned long)
	if(rPacket != NULL)
	{
		GetDlgItem(IDC_EDIT_READ)->SetWindowTextA(static_cast<LPCTSTR>(rPacket));
		//UpdateData(FALSE); ������ ����� updatedata�� ������ �߻���Ų��.�� ��Ʈ�Ѹ��� setwindowtest�� ���� ó���ϰų�, 
		//PostMessage�� ���� �����޼����� ����� �������Ѵ�.
	}
	else if(rPacket == NULL)
	{
		return;
	}
}

//UINT CSerialTestDlg::ThreadFunction(LPVOID _mothod)
//{	
//	CSerialTestDlg* pDlg = (CSerialTestDlg*)AfxGetApp()->GetMainWnd();
//
//	while(pDlg->bOpenFlag == true)
//	{
//		if(pDlg == NULL)
//		{
//			AfxMessageBox(_T("NULL POINTER ERROR"));
//			break;
//		}
//		else
//		{
//			pDlg->ReadMessage();
//		}		
//	}
//	return 0;
//}
	/*
	
	CServerManagerDlg ���� ���̾�α� 

	������� ���þ��� �Լ����� Ÿ Ŭ��������
	CServerManagerDlg* lpDlg = (CServerManagerDlg*)AfxGetMainWnd();
	�̷��� �����ؼ� ������ ���.
	������, ������ȿ����� ���δ��̾�α� �����͸� ������ ����.
	������ ��, AfxGetMainWnd() ������ ������ 

	_AFXWIN_INLINE CWnd* AFXAPI AfxGetMainWnd()
	{
		CWinThread* pThread = AfxGetThread();

		return pTrhead != NULL ? pThread->GetMainWnd() : NULL;
	}
	���ο��� AfxGetThread() �Լ��� ����ϴ� ���� ������ �ִ�.
	�̰��� ���� �������� GetMainWnd() �Լ��� ȣ���Ѵ�.
	�̰��� �ٸ� �����忡�� ���� �������� �ڵ��� �ʿ��Ҷ��� ������ �ȴ�.
	�� �ٸ� �����忡�� ������ ���� AfxGetMainWnd() �Լ��� ȣ���ϸ� ���� �������� �ڵ��� �������� ���� �ȴ�.
	�ؼ�, �����忡 ����� ��� �ɹ� �Լ���������

	CServerManagerDlg* lpDlg = (CServerManagerDlg*)AfxGetApp()->GetMainWnd();

	�̷��� ���ָ� �ȴ�.
	[��ó] [MFC] �ٸ� �����忡�� ���δ��̾�α� ������ �޾ƿ��� AfxGetMainWnd()|�ۼ��� BaseJoe
	*/



HBRUSH CSerialTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CNTSTATE);
	if((nCtlColor == CTLCOLOR_EDIT)&&(pEdit->GetSafeHwnd() == pWnd->GetSafeHwnd()))
	{
		pDC->SetBkMode(TRANSPARENT); // ��׶��� ��� ����
		pDC->SetTextColor(RGB(255,255,255));
		return m_brush;
	}

	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

void CSerialTestDlg::ConnectState(bool isOpen)
{		
	if(isOpen == true)
	{
		GetDlgItem(IDC_EDIT_CNTSTATE)->SetWindowTextA(TEXT("���� ��"));
	}
	else if(isOpen == false)
	{
		GetDlgItem(IDC_EDIT_CNTSTATE)->SetWindowTextA(TEXT("���� ��� ��"));
	}
}

void CSerialTestDlg::Delay(int ms)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();	

	while(GetTickCount() - dwStart < ms)  
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
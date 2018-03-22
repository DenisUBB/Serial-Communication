
// MFCApplication3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "MFCApplication3Dlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_COM_PORTS_TO_ITERATE 255

// CAboutDlg dialog used for App About

HANDLE serialPort = INVALID_HANDLE_VALUE;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication3Dlg dialog



CMFCApplication3Dlg::CMFCApplication3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo_1);
	DDX_Control(pDX, IDC_COMBO2, m_combo_2);
	DDX_Control(pDX, IDC_EDIT1, m_text_1);
	DDX_Control(pDX, IDC_EDIT2, m_text_2);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress_bar);
	DDX_Control(pDX, IDC_EDIT3, m_text_3);
}

BEGIN_MESSAGE_MAP(CMFCApplication3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication3Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication3Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication3Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication3Dlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CMFCApplication3Dlg message handlers

BOOL CMFCApplication3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	findPorts();
	m_text_1.SetWindowTextW(CString("Received here"));
	m_text_2.SetWindowTextW(CString("Enter here"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCApplication3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplication3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCApplication3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication3Dlg::findPorts()
{
	TCHAR lpTargetPath[5000];
	DWORD searchCom;
	bool gotPort = false;

	for (int i = 0; i < MAX_COM_PORTS_TO_ITERATE; i++) // Check all the ports from COM0 to COM255
	{
		CString str;
		str.Format(_T("%d"), i);
		CString ComName = CString("COM") + CString(str); // COM0, COM1, ... , COM255
		searchCom = QueryDosDevice(ComName, LPWSTR(lpTargetPath), 5000);

		if (searchCom != 0) // QueryDosDevice succeeded
		{
			m_combo_1.AddString(ComName);
			gotPort = 1;
		}

		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			lpTargetPath[10000];
			continue;
		}
	}

	if (!gotPort) // not even 1 port found
		m_combo_1.AddString((CString)"No active ports found!");
}



void CMFCApplication3Dlg::OnBnClickedButton2()
{
	// Init button
	m_text_3.SetWindowTextW(CString(""));
	m_progress_bar.SetPos(0);
	int idx_combo_port = m_combo_1.GetCurSel();
	int idx_combo_baud_rate = m_combo_2.GetCurSel();

	if (idx_combo_port == -1 || idx_combo_baud_rate == -1)
		m_text_1.SetWindowTextW(CString("Please Select Port Settings"));
	else
	{
		BOOL fSuccess;
		int n_combo_port = m_combo_1.GetLBTextLen(idx_combo_port);
		int n_combo_baud_rate = m_combo_2.GetLBTextLen(idx_combo_baud_rate);

		CString portName, baudRate;
		m_combo_1.GetLBText(idx_combo_port, portName.GetBuffer(n_combo_port));
		m_combo_2.GetLBText(idx_combo_baud_rate, baudRate.GetBuffer(n_combo_baud_rate));

		//
		// Open selected port
		//

		int baudRateInt = _wtoi(baudRate);

		serialPort = CreateFile(portName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			0,
			OPEN_EXISTING,
			0,
			0
		);

		if (serialPort == INVALID_HANDLE_VALUE)
		{
			m_text_3.SetWindowTextW(CString("HANDLE ERROR ( COM port might be already opened ! )"));
			return;
		}

		
		DCB properties;
		fSuccess = GetCommState(serialPort, &properties);

		if (!fSuccess)
		{
			// Handle the error
			//
			//
			m_text_3.SetWindowTextW(CString("GetCommState Error!"));
			return;
		}
		switch (baudRateInt)
		{
		case 9600:
			properties.BaudRate = CBR_9600;
			break;
		case 115200:
			properties.BaudRate = CBR_115200;
			break;
		}

		properties.Parity = NOPARITY;
		properties.ByteSize = 8;
		properties.StopBits = ONESTOPBIT;

		fSuccess = SetCommState(serialPort, &properties);

		if (!fSuccess)
		{
			// Handle the error
			//
			//
			m_text_3.SetWindowTextW(CString("SetCommState Error!"));
			return;
		}
		m_progress_bar.SetPos(100);
	}
}


void CMFCApplication3Dlg::OnBnClickedButton3()
{
	// Send button
	if (serialPort == INVALID_HANDLE_VALUE)
		m_text_3.SetWindowTextW(CString("INVALID_FILE_HANDLE!"));
	else
	{
		CString enteredText;
		BOOL fSuccess;
		m_text_2.GetWindowTextW(enteredText);
		CT2A inputText(enteredText);
		DWORD inputTextSize = strlen(inputText);
		DWORD bytesWritten;

		// send the length of the string first

		fSuccess = WriteFile(serialPort,
			(void *)&inputTextSize,
			sizeof(DWORD),
			&bytesWritten,
			NULL); 

		if (!fSuccess)
		{
			DWORD x = GetLastError();
			m_text_3.SetWindowTextW((CString)std::to_string(x).c_str());
			return;
		}

		fSuccess = WriteFile(serialPort,
			(void *)inputText,
			inputTextSize,
			&bytesWritten,
			NULL
			);
	}
}


void CMFCApplication3Dlg::OnBnClickedButton1()
{
	m_text_3.SetWindowTextW(CString(""));
	if (serialPort != INVALID_HANDLE_VALUE)
		if (CloseHandle(serialPort) == 0)
			m_text_3.SetWindowTextW((CString)"CloseHandle Error!");
}


void CMFCApplication3Dlg::OnBnClickedButton4()
{
	if (serialPort == INVALID_HANDLE_VALUE)
		m_text_3.SetWindowTextW(CString("INVALID_FILE_HANDLE!"));
	else
	{
		PCHAR textSent;
		DWORD inputTextSize;
		DWORD bytesReceived;
		DWORD fSuccess;
		fSuccess = ReadFile(serialPort,
			(void *)&inputTextSize,
			sizeof(DWORD),
			&bytesReceived,
			NULL
		);

		if (!fSuccess)
		{
			m_text_3.SetWindowTextW(CString("ReadFile Error!"));
			return;
		}
		textSent = (char *)malloc(inputTextSize * sizeof(char) + 1);
		ReadFile(serialPort,
			(void *)textSent,
			inputTextSize,
			&bytesReceived,
			NULL
			);


		textSent[inputTextSize] = '\0';
		CString str(textSent);
		m_text_1.SetWindowTextW(str);

		free(textSent);


	}
}

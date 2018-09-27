
// zeromq_testDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "zeromq_test.h"
#include "zeromq_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::string ConverCStringToStdString(CString cstrValue)
{
	std::string strRet;
	strRet.clear();
	int nSize = ::WideCharToMultiByte(CP_UTF8, 0, cstrValue.GetBuffer(), -1, NULL, 0, NULL, NULL);
	char * szcSrc = new char[nSize];
	memset(szcSrc, 0, sizeof(char) * nSize);
	::WideCharToMultiByte(CP_UTF8, 0, cstrValue.GetBuffer(), -1, szcSrc, nSize, NULL, NULL);
	strRet = szcSrc;
	delete[] szcSrc;
	szcSrc = NULL;
	return strRet;

}

std::string ConverCStringToStdString(CString cstrValue, DWORD dwFormatW)
{
	std::string strRet;
	strRet.clear();
	int nSize = ::WideCharToMultiByte(dwFormatW, 0, cstrValue.GetBuffer(), -1, NULL, 0, NULL, NULL);
	char * szcSrc = new char[nSize];
	memset(szcSrc, 0, sizeof(char) * nSize);
	::WideCharToMultiByte(dwFormatW, 0, cstrValue.GetBuffer(), -1, szcSrc, nSize, NULL, NULL);
	strRet = szcSrc;
	delete[] szcSrc;
	szcSrc = NULL;
	return strRet;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// Czeromq_testDlg 对话框



Czeromq_testDlg::Czeromq_testDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ZEROMQ_TEST_DIALOG, pParent)
	, m_pNetContex(nullptr)
	, m_pReplyServer(nullptr)
	, m_pPubServer(nullptr)
	, m_pSubClient(nullptr)
	, m_pReplyClient(nullptr)
	, m_pServerRecvThread(nullptr)
	, m_pClientRecvThread(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Czeromq_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Czeromq_testDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_STARTSERVER, &Czeromq_testDlg::OnBnClickedButtonStartserver)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &Czeromq_testDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SUB, &Czeromq_testDlg::OnBnClickedButtonSub)
	ON_BN_CLICKED(IDC_BUTTON_UNSUB, &Czeromq_testDlg::OnBnClickedButtonUnsub)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &Czeromq_testDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &Czeromq_testDlg::OnBnClickedButtonSend)
	ON_MESSAGE(WM_USER_MSG_ARRIVE, &Czeromq_testDlg::OnMsgArrived)
END_MESSAGE_MAP()


// Czeromq_testDlg 消息处理程序

BOOL Czeromq_testDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_EDIT_PUB_PORT)->SetWindowText(_T("5555"));
	GetDlgItem(IDC_EDIT_SERVER_PORT)->SetWindowText(_T("5556"));
	GetDlgItem(IDC_EDIT_SERVER_IP)->SetWindowText(_T("192.168.100.69"));
	GetDlgItem(IDC_EDIT_CLIENT_IP)->SetWindowText(_T("192.168.100.69"));
	GetDlgItem(IDC_EDIT_THEME)->SetWindowText(_T("main_theme"));

	m_pNetContex = zmq_ctx_new();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Czeromq_testDlg::OnDestroy()
{
	CloseReplyClient();
	CloseReplyServer();
	CloseSubClient();
	ClosePubServer();
	if (m_pNetContex)
	{
		zmq_ctx_destroy(m_pNetContex);
		m_pNetContex = nullptr;
	}
}

void Czeromq_testDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Czeromq_testDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Czeromq_testDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Czeromq_testDlg::ThreadServerRecv(Czeromq_testDlg * pThis)
{
	if (pThis)
	{
		pThis->DoServerRecv();
	}
}

bool Czeromq_testDlg::DoServerRecv()
{
	while (m_pReplyServer)
	{
		if (m_evtQuitServerRecv.Wait(5))
		{
			break;
		}
		char buffer[MAX_PATH] = {0};
		int nRecvLen = zmq_recv(m_pReplyServer, buffer, MAX_PATH, ZMQ_DONTWAIT);
		if (nRecvLen > 0)
		{
			//将消息通过pubserver扔出去
			if (m_pPubServer)
			{
				if (zmq_send(m_pPubServer, buffer, nRecvLen, 0) == nRecvLen)
				{
					TCHAR * szMsg = new TCHAR[nRecvLen / sizeof(TCHAR) + 1];
					memcpy(szMsg, buffer, nRecvLen);
					szMsg[nRecvLen / sizeof(TCHAR)] = _T('\0');
					OutputDebugString(_T("recieved and broadcast:"));
					OutputDebugString(szMsg);
					OutputDebugString(_T("\r\n"));
				}
			}
			if (zmq_send(m_pReplyServer, "recved", 6, 0) == 6)//收到所有消息都返回一个recved
			{
				OutputDebugStringA("replyed\r\n");
			}
		}
	}
	return true;
}

void Czeromq_testDlg::ThreadClientRecv(Czeromq_testDlg * pThis)
{
	if (pThis)
	{
		pThis->DoClientRecv();
	}
}

bool Czeromq_testDlg::DoClientRecv()
{
	while (m_pSubClient)
	{
		if (m_evtQuitClientRecv.Wait(5))
		{
			break;
		}
		char buffer[MAX_PATH] = { 0 };
		int nLen = zmq_recv(m_pSubClient, buffer, MAX_PATH, ZMQ_DONTWAIT);
		if (nLen > 0 && (nLen % sizeof(TCHAR) == 0))
		{
			//将消息显示在edit里
			TCHAR * szMsg = new TCHAR[nLen / sizeof(TCHAR) + 1];
			memcpy(szMsg, buffer, nLen);
			szMsg[nLen / sizeof(TCHAR)] = _T('\0');
			m_lockMsgQueue.lock();
			m_queRecvMsg.push(szMsg);
			m_lockMsgQueue.unlock();
			delete szMsg;
			szMsg = nullptr;
			PostMessage(WM_USER_MSG_ARRIVE);
		}
	}
	return true;
}

bool Czeromq_testDlg::StartPubServer()
{
	if (m_pNetContex)
	{
		ClosePubServer();
		CString strIP;
		GetDlgItem(IDC_EDIT_SERVER_IP)->GetWindowText(strIP);
		CString strPort;
		GetDlgItem(IDC_EDIT_PUB_PORT)->GetWindowText(strPort);
		if (!strIP.IsEmpty() && !strPort.IsEmpty())
		{
			CString strConn;
			strConn.Format(_T("tcp://%s:%s"), strIP, strPort);
			if (!m_pPubServer)
			{
				m_pPubServer = zmq_socket(m_pNetContex, ZMQ_PUB);
				if (m_pPubServer)
				{
					if (zmq_bind(m_pPubServer, ConverCStringToStdString(strConn).c_str()) == 0)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Czeromq_testDlg::ClosePubServer()
{
	if (m_pNetContex)
	{
		if (m_pPubServer)
		{
			zmq_close(m_pPubServer);
			m_pPubServer = nullptr;
			return true;
		}
	}
	return false;
}

bool Czeromq_testDlg::StartReplyServer()
{
	if (m_pNetContex)
	{
		CloseReplyServer();
		CString strIP;
		GetDlgItem(IDC_EDIT_SERVER_IP)->GetWindowText(strIP);
		CString strPort;
		GetDlgItem(IDC_EDIT_SERVER_PORT)->GetWindowText(strPort);
		if (!strIP.IsEmpty() && !strPort.IsEmpty())
		{
			CString strConn;
			strConn.Format(_T("tcp://%s:%s"), strIP, strPort);
			m_strConnReplyServer = strConn;
			if (!m_pReplyServer)
			{
				m_pReplyServer = zmq_socket(m_pNetContex, ZMQ_REP);
				if (m_pReplyServer)
				{
					if (zmq_bind(m_pReplyServer, ConverCStringToStdString(strConn).c_str()) == 0)
					{
						m_evtQuitServerRecv.ResetEvent();
						m_pServerRecvThread = new std::thread(&ThreadServerRecv, this);
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Czeromq_testDlg::CloseReplyServer()
{
	if (m_pNetContex)
	{
		m_evtQuitServerRecv.SetEvent();
		if (m_pServerRecvThread)
		{
			m_pServerRecvThread->join();
			delete m_pServerRecvThread;
			m_pServerRecvThread = nullptr;
		}
		if (m_pReplyServer)
		{
			zmq_close(m_pReplyServer);
			m_pReplyServer = nullptr;
		}
		return true;
	}
	return false;
}

bool Czeromq_testDlg::StartSubClient()
{
	if (m_pNetContex)
	{
		CloseSubClient();
		CString strIP;
		GetDlgItem(IDC_EDIT_CLIENT_IP)->GetWindowText(strIP);
		CString strPort;
		GetDlgItem(IDC_EDIT_PUB_PORT)->GetWindowText(strPort);
		if (!strIP.IsEmpty() && !strPort.IsEmpty())
		{
			CString strConn;
			strConn.Format(_T("tcp://%s:%s"), strIP, strPort);
			if (!m_pSubClient)
			{
				m_pSubClient = zmq_socket(m_pNetContex, ZMQ_SUB);
				if (m_pSubClient)
				{
					if (zmq_connect(m_pSubClient, ConverCStringToStdString(strConn).c_str()) == 0)
					{
						if (zmq_setsockopt(m_pSubClient, ZMQ_SUBSCRIBE, nullptr, 0) == 0)//订阅所有
						{
							m_evtQuitClientRecv.ResetEvent();
							m_pClientRecvThread = new std::thread(&ThreadClientRecv, this);
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool Czeromq_testDlg::CloseSubClient()
{
	if (m_pNetContex)
	{
		m_evtQuitClientRecv.SetEvent();
		if (m_pClientRecvThread)
		{
			m_pClientRecvThread->join();
			delete m_pClientRecvThread;
			m_pClientRecvThread = nullptr;
		}
		if (m_pSubClient)
		{
			zmq_close(m_pSubClient);
			m_pSubClient = nullptr;
		}
		return true;
	}
	return false;
}

bool Czeromq_testDlg::StartReplyClient()
{
	if (m_pNetContex)
	{
		CloseReplyClient();
		CString strIP;
		GetDlgItem(IDC_EDIT_CLIENT_IP)->GetWindowText(strIP);
		CString strPort;
		GetDlgItem(IDC_EDIT_SERVER_PORT)->GetWindowText(strPort);
		if (!strIP.IsEmpty() && !strPort.IsEmpty())
		{
			CString strConn;
			strConn.Format(_T("tcp://%s:%s"), strIP, strPort);
			if (!m_pReplyClient)
			{
				m_pReplyClient = zmq_socket(m_pNetContex, ZMQ_REQ);
				if (m_pReplyClient)
				{
					if (zmq_connect(m_pReplyClient, ConverCStringToStdString(strConn).c_str()) == 0)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Czeromq_testDlg::CloseReplyClient()
{
	if (m_pNetContex)
	{
		if (m_pReplyClient)
		{
			zmq_close(m_pReplyClient);
			m_pReplyClient = nullptr;
			return true;
		}
	}
	return false;
}

void Czeromq_testDlg::RefreshThemeText()
{
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_CUR_THEME);
	pEdit->SetWindowText(_T(""));
	for (auto it : m_setCurTheme)
	{
		CString strContent = it;
		pEdit->SetSel(-1, 0);
		pEdit->ReplaceSel(strContent + _T("\r\n"));
	}
}

void Czeromq_testDlg::OnBnClickedButtonStartserver()
{
	//启动服务其实是启动两个服务，reply和pub
	if (StartReplyServer())
	{
		if (StartPubServer())
		{
			AfxMessageBox(_T("server started"));
			return;
		}
	}
	AfxMessageBox(_T("server start failed"));
}


void Czeromq_testDlg::OnBnClickedButtonConnect()
{
	if (StartReplyClient())
	{
		if (StartSubClient())
		{
			AfxMessageBox(_T("client connect"));
			return;
		}
	}
	AfxMessageBox(_T("connect failed"));
}


void Czeromq_testDlg::OnBnClickedButtonSub()
{
	CString strTheme;
	GetDlgItem(IDC_EDIT_THEME)->GetWindowText(strTheme);
	m_setCurTheme.insert(strTheme);
	RefreshThemeText();
}


void Czeromq_testDlg::OnBnClickedButtonUnsub()
{
	CString strTheme;
	GetDlgItem(IDC_EDIT_THEME)->GetWindowText(strTheme);
	m_setCurTheme.erase(strTheme);
	RefreshThemeText();
}


void Czeromq_testDlg::OnBnClickedButtonClear()
{
	GetDlgItem(IDC_EDIT_RECV)->SetWindowText(_T(""));
}

LRESULT Czeromq_testDlg::OnMsgArrived(WPARAM wParam, LPARAM lParam)
{
	std::queue<CString> queMsg;
	m_lockMsgQueue.lock();
	while (!m_queRecvMsg.empty())
	{
		queMsg.push(m_queRecvMsg.front());
		m_queRecvMsg.pop();
	}
	m_lockMsgQueue.unlock();

	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_RECV);
	while (!queMsg.empty())
	{
		CString strContent = queMsg.front();
		queMsg.pop();
		pEdit->SetSel(-1, 0);
		pEdit->ReplaceSel(strContent + _T("\r\n"));
	}
	return LRESULT();
}


void Czeromq_testDlg::OnBnClickedButtonSend()
{
	if (m_pReplyClient)
	{
		CString strSend;
		GetDlgItem(IDC_EDIT_SEND)->GetWindowText(strSend);
		if (!strSend.IsEmpty())
		{
			zmq_send(m_pReplyClient, strSend.GetBuffer(), strSend.GetLength() * sizeof(TCHAR), 0);
			char buffer[10] = { 0 };
			zmq_recv(m_pReplyClient, buffer, 10, 0);
			OutputDebugStringA(buffer);
			OutputDebugStringA("\r\n");
		}
	}
}

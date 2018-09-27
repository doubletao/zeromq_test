
// zeromq_testDlg.h : ͷ�ļ�
//

#pragma once
#include "include\zmq.h"
#ifdef _DEBUG
#pragma comment(lib, "lib\\libzmqD.lib")
#else
#pragma comment(lib, "lib\\libzmq.lib")
#endif

#include <set>
#include <queue>
#include "StdEvent.h"

const int WM_USER_MSG_ARRIVE = WM_USER + 0x1001;

// Czeromq_testDlg �Ի���
class Czeromq_testDlg : public CDialogEx
{
// ����
public:
	Czeromq_testDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZEROMQ_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	std::set<CString> m_setCurTheme;
	std::queue<CString> m_queRecvMsg;
	std::mutex m_lockMsgQueue;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	//��Ϣ����:�ͻ���ͨ��replyclient������Ϣ��replyserver��replyserver�յ���Ϣ��ͨ��pubserver�㲥��ȥ���ͻ���ͨ��subclient���գ������Ϣ��ת������
	void * m_pNetContex;
	void * m_pReplyServer;
	void * m_pPubServer;
	void * m_pSubClient;
	void * m_pReplyClient;

	CStdEvent m_evtQuitServerRecv;
	std::thread * m_pServerRecvThread;//����˽����̣߳�reply���գ�
	static void ThreadServerRecv(Czeromq_testDlg * pThis);
	bool DoServerRecv();
	CStdEvent m_evtQuitClientRecv;
	std::thread * m_pClientRecvThread;//�ͻ��˽����߳�(���Ľ���)
	static void ThreadClientRecv(Czeromq_testDlg * pThis);
	bool DoClientRecv();

	CString m_strConnPubServer;
	CString m_strConnReplyServer;
	CString m_strConnSubClient;
	CString m_strConnReplyClient;
	bool StartPubServer();
	bool ClosePubServer();
	bool StartReplyServer();
	bool CloseReplyServer();
	bool StartSubClient();
	bool CloseSubClient();
	bool StartReplyClient();
	bool CloseReplyClient();
protected:
	void RefreshThemeText();
public:
	afx_msg void OnBnClickedButtonStartserver();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonSub();
	afx_msg void OnBnClickedButtonUnsub();
	afx_msg void OnBnClickedButtonClear();
	afx_msg LRESULT OnMsgArrived(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonSend();
};

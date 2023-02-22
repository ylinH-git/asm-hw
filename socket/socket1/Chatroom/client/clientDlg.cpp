
// clientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "client.h"
#include "clientDlg.h"
#include "afxdialogex.h"
#include <Windows.h>
#include <WinSock2.h>
#define MAX_MSG_LENGTH 0x1000
#define CUSTOM_UPDATE (WM_USER+1)
void InitWs2_32()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);
	// 微软用来初始化"Ws2_32.lib"的api，这个是微软特有的
	// linux不需要，虽然上面的创建socket的代码其实是一样的
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup();
		return;
	}

	/* The WinSock DLL is acceptable. Proceed. */
}



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CclientDlg 对话框



CclientDlg::CclientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, m_chatInfo(_T(""))
	, m_loginName(_T(""))
	, m_speakInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CclientDlg::~CclientDlg()
{
	OnBnClickedLogout();
}

void CclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDT_CHATBOX, m_chatInfo);
	DDX_Text(pDX, EDT_LOGINNAME, m_loginName);
	DDX_Control(pDX, LIST_CONTACT, m_contactList);
	DDX_Text(pDX, EDT_SPEAK, m_speakInfo);
}

BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_LOGIN, &CclientDlg::OnBnClickedLogin)
	ON_BN_CLICKED(BTN_LOGOUT, &CclientDlg::OnBnClickedLogout)
	ON_BN_CLICKED(BTN_SEND, &CclientDlg::OnBnClickedSend)
	ON_BN_CLICKED(BTN_SEND_PRIVATE, &CclientDlg::OnBnClickedSendPrivate)
	ON_MESSAGE(CUSTOM_UPDATE, &CclientDlg::OnCustomUpdate)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CclientDlg 消息处理程序

BOOL CclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	InitWs2_32();
	m_siServer.sin_family = AF_INET;
	m_siServer.sin_port = htons(8888);
	m_siServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CclientDlg::OnPaint()
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
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void DeleteListAll(CclientDlg* dlg)
{
	int listNum = dlg->m_contactList.GetCount();
	for (int i = 0; i < listNum; i++)
	{
		dlg->m_contactList.DeleteString(0);
	}
}

DWORD WINAPI WatchNetThreadProc(
	_In_ LPVOID lpParameter
)
{
	CclientDlg* dlg = (CclientDlg*)lpParameter;
	sockaddr_in siRecv = {};
	int nNameLen = sizeof(siRecv);
	char szBuf[MAX_MSG_LENGTH] = {};
	while (true)
	{
		if (!dlg->m_isLogin)
		{
			Sleep(1);
		}
		else
		{
			int nRet = recvfrom(
				dlg->m_socket,
				szBuf,
				sizeof(szBuf),
				0,
				(sockaddr*)&siRecv,
				&nNameLen
			);
			if (nRet != 0 && nRet != SOCKET_ERROR)
			{
				CTime time;
				time = CTime::GetCurrentTime();
				CString curDate = time.Format("%Y-%m-%d %H:%M:%S  \r\n");
				(dlg->m_chatInfo += curDate.GetString());
				int msgErrorType = *(int*)szBuf;
				int msgType = *(int*)&szBuf[4];
				if (msgErrorType != 0)
				{
					switch (msgErrorType)
					{
					case 1:
					{
						(dlg->m_chatInfo += dlg->m_loginName.GetString()) += "已经登录，请勿重复登录\r\n";
						break;
					}
					case 2:
					{
						break;
					}
					case 3:
					{
						dlg->m_chatInfo += "消息发送失败\r\n";
						break;
					}
					default:
					{
						dlg->m_chatInfo += "信息接收错误\r\n";
						break;
					}
					}
				}
				else
				{
					switch (msgType)
					{
					case LOGIN:
					{
						int usersNum = *(int*)&szBuf[8];
						CList<CString> userList;
						CString loginUser;
						int curPoint = 12;
						for (int i = 0; i < usersNum + 1; i++)
						{
							if (i == usersNum)
							{
								loginUser = &szBuf[curPoint];
								curPoint += loginUser.GetLength() + 1;
							}
							else
							{
								CString userName = &szBuf[curPoint];
								userList.AddTail(userName);
								curPoint += userName.GetLength() + 1;
							}
						}
						(dlg->m_chatInfo += loginUser.GetString()) += "进入聊天室\r\n";
						CWnd* contList = dlg->GetDlgItem(EDT_LOGINNAME);
						contList->EnableWindow(FALSE);

						DeleteListAll(dlg);
						POSITION pos = userList.GetHeadPosition();
						for (int i = 0; i < userList.GetCount(); i++)
						{
							dlg->m_contactList.AddString(userList.GetNext(pos));
						}
						break;
					}
					case LOGOUT:
					{
						CString msgContent = &szBuf[8];
						(dlg->m_chatInfo += msgContent.GetString()) += "退出聊天室\r\n";
						int nIdx = dlg->m_contactList.FindString(-1, msgContent.GetString());
						dlg->m_contactList.DeleteString(nIdx);
						if (msgContent == dlg->m_loginName)
						{
							closesocket(dlg->m_socket);
							dlg->m_socket = NULL;
							dlg->m_loginName = "";
							CWnd* contList = dlg->GetDlgItem(EDT_LOGINNAME);
							contList->EnableWindow(TRUE);
							DeleteListAll(dlg);
							dlg->m_speakInfo = "";
							dlg->m_chatInfo = "";
							dlg->m_isLogin = FALSE;
						}
						break;
					}
					default:
					{
						(dlg->m_chatInfo += &szBuf[8]) += "\r\n";
						break;
					}
					}
				}
			}
			dlg->SendMessage(CUSTOM_UPDATE);
		}
	}
	return 0;
}


void CclientDlg::OnBnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	//创建socket
	m_socket = socket(AF_INET/*ipv4*/, SOCK_DGRAM/*数据报*/, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		AfxMessageBox("socket创建失败");
		return;
	}


	char* msgBuf = nullptr;
	UpdateData(TRUE);
	if (!m_loginName.GetLength())
	{
		return;
	}

	tagLLMsgInfo tLgInfo = {
		m_loginName.GetLength(),
		(char*)m_loginName.GetString(),
	};

	int msgLen = 8 + m_loginName.GetLength() + 1;
	msgBuf = new char[msgLen];
	int msgType = LOGIN;
	memcpy_s(msgBuf, 4, (char*)&msgType, sizeof(int));
	memcpy_s(&msgBuf[4], 4, (char*)&tLgInfo.m_msgLength, sizeof(int));
	memcpy_s(&msgBuf[8], tLgInfo.m_msgLength + 1, (char*)tLgInfo.m_msg, tLgInfo.m_msgLength + 1);

	//收发数据
	int nRet = sendto(
		m_socket, 
		msgBuf,
		msgLen,
		0,
		(sockaddr*)&m_siServer, 
		sizeof(m_siServer)
	);
	if (nRet == SOCKET_ERROR)
	{
		AfxMessageBox("发送失败");
	}

	delete[] msgBuf;

	m_isLogin = TRUE;
	m_hThread = CreateThread(
		NULL,
		0,              //使用默认的栈大小
		WatchNetThreadProc, //线程回调函数地址，新线程从这个函数开始执行代码
		this,           //自定参数，会传递给线程回调函数
		0,              //线程立即运行
		NULL            //线程id，不需要
	);
	CloseHandle(m_hThread);

}


void CclientDlg::OnBnClickedLogout()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_isLogin)
	{
		return;
	}
	tagLLMsgInfo tLoutInfo = {
		m_loginName.GetLength(),
		(char*)m_loginName.GetString(),
	};

	char* msgBuf = nullptr;
	int msgLen = 8 + m_loginName.GetLength() + 1;
	msgBuf = new char[msgLen];
	int msgType = LOGOUT;
	memcpy_s(msgBuf, 4, (char*)&msgType, sizeof(int));
	memcpy_s(&msgBuf[4], 4, (char*)&tLoutInfo.m_msgLength, sizeof(int));
	memcpy_s(&msgBuf[8], tLoutInfo.m_msgLength + 1, (char*)tLoutInfo.m_msg, tLoutInfo.m_msgLength + 1);

	//收发数据
	int nRet = sendto(
		m_socket,
		msgBuf,
		msgLen,
		0,
		(sockaddr*)&m_siServer,
		sizeof(m_siServer)
	);
	if (nRet == SOCKET_ERROR)
	{
		AfxMessageBox("发送失败");
	}

	delete[] msgBuf;
}


void CclientDlg::OnBnClickedSend()
{
	char* msgBuf = nullptr;
	UpdateData(TRUE);
	if (!m_isLogin || !m_speakInfo.GetLength())
	{
		return;
	}

	tagMsgInfo sendMsgInfo = {
		m_loginName.GetLength(),
		m_speakInfo.GetLength(),
		(char*)m_loginName.GetString(),
		(char*)m_speakInfo.GetString(),
	};

	int msgLen = 12 + m_speakInfo.GetLength() + m_loginName.GetLength() + 2;
	msgBuf = new char[msgLen];
	int msgType = MESSAGE;
	memcpy_s(msgBuf, 4, (char*)&msgType, 4);
	memcpy_s(&msgBuf[4], 4, (char*)&sendMsgInfo.m_sourceNameLength, 4);
	memcpy_s(&msgBuf[8], 4, (char*)&sendMsgInfo.m_msgLength, 4);
	memcpy_s(&msgBuf[12], sendMsgInfo.m_sourceNameLength + 1, (char*)sendMsgInfo.m_sourceName, sendMsgInfo.m_sourceNameLength + 1);
	memcpy_s(&msgBuf[12 + sendMsgInfo.m_sourceNameLength + 1], sendMsgInfo.m_msgLength + 1, (char*)sendMsgInfo.m_msg, sendMsgInfo.m_msgLength + 1);

	//收发数据
	int nRet = sendto(
		m_socket,
		msgBuf,
		msgLen,
		0,
		(sockaddr*)&m_siServer,
		sizeof(m_siServer)
	);
	delete[] msgBuf;
	if (nRet == SOCKET_ERROR)
	{
		AfxMessageBox("发送失败");
		return;
	}
	m_speakInfo = "";
	UpdateData(FALSE);
}


void CclientDlg::OnBnClickedSendPrivate()
{
	int nCurIdx = m_contactList.GetCurSel();
	if (nCurIdx == LB_ERR)
	{
		return;
	}
	CString targetName;
	m_contactList.GetText(nCurIdx, targetName);

	char* msgBuf = nullptr;
	UpdateData(TRUE);
	if (!m_isLogin || !m_speakInfo.GetLength())
	{
		return;
	}

	tagPrivateMessageInfo sendMsgInfo = {
		m_loginName.GetLength(),
		targetName.GetLength(),
		m_speakInfo.GetLength(),
		(char*)m_loginName.GetString(),
		(char*)targetName.GetString(),
		(char*)m_speakInfo.GetString(),
	};

	int msgLen = 16 + targetName.GetLength() + m_speakInfo.GetLength() + m_loginName.GetLength() + 3;
	msgBuf = new char[msgLen];
	int msgType = PRIVATEMESSAGE;
	memcpy_s(msgBuf, 4, (char*)&msgType, 4);
	memcpy_s(&msgBuf[4], 4, (char*)&sendMsgInfo.m_sourceNameLength, 4);
	memcpy_s(&msgBuf[8], 4, (char*)&sendMsgInfo.m_targetNameLength, 4);
	memcpy_s(&msgBuf[12], 4, (char*)&sendMsgInfo.m_msgLength, 4);
	memcpy_s(&msgBuf[16], sendMsgInfo.m_sourceNameLength + 1, (char*)sendMsgInfo.m_sourceName, sendMsgInfo.m_sourceNameLength + 1);
	memcpy_s(&msgBuf[16 + sendMsgInfo.m_sourceNameLength + 1], sendMsgInfo.m_targetNameLength + 1, (char*)sendMsgInfo.m_targetName, sendMsgInfo.m_targetNameLength + 1);
	memcpy_s(&msgBuf[16 + sendMsgInfo.m_sourceNameLength + sendMsgInfo.m_targetNameLength + 2], sendMsgInfo.m_msgLength + 1, (char*)sendMsgInfo.m_msg, sendMsgInfo.m_msgLength + 1);

	//收发数据
	int nRet = sendto(
		m_socket,
		msgBuf,
		msgLen,
		0,
		(sockaddr*)&m_siServer,
		sizeof(m_siServer)
	);
	delete[] msgBuf;
	if (nRet == SOCKET_ERROR)
	{
		AfxMessageBox("发送失败");
		return;
	}
	m_speakInfo = "";
	UpdateData(FALSE);
}

afx_msg LRESULT CclientDlg::OnCustomUpdate(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);
	return 0;
}


void CclientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnBnClickedLogout();
	CDialogEx::OnClose();
}


// petool.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "petool.h"
#include "petoolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CpetoolApp

BEGIN_MESSAGE_MAP(CpetoolApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CpetoolApp 构造

CpetoolApp::CpetoolApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CpetoolApp 对象

CpetoolApp theApp;


// CpetoolApp 初始化

BOOL CpetoolApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CpetoolDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

ULONG CpetoolApp::GetRVAtoFA(ULONG rva)
{
	ULONG offset = 0;
	for (int i = 0; i < m_sectionHeaderLen - 1; i++)
	{
		if (rva >= m_sectionHeaders[i].VirtualAddress
			&& rva < m_sectionHeaders[i + 1].VirtualAddress)
		{
			offset = rva - m_sectionHeaders[i].VirtualAddress + m_sectionHeaders[i].PointerToRawData;
			break;
		}

		if (rva >= m_sectionHeaders[m_sectionHeaderLen - 1].VirtualAddress)
		{
			offset = rva - m_sectionHeaders[m_sectionHeaderLen - 1].VirtualAddress + m_sectionHeaders[m_sectionHeaderLen - 1].PointerToRawData;
			break;
		}
	}
	return offset;
}

ULONG CpetoolApp::GetRVAtoVA(ULONG rva)
{
	if (theApp.isx86) {
		return rva + theApp.m_optional32Header.ImageBase;
	}
	return rva + theApp.m_optional64Header.ImageBase;
}

ULONG CpetoolApp::GetFAtoRVA(ULONG fa)
{

	ULONG offset = 0;
	for (int i = 0; i < m_sectionHeaderLen - 1; i++)
	{
		if (fa >= m_sectionHeaders[i].PointerToRawData
			&& fa < m_sectionHeaders[i + 1].PointerToRawData)
		{
			offset = fa - m_sectionHeaders[i].PointerToRawData + m_sectionHeaders[i].VirtualAddress;
			break;
		}

		if (fa >= m_sectionHeaders[m_sectionHeaderLen - 1].PointerToRawData)
		{
			offset = fa - m_sectionHeaders[m_sectionHeaderLen - 1].PointerToRawData + m_sectionHeaders[m_sectionHeaderLen - 1].VirtualAddress;
			break;
		}
	}
	return offset;
}

ULONG CpetoolApp::GetFAtoVA(ULONG fa)
{
	ULONG offset = GetFAtoRVA(fa);
	return GetRVAtoVA(offset);
}

ULONG CpetoolApp::GetVAtoFA(ULONG va)
{
	ULONG offset = GetVAtoRVA(va);
	return GetRVAtoFA(offset);
}

ULONG CpetoolApp::GetVAtoRVA(ULONG va)
{
	if (theApp.isx86) {
		return va - theApp.m_optional32Header.ImageBase;
	}
	return va - theApp.m_optional64Header.ImageBase;
}

ULONGLONG CpetoolApp::GetHex(std::string str)
{
	ULONGLONG hexNum = 0;
	std::istringstream iss(str);

	iss >> std::hex >> hexNum;
	return hexNum;
}


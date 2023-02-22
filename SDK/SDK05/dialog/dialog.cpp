// dialog.cpp : 定义应用程序的入口点。
//
#include <Windows.h>
#include "resource.h"

const char g_sLoginName[] = "admin";
const char g_sLoginPwd[] = "123456";
INT_PTR CALLBACK DialogProc(
    HWND hwndDlg,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
)
{
    switch (uMsg)
    {
    case  WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
        break;
    }
    case WM_COMMAND:
    {
        WORD dlgID = LOWORD(wParam);
        WORD msgKey = HIWORD(wParam);
        switch (msgKey)
        {
        case  BN_CLICKED:
        {
            if (dlgID == LOGIN)
            {
                char sLoginName[MAXBYTE] = {};
                char sLoginPwd[MAXBYTE] = {};
                SendDlgItemMessage(hwndDlg, LOGINNAME, WM_GETTEXT, sizeof(sLoginName), (LPARAM)sLoginName);
                SendDlgItemMessage(hwndDlg, PASSWORD, WM_GETTEXT, sizeof(sLoginPwd), (LPARAM)sLoginPwd);
                if (strcmp(sLoginName, g_sLoginName) == 0 && strcmp(sLoginPwd, g_sLoginPwd) == 0)
                {
                    MessageBox(hwndDlg, "登录成功", "消息", MB_OK);
                }
                else
                {
                    MessageBox(hwndDlg, "登录失败", "消息", MB_OK);
                }
            }
            break;
        }
        }
    }
    default:
        return FALSE;
    }  
    return TRUE;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    DialogBox(hInstance, MAKEINTRESOURCE(DIALOG_CUSTOM), NULL, DialogProc);
}

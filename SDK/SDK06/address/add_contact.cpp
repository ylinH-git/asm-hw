#include <Windows.h>
#include <Windowsx.h>
#include "address.h"
void OnAddCommand(HWND hwndDlg,      // handle to window
    WPARAM wParam,  // first message parameter
    LPARAM lParam   // second message parameter
)
{
    WORD wID = LOWORD(wParam);
    WORD wNotiCode = HIWORD(wParam);
    switch (wNotiCode)
    {
    case BN_CLICKED:
    {
        switch (wID)
        {
        case BTN_ADDCONFIRM:
        {
            char sName[MAXBYTE] = {};
            HWND nameEdHwnd = GetDlgItem(hwndDlg, ED_NAME);
            Edit_GetText(nameEdHwnd, (LPSTR)sName,sizeof(sName));
            char sTel[MAXBYTE] = {};
            HWND telEdHwnd = GetDlgItem(hwndDlg, ED_TEL);
            Edit_GetText(telEdHwnd, (LPSTR)sTel, sizeof(sTel));
            eGender eMemberGender;
            if (IsDlgButtonChecked(hwndDlg, RD_MALE) == BST_CHECKED)
            {
                eMemberGender = eGender::MALE;
            }
            if (IsDlgButtonChecked(hwndDlg, RD_FEMALE) == BST_CHECKED)
            {
                eMemberGender = eGender::FEMALE;
            }
            if (IsDlgButtonChecked(hwndDlg, RD_UNKNOW) == BST_CHECKED)
            {
                eMemberGender = eGender::UNKNOW;
            }
            struct tagMember member = {
                g_members.size() + 1,
                sName,
                sTel,
                eMemberGender
            };
            g_members.push_back(member);
            EndDialog(hwndDlg, 0);
            break;
        }
        default:
            break;
        }
        break;
    }

    default:
        break;
    }
}

INT_PTR CALLBACK AddDialogProc(HWND hwndDlg,  // handle to dialog box
    UINT uMsg,     // message  
    WPARAM wParam, // first message parameter
    LPARAM lParam  // second message parameter
)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        break;
    case WM_COMMAND:
        OnAddCommand(hwndDlg, wParam, lParam);
        break;
    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
        break;
    }
    default:
        return FALSE;//处理了的消息返回TRUE，不处理的返回FALSE
    }
    return TRUE;
}

void OpenAddDialog()
{
    DialogBox(g_hIns, MAKEINTRESOURCE(DLG_ADDCONTACT), NULL, AddDialogProc);
}
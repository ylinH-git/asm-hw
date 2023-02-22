#include <Windows.h>
#include <Windowsx.h>
#include <commctrl.h>
#include "address.h"

int findIdx = std::string::npos;
void SearchContact(HWND hwndDlg)
{
    HWND hEdSearch = GetDlgItem(hwndDlg, ED_MODSEARCH);
    HWND hEdName = GetDlgItem(hwndDlg, ED_MODNAME);
    HWND hEdTEL = GetDlgItem(hwndDlg, ED_MODTEL);
    HWND hRdMale = GetDlgItem(hwndDlg, RD_MODMALE);
    HWND hRdFemale = GetDlgItem(hwndDlg, RD_MODFEMALE);
    HWND hRdUnknow = GetDlgItem(hwndDlg, RD_MODUNKNOW);
    char sSearchKey[MAXBYTE] = {};
    Edit_GetText(hEdSearch, (LPSTR)sSearchKey, sizeof(sSearchKey));
    if (g_members.size() > 0)
    {
        for (auto lt = g_members.begin(); lt != g_members.end(); lt++)
        {
            findIdx = (lt->m_name).find(sSearchKey);
            if (findIdx == std::string::npos)
            {
                MessageBox(hwndDlg, "未查询到指定联络人，请重新查找", NULL, MB_OK);
            }
            else
            {
                Edit_SetText(hEdName, lt->m_name.c_str());
                Edit_SetText(hEdTEL, lt->m_tel.c_str());
                if (lt->m_gender == eGender::MALE)
                {
                    Button_SetCheck(hRdMale, BST_CHECKED);
                }

                if (lt->m_gender == eGender::FEMALE)
                {
                    Button_SetCheck(hRdFemale, BST_CHECKED);
                }

                if (lt->m_gender == eGender::UNKNOW)
                {
                    Button_SetCheck(hRdUnknow, BST_CHECKED);
                }
            }
        }
    }
    else
    {
        MessageBox(hwndDlg, "列表为空，请先添加联系人信息", NULL, MB_OK);
    }
   
}

void ModContact(HWND hwndDlg)
{
    if (findIdx == std::string::npos)
    {
        MessageBox(hwndDlg, "此操作人不存在，无法修改", NULL, MB_OK);
    }
    else
    {
        char sName[MAXBYTE] = {};
        HWND nameEdHwnd = GetDlgItem(hwndDlg, ED_MODNAME);
        Edit_GetText(nameEdHwnd, (LPSTR)sName, sizeof(sName));
        char sTel[MAXBYTE] = {};
        HWND telEdHwnd = GetDlgItem(hwndDlg, ED_MODTEL);
        Edit_GetText(telEdHwnd, (LPSTR)sTel, sizeof(sTel));
        eGender eMemberGender;
        if (IsDlgButtonChecked(hwndDlg, RD_MODMALE) == BST_CHECKED)
        {
            eMemberGender = eGender::MALE;
        }
        if (IsDlgButtonChecked(hwndDlg, RD_MODFEMALE) == BST_CHECKED)
        {
            eMemberGender = eGender::FEMALE;
        }
        if (IsDlgButtonChecked(hwndDlg, RD_MODUNKNOW) == BST_CHECKED)
        {
            eMemberGender = eGender::UNKNOW;
        }
        g_members[findIdx].m_name = sName;
        g_members[findIdx].m_tel = sTel;
        g_members[findIdx].m_gender = eMemberGender;
        MessageBox(hwndDlg, "修改成功", "消息", MB_OK);
    }
}
void OnModCommand(HWND hwndDlg,      // handle to window
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
        case BTN_MODSEARCH:
        {
            SearchContact(hwndDlg);
            break;
        }
        case BTN_MODIFY:
        {
            ModContact(hwndDlg);
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

INT_PTR CALLBACK ModDialogProc(HWND hwndDlg,  // handle to dialog box
    UINT uMsg,     // message  
    WPARAM wParam, // first message parameter
    LPARAM lParam  // second message parameter
)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        findIdx = std::string::npos;
        break;
    }
    case WM_COMMAND:
        OnModCommand(hwndDlg, wParam, lParam);
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

void OpenModDialog()
{
    DialogBox(g_hIns, MAKEINTRESOURCE(DLG_MODCONTACT), NULL, ModDialogProc);
}
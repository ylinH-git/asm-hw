#include <Windows.h>
#include <Windowsx.h>
#include <commctrl.h>
#include "address.h"

void InsertColumn(HWND hLstc, int nRowIdx, int nColIdx, std::vector<tagMember>::iterator lt)
{
    LVITEM li = {};
    li.mask = LVIF_TEXT | LVIF_PARAM;
    li.iItem = nRowIdx;
    li.pszText = (LPSTR)lt->m_name.c_str();
    li.lParam = lt->m_id;
    ListView_InsertItem(hLstc, &li);
    if (lt->m_gender == eGender::MALE)
    {
        ListView_SetItemText(hLstc, nRowIdx, ++nColIdx, (LPSTR)"男");
    }
    else  if (lt->m_gender == eGender::FEMALE)
    {
        ListView_SetItemText(hLstc, nRowIdx, ++nColIdx, (LPSTR)"女");
    }
    else
    {
        ListView_SetItemText(hLstc, nRowIdx, ++nColIdx, (LPSTR)"未知");
    }
    ListView_SetItemText(hLstc, nRowIdx, ++nColIdx, (LPSTR)lt->m_tel.c_str());
}
void InitList(HWND hwndDlg)
{
    HWND hLstc = GetDlgItem(hwndDlg, LT_MEMBER);

    //添加列
    LVCOLUMN lvc = {};
    lvc.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
    lvc.cx = 50;
    lvc.fmt = LVCFMT_LEFT;
    lvc.pszText = (LPSTR)"姓名";
    ListView_InsertColumn(hLstc, 0, &lvc);

    lvc.pszText = (LPSTR)"性别";
    ListView_InsertColumn(hLstc, 1, &lvc);

    lvc.pszText = (LPSTR)"电话";
    lvc.cx = 200;
    ListView_InsertColumn(hLstc, 2, &lvc);

    //添加内容
    int nRowIdx = 0;
    for (auto lt = g_members.begin(); lt != g_members.end(); lt++)
    {
        int nColIdx = 0;
        InsertColumn(hLstc, nRowIdx, nColIdx, lt);
        ++nRowIdx;
    }
}

void SearchList(HWND hwndDlg)
{
    HWND hEdSearch = GetDlgItem(hwndDlg, ED_SEARCH);
    HWND hLstc = GetDlgItem(hwndDlg, LT_MEMBER);
    ListView_DeleteAllItems(hLstc);
    std::vector<tagMember> filterMember = {};
    char sSearchKey[MAXBYTE] = {};
    Edit_GetText(hEdSearch, (LPSTR)sSearchKey, sizeof(sSearchKey));
    for (auto lt = g_members.begin(); lt != g_members.end(); lt++)
    {
        int idx = -1;
        idx = (lt->m_name).find(sSearchKey);
        if (idx != std::string::npos)
        {
            filterMember.push_back(*lt);
        }
    }
    //添加内容
    int nRowIdx = 0;
    for (auto lt = filterMember.begin(); lt != filterMember.end(); lt++)
    {
        int nColIdx = 0;
        InsertColumn(hLstc, nRowIdx, nColIdx, lt);
        ++nRowIdx;
    }
}
void OnShowCommand(HWND hwndDlg,      // handle to window
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
        case BTN_SEARCH:
        {
            SearchList(hwndDlg);
            break;
        }
        case BTN_DELETE:
        {
            HWND hLstc = GetDlgItem(hwndDlg, LT_MEMBER);

            //获取列表控件选中的内容
            int n = ListView_GetSelectionMark(hLstc); //获取选中的行数
            LVITEM li = {};
            li.mask = LVIF_PARAM;
            li.iItem = n;
            ListView_GetItem(hLstc, &li);
            std::vector<tagMember>::iterator lt;
            for (lt = g_members.begin(); lt != g_members.end(); lt++)
            {
                if (lt->m_id == li.lParam)
                {
                    break;
                }
            }
            g_members.erase(lt);
            ListView_DeleteAllItems(hLstc);
            InitList(hwndDlg);
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

INT_PTR CALLBACK ShowDialogProc(HWND hwndDlg,  // handle to dialog box
    UINT uMsg,     // message  
    WPARAM wParam, // first message parameter
    LPARAM lParam  // second message parameter
)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        InitList(hwndDlg);
        HWND hLstc = GetDlgItem(hwndDlg, LT_MEMBER);
        DWORD dwStyle = ListView_GetExtendedListViewStyle(hLstc);
        ListView_SetExtendedListViewStyle(hLstc,
            dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        break;
    }
    case WM_COMMAND:
        OnShowCommand(hwndDlg, wParam, lParam);
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

void OpenShowDialog()
{
    DialogBox(g_hIns, MAKEINTRESOURCE(DLG_SHOWCONTACT), NULL, ShowDialogProc);
}
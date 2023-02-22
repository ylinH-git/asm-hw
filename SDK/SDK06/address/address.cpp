#include "address.h"
#include "add_contact.h"
#include "show_contact.h"
#include "mod_contact.h"
HINSTANCE g_hIns;
extern std::vector<tagMember> g_members = {};
void OnCommand(HWND hwndDlg,      // handle to window
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
        case BTN_ADDCONTACT:
        {
            OpenAddDialog();
            break;
        }
        case BTN_VIEWCONTACT:
        {
            OpenShowDialog();
            break;
        }
        case BTN_MODCONTACT:
        {
            OpenModDialog();
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

INT_PTR CALLBACK DialogProc(HWND hwndDlg,  // handle to dialog box
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
        OnCommand(hwndDlg, wParam, lParam);
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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    g_hIns = hInstance;
    DialogBox(g_hIns, MAKEINTRESOURCE(DLG_MAIN), NULL, DialogProc);
}
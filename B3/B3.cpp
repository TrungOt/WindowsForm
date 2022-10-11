#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#pragma comment(lib, "comctl32.lib")
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <string>
#include <strsafe.h>

#define ID_TREEVIEW 1
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
#define IDC_LISTVIEW 2
using namespace std;
HWND hTreeView;
HWND hListView;
HTREEITEM ThisPC;
BOOL loaded = false;
BOOL loaded2 = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND AddTreeView(HWND hwnd);
HTREEITEM AddItemToTree(HWND hwndTV, LPTSTR lpszItem, int nLevel);
void ShowItemReg(HWND hwnd, HTREEITEM hItem);
//void CreateItem(HWND hwndTV);
void CreateReg(HWND hwndTV);
void QueryKey(HKEY hKey, TVINSERTSTRUCT tvInsert, LPARAM path);
void AddToNode(HTREEITEM node, LPARAM path);
HWND AddListView(HWND hwnd);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"MyClass";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Registry",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, 860, 640,        // Size and position
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );



    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, 5);

    // Run the message loop.

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return 0;
}

//-----------------ProcWindow
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

    case WM_CLOSE:
    {
        if (MessageBox(hwnd, L"Thoat Chuong Trinh", L"Registry", MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(hwnd);
            break;
        }

    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);



        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    case WM_CREATE:
    {
        HWND hStart;
        hTreeView = AddTreeView(hwnd);
        CreateReg(hTreeView);
        hListView = AddListView(hwnd);
        break;
    }
    case WM_NOTIFY:
        if (loaded)
        {
            LPNMTREEVIEW lpnmTree = (LPNMTREEVIEW)lParam;   //Cau truc cua mes TVN_ITEMEXPANDING
            HTREEITEM node = lpnmTree->itemNew.hItem;
            switch (((LPNMHDR)lParam)->code)
            {
            case TVN_ITEMEXPANDING:
            {

                UINT check_action = lpnmTree->action;// 2= mo, 1=tat
                UINT check_status = lpnmTree->itemNew.state;

                if (node != ThisPC) 
                {
                    TVITEMEX tv;
                    tv.hItem = node;
                    TreeView_GetItem(hTreeView, &tv);
                    if (check_action == 2 && check_status != 64 && check_status != 66)//Check len xuong
                    {
                        LPARAM path = tv.lParam;
                        AddToNode(node, path);
                    }
                }
                break;
            }
            case TVN_SELCHANGED:
            {
                TreeView_GetNextItem(hTreeView, node, TVGN_CARET);
                break;
            }

            }
        }

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

HWND AddTreeView(HWND hwnd)
{
    RECT m_rcClient;
    HWND m_hTreeView;
    InitCommonControls();
    GetClientRect(hwnd, &m_rcClient);   //Lay vi tri kich thuoc duoc to cua cua so me
    m_hTreeView = CreateWindowEx(
        0,
        WC_TREEVIEW, TEXT("Tree View"),
        WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | WS_HSCROLL | WS_VSCROLL,
        0, 0, m_rcClient.right / 3, m_rcClient.bottom,
        hwnd,
        (HMENU)ID_TREEVIEW,
        NULL, NULL
    );
    return m_hTreeView;
}
HWND AddListView(HWND hwnd)
{
    HWND m_hwndLV;
    RECT m_rcClient;
    GetClientRect(hwnd, &m_rcClient);   //Lay vi tri kich thuoc duoc to cua cua so me
    // Create the list-view window in report view with label editing enabled.
    m_hwndLV = CreateWindowEx(WS_EX_CLIENTEDGE,
        WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT,
        m_rcClient.right / 3, 0, 2 * m_rcClient.right / 3, m_rcClient.bottom,
        hwnd,
        (HMENU)IDC_LISTVIEW, NULL, NULL);
    LVCOLUMN col;
    col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    col.fmt = LVCFMT_LEFT;

    // Cot 1
    col.cx = (2 * m_rcClient.right / 3) / 3;
    col.pszText = (LPWSTR)L"Name";
    ListView_InsertColumn(m_hwndLV, 0, &col);

    // Cot 2
    col.cx = (2 * m_rcClient.right / 3) / 3;
    col.pszText = (LPWSTR)L"Type";
    ListView_InsertColumn(m_hwndLV, 1, &col);

    // Cot 3
    col.cx = (2 * m_rcClient.right / 3) / 3;
    col.pszText = (LPWSTR)L"Value";
    ListView_InsertColumn(m_hwndLV, 2, &col);


    return m_hwndLV;
}
void CreateReg(HWND hwndTV)
{

    TVINSERTSTRUCT tvInsert;
    tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
    tvInsert.item.pszText = (LPWSTR)L"This PC";
    tvInsert.item.cchTextMax = sizeof(tvInsert.item.pszText) / sizeof(tvInsert.item.pszText[0]);
    tvInsert.item.lParam = (LPARAM)L"Root";
    tvInsert.item.cChildren = 1;
    tvInsert.hParent = NULL;
    tvInsert.hInsertAfter = TVI_ROOT;//Chen ngay dau


//CREAT Computer Item
    ThisPC = TreeView_InsertItem(hTreeView, &tvInsert);

    //Create 5 hKey
    tvInsert.hParent = ThisPC;

    tvInsert.item.pszText = (LPWSTR)L"HKEY_CLASSES_ROOT";
    tvInsert.item.lParam = (LPARAM)L"HKEY_CLASSES_ROOT";
    HTREEITEM hHKCR = TreeView_InsertItem(hTreeView, &tvInsert);

    tvInsert.item.pszText = (LPWSTR)L"HKEY_CURRENT_USER";
    tvInsert.item.lParam = (LPARAM)L"HKEY_CURRENT_USER";
    HTREEITEM hHKCU = TreeView_InsertItem(hTreeView, &tvInsert);

    tvInsert.item.pszText = (LPWSTR)L"HKEY_LOCAL_MACHINE";
    tvInsert.item.lParam = (LPARAM)L"HKEY_LOCAL_MACHINE";
    HTREEITEM hHKLM = TreeView_InsertItem(hTreeView, &tvInsert);

    tvInsert.item.pszText = (LPWSTR)L"HKEY_USERS";
    tvInsert.item.lParam = (LPARAM)L"HKEY_USERS";
    HTREEITEM hHKUS = TreeView_InsertItem(hTreeView, &tvInsert);

    tvInsert.item.pszText = (LPWSTR)L"HKEY_CURRENT_CONFIG";
    tvInsert.item.lParam = (LPARAM)L"HKEY_CURRENT_CONFIG";
    HTREEITEM hHKCC = TreeView_InsertItem(hTreeView, &tvInsert);

    TreeView_Expand(hTreeView, ThisPC, TVE_EXPAND);
    TreeView_SelectItem(hTreeView, ThisPC);             //EXPAND luoon 
    loaded = true;
    //----------------------------CREAT SubKey





}

void AddToNode(HTREEITEM node, LPARAM path)
{
    TVINSERTSTRUCT tvInsert;
    tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;

    tvInsert.item.cchTextMax = sizeof(tvInsert.item.pszText) / sizeof(tvInsert.item.pszText[0]);
    //tvInsert.item.lParam = (LPARAM)L"";
    tvInsert.item.cChildren = 1;
    //tvInsert.hParent = NULL;
    tvInsert.hInsertAfter = TVI_ROOT;                       //Chen ngay dau

    //Cut lay hKey o day

    HKEY hKey;
    wchar_t str_hKey[50] = { 0 };
    int len_hKey;
    wchar_t* linkold = new wchar_t[lstrlen((LPWSTR)path)];  //Cap phat cho 1 vung rieng khong bi de
    wchar_t* zlink = new wchar_t[lstrlen((LPWSTR)path)];
    for (int i = 0; i < lstrlen((LPWSTR)path); i++)
    {
        zlink[0] = 0;
    }
    lstrcpy(linkold, (LPWSTR)path);

    //wchar_t link[512] = { 0 };
    //lstrcpy((LPWSTR)link, (LPWSTR)path);
    for (int i = 0; i <= lstrlen((LPWSTR)linkold); i++)    //Tinh do dai tu dau toi dau /
    {
        if (linkold[i] == 92 || linkold[i] == 0)      //Dau / va khoang trong
        {
            len_hKey = i + 1;
            break;
        }
    }
    hKey = 0;
    lstrcpyn((LPWSTR)str_hKey, (LPWSTR)linkold, len_hKey); //Copy 1 doan bang len_hKey---- link van giu nguyen
    if (lstrcmp((LPWSTR)str_hKey, L"HKEY_CURRENT_USER") == 0)
        hKey = (HKEY)(ULONG_PTR)((LONG)HKEY_CURRENT_USER);//(HKEY)(ULONG_PTR)((LONG)0x80000001)
    if (lstrcmp((LPWSTR)str_hKey, L"HKEY_CLASSES_ROOT") == 0)
        hKey = (HKEY)(ULONG_PTR)((LONG)HKEY_CLASSES_ROOT);
    if (lstrcmp((LPWSTR)str_hKey, L"HKEY_LOCAL_MACHINE") == 0)
        hKey = (HKEY)(ULONG_PTR)((LONG)HKEY_LOCAL_MACHINE);
    if (lstrcmp((LPWSTR)str_hKey, L"HKEY_USERS") == 0)
        hKey = (HKEY)(ULONG_PTR)((LONG)HKEY_USERS);
    if (lstrcmp((LPWSTR)str_hKey, L"HKEY_CURRENT_CONFIG") == 0)
        hKey = (HKEY)(ULONG_PTR)((LONG)HKEY_CURRENT_CONFIG);
    //Thay doi link ----------------XOA HKCU (hKey)
    for (int i = 0; i <= lstrlen((LPWSTR)linkold); i++)    //Tinh do dai tu dau toi dau /
    {

        if (linkold[i] == 0)
        {
            break;
        }
        if (linkold[i] == 92)
        {
            int ci = i;
            for (int j = 0; j <= (lstrlen((LPWSTR)linkold) - len_hKey); j++)
            {
                zlink[j] = linkold[ci + 1];
                ci++;
            }
            break;
        }
    }
    HKEY hTestKey;

    if (RegOpenKeyEx(hKey, zlink, 0, KEY_READ, &hTestKey) == ERROR_SUCCESS)
    {
        tvInsert.hParent = node;
        QueryKey(hTestKey, tvInsert, path);
    }
    RegCloseKey(hTestKey);
    loaded2 = true;
}
void QueryKey(HKEY hKey, TVINSERTSTRUCT tvInsert, LPARAM path)
{
    TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = _T("");  // buffer for class name  
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys = 0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 

    DWORD i, retCode;

    TCHAR  achValue[MAX_VALUE_NAME];
    DWORD cchValue = MAX_VALUE_NAME;

    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
    // Enumerate the subkeys, until RegEnumKeyEx fails.
    if (cSubKeys)
    {
        //printf("\nNumber of subkeys: %d\n", cSubKeys);

        for (i = 0; i < cSubKeys; i++)
        {
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(hKey, i,
                achKey,
                &cbName,
                NULL,
                NULL,//achClass,
                NULL,//&cchClassName,
                &ftLastWriteTime);
            if (retCode == ERROR_SUCCESS)
            {

                tvInsert.item.pszText = achKey;

                wchar_t* link = new wchar_t[wcslen(achKey) + lstrlen((LPWSTR)path) + 1];  //Cap phat 1 vung rieng khong bi de
                lstrcpy(link, (LPWSTR)path);
                lstrcat(link, (LPWSTR)(L"\\"));
                lstrcat(link, achKey);
                tvInsert.item.lParam = (LPARAM)link;

                TreeView_InsertItem(hTreeView, &tvInsert);
                //_tprintf(TEXT("(%d) %s\n"), i + 1, achKey);
            }
        }
    }

    // Enumerate the key values. 

    //if (cValues)
    //{
    //    //printf("\nNumber of values: %d\n", cValues);
    //
    //    for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
    //    {
    //        cchValue = MAX_VALUE_NAME;
    //        achValue[0] = '\0';
    //        retCode = RegEnumValue(hKey, i,
    //            achValue,
    //            &cchValue,
    //            NULL,
    //            NULL,
    //            NULL,
    //            NULL);
    //
    //        if (retCode == ERROR_SUCCESS)
    //        {
    //            _tprintf(TEXT("(%d) %s\n"), i + 1, achValue);
    //        }
    //    }
    //}
}

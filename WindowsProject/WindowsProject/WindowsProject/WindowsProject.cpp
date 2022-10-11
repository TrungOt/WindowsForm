
#ifndef UNICODE

#endif // !1

#include "Windows.h"
#include "stdlib.h"
#include "iostream"
#include "cctype"


#define GENERATE_BUTTON 1
#define CHECKBOX_REVERSE 2
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void AddControl(HWND hwnd);
HWND hOutput;
HWND hInput;
HWND hCheckBox;
HWND hStaticOut;
//------------------Main
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
        L"REVERSE WINDOW",    // Window text
        WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_ICONIC,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, 480, 640,        // Size and position
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handles
        NULL        // Additional application data
    );
    AddControl(hwnd);
    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

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
        //case WM_DESTROY:
        //    PostQuitMessage(0);           //tắt luôn Window
        //    return 0;
    case WM_CLOSE:
    {
        if (MessageBox(hwnd, L"Thoát", L"Dừng Chương Trình", MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(hwnd);
            break;
        }

    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        //Paint
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        break;
    }


    case WM_COMMAND:
    {
        switch (wParam)
        {
        case GENERATE_BUTTON:
        {
            char strIn[1024], strOut[1024];
            LRESULT checkbox;
            SendMessageA(hInput, WM_GETTEXT, sizeof(strIn), LPARAM(strIn));  //GetWindowTextA(hInput, strIn, 100);  //SendMessageA(hInput,WM_GETTEXT,64,*strOut);   
            checkbox = SendMessage(hCheckBox, BM_GETSTATE, 0, 0);   //gui tin nhan checkbox

            _strrev(strIn); //dao chuoi
            SendMessageA(hStaticOut, WM_SETTEXT, 0, LPARAM(strIn));//SetWindowTextA(hOutput, strIn);

            if (BST_CHECKED == checkbox)
                for (int i = 0; i <= strlen(strIn); i++)
                {
                    {
                        if (islower(strIn[i]))
                            strOut[i] = toupper(strIn[i]);
                        else
                            strOut[i] = tolower(strIn[i]);
                    }
                    SendMessageA(hStaticOut, WM_SETTEXT, 0, LPARAM(strOut));
                }
            break;
        }
        }
    }




    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

void AddControl(HWND hwnd)
{
    HWND hStatic = CreateWindow(L"static", L"Text Here", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 150, 50, 100, 20, hwnd, NULL, NULL, NULL);
    hInput = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 150, 70, 180, 100, hwnd, NULL, NULL, NULL);

    hStaticOut = CreateWindow(L"static", L"Output", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER, 150, 200, 80, 20, hwnd, NULL, NULL, NULL);
    hStaticOut = CreateWindow(L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 220, 180, 100, hwnd, NULL, NULL, NULL);
    //hOutput = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE ,    150, 220, 180, 100, hwnd, NULL, NULL, NULL);

    HWND hButton = CreateWindow(L"Button", L"Reverse", WS_VISIBLE | WS_CHILD, 190, 400, 100, 50, hwnd, (HMENU)GENERATE_BUTTON, NULL, NULL);

    hCheckBox = CreateWindowW(L"Button", L"Change Uptower/Lowtower ", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTOCHECKBOX | SS_CENTER, 150, 350, 195, 20, hwnd, (HMENU)CHECKBOX_REVERSE, NULL, NULL);
}
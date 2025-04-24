// iksoks.cpp : Defines the entry point for the application.
//

//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "framework.h"
#include "iksoks.h"
#include <windowsx.h>
#include <string>
#include "tabla.h"
#include "connection.h"

#define MAX_LOADSTRING 100

// Global Variables:
tabla* game;
WSADATA wsa;
connection* conn;
bool multi = false, urTurn = false;
char* inst = nullptr;
int win = 0;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Multi(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    ::game = new tabla();
    ::inst = new char[3];
    ::inst[2] = '\0';

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_IKSOKS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IKSOKS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    if (::game != nullptr) { delete ::game; ::game = nullptr; }
    if (::conn != nullptr) { delete ::conn; ::conn = nullptr; }
    if (::inst != nullptr) { delete[] ::inst; ::inst = nullptr; }
    WSACleanup();

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
       CW_USEDEFAULT, 0, 3 * CELL_SIZE + 16, 3 * CELL_SIZE + 39, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_KEYDOWN:
        if (wParam == VK_SPACE && win) {
            win = 0;
            if (::conn != nullptr) { delete ::conn; ::conn = nullptr; multi = false; }
            ::game = new tabla();
            InvalidateRect(hWnd, nullptr, TRUE);
        } else if (wParam == 'M') {
            DialogBoxW(hInst, MAKEINTRESOURCE(IDD_MULTIBOX), hWnd, Multi);
            if (multi) {
                delete ::game;
                game = new tabla();
                urTurn = ::conn->isServ();
                InvalidateRect(hWnd, nullptr, TRUE);
                if (multi && !urTurn) PostMessage(hWnd, MULTI_PORT, 0, 0);
            }
        }
        break;
    case WM_LBUTTONDOWN:
    {
        if (win) break;
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        int col = xPos / CELL_SIZE;
        int row = yPos / CELL_SIZE;

        int a = ::game->handleInput(row, col);
        if (a != 1) {
            inst[0] = row; inst[1] = col;
            if (multi) ::conn->sendSignal(inst);
            if (a == 101) ::win = ::game->getValue(row, col);
            InvalidateRect(hWnd, nullptr, TRUE);
            urTurn = false;
            if (multi && !urTurn) SetTimer(hWnd, 1, 1, nullptr);
        }
    }
    break;
    case MULTI_PORT:
    {
        ::conn->getSignal(inst);
        int row = inst[0], col = inst[1];
        int a = ::game->handleInput(row, col);
        if (a != 1) {
            if (a == 101) ::win = ::game->getValue(row, col);
            InvalidateRect(hWnd, nullptr, TRUE);
            urTurn = true;
        }
    }
    break;

    case WM_TIMER:
        KillTimer(hWnd, 1);
        PostMessage(hWnd, MULTI_PORT, 0, 0);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        HGDIOBJ oldPen = SelectObject(hdc, hPen);

        if (!win) {
            for (int i = 1; i < 3; i++) {
                MoveToEx(hdc, i * CELL_SIZE, 0, nullptr);
                LineTo(hdc, i * CELL_SIZE, 3 * CELL_SIZE);

                MoveToEx(hdc, 0, i * CELL_SIZE, nullptr);
                LineTo(hdc, 3 * CELL_SIZE, i * CELL_SIZE);
            }

            HFONT hFont = CreateFontW(
                48,
                0, 0, 0, FW_BLACK,
                FALSE, FALSE, FALSE,
                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                L"Arial"
            );
            HFONT oldFont = (HFONT)SelectObject(hdc, hFont);


            wchar_t* text = new wchar_t[2];
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    if (::game->getValue(row, col) != 0) {
                        SetBkMode(hdc, TRANSPARENT);
                        bool isNTV = std::get<0>(::game->nextToVanish()) == row && std::get<1>(::game->nextToVanish()) == col;
                        SetTextColor(hdc, ((::game->getValue(row, col) & 1) == 0) ? isNTV ? RGB(0, 0, 100) : RGB(0, 0, 255) : isNTV ? RGB(100, 0, 0) : RGB(255, 0, 0));

                        mbstowcs(text, (ToString((turn) ::game->getValue(row, col))), 2);
                        TextOutW(hdc, col * CELL_SIZE + 35, row * CELL_SIZE + 35, text, 1);
                    }
                }
            }
            delete[] text;

            SelectObject(hdc, oldFont);
            DeleteObject(hFont);
        } else {
            { delete ::game; ::game = nullptr; }
            const wchar_t* text = (win == 1 ? L"Igrac X je pobedio.\nPRITISNI SPACE ZA NOVU PARTIJU" : L"Igrac O je pobedio.\nPRITISNI SPACE ZA NOVU PARTIJU");

            RECT rect;
            GetClientRect(hWnd, &rect);

            HBRUSH bgBrush = CreateSolidBrush(win == 1 ? RGB(255, 0, 0) : RGB(0, 0, 255));
            FillRect(hdc, &rect, bgBrush);
            DeleteObject(bgBrush);

            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);

            RECT textRect = rect;
            DrawTextW(hdc, text, -1, &textRect, DT_CALCRECT | DT_CENTER | DT_WORDBREAK);

            int textHeight = textRect.bottom - textRect.top;
            int offsetY = (rect.bottom - textHeight) / 2;
            textRect.top = offsetY;
            textRect.bottom = offsetY + textHeight;
            textRect.left = rect.left;
            textRect.right = rect.right;

            DrawTextW(hdc, text, -1, &textRect, DT_CENTER | DT_WORDBREAK);
        }

        SelectObject(hdc, oldPen);
        DeleteObject(hPen);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        if (::inst != nullptr) { delete[] ::inst; ::inst = nullptr; }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool isServ = false;

INT_PTR CALLBACK Multi(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG: {
        try {
            if (!multi) if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { EndDialog(hDlg, IDCANCEL); multi = true; }
            char hostname[256];
            gethostname(hostname, sizeof(hostname));
            struct addrinfo hints = {}, * res;
            hints.ai_family = AF_INET;
            getaddrinfo(hostname, nullptr, &hints, &res);
            struct sockaddr_in* addr = (struct sockaddr_in*)res->ai_addr;
            const char* ip = inet_ntoa(addr->sin_addr);
            std::wstring text = L"IP Address: ";
            text += std::wstring(ip, ip + strlen(ip));
            SetDlgItemText(hDlg, IDC_STATIC, text.c_str());
        } catch (...) {
            SetDlgItemText(hDlg, IDC_STATIC, L"IP Address: Error");
        }
    }
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            if (LOWORD(wParam) == IDCANCEL && multi) { WSACleanup(); multi = false; }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_CHECK1) {
            if (HIWORD(wParam) == BN_CLICKED) {
                if (!IsDlgButtonChecked(hDlg, IDC_CHECK1)) {
                    ShowWindow(GetDlgItem(hDlg, IDC_STATIC), SW_HIDE);
                    ShowWindow(GetDlgItem(hDlg, IDC_EDIT1), SW_SHOW);
                    ::isServ = false;
                } else {
                    ShowWindow(GetDlgItem(hDlg, IDC_STATIC), SW_SHOW);
                    ShowWindow(GetDlgItem(hDlg, IDC_EDIT1), SW_HIDE);
                    ::isServ = true;
                }
            }
            return (INT_PTR)TRUE;
        }

        if (LOWORD(wParam) == IDC_BUTTON1) {
            char buffer[3*4+3+1];
            GetDlgItemTextA(hDlg, IDC_EDIT1, buffer, sizeof(buffer));
            try {
                ::conn = new connection(isServ, buffer);
                multi = true;
                EndDialog(hDlg, IDOK);
            } catch (...) {
                MessageBox(hDlg, L"An unknown error occurred.", L"Error", MB_OK | MB_ICONERROR);
            }
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

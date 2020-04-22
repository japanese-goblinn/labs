// lab12.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lab12.h"

#define MAX_LOADSTRING 100

#define TIMER_ID 121
#define TIMER_INTERVAL 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HANDLE thread1;
HANDLE thread2;
HANDLE thread3;
CRITICAL_SECTION cs;

int margin = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void line(HDC hdc, int x1, int y1, int x2, int y2);
void circle(HDC hdc, int x, int y, int r);
void drawTrain(HDC hdc);

DWORD WINAPI threadProc(CONST LPVOID lpParam);
void initThreads();
void startThreads();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB12, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB12));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB12));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB12);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
//  PURPOSE:  Processes messages for the main window.
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
	case WM_CREATE:
		InitializeCriticalSection(&cs);
		initThreads();
		SetTimer(hWnd, TIMER_ID, TIMER_INTERVAL, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			drawTrain(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, TRUE);
		break;
    case WM_DESTROY:
		CloseHandle(thread1);
		CloseHandle(thread2);
		CloseHandle(thread3);
		DeleteCriticalSection(&cs);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void drawTrain(HDC hdc) {
	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
	HBRUSH skyBlueBrush = CreateSolidBrush(RGB(126, 192, 238));
	line(hdc, 0, 300, 1600, 300);
	line(hdc, margin + 380, 200, margin + 410, 200);
	line(hdc, margin + 380, 200, margin + 390, 210);
	line(hdc, margin + 400, 210, margin + 410, 200);
	line(hdc, margin + 100, 270, margin + 120, 270);
	line(hdc, margin + 300, 270, margin + 320, 270);
	line(hdc, margin + 200, 270, margin + 220, 270);
	
	SelectObject(hdc, redBrush);

	Rectangle(hdc, margin + 20, 230, margin + 100, 280);
	Rectangle(hdc, margin + 120, 230, margin + 200, 280);
	Rectangle(hdc, margin + 220, 230, margin + 300, 280);
	SelectObject(hdc, yellowBrush);
	RoundRect(hdc, margin + 320, 200, margin + 365, 280, 30, 20);
	SelectObject(hdc, skyBlueBrush);
	RoundRect(hdc, margin + 320, 230, margin + 420, 280, 30, 20);
	Rectangle(hdc, margin + 390, 230, margin + 400, 210);
	Rectangle(hdc, margin + 390, 230, margin + 400, 210);
	Rectangle(hdc, margin + 390, 230, margin + 400, 210);
	Rectangle(hdc, margin + 390, 230, margin + 400, 210);
	SelectObject(hdc, yellowBrush);
	circle(hdc, margin + 30, 290, 10);
	circle(hdc, margin + 90, 290, 10);
	circle(hdc, margin + 130, 290, 10);
	circle(hdc, margin + 190, 290, 10);
	circle(hdc, margin + 230, 290, 10);
	circle(hdc, margin + 230, 290, 10);
	circle(hdc, margin + 230, 290, 10);
	circle(hdc, margin + 230, 290, 10);
	circle(hdc, margin + 290, 290, 10);
	circle(hdc, margin + 345, 278, 22);
	circle(hdc, margin + 377, 288, 12);
	circle(hdc, margin + 400, 288, 12);
}

void line(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, nullptr);
	LineTo(hdc, x2, y2);
}

void circle(HDC hdc, int x, int y, int r) {
	Ellipse(hdc, x - r, y - r, x + r, y + r);
}

void initThreads() {
	thread1 = CreateThread(NULL, 0, &threadProc, NULL, 0, NULL);
	thread2 = CreateThread(NULL, 0, &threadProc, NULL, 0, NULL);
	thread3 = CreateThread(NULL, 0, &threadProc, NULL, 0, NULL);
}

void startThreads() {
	ResumeThread(thread1);
	ResumeThread(thread2);
	ResumeThread(thread3);
}

DWORD WINAPI threadProc(CONST LPVOID lpParam) {
	while (true) {
		EnterCriticalSection(&cs);
		margin += 2;
		Sleep(100);
		LeaveCriticalSection(&cs);
	}

	return 0;
}
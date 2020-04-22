#include "stdafx.h"
#include "lab9.h"

HINSTANCE hInst;				
HBITMAP hImages[4];

TCHAR* szTitle = _T("Lab 4");				
TCHAR* szWindowClass = _T("Lab4");

ATOM				RegisterWindowClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				DoPaint(HWND);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPTSTR    lpCmdLine,
                       _In_ int       nCmdShow)
{
    RegisterWindowClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) (msg.wParam);
}


ATOM RegisterWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LAB4);
    wcex.lpszClassName	= szWindowClass;
    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;

    hImages[0] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
    hImages[1] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
    hImages[2] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
    hImages[3] = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP4));

    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    MoveWindow(hWnd, 100, 100, 640, 480, FALSE);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    SetTimer(hWnd, 1, 50, NULL);
    return TRUE;
}


//dc  - context
HGDIOBJ brushWhite = CreateSolidBrush(RGB(255,255,255));
int imageIdx = 0;

void PaintImage(HWND hWnd, HBITMAP image, HDC hDc, int x, int y) {
    HDC hCDC = CreateCompatibleDC(hDc);
    SelectObject(hCDC, image);
    StretchBlt(hDc, x, y, 64, 64, hCDC, 0, 0, 200, 200, SRCCOPY);
    DeleteDC(hCDC);
}

int x = 200, y = 100, xt = x, yt = y;

void DoPaint(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hDc = BeginPaint(hWnd, &ps);
    RECT rect;
    GetClientRect(hWnd, &rect);
    HDC hCDC = CreateCompatibleDC(hDc);
    HBITMAP buffer = CreateCompatibleBitmap(hDc, 640, 480);

    SelectObject(hCDC, buffer);
    SelectObject(hCDC, brushWhite);


    //delete old frame
    Rectangle(hCDC, -1, -1, 640, 480);


    //paint image in new coords
    PaintImage(hWnd, hImages[imageIdx], hCDC, x - 32, y - 32);

    BitBlt(hDc, 0, 0, 640, 480, hCDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    DeleteDC(hCDC);
    DeleteObject(buffer);
}

void DoTimer(HWND hWnd) {
    int newIdx = imageIdx;
    bool moving = FALSE;
    //waiting for the changed coordinates while timer is ticking

    //moving thought x axis
    if (x != xt) {
        x -= (x-xt)/abs(x-xt) * 10;
        moving = TRUE;
    }

    //moving thought y axis

    if (y != yt) {
        y -= (y-yt)/abs(y-yt) * 10;
        moving = TRUE;
    }

    while (newIdx == imageIdx && moving)//frame changing
        newIdx = rand() % 4;//random frame
    imageIdx = newIdx;
    InvalidateRect(hWnd, NULL, FALSE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;

    switch (message)
    {
    case WM_PAINT:
        DoPaint(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_TIMER:
        DoTimer(hWnd);
        break;
    case WM_LBUTTONUP: 
        {
        // new coordinates
            xt = GET_X_LPARAM(lParam) / 10 * 10;
            yt = GET_Y_LPARAM(lParam) / 10 * 10; 
            break;
        }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

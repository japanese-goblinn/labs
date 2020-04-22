#include "stdafx.h"
#include "lab14.h"

#define MAX_LOADSTRING 100


HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];			
HWND hList;
DWORD threadId;


ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	
	MSG msg;
	HACCEL hAccelTable;

	
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAB8, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB8));

	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



void Scan(HKEY root, LPWSTR subkey, HWND hwnd) {
	HKEY key;
	int err = RegOpenKeyEx(root, subkey, 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &key);

	if (err != 0) {
		return;
	}

	int index = 0;
	char* name[10240];
	while ((err = RegEnumKey(key, index++, (LPWSTR)name, 10240)) != ERROR_NO_MORE_ITEMS) {
		if (lstrlen(subkey) == 0)
			Scan(root, (LPWSTR)name, hwnd);
		else {
			char path[10240];
			swprintf((LPWSTR)path, L"%s\\%s", subkey, name);
			Scan(root, (LPWSTR)path, hwnd);
		}
	}

	index = 0;
	int type, size = 1024;
	char data[1024];
	int namesize = 1024;
	while ((err = RegEnumValue(key, index++, (LPWSTR)name, (LPDWORD)&namesize, 0, (LPDWORD)&type, (LPBYTE)data, (LPDWORD)&size)) == 0) {
		if (type == REG_SZ) {
			char str[10240];
			swprintf((LPWSTR)str, L"%s\\%s = %s\n", subkey, name, data);
			if (lstrlen((LPWSTR)data) > 2) 
				if (wcsstr((LPWSTR)data, L"C:")) {
					HANDLE f = CreateFile((LPWSTR)data, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
					if (f == INVALID_HANDLE_VALUE) {
						WIN32_FIND_DATA d;
						//found wrong key value - send to list
						if (FindFirstFile((LPWSTR)data, &d) == INVALID_HANDLE_VALUE)
							SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)str);
					} else
						CloseHandle(f);
				}
		}
		namesize = size = 1024;
	}
	RegCloseKey(key);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB8));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LAB8);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

DWORD WINAPI StartScan(void* p) {
	Scan(HKEY_CURRENT_USER, L"", NULL);
	return NULL;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; 

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return FALSE;
	

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MoveWindow(hWnd, 200, 200, 1024, 768, FALSE);

	hList = CreateWindowEx(WS_EX_CLIENTEDGE
		, L"LISTBOX", NULL
		, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL
		, 0, 0, 1000, 768
		, hWnd, (HMENU)1, hInstance, NULL);


	threadId = GetCurrentThreadId();
	CreateThread(NULL, 10240000, StartScan, NULL, NULL, 0);

	return TRUE;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		switch (wmId)
		{

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


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

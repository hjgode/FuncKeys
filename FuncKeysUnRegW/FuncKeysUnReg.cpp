// FuncKeysUnReg.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FuncKeysUnReg.h"
#include <commctrl.h>

#include "..\common\registerhotkeys.h"
#include "..\common\registry.h"

#include "tlhelp32.h"
#pragma comment(lib, "toolhelp.lib")

#define TH32CS_SNAPNOHEAPS 0x40000000

const UINT MAX_KEYCOUNT = 10;

HWND				g_hWnd;			//global handle to window

/*$DOCBEGIN$
 =======================================================================================================================
 *    �
 *    BOOL IsProcessRunning( TCHAR * pname ); �
 *    * Description: Get process table snapshot, look for pname running. �
 *    * Arguments: pname - pointer to name of program to look for. �
 *    for example, app.exe. �
 *    * Returns: TRUE - process is running. �
 *    FALSE - process is not running. �
 *    $DOCEND$ �
 *
 =======================================================================================================================
 */

BOOL IsProcessRunning( TCHAR *pname )
{
    HANDLE          hProcList;
    PROCESSENTRY32  peProcess;
    DWORD           thDeviceProcessID;
    TCHAR           lpname[MAX_PATH];
    if ( wcslen(pname)==0 )
    {
        return FALSE;
    }

    wcscpy( lpname, pname );
    _tcslwr( lpname );
	//init ToolHelp for process list snapshot
    hProcList = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS | TH32CS_SNAPNOHEAPS, 0 );
    if ( hProcList == INVALID_HANDLE_VALUE )
    {
		ULONG err = GetLastError();
		if(err == ERROR_NOT_ENOUGH_MEMORY)
			DEBUGMSG(1, (L"IsProcessRunning:CreateToolhelp32Snapshot failed with ERROR_NOT_ENOUGH_MEMORY\n"));
		else
			DEBUGMSG(1, (L"IsProcessRunning:CreateToolhelp32Snapshot failed with GetLastError()=0x0%x\n", GetLastError()));
        return FALSE;
    }       /* end if */

    memset( &peProcess, 0, sizeof(peProcess) );
    peProcess.dwSize = sizeof( peProcess );
    if ( !Process32First( hProcList, &peProcess ) )
    {
        CloseToolhelp32Snapshot( hProcList );
        return FALSE;
    }       /* end if */

    thDeviceProcessID = 0;
    
	//try to find the process name
	do{
        //_wcslwr( peProcess.szExeFile );
		DEBUGMSG(1, (L"IsProcessRunning: comparing '%s' against '%s'\n", lpname, peProcess.szExeFile));
        if ( wcsicmp( peProcess.szExeFile, lpname ) == 0) //replaced wcsstr by wcsicmp
        {
            thDeviceProcessID = peProcess.th32ProcessID;
            break;
        }   /* end if */
    }while ( Process32Next( hProcList, &peProcess ) );

    if ( (GetLastError() == ERROR_NO_MORE_FILES) && (thDeviceProcessID == 0) )
    {
        CloseToolhelp32Snapshot( hProcList );
        return FALSE;
    }       /* end if */

    CloseToolhelp32Snapshot( hProcList );
    return TRUE;
}           /* IsProcessRunning */

void doUnReg(){
 	// wait until cprog is started but max 30 seconds
	int MaxTry=30;
	int test=0;
	int bRun=false;
	do{
		bRun = IsProcessRunning(L"cprog.exe");
		Sleep(1000);
		test++;
	}while( (test < MaxTry) && (bRun==false));

#ifndef DEBUG
	Sleep(10000); // wait for cprog to initialize
#endif

	BYTE* b;
	b = new BYTE[MAX_KEYCOUNT];
	ULONG size = MAX_KEYCOUNT;
	readReg(b, &size);
	UINT x=0;
	DWORD dwKey;
	while ( x < size ){
		dwKey=b[x];
		//unRegKey(dwKey);
		regKey(g_hWnd, dwKey); //unRegKey and then RegKey
		x++;
	}

}

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Perform application initialization:
	if (!InitInstance (hInstance, SW_HIDE))//nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_FUNCKEYSUNREG);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    It is important to call this function so that the application 
//    will get 'well formed' small icons associated with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS	wc;

    wc.style			= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc		= (WNDPROC) WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FUNCKEYSUNREG));
    wc.hCursor			= 0;
    wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName		= 0;
    wc.lpszClassName	= szWindowClass;

	return RegisterClass(&wc);
}

//
//  FUNCTION: InitInstance(HANDLE, int)
//
//  PURPOSE: Saves instance handle and creates main window
//
//  COMMENTS:
//
//    In this function, we save the instance handle in a global variable and
//    create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND	hWnd;
	TCHAR	szTitle[MAX_LOADSTRING];			// The title bar text
	TCHAR	szWindowClass[MAX_LOADSTRING];		// The window class name

	hInst = hInstance;		// Store instance handle in our global variable
	// Initialize global strings
	LoadString(hInstance, IDC_FUNCKEYSUNREG, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{	
		return FALSE;
	}

	ShowWindow(hWnd, SW_HIDE);//nCmdShow); 
	UpdateWindow(hWnd);
	if (hwndCB)
		CommandBar_Show(hwndCB, TRUE);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	TCHAR szHello[MAX_LOADSTRING];
	g_hWnd = hWnd;

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_HELP_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_FILE_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_CREATE:
			/*
			hwndCB = CommandBar_Create(hInst, hWnd, 1);			
			CommandBar_InsertMenubar(hwndCB, hInst, IDM_MENU, 0);
			CommandBar_AddAdornments(hwndCB, 0, 0);
			*/
			doUnReg();
			MessageBeep(MB_OK);
			Sleep(1000);
			MessageBeep(MB_ICONEXCLAMATION);
			PostQuitMessage(1);
			break;
		case WM_PAINT:
			RECT rt;
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rt);
			LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
			DrawText(hdc, szHello, _tcslen(szHello), &rt, 
				DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			/*
			CommandBar_Destroy(hwndCB);
			*/
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for the About box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;

	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			return TRUE;

		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
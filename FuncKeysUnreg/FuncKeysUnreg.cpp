// FuncKeysUnreg.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "..\common\registerhotkeys.h"
#include "..\common\registry.h"

#include "tlhelp32.h"
#pragma comment(lib, "toolhelp.lib")

#define TH32CS_SNAPNOHEAPS 0x40000000

const UINT MAX_KEYCOUNT = 12;

/*$DOCBEGIN$
 =======================================================================================================================
 *    £
 *    BOOL IsProcessRunning( TCHAR * pname ); £
 *    * Description: Get process table snapshot, look for pname running. £
 *    * Arguments: pname - pointer to name of program to look for. £
 *    for example, app.exe. £
 *    * Returns: TRUE - process is running. £
 *    FALSE - process is not running. £
 *    $DOCEND$ £
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

void initKeysArray(BYTE* b, int size){
	BYTE startFKey=0x70;
	BYTE* ptr=b;
	for(byte i=0; i<size; i++){
		*ptr = (BYTE)(startFKey + i);
		ptr++;
	}
}

int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
	BYTE* b;
	b = new BYTE[MAX_KEYCOUNT];
	ULONG size = MAX_KEYCOUNT;
	initKeysArray(b, MAX_KEYCOUNT);

	DWORD _timeOut=30;

	if(wcsstr(lpCmdLine, L"writereg")!=NULL){
		writeReg(b, size);
		writeTimeout(_timeOut);
	}

	int iTimeOut=30;
	if(readTimeOut(&iTimeOut)==0)
		_timeOut=iTimeOut;

	int test=0;
 	// wait until timeout (default 30 seconds)
	do{
		Sleep(1000);
		test++;
	}while( test < _timeOut );

#ifndef DEBUG
	Sleep(10000); // wait for cprog to initialize
#endif

	readReg(b, &size);
	UINT x=0;
	DWORD dwKey;
	while ( x < size ){
		dwKey=b[x];
		//unRegKey(dwKey);
		regKey(NULL, dwKey); //unRegKey and then RegKey
		x++;
	}
	MessageBeep(MB_ICONQUESTION);
	return 0;
}


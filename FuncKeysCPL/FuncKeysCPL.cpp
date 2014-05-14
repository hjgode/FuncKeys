// FuncKeysCPL.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "resource.h"

LONG nApplets = 1;
HINSTANCE g_hModule = NULL;

LONG CALLBACK CPlApplet (HWND hwndCPL, UINT uMsg, LONG lParam1,
                         LONG lParam2)
{
	int iApplet;
   switch (uMsg)
   {
      // First message sent. It is sent only once to allow the
      // DLL to initialize its applet(s)
      case CPL_INIT:
         return TRUE;

      // Second message sent. Return the count of applets supported
      // by this DLL
      case CPL_GETCOUNT:
         // Return the number of applets we support
         return (LONG) nApplets;

      // Third message sent. Sent once for each applet supported by
      // this DLL.
      // The lParam1 contains the number that indicates which applet
      // this is for, from 0 to 1 less than the count of applets.
      // lParam2 is a NEWCPLINFO that should be filled with
      // information about this applet before returning
      case CPL_NEWINQUIRE:
		  if(lParam1==0)
         {
            // Fill this struct in
			int            iApplet      = (int)lParam1;
			LPNEWCPLINFO   lpNewCPlInfo = (LPNEWCPLINFO)lParam2;

			lpNewCPlInfo->dwSize        = (DWORD)sizeof(NEWCPLINFO);
			lpNewCPlInfo->dwFlags       = 0;
			lpNewCPlInfo->dwHelpContext = 0;
			lpNewCPlInfo->lData         = IDI_ICON1;
			lpNewCPlInfo->hIcon         = LoadIcon(g_hModule,
										  (LPCTSTR)MAKEINTRESOURCE(IDI_ICON1));
			lpNewCPlInfo->szHelpFile[0] = '\0';

			LoadString(g_hModule,IDS_NAME,lpNewCPlInfo->szName,32);
			LoadString(g_hModule,IDS_DESC,lpNewCPlInfo->szInfo,64);
            
         }

         break;

      // This is sent whenever the user clicks an icon in Settings
      // for one of the applets supported by this DLL. lParam1
      // contains the number indicating which applet. Returns 0 if
      // applet was successfully launched; non-zero otherwise
      case CPL_DBLCLK:
			iApplet = (UINT)lParam1;
			PROCESS_INFORMATION pi;
			if ( CreateProcess(L"\\Windows\\FuncKeys.exe",L"",0,0,0,0,0,0,0,&pi) )
			{
			  CloseHandle(pi.hThread);
			  CloseHandle(pi.hProcess);
			}
			else{
			  return 1;
			}
         
         break;

      // Sent once per applet, before CPL_EXIT
      case CPL_STOP:
         break;

      // Sent once before the DLL is unloaded
      case CPL_EXIT:
         
         break;

      default:
         break;
   }

   return 0;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
    {
      g_hModule = (HMODULE) hModule;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
       break;
    }
    return TRUE;

}


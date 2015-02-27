//registerHotkeys.h
#pragma once
#ifndef _REGHOTKEYS_
#define _REGHOTKEYS_

#include <windows.h>
//#include <winuser.h>

int USED_IN_DLL = FALSE;

	typedef UINT (*PFN_RegisterHotKey) (HWND, int, UINT, UINT);
	typedef bool (*PFN_UnregisterFunc1) (UINT, int);

#define MOD_NONE 0
#define MOD_KEYUP		0x1000
#define MOD_WIN         0x0008

/*
#define WM_HOTKEY						0x0312

#define MOD_ALT         0x0001
#define MOD_CONTROL     0x0002
#define MOD_SHIFT       0x0004
#define MOD_WIN         0x0008
#define MOD_KEYUP		0x1000

[DllImport("coredll.dll")] 
protected static extern uint RegisterHotKey(IntPtr hWnd, int id, uint fsModifiers, uint vk); 

[DllImport("coredll.dll")] 
protected static extern bool UnregisterFunc1(uint fsModifiers, int id);
*/
	//RegisterHotKey
	PFN_RegisterHotKey RegisterHotKey1 = NULL;
	PFN_UnregisterFunc1 UnregisterFunc1 = NULL;
	HMODULE hCoreDll = NULL;

	int regHotKeys(HWND hWnd);

	DWORD vkValues[]={
		VK_LWIN,
		VK_F1,
		VK_F2,
		VK_F3,
		VK_F4,
		VK_F5,
		VK_F6,
		VK_F7,
		VK_F8,
		VK_F9,
		VK_F10,
		0
	};

	int loadFuncs(){
		if (hCoreDll==NULL){
			hCoreDll = LoadLibrary(L"CoreDLL.dll");
			if (hCoreDll == NULL)
				return -1;
		}
		if(UnregisterFunc1==NULL ){
			UnregisterFunc1 = (PFN_UnregisterFunc1) GetProcAddress(hCoreDll, L"UnregisterFunc1");
			if(UnregisterFunc1==NULL){
				FreeLibrary(hCoreDll);
				return -3;
			}
		}
		if(RegisterHotKey1==NULL ){
			RegisterHotKey1 = (PFN_RegisterHotKey) GetProcAddress(hCoreDll, L"RegisterHotKey");
			if(RegisterHotKey1==NULL){
				FreeLibrary(hCoreDll);
				return -2;
			}
		}
		return 0;
	}

int regKey(HWND hWnd, DWORD vk_val){
	bool bSuccess=false;
	UINT keyMod = MOD_KEYUP;

	if(loadFuncs()!=0)
		return -1;

	bSuccess=UnregisterFunc1(keyMod, vk_val); // 0x5B
	if(!bSuccess){
		DEBUGMSG(1, (L"UnregisterFunc1 failed for %04x (%i)\n", vk_val, vk_val));
	}
	else
		DEBUGMSG(1, (L"UnregisterFunc1 OK for %04x\n (%i)", vk_val, vk_val));

	uint result; 
	DWORD dwErr=0;

	DWORD vk_valID = vk_val;
	if(USED_IN_DLL)
		vk_valID+=0xC000;

	result = RegisterHotKey1(hWnd, vk_valID, MOD_NONE, vk_val); // 
	if(result == 0){
		dwErr = GetLastError();
		DEBUGMSG(1, (L"RegisterHotkey failed for %04x with GetLastError()=%i\n", vk_val, dwErr));
	}
	else
		DEBUGMSG(1, (L"RegisterHotkey OK for %04x \n", vk_val, dwErr));

	return result;
}

int unRegKey(DWORD vk_val)
{
	if(loadFuncs()!=0)
		return -1;
	bool bSuccess=false;
	UINT keyMod = MOD_KEYUP;

	bSuccess=UnregisterFunc1(keyMod, vk_val); // 0x5B
	FreeLibrary(hCoreDll);
	if(!bSuccess){
		DEBUGMSG(1, (L"UnregisterFunc1 failed for %04x\n", vk_val));
		return -1;
	}
	else{
		DEBUGMSG(1, (L"UnregisterFunc1 OK for %04x\n", vk_val));
		return 0;
	}
}

int unRegKeys(){
	if(loadFuncs()!=0)
		return -1;
	int iKey=0;
	int result=0;
	do{
		result = unRegKey(vkValues[iKey++]);
		//this will not restore the hotkeys for Phone (F3) / Hangup (F4), 
		//you have to kill and restart cprog.exe to let it reregister 'its' hotkeys
	}while (vkValues[iKey]!=0);
	//test to reregister F3/F4 to cprog
	//HWND hCprog = FindWindow(NULL, L"Phone");
	//if(hCprog!=NULL){
	//	regKey(hCprog, VK_F3);
	//	regKey(hCprog, VK_F4);
	//} // THIS did not work
	return 0;
}

int regHotKeys(HWND hWnd){
	if(loadFuncs()!=0)
		return -1;
	int iKey=0;
	int result=0;
	do{
		result = regKey(hWnd, vkValues[iKey++]);
	}while (vkValues[iKey]!=0);

	bool bSuccess=false;
	UINT keyMod = MOD_KEYUP;
	FreeLibrary(hCoreDll);
	return result;
}


#endif
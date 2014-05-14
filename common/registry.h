//registry.h

//global

const TCHAR* mainRegKey = L"SOFTWARE\\Intermec\\FuncKeys";

int readTimeOut(int* timeOut){
	HKEY hKey;
	//open reg
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, mainRegKey, 0, 0, &hKey)!=ERROR_SUCCESS){
		return -1;
	}
	DWORD dwVal=0;
	ULONG type = REG_DWORD;
	DWORD dwSize=sizeof(DWORD);
	if (RegQueryValueEx(hKey, L"TimeOut", 0, &type, (BYTE*)&dwVal, &dwSize)!=ERROR_SUCCESS){
		RegCloseKey(hKey);
		return -3;
	}
	RegCloseKey(hKey);
	*timeOut=dwVal;
	return 0;
}

int readReg(BYTE *b, ULONG *iCount){
	HKEY hKey;
	//open reg
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, mainRegKey, 0, 0, &hKey)!=ERROR_SUCCESS){
		b=NULL;
		*iCount = 0;
		return -1;
	}

	//query size of byte data 
	ULONG type = REG_BINARY;
	BYTE *buf = NULL;
	ULONG cnt = 0;
	if (RegQueryValueEx(hKey, L"KeyList", 0, &type, buf, &cnt)!=ERROR_SUCCESS){
		b=NULL;
		*iCount = 0;
		RegCloseKey(hKey);
		return -2;
	}
	//read data bytes
	buf = (BYTE*) malloc(cnt);
	if (RegQueryValueEx(hKey, L"KeyList", 0, &type, buf, &cnt)!=ERROR_SUCCESS){
		b=NULL;
		*iCount = 0;
		RegCloseKey(hKey);
		return -3;
	}
	memcpy(b, buf, cnt);
	//memcpy(iCount, cnt, sizeof(int));
	*iCount=cnt;
	RegCloseKey(hKey);
	return 0;
}

int writeReg(BYTE *b, int iCount){
	DEBUGMSG(1, (L"### entering writeReg\n"));
	HKEY hKey;
	//open reg
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, mainRegKey, 0, 0, &hKey)!=ERROR_SUCCESS){
		DEBUGMSG(1, (L" writeReg: error 0x%0x in RegOpenKeyEx\n", GetLastError()));
		//create key if not exists
		DWORD dwDisposition;
		if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, mainRegKey, 
			0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dwDisposition) != ERROR_SUCCESS){
			b=NULL;
			iCount = 0;
			return -1;
		}
	}
	ULONG type = REG_BINARY;
	if(iCount==0){
		DEBUGMSG(1, (L" writeReg: iCount is 0\n"));
		if(RegSetValueEx(hKey, L"KeyList", 0, type, NULL, 0)!=ERROR_SUCCESS){
			DEBUGMSG(1, (L" writeReg: error 0x%0x in RegSetValueEx\n", GetLastError()));
			RegCloseKey(hKey);
			return -2;
		}
	}else{
		if(RegSetValueEx(hKey, L"KeyList", 0, type, b, iCount)!=ERROR_SUCCESS){
			DEBUGMSG(1, (L" writeReg: error 0x%0x in RegSetValueEx\n", GetLastError()));
			RegCloseKey(hKey);
			return -2;
		}
	}
	/*
	type=REG_DWORD;
	DWORD dwVal=30;
	DWORD dwSize=sizeof(DWORD);
	if(RegSetValueEx(hKey, L"TimeOut", 0, type, (BYTE*)&dwVal, dwSize)!=ERROR_SUCCESS){
		DEBUGMSG(1, (L" writeReg: error 0x%0x in RegSetValueEx for TimeOut\n", GetLastError()));
	}
	else
		DEBUGMSG(1, (L" writeReg: success RegSetValueEx for TimeOut\n"));
	*/

	RegCloseKey(hKey);
	DEBUGMSG(1, (L"### leaving writeReg with no error\n"));
	return 0;
}

int writeTimeout(DWORD dwT){
	DEBUGMSG(1, (L"### entering writeTimeout\n"));
	HKEY hKey;
	//open reg
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, mainRegKey, 0, 0, &hKey)!=ERROR_SUCCESS){
		DEBUGMSG(1, (L" writeTimeout: error 0x%0x in RegOpenKeyEx\n", GetLastError()));
		//create key if not exists
		DWORD dwDisposition;
		if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, mainRegKey, 
			0, NULL, REG_OPTION_NON_VOLATILE, 0, NULL, &hKey, &dwDisposition) != ERROR_SUCCESS){
			return -1;
		}
	}
	ULONG type = REG_DWORD;

	type=REG_DWORD;
	DWORD dwVal=dwT;
	DWORD dwSize=sizeof(DWORD);
	if(RegSetValueEx(hKey, L"TimeOut", 0, type, (BYTE*)&dwVal, dwSize)!=ERROR_SUCCESS){
		DEBUGMSG(1, (L" writeTimeout: error 0x%0x in RegSetValueEx for TimeOut\n", GetLastError()));
		RegCloseKey(hKey);
		return -2;
	}
	else
		DEBUGMSG(1, (L" writeTimeout: success RegSetValueEx for TimeOut\n"));

	RegCloseKey(hKey);
	return 0;
}

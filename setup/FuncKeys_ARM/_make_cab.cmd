set RTDIR=D:\C-Source\Active\FuncKeys

copy %RTDIR%\FuncKeys\ARMV4Rel\FuncKeys.exe windows
copy %RTDIR%\FuncKeysCPL\ARMV4Rel\FuncKeysCPL.cpl windows
copy %RTDIR%\FuncKeysUnReg\ARMV4Rel\FuncKeysUnReg.exe windows\StartUp
copy %RTDIR%\FuncKeys.htm windows
copy %RTDIR%\FuncKeys0.jpg windows
copy %RTDIR%\FuncKeys1.jpg windows

pause

CabWiz "HGO FuncKeys.inf" /err error.txt

type error.txt

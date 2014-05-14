<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: FuncKeysUnreg - Win32 (WCE ARMV4I) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\Temp\RSP14B.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=500 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_CK60Prem" /D "ARMV4I" /D UNDER_CE=500 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4IRel/FuncKeysUnreg.pch" /Yu"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"D:\C-Source\Active\FuncKeys\FuncKeysUnreg\FuncKeysUnreg.cpp"
]
Creating command line "clarm.exe @C:\Temp\RSP14B.tmp" 
Creating temporary file "C:\Temp\RSP14C.tmp" with contents
[
commctrl.lib coredll.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"ARMV4IRel/FuncKeysUnreg.pdb" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib" /out:"ARMV4IRel/FuncKeysUnreg.exe" /subsystem:windowsce,5.00 /MACHINE:THUMB 
".\ARMV4IRel\FuncKeysUnreg.obj"
".\ARMV4IRel\StdAfx.obj"
".\ARMV4IRel\FuncKeysUnreg.res"
]
Creating command line "link.exe @C:\Temp\RSP14C.tmp"
<h3>Output Window</h3>
Compiling...
FuncKeysUnreg.cpp
d:\c-source\active\funckeys\funckeysunreg\funckeysunreg.cpp(4) : warning C4652: compiler option 'Generate Browser Info' inconsistent with precompiled header; current command-line option will override that defined in the precompiled header
Linking...




<h3>Results</h3>
FuncKeysUnreg.exe - 0 error(s), 1 warning(s)
</pre>
</body>
</html>

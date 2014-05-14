========================================================================
     FuncKeys, FuncKeysCPL, FuncKeysUnreg
========================================================================

	FuncKeys is part of a tool set to unregister functions keys
	on Windows Mobile devices.
	
	FuncKeys should be copied to \Windows. It is used to define 
	the function keys to be unregistered and a delay for 
	FuncKeysUnreg.
	
	FuncKeysCPL is a settings applet that does nothing else than
	show an icon in Start-Settings-System and launch 
	\Windows\FuncKeys if the icon is tapped.
		
	FuncKeysUnreg should be copied to \windows and a link be placed
	in \Windows\StartUp. When the OS starts or is rebooted
	FuncKeysUnreg will be started and unregisters the defined
	function keys after a defined delay. The delay is used to 
	ensure that all tools and processes that register function 
	keys are started before FuncKeysUnreg does unregister these.
	
	registry
	
	[HKLM]SOFTWARE\Intermec\FuncKeys
	TimeOut=DWORD:30		;delay in seconds on start for FuncKeysUnreg
	KeyList=BINARY			;list of vkey values to use for UnregisterFunc1
							;ie 70 71 will unregister VK_F1 and VK_F2
	

/////////////////////////////////////////////////////////////////////////

// FuncKeys.h : main header file for the FUNCKEYS application
//

#if !defined(AFX_FUNCKEYS_H__096FD008_62CE_4F68_A12E_2447469E31FD__INCLUDED_)
#define AFX_FUNCKEYS_H__096FD008_62CE_4F68_A12E_2447469E31FD__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFuncKeysApp:
// See FuncKeys.cpp for the implementation of this class
//

class CFuncKeysApp : public CWinApp
{
public:
	CFuncKeysApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFuncKeysApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFuncKeysApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNCKEYS_H__096FD008_62CE_4F68_A12E_2447469E31FD__INCLUDED_)

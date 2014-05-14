// FuncKeysDlg.h : header file
//

#include "afxwin.h"
#if !defined(AFX_FUNCKEYSDLG_H__9DC614B8_65B1_4B0E_8C70_B9E741F9BDA7__INCLUDED_)
#define AFX_FUNCKEYSDLG_H__9DC614B8_65B1_4B0E_8C70_B9E741F9BDA7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CFuncKeysDlg dialog

class CFuncKeysDlg : public CDialog
{
// Construction
public:
	UINT MAX_KEYCOUNT;
	void updateUI();
	CFuncKeysDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFuncKeysDlg)
	enum { IDD = IDD_FUNCKEYS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFuncKeysDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFuncKeysDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtntest();
	afx_msg void OnBtnclose();
	afx_msg void OnBtnsave();
	afx_msg void OnBtnapply();
	afx_msg void OnBtnhelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox listTimout;
	afx_msg void OnCbnSelchangeCombo1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNCKEYSDLG_H__9DC614B8_65B1_4B0E_8C70_B9E741F9BDA7__INCLUDED_)

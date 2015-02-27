// FuncKeysDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FuncKeys.h"
#include "FuncKeysDlg.h"

#include "..\common\registerhotkeys.h"
#include "..\common\registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFuncKeysDlg dialog

CFuncKeysDlg::CFuncKeysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFuncKeysDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFuncKeysDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFuncKeysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFuncKeysDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO1, listTimout);
}

BEGIN_MESSAGE_MAP(CFuncKeysDlg, CDialog)
	//{{AFX_MSG_MAP(CFuncKeysDlg)
	ON_BN_CLICKED(IDC_BTNTEST, OnBtntest)
	ON_BN_CLICKED(IDC_BTNCLOSE, OnBtnclose)
	ON_BN_CLICKED(IDC_BTNSAVE, OnBtnsave)
	ON_BN_CLICKED(IDC_BTNAPPLY, OnBtnapply)
	ON_BN_CLICKED(IDC_BTNHELP, OnBtnhelp)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CFuncKeysDlg::OnNMCustomdrawSlider1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CFuncKeysDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFuncKeysDlg message handlers

BOOL CFuncKeysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	// TODO: Add extra initialization here

	ModifyStyle(WS_SYSMENU, WS_MINIMIZEBOX); //remove sysmenu and add minimize
	ModifyStyleEx(0x80000000L, 0); //remove OK button
	ShowWindow(SW_SHOWMAXIMIZED);
	MAX_KEYCOUNT = 12;

	TCHAR str[6];
	for (int i=10; i<=60; i+=10){
		wsprintf(str, L"%i", i);
		listTimout.AddString(str);
	}
	listTimout.SetCurSel(2);

	updateUI();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CFuncKeysDlg::updateUI()
{
	int timeOut=30;
	if(readTimeOut(&timeOut)!=0)
		timeOut=30;

	ULONG size=MAX_KEYCOUNT;
	BYTE* b;
	b=new BYTE[MAX_KEYCOUNT];
	// read Values from registry into b[]
	readReg(b, &size);
	int idCheck;
	UINT x=0;
	//#define IDC_CHECK1     1070 //!! 0x70 = 112 ==>> 1112
	//#define IDC_CHECK2     1071 //
	//F3 is 0x72 , F4 is 0x73
	UpdateData(false);

	int max=listTimout.GetCount();
	int foundID=-1;
	TCHAR str[6];
	TCHAR num[6];
	wsprintf(num, L"%i", timeOut);	//to compare with list items

	for(x=0; x<max; x++){
		if(listTimout.GetLBText(x, str)!=CB_ERR){			
			if(wcscmp(str, num)==0){
				foundID=x;
				break;
			}
		}
	}
	if(foundID!=-1)
		listTimout.SetCurSel(foundID);

	for (x=0; x<MAX_KEYCOUNT; x++){
		idCheck=1000 + 112 + x;
		HWND hBut = ::GetDlgItem(m_hWnd, idCheck);
		::SendMessage(hBut, BM_SETCHECK, false, 0);
	}
	x=0; //reset to zero!
	while(x < size){
		idCheck=1000+b[x];
/*		
		CButton *pButton = (CButton *)GetDlgItem(idCheck);
		pButton->SetCheck(1);
*/		
		HWND hBut = ::GetDlgItem(m_hWnd, idCheck);
		::SendMessage(hBut, BM_SETCHECK, true, 0);
		x++;
	}
		UpdateData(true);
	delete(b);
}

void CFuncKeysDlg::OnBtntest() 
{
	// TODO: Add your control notification handler code here
	updateUI();
}

void CFuncKeysDlg::OnBtnclose() 
{
	// TODO: Add your control notification handler code here
	PostQuitMessage(0);
}

void CFuncKeysDlg::OnBtnsave() 
{
	// TODO: Add your control notification handler code here
	BYTE* b;
	ULONG size = MAX_KEYCOUNT;
	b=new BYTE[MAX_KEYCOUNT];
	UINT x=0;	//to iterate thru the check buttons
	UINT i=0;	//to interate thru the byte array
	int idCheck;
	BYTE bCount=0;
	UpdateData(true);
	long lRes=0;
	while(x < MAX_KEYCOUNT){	//0 -> idCheck=1112 -> FkeyValue=112
		idCheck=1000 + 112 + x;
		HWND hBut = ::GetDlgItem(m_hWnd, idCheck);
		lRes = ::SendMessage(hBut, BM_GETCHECK, true, 0);
		if(lRes){
			b[i]=idCheck-1000;
			bCount++;
			i++;
		}
		x++;
	}
	writeReg(b, bCount);
	
	//get current selection
	DWORD timeOut=30;
	TCHAR str[6];
	listTimout.GetLBText(listTimout.GetCurSel(), str);
	int t = _wtoi(str);
	if(t!=0)
		timeOut=t;
	//write value
	writeTimeout(timeOut);
}

void CFuncKeysDlg::OnBtnapply() 
{
	// TODO: Add your control notification handler code here
	OnBtnsave();
	MessageBox(L"Changes may need a warmboot before they are 'in Place' (needs FuncKeysUnreg in StartUp)", L"Unregister FuncKeys", MB_OK);
	BYTE* b;
	b = new BYTE[MAX_KEYCOUNT];
	ULONG size = MAX_KEYCOUNT;
	readReg(b, &size);
	UINT x=0;
	DWORD dwKey;
	while ( x < size ){
		dwKey=b[x];
		unRegKey(dwKey);
		x++;
	}

}

void CFuncKeysDlg::OnBtnhelp() 
{
	// TODO: Add your control notification handler code here
	HANDLE	hProc = NULL;
	SHELLEXECUTEINFO ExeInfo;
	ExeInfo.cbSize	=	sizeof(SHELLEXECUTEINFO);
	ExeInfo.fMask	=	0;
	ExeInfo.hwnd	=	NULL;
	ExeInfo.lpVerb	=	L"Open";
	ExeInfo.lpFile	=	L"\\Windows\\FuncKeys.htm";
	ExeInfo.lpParameters	=	NULL;
	ExeInfo.lpDirectory		=	0;
	ExeInfo.nShow	=	0;
	ExeInfo.hInstApp	=	NULL; 
	ExeInfo.hProcess	=	hProc;

	ShellExecuteEx(&ExeInfo);
}

void CFuncKeysDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CFuncKeysDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here

}

void CFuncKeysDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

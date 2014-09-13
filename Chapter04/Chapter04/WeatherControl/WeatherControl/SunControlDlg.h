#pragma once
#include "afxwin.h"


// CSunControlDlg dialog

class CSunControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSunControlDlg)

public:
	CSunControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSunControlDlg();

// Dialog Data
	enum { IDD = IDD_SUN_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Action;
	afx_msg void OnBnClickedSunAction();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

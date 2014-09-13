#pragma once
#include "afxwin.h"


// CSnowControlDlg dialog

class CSnowControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSnowControlDlg)

public:
	CSnowControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSnowControlDlg();

// Dialog Data
	enum { IDD = IDD_SNOW_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CButton m_Action;
	afx_msg void OnRainAction();
};

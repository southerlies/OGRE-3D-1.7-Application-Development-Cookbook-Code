#pragma once


// CFogControlDlg dialog

class CFogControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFogControlDlg)

public:
	CFogControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFogControlDlg();

// Dialog Data
	enum { IDD = IDD_FOG_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

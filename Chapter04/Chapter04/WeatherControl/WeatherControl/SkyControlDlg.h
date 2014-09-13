#pragma once


// CSkyControlDlg dialog

class CSkyControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSkyControlDlg)

public:
	CSkyControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSkyControlDlg();

// Dialog Data
	enum { IDD = IDD_SKY_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

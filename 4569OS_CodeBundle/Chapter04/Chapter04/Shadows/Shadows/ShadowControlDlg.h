#pragma once


// CShadowControlDlg dialog

class CShadowControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShadowControlDlg)

public:
	CShadowControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShadowControlDlg();

// Dialog Data
	enum { IDD = IDD_SHADOW_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

#pragma once


// CLightsControlDlg dialog

class CLightsControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLightsControlDlg)

public:
	CLightsControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightsControlDlg();

// Dialog Data
	enum { IDD = IDD_LIGHTS_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

#pragma once


// CWalkControlDlg dialog

class CWalkControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWalkControlDlg)

public:
	CWalkControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWalkControlDlg();

// Dialog Data
	enum { IDD = IDD_WALK_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

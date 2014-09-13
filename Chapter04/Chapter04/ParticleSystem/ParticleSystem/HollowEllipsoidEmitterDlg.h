#pragma once


// CHollowEllipsoidEmitterDlg dialog

class CHollowEllipsoidEmitterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHollowEllipsoidEmitterDlg)

public:
	CHollowEllipsoidEmitterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHollowEllipsoidEmitterDlg();

// Dialog Data
	enum { IDD = IDD_HOLLOW_ELLIPSOID_EMITTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

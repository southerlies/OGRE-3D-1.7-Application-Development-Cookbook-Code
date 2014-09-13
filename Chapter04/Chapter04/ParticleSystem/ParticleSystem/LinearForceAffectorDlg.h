#pragma once


// CLinearForceAffectorDlg dialog

class CLinearForceAffectorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLinearForceAffectorDlg)

public:
	CLinearForceAffectorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLinearForceAffectorDlg();

// Dialog Data
	enum { IDD = IDD_LINEAR_FORCE_AFFECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

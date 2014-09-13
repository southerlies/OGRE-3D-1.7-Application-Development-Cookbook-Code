#pragma once


// CRotatorAffectorDlg dialog

class CRotatorAffectorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRotatorAffectorDlg)

public:
	CRotatorAffectorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRotatorAffectorDlg();

// Dialog Data
	enum { IDD = IDD_ROTATOR_AFFECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

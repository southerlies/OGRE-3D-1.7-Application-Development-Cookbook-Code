#pragma once


// CDirectionRandomiserDlg dialog

class CDirectionRandomiserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDirectionRandomiserDlg)

public:
	CDirectionRandomiserDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDirectionRandomiserDlg();

// Dialog Data
	enum { IDD = IDD_DIRECTION_RANDOMISER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Randomness;
//	CString m_DirectionRandomiserName;
	CString m_AffectorName;
};

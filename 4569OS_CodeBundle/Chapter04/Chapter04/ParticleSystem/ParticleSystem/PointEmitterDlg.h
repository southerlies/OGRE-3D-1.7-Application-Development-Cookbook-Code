#pragma once
#include "afxwin.h"


// CPointEmitterDlg dialog

class CPointEmitterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPointEmitterDlg)

public:
	CPointEmitterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPointEmitterDlg();

// Dialog Data
	enum { IDD = IDD_POINT_EMITTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_EmitterName;
};

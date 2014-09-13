#pragma once


// CBoxEmitterDlg dialog

class CBoxEmitterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBoxEmitterDlg)

public:
	CBoxEmitterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBoxEmitterDlg();

// Dialog Data
	enum { IDD = IDD_BOX_EMITTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_EmitterName;
};

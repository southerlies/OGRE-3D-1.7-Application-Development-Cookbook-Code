#pragma once


// CRingEmitterDlg dialog

class CRingEmitterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRingEmitterDlg)

public:
	CRingEmitterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRingEmitterDlg();

// Dialog Data
	enum { IDD = IDD_RING_EMITTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_EmitterName;
};

#pragma once


// CEllipsoidEmitterDlg dialog

class CEllipsoidEmitterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEllipsoidEmitterDlg)

public:
	CEllipsoidEmitterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEllipsoidEmitterDlg();

// Dialog Data
	enum { IDD = IDD_ELLIPSOID_EMITTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_EmitterName;
};

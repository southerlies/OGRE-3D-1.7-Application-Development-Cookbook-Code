#pragma once


// CCylinderEmitterDlg dialog

class CCylinderEmitterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCylinderEmitterDlg)

public:
	CCylinderEmitterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCylinderEmitterDlg();

// Dialog Data
	enum { IDD = IDD_CYLINDER_EMITTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_EmitterName;
};

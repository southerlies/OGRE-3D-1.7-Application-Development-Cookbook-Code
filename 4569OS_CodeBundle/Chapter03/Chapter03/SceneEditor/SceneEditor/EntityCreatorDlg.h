#pragma once


// CEntityCreatorDlg dialog

class CEntityCreatorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEntityCreatorDlg)

public:
	CEntityCreatorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEntityCreatorDlg();

// Dialog Data
	enum { IDD = IDD_ENTITY_CREATOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_EntityName;
	CString m_MeshName;
};

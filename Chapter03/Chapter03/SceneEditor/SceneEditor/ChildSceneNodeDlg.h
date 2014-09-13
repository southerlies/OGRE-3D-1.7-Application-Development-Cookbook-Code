#pragma once
#include "afxwin.h"


// CChildSceneNodeDlg dialog

class CChildSceneNodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChildSceneNodeDlg)

public:
	CChildSceneNodeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChildSceneNodeDlg();

// Dialog Data
	enum { IDD = IDD_CHILD_SCENE_NODE_CREATOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CString m_NodeName;
};

// ChildSceneNodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "ChildSceneNodeDlg.h"
#include "afxdialogex.h"


// CChildSceneNodeDlg dialog

IMPLEMENT_DYNAMIC(CChildSceneNodeDlg, CDialogEx)

CChildSceneNodeDlg::CChildSceneNodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildSceneNodeDlg::IDD, pParent)
{

	m_NodeName = _T("");
}

CChildSceneNodeDlg::~CChildSceneNodeDlg()
{
}

void CChildSceneNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NODE_NAME, m_NodeName);
}


BEGIN_MESSAGE_MAP(CChildSceneNodeDlg, CDialogEx)
END_MESSAGE_MAP()


// CChildSceneNodeDlg message handlers

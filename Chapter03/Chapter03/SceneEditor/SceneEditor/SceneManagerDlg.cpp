// SceneManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "SceneManagerDlg.h"
#include "afxdialogex.h"


// CSceneManagerDlg dialog

IMPLEMENT_DYNAMIC(CSceneManagerDlg, CDialogEx)

CSceneManagerDlg::CSceneManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSceneManagerDlg::IDD, pParent)
{

}

CSceneManagerDlg::~CSceneManagerDlg()
{
}

void CSceneManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCENE_TREE, m_SceneTree);
}


BEGIN_MESSAGE_MAP(CSceneManagerDlg, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SCENE_TREE, &CSceneManagerDlg::OnSelchangedSceneTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_SCENE_TREE, &CSceneManagerDlg::OnBeginlabeleditSceneTree)
	ON_NOTIFY(TVN_DELETEITEM, IDC_SCENE_TREE, &CSceneManagerDlg::OnDeleteitemSceneTree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_SCENE_TREE, &CSceneManagerDlg::OnEndlabeleditSceneTree)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_SCENE_TREE, &CSceneManagerDlg::OnItemexpandedSceneTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_SCENE_TREE, &CSceneManagerDlg::OnItemexpandingSceneTree)
	ON_NOTIFY(TVN_KEYDOWN, IDC_SCENE_TREE, &CSceneManagerDlg::OnKeydownSceneTree)
	ON_NOTIFY(TVN_SELCHANGING, IDC_SCENE_TREE, &CSceneManagerDlg::OnSelchangingSceneTree)
	ON_NOTIFY(TVN_SINGLEEXPAND, IDC_SCENE_TREE, &CSceneManagerDlg::OnSingleexpandSceneTree)
	ON_NOTIFY(TVN_ITEMCHANGED, IDC_SCENE_TREE, &CSceneManagerDlg::OnItemchangedSceneTree)
	ON_NOTIFY(TVN_ITEMCHANGING, IDC_SCENE_TREE, &CSceneManagerDlg::OnItemchangingSceneTree)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CSceneManagerDlg message handlers


void CSceneManagerDlg::OnSelchangedSceneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CSceneManagerDlg::OnBeginlabeleditSceneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CSceneManagerDlg::OnDeleteitemSceneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CSceneManagerDlg::OnEndlabeleditSceneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CSceneManagerDlg::OnItemexpandedSceneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CSceneManagerDlg::OnItemexpandingSceneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CSceneManagerDlg::OnKeydownSceneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CSceneManagerDlg::OnSelchangingSceneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CSceneManagerDlg::OnSingleexpandSceneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CSceneManagerDlg::OnItemchangedSceneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CSceneManagerDlg::OnItemchangingSceneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE *>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CSceneManagerDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnRButtonDown(nFlags, point);
}


BOOL CSceneManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HTREEITEM Scene = m_SceneTree.InsertItem("Scene", NULL, NULL);
	m_SceneTree.SetCheck(Scene, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

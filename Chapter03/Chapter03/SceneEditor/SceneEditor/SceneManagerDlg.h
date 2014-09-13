#pragma once
#include "afxcmn.h"
#include "SceneTreeCtrl.h"


// CSceneManagerDlg dialog

class CSceneManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSceneManagerDlg)

public:
	CSceneManagerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSceneManagerDlg();

// Dialog Data
	enum { IDD = IDD_SCENE_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSceneTreeCtrl m_SceneTree;
	afx_msg void OnSelchangedSceneTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBeginlabeleditSceneTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeleteitemSceneTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEndlabeleditSceneTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemexpandedSceneTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemexpandingSceneTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeydownSceneTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangingSceneTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSingleexpandSceneTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedSceneTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangingSceneTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
};

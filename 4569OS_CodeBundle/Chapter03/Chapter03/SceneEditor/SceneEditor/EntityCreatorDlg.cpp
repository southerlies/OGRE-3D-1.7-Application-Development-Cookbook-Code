// EntityCreatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEditor.h"
#include "EntityCreatorDlg.h"
#include "afxdialogex.h"


// CEntityCreatorDlg dialog

IMPLEMENT_DYNAMIC(CEntityCreatorDlg, CDialogEx)

CEntityCreatorDlg::CEntityCreatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEntityCreatorDlg::IDD, pParent)
	, m_EntityName(_T(""))
	, m_MeshName(_T(""))
{
}


CEntityCreatorDlg::~CEntityCreatorDlg()
{
}

void CEntityCreatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ENTITY_NAME, m_EntityName);
	DDX_Text(pDX, IDC_MESH_NAME, m_MeshName);

}


BEGIN_MESSAGE_MAP(CEntityCreatorDlg, CDialogEx)
END_MESSAGE_MAP()


// CEntityCreatorDlg message handlers

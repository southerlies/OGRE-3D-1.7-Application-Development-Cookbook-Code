// PointEmitterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ParticleSystem.h"
#include "PointEmitterDlg.h"
#include "afxdialogex.h"


// CPointEmitterDlg dialog

IMPLEMENT_DYNAMIC(CPointEmitterDlg, CDialogEx)

CPointEmitterDlg::CPointEmitterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPointEmitterDlg::IDD, pParent)
{

}

CPointEmitterDlg::~CPointEmitterDlg()
{
}

void CPointEmitterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POINT_EMITTER_NAME, m_EmitterName);
}


BEGIN_MESSAGE_MAP(CPointEmitterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPointEmitterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPointEmitterDlg message handlers


void CPointEmitterDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

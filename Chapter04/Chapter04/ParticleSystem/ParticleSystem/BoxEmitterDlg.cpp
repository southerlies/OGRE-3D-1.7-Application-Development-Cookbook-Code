// BoxEmitterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ParticleSystem.h"
#include "BoxEmitterDlg.h"
#include "afxdialogex.h"


// CBoxEmitterDlg dialog

IMPLEMENT_DYNAMIC(CBoxEmitterDlg, CDialogEx)

CBoxEmitterDlg::CBoxEmitterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBoxEmitterDlg::IDD, pParent)
	, m_EmitterName(_T(""))
{

}

CBoxEmitterDlg::~CBoxEmitterDlg()
{
}

void CBoxEmitterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BOX_EMITTER_NAME, m_EmitterName);
}


BEGIN_MESSAGE_MAP(CBoxEmitterDlg, CDialogEx)
END_MESSAGE_MAP()


// CBoxEmitterDlg message handlers

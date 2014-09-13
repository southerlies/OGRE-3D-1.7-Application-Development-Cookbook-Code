// CylinderEmitterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ParticleSystem.h"
#include "CylinderEmitterDlg.h"
#include "afxdialogex.h"


// CCylinderEmitterDlg dialog

IMPLEMENT_DYNAMIC(CCylinderEmitterDlg, CDialogEx)

CCylinderEmitterDlg::CCylinderEmitterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCylinderEmitterDlg::IDD, pParent)
	, m_EmitterName(_T(""))
{

}

CCylinderEmitterDlg::~CCylinderEmitterDlg()
{
}

void CCylinderEmitterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCylinderEmitterDlg, CDialogEx)
END_MESSAGE_MAP()


// CCylinderEmitterDlg message handlers

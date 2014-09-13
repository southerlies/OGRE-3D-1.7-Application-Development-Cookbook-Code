// RingEmitterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ParticleSystem.h"
#include "RingEmitterDlg.h"
#include "afxdialogex.h"


// CRingEmitterDlg dialog

IMPLEMENT_DYNAMIC(CRingEmitterDlg, CDialogEx)

CRingEmitterDlg::CRingEmitterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRingEmitterDlg::IDD, pParent)
	, m_EmitterName(_T(""))
{

}

CRingEmitterDlg::~CRingEmitterDlg()
{
}

void CRingEmitterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRingEmitterDlg, CDialogEx)
END_MESSAGE_MAP()


// CRingEmitterDlg message handlers

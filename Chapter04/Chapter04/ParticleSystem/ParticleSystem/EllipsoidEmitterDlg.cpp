// EllipsoidEmitterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ParticleSystem.h"
#include "EllipsoidEmitterDlg.h"
#include "afxdialogex.h"


// CEllipsoidEmitterDlg dialog

IMPLEMENT_DYNAMIC(CEllipsoidEmitterDlg, CDialogEx)

CEllipsoidEmitterDlg::CEllipsoidEmitterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEllipsoidEmitterDlg::IDD, pParent)
	, m_EmitterName(_T(""))
{

}

CEllipsoidEmitterDlg::~CEllipsoidEmitterDlg()
{
}

void CEllipsoidEmitterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEllipsoidEmitterDlg, CDialogEx)
END_MESSAGE_MAP()


// CEllipsoidEmitterDlg message handlers

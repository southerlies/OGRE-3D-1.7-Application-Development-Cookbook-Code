// ShadowControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Shadows.h"
#include "ShadowControlDlg.h"
#include "afxdialogex.h"


// CShadowControlDlg dialog

IMPLEMENT_DYNAMIC(CShadowControlDlg, CDialogEx)

CShadowControlDlg::CShadowControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShadowControlDlg::IDD, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CShadowControlDlg::~CShadowControlDlg()
{
}

void CShadowControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShadowControlDlg, CDialogEx)
END_MESSAGE_MAP()


// CShadowControlDlg message handlers


BOOL CShadowControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

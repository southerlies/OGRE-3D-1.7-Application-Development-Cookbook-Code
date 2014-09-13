// LightsControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lights.h"
#include "LightsControlDlg.h"
#include "afxdialogex.h"


// CLightsControlDlg dialog

IMPLEMENT_DYNAMIC(CLightsControlDlg, CDialogEx)

CLightsControlDlg::CLightsControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLightsControlDlg::IDD, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CLightsControlDlg::~CLightsControlDlg()
{
}

void CLightsControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLightsControlDlg, CDialogEx)
END_MESSAGE_MAP()


// CLightsControlDlg message handlers


BOOL CLightsControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

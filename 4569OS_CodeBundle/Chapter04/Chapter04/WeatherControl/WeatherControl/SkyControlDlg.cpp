// SkyControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WeatherControl.h"
#include "SkyControlDlg.h"
#include "afxdialogex.h"


// CSkyControlDlg dialog

IMPLEMENT_DYNAMIC(CSkyControlDlg, CDialogEx)

CSkyControlDlg::CSkyControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSkyControlDlg::IDD, pParent)
{

}

CSkyControlDlg::~CSkyControlDlg()
{
}

void CSkyControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSkyControlDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CSkyControlDlg message handlers


BOOL CSkyControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSkyControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

// FogControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WeatherControl.h"
#include "FogControlDlg.h"
#include "afxdialogex.h"


// CFogControlDlg dialog

IMPLEMENT_DYNAMIC(CFogControlDlg, CDialogEx)

CFogControlDlg::CFogControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFogControlDlg::IDD, pParent)
{

}

CFogControlDlg::~CFogControlDlg()
{
}

void CFogControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFogControlDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CFogControlDlg message handlers


BOOL CFogControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CFogControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

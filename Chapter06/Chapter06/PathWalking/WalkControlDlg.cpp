// WalkControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PathWalking.h"
#include "WalkControlDlg.h"
#include "afxdialogex.h"


// CWalkControlDlg dialog

IMPLEMENT_DYNAMIC(CWalkControlDlg, CDialogEx)

CWalkControlDlg::CWalkControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWalkControlDlg::IDD, pParent)
{

}

CWalkControlDlg::~CWalkControlDlg()
{
}

void CWalkControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWalkControlDlg, CDialogEx)
END_MESSAGE_MAP()


// CWalkControlDlg message handlers

// DirectionRandomiserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ParticleSystem.h"
#include "DirectionRandomiserDlg.h"
#include "afxdialogex.h"


// CDirectionRandomiserDlg dialog

IMPLEMENT_DYNAMIC(CDirectionRandomiserDlg, CDialogEx)

CDirectionRandomiserDlg::CDirectionRandomiserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDirectionRandomiserDlg::IDD, pParent)
	, m_Randomness(_T(""))
{

	m_AffectorName = _T("");
}

CDirectionRandomiserDlg::~CDirectionRandomiserDlg()
{
}

void CDirectionRandomiserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RANDOMNESS, m_Randomness);
	DDX_Text(pDX, IDC_DIRECTION_RANDOMISER_NAME, m_AffectorName);
}


BEGIN_MESSAGE_MAP(CDirectionRandomiserDlg, CDialogEx)
END_MESSAGE_MAP()


// CDirectionRandomiserDlg message handlers

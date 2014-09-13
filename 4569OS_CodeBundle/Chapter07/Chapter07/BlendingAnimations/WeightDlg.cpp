// WeightDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BlendingAnimations.h"
#include "WeightDlg.h"
#include "afxdialogex.h"


// CWeightDlg dialog

IMPLEMENT_DYNAMIC(CWeightDlg, CDialogEx)

CWeightDlg::CWeightDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWeightDlg::IDD, pParent)
	, m_IsAverage(true)
	, m_IsCumulative(false)
{

}

CWeightDlg::~CWeightDlg()
{
}

void CWeightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WALK, m_WalkWeight);
	DDX_Control(pDX, IDC_SLUMP, m_SlumpWeight);
}


BEGIN_MESSAGE_MAP(CWeightDlg, CDialogEx)
	ON_BN_CLICKED(IDC_AVERAGE, &CWeightDlg::OnBnClickedAverage)
	ON_BN_CLICKED(IDC_CUMULATIVE, &CWeightDlg::OnBnClickedCumulative)
END_MESSAGE_MAP()


// CWeightDlg message handlers


BOOL CWeightDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_IsAverage = true;
	m_IsCumulative = false;

	m_WalkWeight.SetRange(0, 50);
	m_SlumpWeight.SetRange(0, 50);
	
	m_WalkWeight.SetPos(10);
	m_SlumpWeight.SetPos(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CWeightDlg::OnBnClickedAverage()
{
	m_IsAverage = true;
	m_IsCumulative = false;
}


void CWeightDlg::OnBnClickedCumulative()
{
	m_IsAverage = false;
	m_IsCumulative = true;
}

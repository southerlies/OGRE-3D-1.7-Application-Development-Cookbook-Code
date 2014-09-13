// DeflectorPlaneAffectorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ParticleSystem.h"
#include "DeflectorPlaneAffectorDlg.h"
#include "afxdialogex.h"


// CDeflectorPlaneAffectorDlg dialog

IMPLEMENT_DYNAMIC(CDeflectorPlaneAffectorDlg, CDialogEx)

CDeflectorPlaneAffectorDlg::CDeflectorPlaneAffectorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeflectorPlaneAffectorDlg::IDD, pParent)
	, m_AffectorName(_T(""))
{

	m_Bounce = _T("");
	m_AffectorName = _T("");
	m_Nx = _T("");
	m_Ny = _T("");
	m_Nz = _T("");
	m_x = _T("");
	m_y = _T("");
	m_z = _T("");
}

CDeflectorPlaneAffectorDlg::~CDeflectorPlaneAffectorDlg()
{
}

void CDeflectorPlaneAffectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BOUNCE, m_Bounce);
	DDX_Text(pDX, IDC_DEFLECTOR_PLANE_NAME, m_AffectorName);
	DDX_Text(pDX, IDC_NX, m_Nx);
	DDX_Text(pDX, IDC_NY, m_Ny);
	DDX_Text(pDX, IDC_NZ, m_Nz);
	DDX_Text(pDX, IDC_X, m_x);
	DDX_Text(pDX, IDC_Y, m_y);
	DDX_Text(pDX, IDC_Z, m_z);
}


BEGIN_MESSAGE_MAP(CDeflectorPlaneAffectorDlg, CDialogEx)
END_MESSAGE_MAP()


// CDeflectorPlaneAffectorDlg message handlers

#pragma once


// CDeflectorPlaneAffectorDlg dialog

class CDeflectorPlaneAffectorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeflectorPlaneAffectorDlg)

public:
	CDeflectorPlaneAffectorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeflectorPlaneAffectorDlg();

// Dialog Data
	enum { IDD = IDD_DEFLECTOR_PLANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	CString m_AffectorName;
	CString m_Bounce;
	CString m_AffectorName;
	CString m_Nx;
	CString m_Ny;
	CString m_Nz;
	CString m_x;
	CString m_y;
	CString m_z;
};

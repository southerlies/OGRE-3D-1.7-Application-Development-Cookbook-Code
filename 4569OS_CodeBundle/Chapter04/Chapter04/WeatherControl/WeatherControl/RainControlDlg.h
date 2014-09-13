#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CRainControlDlg dialog

class CRainControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRainControlDlg)

public:
	CRainControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRainControlDlg();

// Dialog Data
	enum { IDD = IDD_RAIN_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	CButton m_Action;
	afx_msg void OnClickedAction();
	CButton m_Action;
	CSliderCtrl m_ParticleWidth;
	CSliderCtrl m_ParticleHeight;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	bool m_PlaySound;
	afx_msg void OnBnClickedPlaySound();
};

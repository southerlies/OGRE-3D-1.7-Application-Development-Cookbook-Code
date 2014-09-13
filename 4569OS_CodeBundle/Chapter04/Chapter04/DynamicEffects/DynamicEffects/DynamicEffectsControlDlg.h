#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDynamicEffectsControlDlg dialog

class CDynamicEffectsControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDynamicEffectsControlDlg)

public:
	CDynamicEffectsControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDynamicEffectsControlDlg();

// Dialog Data
	enum { IDD = IDD_DYNAMIC_EFFECTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_Action;
	afx_msg void OnBnClickedAction();
	CSliderCtrl m_Angle;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_Intensity;
	CButton m_TV;
	afx_msg void OnBnClickedTv();
};

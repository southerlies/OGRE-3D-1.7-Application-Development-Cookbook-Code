#pragma once
#include "afxcmn.h"


// CWeightDlg dialog

class CWeightDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWeightDlg)

public:
	CWeightDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWeightDlg();

// Dialog Data
	enum { IDD = IDD_WEIGHT_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	bool m_IsAverage;
	bool m_IsCumulative;
	CSliderCtrl m_WalkWeight;
	CSliderCtrl m_SlumpWeight;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAverage();
	afx_msg void OnBnClickedCumulative();
};

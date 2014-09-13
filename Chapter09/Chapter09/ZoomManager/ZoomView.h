#if !defined(AFX_ZOOMVIEW_H__461510AD_E0E4_4CB6_B4B7_3E186AC2B08C__INCLUDED_)
#define AFX_ZOOMVIEW_H__461510AD_E0E4_4CB6_B4B7_3E186AC2B08C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZoomView.h : header file
//


class CZoomView : public CScrollView
{
protected:
	CZoomView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CZoomView)

// Attributes
public:
	// Flicker free attributes
	CDC*		m_pdcMemory;
	CBitmap*	m_pBitmap;
	BOOL		m_bInitialSize;

protected:
	// Zooom attributes
	float		m_zoomFactor;
	float		m_minZoomFactor;
	float		m_maxZoomFactor;
	CSize		m_line;
	CSize		m_page;
	CSize		m_layout;
	CSize		m_displaySize;

// Operations
public:
	// Display Operation /////////////////////////////////////////////////////
	int  SetMapMode(CDC* pDC);
	void SetDisplaySize(void);
	void SetZoomScale(float fScale);
	void SetZoomArea(CRect zoomRect);
	CPoint GetCenterScrollPosition(void);
	void LPtoDP(LPPOINT lpPoint);
	void LPtoDP(LPRECT lpRect);
	void LPtoDP(LPSIZE lpSize);
	void DPtoLP(LPPOINT lpPoint);
	void DPtoLP(LPRECT lpRect);
	void DPtoLP(LPSIZE lpSize);
	CSize GetDisplaySize()	{return m_displaySize;};	
	//////////////////////////////////////////////////////////////////////////

	int FloatToInt(float fNumber);

protected:
	void SetZoomFactor(float fZoomFactor);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoomView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CZoomView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CZoomView)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOOMVIEW_H__461510AD_E0E4_4CB6_B4B7_3E186AC2B08C__INCLUDED_)

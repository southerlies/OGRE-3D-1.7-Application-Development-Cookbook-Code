#pragma once
#include "particlesystemtree.h"

// CParticleSystemControlDlg dialog

class CParticleSystemControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParticleSystemControlDlg)

public:
	CParticleSystemControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParticleSystemControlDlg();

// Dialog Data
	enum { IDD = IDD_PARTICLE_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CParticleSystemTree m_ParticleSystemTree;
	HTREEITEM m_EmittersItem;
	HTREEITEM m_AffectorsItem;

	Ogre::ParticleSystem *m_ParticleSystem;

	afx_msg void OnEmittersAddPointEmitter();
	afx_msg void OnEmittersAddBoxEmitter();
	afx_msg void OnEmittersAddCylinderEmitter();
	afx_msg void OnEmittersAddellipsoidemitter();
	afx_msg void OnEmittersAddhollowellipsoidemitter();
	afx_msg void OnEmittersAddringemitter();
	afx_msg void OnAffectorsAddlinearforceaffector();
	afx_msg void OnAffectorsAddrotatoraffector();
	afx_msg void OnAffectorsAdddeflectorplaneaffector();
	afx_msg void OnAffectorsAddcolourfaderaffector();
	afx_msg void OnBnClickedCreateParticleSystem();
	afx_msg void OnBnClickedGetParticleSystem();
	afx_msg void OnBnClickedTryParticleSystem();
	afx_msg void OnAffectorsAdddirectionrandomiseraffector();
};

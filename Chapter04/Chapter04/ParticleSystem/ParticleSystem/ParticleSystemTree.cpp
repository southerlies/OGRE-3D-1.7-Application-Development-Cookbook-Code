// ParticleSystemTree.cpp : implementation file
//

#include "stdafx.h"
#include "ParticleSystem.h"
#include "ParticleSystemTree.h"


// CParticleSystemTree

IMPLEMENT_DYNAMIC(CParticleSystemTree, CTreeCtrl)

CParticleSystemTree::CParticleSystemTree()
{

}

CParticleSystemTree::~CParticleSystemTree()
{
}


BEGIN_MESSAGE_MAP(CParticleSystemTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, &CParticleSystemTree::OnTvnDeleteitem)
	ON_NOTIFY_REFLECT(TVN_ITEMCHANGED, &CParticleSystemTree::OnTvnItemChanged)
	ON_NOTIFY_REFLECT(TVN_ITEMCHANGING, &CParticleSystemTree::OnTvnItemChanging)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &CParticleSystemTree::OnTvnItemexpanded)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, &CParticleSystemTree::OnTvnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CParticleSystemTree::OnTvnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, &CParticleSystemTree::OnTvnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CParticleSystemTree::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, &CParticleSystemTree::OnTvnSelchanging)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CParticleSystemTree message handlers




void CParticleSystemTree::OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CParticleSystemTree::OnTvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CParticleSystemTree::OnTvnItemChanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVITEMCHANGE *pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE *>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CParticleSystemTree::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CParticleSystemTree::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CParticleSystemTree::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CParticleSystemTree::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CParticleSystemTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CParticleSystemTree::OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CParticleSystemTree::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnRButtonDown(nFlags, point);
}

#include "stdafx.h"
#include "FMonitor2.h"
#include "CategoryView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCategoryView, CTreeView)

BEGIN_MESSAGE_MAP(CCategoryView, CTreeView)
END_MESSAGE_MAP()

CCategoryView::CCategoryView()
{
}

CCategoryView::~CCategoryView()
{
}

#ifdef _DEBUG
void CCategoryView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CCategoryView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif

void CCategoryView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	GetTreeCtrl().InsertItem(_T("Root"));
}

void CCategoryView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
}

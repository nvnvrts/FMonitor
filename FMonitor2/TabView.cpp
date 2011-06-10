#include "stdafx.h"
#include "FMonitor2.h"
#include "DataTreeView.h"
#include "ConfigTreeView.h"
#include "TabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CTabView, CView)

BEGIN_MESSAGE_MAP(CTabView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CTabView::CTabView()
{
}

CTabView::~CTabView()
{
}

void CTabView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

#ifdef _DEBUG
void CTabView::AssertValid() const
{
	CView::AssertValid();
}

void CTabView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif

void CTabView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle)
{
	CCreateContext context;
	context.m_pCurrentDoc = GetDocument();
	context.m_pNewViewClass = pViewClass;
	context.m_pNewDocTemplate = GetDocument()->GetDocTemplate();

	CWnd* pWnd = (CWnd*)(pViewClass->CreateObject());
	if (pWnd)
	{
		CRect rect(0, 0, 0, 0);

		if (pWnd->Create(NULL, 
			             NULL,
						 WS_CHILD | WS_VISIBLE | WS_BORDER,
						 rect,
						 &m_tab,
						 0,
						 &context) == TRUE)
		{
			int nTab = m_tab.GetItemCount();

			m_tab.InsertItem(nTab, lpszTitle, pWnd->GetSafeHwnd(), 0);
		
			pWnd->SetOwner(this);
		}
	}
}

int CTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_tab.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		         CRect(0, 0, 0, 0),
				 this,
				 ID_TAB_CONTROL);

	//m_tab.GetPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2003);
	m_tab.GetPaintManager()->m_bHotTracking = TRUE;
	//m_tab.GetPaintManager()->m_bShowIcons = TRUE;
	m_tab.GetPaintManager()->DisableLunaColors(FALSE);

	AddView(RUNTIME_CLASS(CDataTreeView), _T("Data"));
	AddView(RUNTIME_CLASS(CConfigTreeView), _T("Config"));

	return 0;
}

void CTabView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_tab.MoveWindow(0, 0, cx, cy);
}

void CTabView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

void CTabView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
}

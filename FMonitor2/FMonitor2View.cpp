#include "stdafx.h"
#include "FMonitor2.h"
#include "FMonitor2Doc.h"
#include "FMonitor2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CFMonitor2View, CScrollView)

BEGIN_MESSAGE_MAP(CFMonitor2View, CScrollView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CFMonitor2View::CFMonitor2View()
{
	SetScrollSizes(MM_TEXT, CSize(0, 0));
}

CFMonitor2View::~CFMonitor2View()
{
}

#ifdef _DEBUG
void CFMonitor2View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFMonitor2View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFMonitor2Doc* CFMonitor2View::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFMonitor2Doc)));
	return (CFMonitor2Doc*)m_pDocument;
}
#endif

BOOL CFMonitor2View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

void CFMonitor2View::OnDraw(CDC* /*pDC*/)
{
	CFMonitor2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}
}

BOOL CFMonitor2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CFMonitor2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CFMonitor2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CFMonitor2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
}

void CFMonitor2View::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CScrollView::OnUpdate(pSender, lHint, pHint);
}

int CFMonitor2View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	return 0;
}

void CFMonitor2View::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
}

#include "stdafx.h"
#include <boost/foreach.hpp>
#include "FMonitor2.h"
#include "FMonitor2Doc.h"
#include "ConfigTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CConfigTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CConfigTreeView, CTreeView)
END_MESSAGE_MAP()

CConfigTreeView::CConfigTreeView()
{
}

CConfigTreeView::~CConfigTreeView()
{
}

#ifdef _DEBUG
void CConfigTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CConfigTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif

BOOL CConfigTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	return CTreeView::PreCreateWindow(cs);
}

void CConfigTreeView::AddTable(HTREEITEM parent, fmlog::Table* table)
{
	if (table)
	{
		BOOST_FOREACH(const fmlog::Table::ValueHash::value_type& v, table->values)
		{
			CString str;
			str.Format("%s : %s", v.first.c_str(), v.second.str.c_str());

			GetTreeCtrl().InsertItem(str, parent);
		}

		BOOST_FOREACH(const fmlog::Table::TableHash::value_type& v, table->tables)
		{
			CString str;
			str.Format("%s", v.first.c_str());

			HTREEITEM item = GetTreeCtrl().InsertItem(str, parent);

			AddTable(item, v.second);
		}

		GetTreeCtrl().SortChildren(parent);
	}
}

void CConfigTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetDocument());
	CFMLogData* data = doc->GetData();

	HTREEITEM root = GetTreeCtrl().InsertItem(_T("Config"));
	AddTable(root, data->GetConfig());

	GetTreeCtrl().SortChildren(root);
	GetTreeCtrl().Expand(root, TVE_EXPAND);
}

void CConfigTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	TRACE2("config tree view update... (%u)(%u)\n", lHint, (UINT)(pHint));
}

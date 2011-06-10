#include "stdafx.h"
#include <boost/foreach.hpp>
#include "FMonitor2.h"
#include "FMonitor2Doc.h"
#include "DataTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CDataTreeView, CXTPTreeView)

BEGIN_MESSAGE_MAP(CDataTreeView, CXTPTreeView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CDataTreeView::OnNMDblclk)
END_MESSAGE_MAP()

CDataTreeView::CDataTreeView()
{
}

CDataTreeView::~CDataTreeView()
{
}

#ifdef _DEBUG
void CDataTreeView::AssertValid() const
{
	CXTPTreeView::AssertValid();
}

void CDataTreeView::Dump(CDumpContext& dc) const
{
	CXTPTreeView::Dump(dc);
}
#endif

void CDataTreeView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	if (item != NULL)
	{
		int id = (int)(GetTreeCtrl().GetItemData(item));
		if (id)
		{
			int index = 0;

			HTREEITEM parent = GetTreeCtrl().GetParentItem(item);
			if (parent)
			{
				HTREEITEM child = GetTreeCtrl().GetChildItem(parent);
				while (child != NULL)
				{
					index++;
					child = GetTreeCtrl().GetNextItem(child, TVGN_NEXT);
				}
			}

			CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetDocument());

			CFMonitor2Doc::Hint hint;
			hint.id = id;

			doc->UpdateAllViews(this, CFMonitor2Doc::UPDATE_DATA_SELECTED, (CObject*)(&hint));
		}
	}

	*pResult = 0;
}

BOOL CDataTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	return CXTPTreeView::PreCreateWindow(cs);
}

void CDataTreeView::OnInitialUpdate()
{
	CXTPTreeView::OnInitialUpdate();

	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetDocument());
	CFMLogData* data = doc->GetData();

	HTREEITEM root = GetTreeCtrl().InsertItem(_T("Data"));

	const CFMLogData::KeyMap& keys = data->GetKeys();

	BOOST_FOREACH(const CFMLogData::KeyMap::value_type& v, keys)
	{
		HTREEITEM parent = GetTreeCtrl().InsertItem(v.first.c_str(), root);

		BOOST_FOREACH(const CFMLogData::IdxMap::value_type& w, v.second)
		{
			fmlog::List* list = data->GetList(w.second);

			CString str;
			str.Format("%s <%.1f><%.1f>", w.first.c_str(), list->GetAvg(), list->GetMax());

			HTREEITEM item = GetTreeCtrl().InsertItem(str, parent);
			GetTreeCtrl().SetItemData(item, w.second);

			m_items.insert(ItemMap::value_type(w.second, item));
		}

		GetTreeCtrl().SortChildren(parent);
	}

	GetTreeCtrl().SortChildren(root);
}

void CDataTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	TRACE2("data tree view update... (%u)(%u)\n", lHint, (ULONG_PTR)(pHint));

	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetDocument());
	CFMonitor2Doc::Hint* hint = (CFMonitor2Doc::Hint*)(pHint);

	switch (lHint)
	{
	case CFMonitor2Doc::UPDATE_ITEM_COLOR:
		{
			HTREEITEM item = m_items[hint->id];
			SetItemColor(item, hint->color);
		}
		break;

	default:
		break;
	}
}

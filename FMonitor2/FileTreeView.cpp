#include "stdafx.h"
#include <string>
#include <vector>
#include <boost/foreach.hpp>
#include "FMonitor2.h"
#include "FMonitor2Doc.h"
#include "FileTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace boost;

IMPLEMENT_DYNCREATE(CFileTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CFileTreeView, CTreeView)
END_MESSAGE_MAP()

CFileTreeView::CFileTreeView()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 13;
	strcpy(lf.lfFaceName, "Tahoma");

	VERIFY(m_font.CreateFontIndirect(&lf));
}

CFileTreeView::~CFileTreeView()
{
}

#ifdef _DEBUG
void CFileTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CFileTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif

BOOL CFileTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	return CTreeView::PreCreateWindow(cs);
}

void CFileTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	GetTreeCtrl().SetFont(&m_font);

	UpdateTree();
}

void CFileTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	TRACE2("file tree view update... (%u)(%u)\n", lHint, (UINT)(pHint));

	switch (lHint)
	{
	case CFMonitor2Doc::UPDATE_FILE_LOADED:
		{
			UpdateTree();
		}
		break;

	default:
		break;
	}
}

void CFileTreeView::UpdateTree()
{
	GetTreeCtrl().DeleteAllItems();

	HTREEITEM root = GetTreeCtrl().InsertItem(_T("File"));

	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetDocument());
	const vector<string> filepaths = doc->GetFilePaths();

	BOOST_FOREACH(string pathname, filepaths)
	{
		HTREEITEM item = GetTreeCtrl().InsertItem(pathname.c_str(), root);
	}

	GetTreeCtrl().SortChildren(root);
	GetTreeCtrl().Expand(root, TVE_EXPAND);
}

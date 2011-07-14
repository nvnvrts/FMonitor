#include "stdafx.h"
#include <string>
#include <algorithm>
#include <boost/foreach.hpp>
#include "FMonitor2.h"
#include "FMonitor2Doc.h"
#include "MainFrm.h"
#include "GraphView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

IMPLEMENT_DYNCREATE(CGraphView, CView)

BEGIN_MESSAGE_MAP(CGraphView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

CGraphView::CGraphView()
{
}

CGraphView::~CGraphView()
{
	BOOST_FOREACH(GraphMap::value_type& v, m_graphs)
	{
		//delete v.second;
	}
}

#ifdef _DEBUG
void CGraphView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif

void CGraphView::ToggleGraph(int id)
{
	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetDocument());

	const CFMLogData::Key& key = doc->GetData()->GetKey(id);

	CGraphCtrl* graph = 0;

	GraphMap::iterator it = m_graphs.find(key.first);
	if (it != m_graphs.end())
	{
		graph = it->second;
	}
	else
	{
		const CFMLogData::TimeLine& timeline = doc->GetData()->GetTimeLine();

		graph = new CGraphCtrl(this, key.first, timeline);
		graph->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0);

		m_graphs.insert(GraphMap::value_type(key.first, graph));

		m_list.push_back(key.first);
	}

	CFMonitor2Doc::Hint hint;

	if (!graph)
	{
		return;
	}
	else
	{
		if (graph->HasData(key.second))
		{
			graph->RemoveData(key.second);

			hint.id = id;
			hint.color = RGB(0, 0, 0);
		}
		else
		{
			fmlog::List* list = doc->GetData()->GetList(id);

			graph->AddData(key.second, list);
			graph->SetOffset(m_scroll.GetScrollPos());
			graph->SetZoom(doc->GetData()->GetZoom());
			graph->SetRuler(m_timeline.GetRuler());

			hint.id = id;
			hint.color = graph->GetColor(key.second);
		}
	}

	UpdateLayout();

	Invalidate();

	doc->UpdateAllViews(this, CFMonitor2Doc::UPDATE_ITEM_COLOR, (CObject*)(&hint));
}

void CGraphView::CloseGraph(const string& name)
{
	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetDocument());

	m_list.erase(std::find(m_list.begin(), m_list.end(), name));

	GraphMap::iterator it = m_graphs.find(name);
	if (it != m_graphs.end())
	{
		CGraphCtrl* graph = (*it).second;

		BOOST_FOREACH(const string& key, graph->GetKeyList())
		{
			CFMonitor2Doc::Hint hint;
			hint.id = doc->GetData()->GetIndex(name, key);		
			hint.color = RGB(0, 0, 0);

			doc->UpdateAllViews(this, CFMonitor2Doc::UPDATE_ITEM_COLOR, (CObject*)(&hint));
		}

		graph->DestroyWindow();

		m_graphs.erase(it);
	}

	UpdateLayout();

	Invalidate();
}

void CGraphView::CloseAllGraphs()
{
	GraphList graphs = m_list;

	BOOST_FOREACH(const string& name, graphs)
	{
		CloseGraph(name);
	}
}

void CGraphView::UpdateLayout()
{
	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetDocument());

	CRect rect;
	GetClientRect(rect);

	int cx = rect.Width();
	int cy = rect.Height();

	m_scroll.GetWindowRect(rect);
	int height = rect.Height();
	m_scroll.MoveWindow(0, cy - height, cx, height);

	m_timeline.GetWindowRect(rect);
	height += rect.Height();
	m_timeline.MoveWindow(0, cy - height, cx, rect.Height());

	int n = static_cast<int>(m_graphs.size());
	if (n > 0)
	{
		int h = (cy - height) / n;
		int y = 0;

		BOOST_FOREACH(string& key, m_list)
		{
			m_graphs[key]->MoveWindow(0, y, cx, h);
			y += h;
		}
	}
}

void CGraphView::OnMouseMove(CGraphCtrl* graph, UINT nFlags, CPoint point)
{
	m_timeline.SetMeter(point.x);

	CRect rect;
	m_timeline.GetWindowRect(rect);

	ScreenToClient(rect);

	InvalidateRect(rect);
}

int CGraphView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_timeline.Create(0, 0, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 25), this, 0);

	m_scroll.Create(SBS_HORZ | SBS_TOPALIGN | WS_CHILD | WS_VISIBLE,
		            CRect(0, 0, 0, 10),
					this,
					0);

	return 0;
}

void CGraphView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	UpdateLayout();
}

void CGraphView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int curpos = pScrollBar->GetScrollPos();

	switch (nSBCode)
	{
	case TB_THUMBPOSITION:
		curpos = nPos;
		break;

	case TB_THUMBTRACK:
		curpos = nPos;
		break;

	case TB_ENDTRACK:
		break;
	}

	pScrollBar->SetScrollPos(curpos);

	m_timeline.SetOffset(curpos);
	pair<int, int> ruler = m_timeline.GetRuler();

	BOOST_FOREACH(GraphMap::value_type& v, m_graphs)
	{
		v.second->SetOffset(curpos);
		v.second->SetRuler(ruler);
	}

	Invalidate();
}

BOOL CGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CGraphView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

void CGraphView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetDocument());
	CFMLogData* data = doc->GetData();
	int length = data->GetLength();

	CRect rect;
	GetClientRect(&rect);

	m_timeline.SetTimeline(data->GetTimeLine());

	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_ALL;
	info.nMin = 0;
	info.nMax = length; 
	info.nPage = rect.Width() - 35;
	info.nPos = 0;
	info.nTrackPos = 0;

	m_scroll.SetScrollInfo(&info);
}

void CGraphView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	TRACE2("graph view update... (%u)(%u)\n", lHint, (ULONG_PTR)(pHint));

	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetDocument());
	CFMonitor2Doc::Hint* hint = (CFMonitor2Doc::Hint*)(pHint);

	switch (lHint)
	{
	case CFMonitor2Doc::UPDATE_RESET:
		{
			CloseAllGraphs();
		}
		break;

	case CFMonitor2Doc::UPDATE_DATA_SELECTED:
		{
			ToggleGraph(hint->id);
		}
		break;

	case CFMonitor2Doc::UPDATE_ZOOM_IN:
	case CFMonitor2Doc::UPDATE_ZOOM_OUT:
		{
			float ratio = hint->ratio;

			CRect rect;
			GetClientRect(&rect);

			int curpos = m_scroll.GetScrollPos();

			SCROLLINFO info;
			info.cbSize = sizeof(info);
			info.fMask = SIF_RANGE | SIF_POS;
			info.nMin = 0;
			info.nMax = doc->GetData()->GetLength();
			info.nPos = curpos * ratio;

			m_scroll.SetScrollInfo(&info);

			int zoom = doc->GetData()->GetZoom();

			m_timeline.SetZoom(zoom);
			m_timeline.SetOffset(info.nPos);
			pair<int, int> ruler = m_timeline.GetRuler();

			BOOST_FOREACH(GraphMap::value_type& v, m_graphs)
			{
				v.second->SetOffset(info.nPos);
				v.second->SetZoom(zoom);
				v.second->SetRuler(ruler);
			}

			Invalidate();

			TRACE3("graph view zoom... (%d/%d)(%.2f)\n", info.nPos, info.nMax, ratio);
		}
		break;

	case CFMonitor2Doc::UPDATE_ZOOM_FIT:
		{
			CRect rect;
			GetClientRect(&rect);

			float width = static_cast<float>(rect.Width());
			float length = doc->GetData()->GetTimeLength();

			if (width > 0 && length > 0 && width < length)
			{
				doc->GetData()->ZoomFit(width);
				int zoom = doc->GetData()->GetZoom();

				m_timeline.SetOffset(0);
				m_timeline.SetZoom(zoom);

				pair<int, int> ruler = m_timeline.GetRuler();

				BOOST_FOREACH(GraphMap::value_type& v, m_graphs)
				{
					v.second->SetOffset(0);
					v.second->SetZoom(zoom);
					v.second->SetRuler(ruler);
				}

				int curpos = m_scroll.GetScrollPos();

				SCROLLINFO info;
				info.cbSize = sizeof(info);
				info.fMask = SIF_RANGE | SIF_POS;
				info.nMin = 0;
				info.nMax = doc->GetData()->GetLength();
				info.nPos = 0;

				m_scroll.SetScrollInfo(&info);

				Invalidate();

				TRACE3("graph view zoom to fit... (%d/%d)(%d)\n", length, width, zoom);
			}
		}
		break;

	case CFMonitor2Doc::UPDATE_SAVE_PRESET:
		{
			fmcfg::Preset::List keys;

			BOOST_FOREACH(const string& name, m_list)
			{
				CGraphCtrl* graph = m_graphs[name];
				if (graph)
				{
					BOOST_FOREACH(const string& key, graph->GetKeyList())
					{
						keys.push_back(make_pair(name, key));
					}
				}
			}

			if (!keys.empty())
			{
				CMainFrame* frame = (CMainFrame*)(AfxGetMainWnd());
				if (frame)
				{
					frame->AddPreset(hint->str, keys, "");
				}
			}
		}
		break;

	default:
		break;
	}
}

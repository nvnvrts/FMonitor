#include "stdafx.h"
#include <utility>
#include <boost/foreach.hpp>
#include "FMonitor2.h"
#include "GraphView.h"
#include "GraphCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

IMPLEMENT_DYNAMIC(CGraphCtrl, CWnd)

BEGIN_MESSAGE_MAP(CGraphCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CGraphCtrl::CGraphCtrl(CGraphView* view,
					   const string& name, 
					   const CFMLogData::TimeLine& timeline)
    :m_view(view),
	 m_name(name),
	 m_timeline(timeline),
	 m_offset(0),
	 m_zoom(1),
	 m_scale(1.0f),
	 m_colorIdx(0),
	 m_meter(CPoint(0, 0)),
	 m_lines(0)
{
}

CGraphCtrl::~CGraphCtrl()
{
	if (m_lines)
	{
		BOOST_FOREACH(Line& line, *m_lines)
		{
			delete [] line.points;
		}

		delete m_lines;
	}
}

BOOL CGraphCtrl::Create(LPCTSTR lpszClassName,
						LPCTSTR lpszWindowName,
						DWORD dwStyle,
						const RECT& rect,
						CWnd* pParentWnd,
						UINT nID,
						CCreateContext* pContext /*= NULL*/)
{
	if (CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CGraphCtrl::SetOffset(unsigned int offset)
{
	m_offset = offset;
}

void CGraphCtrl::SetZoom(int zoom)
{
	m_zoom = zoom;
}

void CGraphCtrl::SetMeter(CPoint point)
{
	m_meter = point;
}

bool CGraphCtrl::HasData(const string& key)
{
	BOOST_FOREACH(const Item& item, m_data)
	{
		if (item.first == key)
		{
			return true;
		}
	}

	return false;
}

bool CGraphCtrl::AddData(const string& key, fmlog::List* list)
{
	m_data.push_back(make_pair(key, list));

	const static COLORREF COLORS[] = 
	{
		RGB(255, 0, 0),
		RGB(139, 0, 0),
		RGB(255, 140, 0),
		RGB(199, 21, 133),
		RGB(0, 100, 0),
		RGB(70, 130, 180),
		RGB(0, 0, 255),
	};

	m_colorMap.insert(ColorMap::value_type(key, COLORS[m_colorIdx++ % 7]));

	return true;
}

bool CGraphCtrl::RemoveData(const string& key)
{
	for (DataList::iterator it = m_data.begin(); it != m_data.end(); ++it)
	{
		if ((*it).first == key)
		{
			m_data.erase(it);
			return true;
		}
	}

	m_colorMap.erase(key);

	return false;
}

COLORREF CGraphCtrl::GetColor(const string& key)
{
	return m_colorMap[key];
}

vector<string> CGraphCtrl::GetKeyList()
{
	vector<string> keys;
	
	keys.reserve(m_data.size());

	BOOST_FOREACH(const Item& item, m_data)
	{
		keys.push_back(item.first);
	}

	return keys;
}

void CGraphCtrl::UpdateLineData(int ox, int oy, int w, int h)
{
	if (m_lines)
	{
		BOOST_FOREACH(Line& line, *m_lines)
		{
			delete [] line.points;
		}

		delete m_lines;
	}

	LineList* lines = new LineList();

	// finding max

	float max = 0.0f;

	BOOST_FOREACH(const Item& item, m_data)
	{
		const fmlog::List* list = item.second;
		fmlog::List::ItemList::const_iterator it = list->list.begin();

		int offset = 0;
		float value = 0.0f;

		int sum = 0;
		int zoom = 1;

		BOOST_FOREACH(const int timestamp, m_timeline)
		{
			if (it == list->list.end())
			{
				break;
			}
			else
			{
				if ((*it).first == timestamp)
				{
					value = (*it++).second;
				}

				sum += value;

				if (zoom != m_zoom)
				{
					zoom++;
				}
				else
				{
					zoom = 1;
					
					value = sum / m_zoom;

					sum = 0;

					if (offset++ >= m_offset)
					{
						if (value > max)
						{
							max = value;
						}

						if (offset >= w + m_offset)
						{
							break;
						}
					}
				}
			}
		}
	}

	// finding scale

	const static float SCALES[] = 
	{ 
		1, 2, 5, 10, 25, 30, 40, 50, 75, 
		100, 200, 250, 300, 400, 500, 600, 700, 750,
		1000, 2000, 2500, 3000, 4000, 5000, 7500,
		10000, 20000, 25000, 50000, 75000,
		100000, 110000, 120000, 130000, 140000, 150000,
		1000000, 1100000, 1200000, 1300000, 1400000, 1500000,
		10000000,
	};

	float scale = 1.0;

	for (int i = 0; i < (sizeof(SCALES) / sizeof(float)); ++i)
	{
		scale = SCALES[i];
		if (scale > max)
		{
			break;
		}
	}

	m_scale = max > scale ? max : scale;

	// line

	BOOST_FOREACH(const Item& item, m_data)
	{
		const string& key = item.first;
		const fmlog::List* list = item.second;

		fmlog::List::ItemList::const_iterator it = list->list.begin();

		int x = ox;
		int y = oy;

		int i = 0;
		int offset = 0;

		int dy = 0;
		float sum = 0;
		int zoom = 1;

		LPPOINT points = new POINT[m_timeline.size()];

		BOOST_FOREACH(const int timestamp, m_timeline)
		{
			if (it == list->list.end())
			{
				break;
			}
			else
			{
				if ((*it).first == timestamp)
				{
					dy = h * (1 - ((*it++).second / scale));
				}

				sum += dy;

				if (zoom != m_zoom)
				{
					zoom++;
				}
				else
				{
					zoom = 1;

					y = oy + sum / m_zoom;

					sum = 0.0f;

					if (offset++ >= m_offset)
					{
						LPPOINT pt = &points[i++];
						pt->x = x++;
						pt->y = y;

						if (offset >= w + m_offset)
						{
							break;
						}
					}
				}
			}
		}

		Line line;
		line.points = points;
		line.count = i;
		line.color = m_colorMap[key];

		lines->push_back(line);
	}

	m_lines = lines;
}

void CGraphCtrl::PostNcDestroy()
{
	CWnd::PostNcDestroy();

	delete this;
}

void CGraphCtrl::OnPaint()
{
	CPaintDC paintDC(this);

	int ox = 30;
	int oy = 2;

	CRect rectClient;
	GetClientRect(rectClient);

	CRect rectGraphs = rectClient;
	rectGraphs.DeflateRect(ox, oy, 5, 2);

	int w = rectGraphs.Width();
	int h = rectGraphs.Height();

	UpdateLineData(ox, oy, w, h);

	CXTPBufferDC dc(paintDC, rectClient);

	{
		dc.FillSolidRect(rectClient, RGB(255, 255, 255));

		//dc.Draw3dRect(rectClient, RGB(0, 0, 0), RGB(0, 0, 0));
		dc.Draw3dRect(rectGraphs, RGB(0, 0, 0), RGB(0, 0, 0));
	}

	{
		int x1 = rectGraphs.right - 12;
		int y1 = rectGraphs.top + 3;
		int x2 = rectGraphs.right - 3;
		int y2 = rectGraphs.top + 12;

		m_rectClose.SetRect(x1, y1, x2, y2);

		dc.MoveTo(x1, y1);
		dc.LineTo(x2, y1);
		dc.LineTo(x2, y2);
		dc.LineTo(x1, y2);
		dc.LineTo(x1, y1);
		dc.LineTo(x2, y2);
		dc.MoveTo(x1, y2);
		dc.LineTo(x2, y1);
	}

	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(198, 198, 198));
		dc.SelectObject(&pen);

		for (int i = 1; i < 5; ++i)
		{
			int cy = oy + i * (h / 5);
			dc.MoveTo(ox, cy);
			dc.LineTo(ox + w, cy);
		}

		for (int x = 1; x < w; x += 240)
		{
			dc.MoveTo(ox + x, oy + h);
			dc.LineTo(ox + x, oy);
		}
	}

	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		dc.SelectObject(&pen);

		CFont font;
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfHeight = 12;
		strcpy(lf.lfFaceName, "Arial");
		VERIFY(font.CreateFontIndirect(&lf));
		dc.SelectObject(&font);

		dc.SetBkColor(RGB(255, 255, 255));

		{
			CString str;
			str.Format("%s", m_name.c_str());
			dc.TextOut(ox + 4, oy + 2, str);
		}

		for (int i = 4; i >= 0; --i)
		{
			int cy = oy + i * (h / 5);
			CString str;
			str.Format("%.1f", (5 - i) * (m_scale / 5.0f));
			dc.TextOut(5, cy, str);
		}
	}

	{
		if (m_lines)
		{
			BOOST_FOREACH(Line& line, *m_lines)
			{
				CPen pen;
				pen.CreatePen(PS_SOLID, 1, line.color);
				dc.SelectObject(&pen);

				dc.MoveTo(0, 0);
				dc.Polyline(line.points, line.count);
			}
		}
	}

	if (rectGraphs.PtInRect(m_meter))
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		dc.SelectObject(&pen);

		dc.MoveTo(m_meter.x, oy);
		dc.LineTo(m_meter.x, oy + h);
		dc.MoveTo(ox, m_meter.y);
		dc.LineTo(ox + w, m_meter.y);

		CString str;
		str.Format("%.2f", m_scale * (1.0f - ((m_meter.y - oy) / (float)(h))));
		dc.TextOut(m_meter.x + 20, m_meter.y + 5, str);
	}
}

void CGraphCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	//TRACE3("graph control mouse move (%s)(%d, %d)...\n", m_name.c_str(), point.x, point.y);

	SetMeter(point);

	m_view->OnMouseMove(this, nFlags, point);

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = GetSafeHwnd();
	tme.dwHoverTime = HOVER_DEFAULT;

	TrackMouseEvent(&tme);

	Invalidate();
}

BOOL CGraphCtrl::OnMouseLeave()
{
	//TRACE1("graph control mouse leave (%s)...\n", m_name.c_str());

	SetMeter(CPoint(0, 0));

	Invalidate();

	return TRUE;
}

void CGraphCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_rectClose.PtInRect(point))
	{
		m_view->CloseGraph(m_name);
	}
}

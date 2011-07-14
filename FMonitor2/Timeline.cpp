#include "stdafx.h"
#include "Timeline.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

IMPLEMENT_DYNAMIC(CTimeline, CWnd)

BEGIN_MESSAGE_MAP(CTimeline, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

CTimeline::CTimeline()
    :m_timeline(0),
	 m_offset(0),
	 m_zoom(1),
	 m_meter(0)
{
}

CTimeline::~CTimeline()
{
}

void CTimeline::SetOffset(unsigned int offset)
{
	m_offset = offset;
}

void CTimeline::SetZoom(int zoom)
{
	m_zoom = zoom;
}

void CTimeline::SetTimeline(const CFMLogData::TimeLine& timeline)
{
	m_timeline = &timeline;
}

void CTimeline::SetMeter(int x)
{
	m_meter = x;
}

pair<int, int> CTimeline::GetRuler()
{
	int offset = -1;
	int width = 10000;

	CRect rc;
	GetClientRect(rc);

	CTime t0;

	for (int x = 0; x < rc.Width(); x++)
	{
		int idx = m_zoom * (m_offset + x);
		int len = static_cast<int>(m_timeline->size());

		if (idx < len)
		{
			CTime t(m_timeline->at(idx));

			if (t.GetMinute() == 0)
			{
				if (offset == -1)
				{
					t0 = t;
					offset = x;
				}
				else
				{
					if (t0.GetHour() != t.GetHour())
					{
						width = x - offset;
						break;
					}
				}
		}
		}
	}

	return make_pair(offset, width);
}

void CTimeline::OnPaint()
{
	int ox = 30;
	int oy = 2;

	CRect rectClient;
	GetClientRect(rectClient);

	CRect rectTimeLine = rectClient;
	rectTimeLine.DeflateRect(ox, oy, 5, 2);

	int w = rectTimeLine.Width();
	int h = rectTimeLine.Height();

	CPaintDC dc(this);

	{
		dc.Draw3dRect(rectClient, RGB(0, 0, 0), RGB(0, 0, 0));
		dc.Draw3dRect(rectTimeLine, RGB(0, 0, 0), RGB(0, 0, 0));
	}

	if (m_timeline)
	{
		CPen pen1;
		pen1.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		dc.SelectObject(&pen1);

		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfHeight = 13;
		lf.lfWeight = FW_BOLD;
		strcpy(lf.lfFaceName, "Arial");

		CFont font;
		VERIFY(font.CreateFontIndirect(&lf));
		dc.SelectObject(&font);

		dc.SetBkColor(RGB(192, 192, 192));

		for (int x = 0; x < w; x++)
		{
			int idx = m_zoom * (m_offset + x);
			int len = static_cast<int>(m_timeline->size());

			CTime t(m_timeline->at(idx));

			if (t.GetMinute() == 0)
			{
				CString str = t.Format("%m/%d %H:%M");
				dc.TextOut(ox + x, oy + 3, str);
			}
		}

		if (m_meter > rectTimeLine.left && m_meter < rectTimeLine.right)
		{
			int x = m_meter - rectTimeLine.left;
			int idx = m_zoom * (m_offset + x);
			int len = static_cast<int>(m_timeline->size());

			if (idx < len)
			{
				CPen pen2;
				pen2.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
				dc.SelectObject(&pen2);

				dc.MoveTo(ox + x, oy);
				dc.LineTo(ox + x, oy + rectTimeLine.Height());

				dc.SetTextColor(RGB(0, 0, 255));

				CTime t(m_timeline->at(idx));
				CString str = t.Format("%m/%d %H:%M:%S");
				dc.TextOut(ox + x, oy + 3, str);
			}
		}
	}
}

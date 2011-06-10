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

		for (int x = 0; x < w; x += 240)
		{
			int idx = m_zoom * (m_offset + x);
			int len = m_timeline->size();

			if (idx < len)
			{
				CTime t(m_timeline->at(idx));
				CString str = t.Format("%m/%d %H:%M:%S");
				dc.TextOut(ox + x, oy + 3, str);
			}
			else
			{
				CTime t(m_timeline->back());
				CString str = t.Format("%m/%d %H:%M:%S");
				dc.TextOut(ox + x - (idx - len) / m_zoom, oy + 3, str);

				break;
			}
		}

		if (m_meter > rectTimeLine.left && m_meter < rectTimeLine.right)
		{
			int x = m_meter - rectTimeLine.left;
			int idx = m_zoom * (m_offset + x);
			int len = m_timeline->size();

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

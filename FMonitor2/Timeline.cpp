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

vector<int> CTimeline::GetRuler(const RulerType type)
{
	vector<int> result;

	int width = 10000;

	CRect rc;
	GetClientRect(rc);

	const int len = static_cast<int>(m_timeline->size());
	
	CTime t0;

	for (int x = 0; x < rc.Width(); x++)
	{
		int idx = m_zoom * (m_offset + x);

		if (idx < 0 || idx >= len)
		{
			break;
		}
		else
		{
			CTime t(m_timeline->at(idx));

			if (x == 0)
			{
				t0 = t;
			}

			switch (type)
			{
			case HOUR:
				{
					if (t0.GetHour() != t.GetHour())
					{
						t0 = t;
						result.push_back(x);
					}
				}
				break;

			case DAY:
			default:
				{
					if (t0.GetDay() != t.GetDay())
					{
						t0 = t;
						result.push_back(x);
					}
				}
				break;
			}
		}
	}

	return result;
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
		dc.SetBkColor(RGB(192, 192, 192));

		const int len = static_cast<int>(m_timeline->size());

		{
			LOGFONT lf;
			memset(&lf, 0, sizeof(LOGFONT));
			lf.lfHeight = 10;
			//lf.lfWeight = FW_BOLD;
			strcpy(lf.lfFaceName, "Tahoma");
			
			CFont font;
			VERIFY(font.CreateFontIndirect(&lf));
			dc.SelectObject(&font);

			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			dc.SelectObject(&pen);
			
			BOOST_FOREACH(int x, GetRuler(HOUR))
			{
				int idx = m_zoom * (m_offset + x);
				if (idx < 0 || idx >= len)
				{
					break;
				}
				else
				{
					CTime t(m_timeline->at(idx));
					CString str = t.Format("%H");
					dc.TextOut(ox + x, oy + 3, str);
				}
			}
		}

		{
			LOGFONT lf;
			memset(&lf, 0, sizeof(LOGFONT));
			lf.lfHeight = 13;
			lf.lfWeight = FW_BOLD;
			strcpy(lf.lfFaceName, "Tahoma");
			
			CFont font;
			VERIFY(font.CreateFontIndirect(&lf));
			dc.SelectObject(&font);

			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			dc.SelectObject(&pen);
			
			BOOST_FOREACH(int x, GetRuler(DAY))
			{
				int idx = m_zoom * (m_offset + x);
				if (idx < 0 || idx >= len)
				{
					break;
				}
				else
				{
					CTime t(m_timeline->at(idx));
					CString str = t.Format("%m/%d");
					dc.TextOut(ox + x, oy + 17, str);
				}
			}
		}

		if (m_meter < rectTimeLine.left || m_meter > rectTimeLine.right)
		{
		}
		else
		{
			int x = m_meter - rectTimeLine.left;

			int idx = m_zoom * (m_offset + x);
			int len = static_cast<int>(m_timeline->size());

			if (idx < 0 || idx >= len)
			{
				TRACE2("timeline meter idx(%d) is out of len(%d)!", idx, len);
			}
			else
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

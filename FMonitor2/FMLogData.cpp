#include "stdafx.h"
#include "FMLogData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

int CFMLogData::s_zooms[] = 
{ 
	1, 2, 3, 4, 5, 6, 7, 8, 9,
	10, 20, 30, 40, 50, 60, 70, 80, 90,
	100, 200, 300, 400, 500,
};

CFMLogData::CFMLogData()
    :m_config(0),
	 m_zoom(1),
	 m_step(0)
{
	memset(m_lists, 0, sizeof(m_lists));

	for (int i = 0; i < MIN_LIST_SIZE; ++i)
	{
		m_lists[i] = new fmlog::List();
	}
}

CFMLogData::~CFMLogData()
{
	if (m_config)
	{
		delete m_config;
	}

	for (int i = 0; i < MAX_LIST_SIZE; ++i)
	{ 
		delete m_lists[i];
	}
}

float CFMLogData::ZoomIn()
{
	if (m_step > 0)
	{
		float a = s_zooms[m_step--];
		float b = s_zooms[m_step];

		m_zoom = b;

		return (a / b);
	}
	else
	{
		return 1.0f;
	}
}

float CFMLogData::ZoomOut()
{
	if (m_step < MAX_ZOOM_LEVEL - 1)
	{
		float a = s_zooms[m_step++];
		float b = s_zooms[m_step];

		m_zoom = b;

		return (a / b);
	}
	else
	{
		return 1.0f;
	}
}

float CFMLogData::ZoomFit(int width)
{
	int length = m_timeline.size();

	if (length < width)
	{
		m_zoom = 1;
	}
	else
	{
		m_zoom = 1 + (length / width);

		for (int i = 0; i < MAX_ZOOM_LEVEL - 2; i++)
		{
			if (m_zoom > s_zooms[i] && m_zoom <= s_zooms[i+1])
			{
				m_step = i;
			}
		}
	}

	return 1.0f;
}

void CFMLogData::Normalize()
{
	TimeLine timeline;

	if (!m_timeline.empty())
	{
		int first = m_timeline.front();
		int last = m_timeline.back();
		timeline.reserve(last - first);
	}

	BOOST_FOREACH(int timestamp, m_timeline)
	{
		if (!timeline.empty())
		{
			int last = timeline.back();
			for (int n = last + 1; n < timestamp; n++)
			{
				timeline.push_back(n);
			}
		}

		timeline.push_back(timestamp);
	}

	m_timeline = timeline;
}

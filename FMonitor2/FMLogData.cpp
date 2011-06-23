#include "stdafx.h"
#include "FMLogData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

int CFMLogData::s_zooms[] = { 1, 2, 3, 6, 12, 20, 30, 60, };

CFMLogData::CFMLogData()
    :m_config(0),
	 m_zoom(0)
{
	memset(m_lists, 0, sizeof(m_lists));
	
	for (int i = 0; i < MIN_LIST_SIZE; ++i)
	{
		m_lists[i] = new fmlog::List();
	}
	memset(&m_info, 0, sizeof(m_info));
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
	if (m_zoom > 0)
	{
		float a = s_zooms[m_zoom--];
		float b = s_zooms[m_zoom];
		return (a / b);
	}
	else
	{
		return 1.0f;
	}
}

float CFMLogData::ZoomOut()
{
	if (m_zoom < MAX_ZOOM_LEVEL - 1)
	{
		float a = s_zooms[m_zoom++];
		float b = s_zooms[m_zoom];
		return (a / b);
	}
	else
	{
		return 1.0f;
	}
}

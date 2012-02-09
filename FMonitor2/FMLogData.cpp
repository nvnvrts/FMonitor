#include "stdafx.h"
#include <boost/foreach.hpp>
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

void CFMLogData::Serialize(CArchive& ar)
{
	ar.WriteString("FMonitor Log Data Dump Format v0.0");
	ar.WriteString("\n");

	ar << static_cast<int>(m_keys.size());

	BOOST_FOREACH(const KeyMap::value_type& key, m_keys)
	{
		ar.WriteString(key.first.c_str());
		ar.WriteString("\n");

		ar << static_cast<int>(key.second.size());

		BOOST_FOREACH(const IdxMap::value_type& idx, key.second)
		{
			ar.WriteString(idx.first.c_str());
			ar.WriteString("\n");

			ar << static_cast<int>(idx.second);
		}
	}

	ar << static_cast<int>(m_index.size());

	BOOST_FOREACH(const Index::value_type& index, m_index)
	{
		ar << static_cast<int>(index.first);

		ar.WriteString(index.second.first.c_str());
		ar.WriteString("\n");

		ar.WriteString(index.second.second.c_str());
		ar.WriteString("\n");
	}

	ar << static_cast<int>(m_timeline.size());

	BOOST_FOREACH(int timestamp, m_timeline)
	{
		ar << timestamp;
	}

	int numList = 0;

	for (int i = 0; i < MAX_LIST_SIZE; i++)
	{
		const fmlog::List* list = m_lists[i];
		if (list)
		{
			numList++;
		}
	}

	ar << numList;

	for (int i = 0; i < MAX_LIST_SIZE; i++)
	{
		const fmlog::List* list = m_lists[i];
		if (list)
		{
			ar << i
			   << list->maximum
			   << list->sum
			   << list->count
			   << static_cast<int>(list->list.size());

			BOOST_FOREACH(const fmlog::List::ItemList::value_type& v, list->list)
			{
				ar << v.first
				   << v.second;
			}
		}
	}
}

void CFMLogData::Deserialize(CArchive& ar)
{
	CString version;

	ar.ReadString(version);

	int numKeys;
	ar >> numKeys;

	for (int i = 0; i < numKeys; i++)
	{
		CString name;
		ar.ReadString(name);

		int numIdx;
		ar >> numIdx;

		IdxMap idxMap;

		for (int j = 0; j < numIdx; j++)
		{
			CString str;
			ar.ReadString(str);

			int idx;
			ar >> idx;

			idxMap[str.GetString()] = idx;
		}

		m_keys[name.GetString()] = idxMap;
	}

	int numIndex;
	ar >> numIndex;

	for (int i = 0; i < numIndex; i++)
	{
		int idx;
		ar >> idx;

		CString key1;
		ar.ReadString(key1);

		CString key2;
		ar.ReadString(key2);

		m_index[idx] = make_pair(key1.GetString(), key2.GetString());
	}

	int sizeTimeline;
	ar >> sizeTimeline;

	m_timeline.reserve(sizeTimeline);

	for (int i = 0; i < sizeTimeline; i++)
	{
		int timestamp;
		ar >> timestamp;

		m_timeline.push_back(timestamp);
	}

	int numList;
	ar >> numList;

	for (int i = 0; i < numList; i++)
	{
		int id;
		ar >> id;

		fmlog::List* list = GetList(id);
		ar >> list->maximum >> list->sum >> list->count;

		int size;
		ar >> size;

		list->list.reserve(size);

		for (int j = 0; j < size; j++)
		{
			int timestamp;
			float value;

			ar >> timestamp >> value;

			list->list.push_back(make_pair(timestamp, value));
		}
	}
}

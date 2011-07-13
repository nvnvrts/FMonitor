#pragma once

#include <string>
#include <map>
#include <hash_map>
#include <boost/foreach.hpp>
#include "FMLogType.h"

using namespace std;
using namespace stdext;

class CFMLogData
{
public:
	typedef hash_map<string, int> IdxMap;
	typedef hash_map<string, IdxMap> KeyMap;
	typedef pair<string, string> Key;
	typedef vector<int> TimeLine;

	struct Info
	{
		int version;
		int timestamp;
	};

private:
	typedef hash_map<int, Key> Index;
	typedef hash_map<int, fmlog::List*> ListMap;

	enum { MIN_LIST_SIZE = 1024 };
	enum { MAX_LIST_SIZE = 8192 };

	Info m_info;
	fmlog::Table* m_config;

	KeyMap m_keys;
	Index m_index;
	TimeLine m_timeline;
	int m_zoom;
	int m_step;
	fmlog::List* m_lists[MAX_LIST_SIZE];

	enum { MAX_ZOOM_LEVEL = 27 };
	static int s_zooms[MAX_ZOOM_LEVEL];

public:
	CFMLogData();

	virtual ~CFMLogData();

	void SetInfo(const Info* info)
	{
		memcpy(&m_info, info, sizeof(m_info));
	}

	const Info& GetInfo()
	{
		return m_info;
	}

	fmlog::Table* GetConfig()
	{
		return m_config;
	}

	void SetConfig(fmlog::Table* table)
	{
		if (m_config)
		{
			delete m_config;
		}

		m_config = table;
	}

	inline const KeyMap& GetKeys()
	{
		return m_keys;
	}

	inline const Key& GetKey(int id)
	{
		return m_index[id];
	}

	inline int GetIndex(const string& k1, const string& k2)
	{
		return m_keys[k1][k2];
	}

	inline void AddIndex(int id, const string& k1, const string& k2)
	{
		m_keys[k1][k2] = id;
		m_index[id] = make_pair(k1, k2);
	}

	inline const TimeLine& GetTimeLine() const
	{
		return m_timeline;
	}

	int GetZoom() const
	{
		return m_zoom;
	}

	float ZoomIn();
	float ZoomOut();
	float ZoomFit(int width);

	bool CanZoomIn() const
	{
		return m_step != 0;
	}

	bool CanZoomOut() const
	{
		return m_step < MAX_ZOOM_LEVEL;
	}

	bool CanZoomFit() const
	{
		// @todo not implemented yet
		return true;
	}

	inline void AddTimeStamp(int timestamp)
	{
		if (m_timeline.empty())
		{
			m_timeline.push_back(timestamp);
		}
		else
		{
			if (timestamp > m_timeline.back())
			{
				m_timeline.push_back(timestamp);
			}
		}
	}

	inline fmlog::List* GetList(int id)
	{
		fmlog::List* list = m_lists[id];
		if (!list)
		{
			list = new fmlog::List();
			m_lists[id] = list;
		}

		return list;
	}

	inline int GetLength()
	{
		return (m_timeline.size() / GetZoom());
	}

	inline int GetTimeLength()
	{
		return m_timeline.size();
	}
};

#pragma once

#include <string>
#include <map>
#include <hash_map>
#include "FMLogType.h"
#include "FMLogData.h"

using namespace std;
using namespace stdext;

class CGraphView;

class CGraphCtrl : public CWnd
{
	typedef pair<string, fmlog::List*> Item;
	typedef vector<Item> DataList;
	typedef map<string, COLORREF> ColorMap;

	CGraphView* m_view;
	string m_name;
	const CFMLogData::TimeLine& m_timeline;
	int m_offset;
	int m_zoom;
	float m_scale;
	DataList m_data;
	CRect m_rectMax;
	CRect m_rectSaturation;
	CRect m_rectClose;
	int m_colorIdx;
	ColorMap m_colorMap;
	CPoint m_meter;
	int m_style;
	bool m_localMax;
	bool m_saturation;

	DECLARE_DYNAMIC(CGraphCtrl)

public:
	CGraphCtrl(CGraphView* view,
		       const string& name,
			   int style,
			   const CFMLogData::TimeLine& timeline);

	virtual ~CGraphCtrl();

	virtual BOOL Create(LPCTSTR lpszClassName,
		                LPCTSTR lpszWindowName,
						DWORD dwStyle,
						const RECT& rect,
						CWnd* pParentWnd,
						UINT nID,
						CCreateContext* pContext = NULL);

	void SetOffset(unsigned int offset);
	void SetZoom(int zoom);
	void SetMeter(CPoint point);
	void SetStyle(int style);

	bool HasData(const string& key);
	bool AddData(const string& key, fmlog::List* list);
	bool RemoveData(const string& key);

	COLORREF GetColor(const string& key);

	vector<string> GetKeyList();

	void OnLButtonUp(CGraphView* view, UINT nFlags, CPoint point);

protected:
	struct Line
	{
		LPPOINT points;
		int count;
		COLORREF color;
		vector<pair<int, int> > timelog;
	};

	typedef vector<Line> LineList;
	LineList* m_lines;

	struct State
	{
		int w;
		int h;
		int offset;
		int zoom;
	};

	State m_state;

	void UpdateLineData(int ox, int oy, int w, int h);

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual void PostNcDestroy();

public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#pragma once

#include <list>
#include "FMLogType.h"
#include "GraphCtrl.h"
#include "Timeline.h"

using namespace std;
using namespace stdext;

class CGraphView : public CView
{
	typedef hash_map<string, CGraphCtrl*> GraphMap;
	GraphMap m_graphs;

	typedef list<string> GraphList;
	GraphList m_list;

	CTimeline m_timeline;
	CScrollBar m_scroll;

	int m_style;

	DECLARE_DYNCREATE(CGraphView)

protected:
	CGraphView();
	virtual ~CGraphView();

public:
	void ToggleGraph(int id, bool show = true, bool hide = true);
	void CloseGraph(const string& name);
	void CloseAllGraphs();

	vector<int> GetHourRuler();
	vector<int> GetDayRuler();

	void UpdateLayout();

	void OnMouseMove(CGraphCtrl* graph, UINT nFlags, CPoint point);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView*, LPARAM, CObject*);

protected:
	void OnUpdateZoomFit();
};

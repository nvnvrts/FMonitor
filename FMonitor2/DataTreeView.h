#pragma once

#include <map>
#include "LogLoader.h"

using namespace std;

class CDataTreeView : public CXTPTreeView
{
	typedef map<int, HTREEITEM> ItemMap;
	ItemMap m_items;

	DECLARE_DYNCREATE(CDataTreeView)

protected:
	CDataTreeView();
	virtual ~CDataTreeView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

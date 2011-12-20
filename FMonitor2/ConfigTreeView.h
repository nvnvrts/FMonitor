#pragma once

class CConfigTreeView : public CTreeView
{
	CFont m_font;

	DECLARE_DYNCREATE(CConfigTreeView)

protected:
	CConfigTreeView();
	virtual ~CConfigTreeView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

	void AddTable(HTREEITEM parent, fmlog::Table* table);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

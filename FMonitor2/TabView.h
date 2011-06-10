#pragma once

class CTabView : public CView
{
	enum
	{
		ID_TAB_CONTROL = 1,
		ID_CATEGORY_VIEW = 2,
	};

	CXTPTabControl m_tab;

protected:
	DECLARE_DYNCREATE(CTabView)

protected:
	CTabView();
	virtual ~CTabView();

	void AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle);

public:
	virtual void OnDraw(CDC* pDC);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

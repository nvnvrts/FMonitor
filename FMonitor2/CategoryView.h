#pragma once

class CCategoryView : public CTreeView
{
	DECLARE_DYNCREATE(CCategoryView)

protected:
	CCategoryView();
	virtual ~CCategoryView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

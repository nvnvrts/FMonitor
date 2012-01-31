#pragma once

class CFileTreeView : public CTreeView
{
	CFont m_font;

	DECLARE_DYNCREATE(CFileTreeView)

protected:
	CFileTreeView();
	virtual ~CFileTreeView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

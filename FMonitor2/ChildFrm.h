#pragma once

class CChildFrame : public CMDIChildWnd
{
	UINT m_preset;

	CSplitterWnd m_splitter;
//	CStatusBar m_status;

	DECLARE_DYNCREATE(CChildFrame)

public:
	CChildFrame();
	virtual ~CChildFrame();

	void SetPreset(UINT nID);
	const UINT GetPreset();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToolFileAttach();
	afx_msg void OnUpdateToolFileAttach(CCmdUI* pCmdUI);
	afx_msg void OnToolZoomIn();
	afx_msg void OnUpdateToolZoomIn(CCmdUI*);
	afx_msg void OnToolZoomOut();
	afx_msg void OnUpdateToolZoomOut(CCmdUI*);
	afx_msg void OnToolZoomFit();
	afx_msg void OnUpdateToolZoomFit(CCmdUI*);
	afx_msg void OnToolShowLogTime();
	afx_msg void OnUpdateToolShowLogTime(CCmdUI* pCmdUI);
	afx_msg void OnToolShowRuler();
	afx_msg void OnUpdateToolShowRuler(CCmdUI* pCmdUI);
	afx_msg void OnToolSavePresetAs();
	afx_msg void OnUpdateToolSavePresetAs(CCmdUI* pCmdUI);
	afx_msg void OnToolPreset(UINT nID);
	afx_msg void OnUpdateToolPreset(CCmdUI*);
};

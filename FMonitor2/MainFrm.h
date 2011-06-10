#pragma once

#include <string>
#include <vector>
#include <utility>
#include "FMCfgData.h"

using namespace std;

class CMainFrame : public CMDIFrameWnd
{
private:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

	CFMCfgData* m_config;

	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

	bool AddPreset(const string& name, const fmcfg::Preset::List& keys, const string& caption);
	const fmcfg::Preset* GetPreset(UINT nID);

protected:
	CString DumpConfig();

	void LoadConfig();
	void SaveConfig();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
};

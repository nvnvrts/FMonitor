#pragma once

#include <string>
#include <vector>
#include <utility>
#include <hash_map>
#include "FMCfgData.h"

using namespace std;

class CMainFrame : public CMDIFrameWnd
{
private:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

	CFMCfgData* m_config;

	typedef hash_map<string, bool> FlagHash;
	FlagHash m_flags;

	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

	bool AddPreset(const string& name, const fmcfg::Preset::List& keys, const string& caption);
	const fmcfg::Preset* GetPreset(UINT nID);

	bool GetFlag(const string& name) { return m_flags[name]; }
	bool ToggleFlag(const string& name);

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

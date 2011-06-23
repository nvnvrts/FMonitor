#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"

class CFMonitor2App : public CWinApp
{
public:
	CFMonitor2App();

public:
	virtual BOOL InitInstance();

public:
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileMerge();
	afx_msg void OnAppAbout();

protected:
	DECLARE_MESSAGE_MAP()
};

extern CFMonitor2App theApp;

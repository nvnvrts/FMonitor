#include "stdafx.h"
#include "FMonitor2.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "FMonitor2Doc.h"
#include "FMonitor2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFMonitor2App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CFMonitor2App::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CFMonitor2App::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CFMonitor2App::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

CFMonitor2App::CFMonitor2App()
{
}

CFMonitor2App theApp;

BOOL CFMonitor2App::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;

	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;

	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if(!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	LoadStdProfileSettings(8);

	CMultiDocTemplate* pDocTemplate = new CMultiDocTemplate(IDR_FMonitor2TYPE,
		                                                    RUNTIME_CLASS(CFMonitor2Doc),
															RUNTIME_CLASS(CChildFrame),
															RUNTIME_CLASS(CFMonitor2View));
	if (!pDocTemplate)
	{
		return FALSE;
	}

	AddDocTemplate(pDocTemplate);

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}

	m_pMainWnd = pMainFrame;

	m_pMainWnd->DragAcceptFiles();

	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!ProcessShellCommand(cmdInfo))
	{
		return FALSE;
	}

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

void CFMonitor2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CFMonitor2App::OnFileNew()
{
}

void CFMonitor2App::OnFileOpen()
{
	CWinApp::OnFileOpen();
}
#include "stdafx.h"
#include <string>
#include <deque>
#include <algorithm>
#include <sstream>
#include "FMonitor2.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "FMonitor2Doc.h"
#include "FMonitor2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

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

	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

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
//	CWinApp::OnFileOpen();

	CFileDialog dlg(TRUE,
		            NULL,
					NULL,
					OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT,
					_T("FMonitor Files (*.log;*.log.gz;*.log.7z)|*.log;*.log.gz;*.log.7z|AllFiles (*.*)|*.*||"),
					m_pMainWnd);

	CString buf;
	const int size = 100 * (_MAX_PATH + 1);
	dlg.GetOFN().lpstrFile = buf.GetBuffer(size);
	dlg.GetOFN().nMaxFile = size;

	if (dlg.DoModal() == IDOK)
	{
		deque<string> filepaths;

		POSITION pos = dlg.GetStartPosition();
		do
		{
			CString pathname = dlg.GetNextPathName(pos);
			filepaths.push_back(static_cast<LPCTSTR>(pathname));
		}
		while (pos != NULL);

		sort(filepaths.begin(), filepaths.end());

		if (!filepaths.empty())
		{
			const string pathname = filepaths.front();
			filepaths.pop_front();

			CFMonitor2Doc* doc = static_cast<CFMonitor2Doc*>(OpenDocumentFile(static_cast<LPCTSTR>(pathname.c_str())));
			if (doc)
			{
				BOOST_FOREACH(string pathname, filepaths)
				{
					doc->LoadFile(pathname.c_str());
				}
			}
			else
			{
				ostringstream oss;
				oss << "can't open document! "
					<< "(filepath=" << pathname << ")\n";

				AfxMessageBox(oss.str().c_str(), MB_ICONEXCLAMATION | MB_OK);
			}
		}
	}
}

#include "stdafx.h"
#include "FMonitor2.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "FMonitor2Doc.h"
#include "FMonitor2View.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFMonitor2App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CFMonitor2App::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CFMonitor2App::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CFMonitor2App::OnFileOpen)
	ON_COMMAND(ID_FILE_MERGE, &CFMonitor2App::OnFileMerge)
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
	CWinApp::OnFileOpen();

	//char szFilter[] = "Log File(*.log)|*.log"; //Filter 설정

	//CFileDialog m_FileOpenDialog( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, szFilter );
	//if( m_FileOpenDialog.DoModal() == IDOK )
	//{
	//	POSITION filePosition = m_FileOpenDialog.GetStartPosition();
	//	while( filePosition )
	//	{
	//		OpenDocumentFile( m_FileOpenDialog.GetNextPathName(filePosition) );
	//	}
	//}
}

void CFMonitor2App::OnFileMerge()
{
	char szFilter[] = "Log File(*.log)|*.log"; //Filter 설정

	CFileDialog m_FileOpenDialog( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, szFilter );
	if( m_FileOpenDialog.DoModal() == IDOK )
	{
		char newname[256];
		memset( newname, 0, sizeof(char)*256 );

		POSITION filePosition = m_FileOpenDialog.GetStartPosition();
		bool bnew = true;
		CFile* newfile = NULL;
		while( filePosition )
		{
			CString pathName = m_FileOpenDialog.GetNextPathName(filePosition);
			
			char* filename = pathName.GetBuffer();

			// 새파일 만들기
			if( !newfile )
			{
				newfile = new CFile;

				int pos = pathName.ReverseFind( '\\' );
				strncpy( newname, filename, pos+16 );
				strncpy( newname+pos+16, filename+pos+23, 15 );

				if( !newfile->Open( newname, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, 0 ) )
				{
					delete newfile;
					return;
				}
			}
			
			FILE* infile = fopen(filename, "r");
			if( infile )
			{
				while( 1 )
				{
					fpos_t filepos, filepos2;
					fgetpos( infile, &filepos );
					int ch = 0;
					do {
						ch = fgetc(infile);
					}
					while( ch != '\n' && ch != EOF );
					
					fgetpos( infile, &filepos2 );
					long linelen = filepos2 - filepos;
					fseek( infile, -linelen, SEEK_CUR );

					char* pBuf = (char*)malloc( sizeof(char) * linelen );

					if( !fgets( pBuf, linelen, infile ) )
						break;

					if( bnew || *(pBuf+1) == 'D' )
					{
						newfile->Write( pBuf, strlen(pBuf) );
					}
					free(pBuf);
				}
				fclose( infile );
			}
						
			bnew = false;
		}

		if( newfile )
		{
			newfile->Close();
			delete newfile;
		}

		OpenDocumentFile( newname );
	}
}
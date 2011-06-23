#include "stdafx.h"
#include "FMonitor2.h"
#include "LogLoader.h"
#include "FMonitor2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CFMonitor2Doc, CDocument)

BEGIN_MESSAGE_MAP(CFMonitor2Doc, CDocument)
END_MESSAGE_MAP()

CFMonitor2Doc::CFMonitor2Doc()
{
	m_hFile = NULL;
	m_hMap = NULL;
	m_pBase = NULL;

	m_pData = 0;
}

CFMonitor2Doc::~CFMonitor2Doc()
{
	CloseHandles();
}

#ifdef _DEBUG
void CFMonitor2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFMonitor2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CFMonitor2Doc::CloseHandles()
{
	if (m_pBase)
	{
		UnmapViewOfFile(m_pBase);
		m_pBase = NULL;
	}

	if (m_hMap)
	{
		CloseHandle(m_hMap);
		m_hMap = NULL;
	}

	if (m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}

BOOL CFMonitor2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CFMonitor2Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	BOOL br = CDocument::OnOpenDocument(lpszPathName);
	if (!br)
	{
		return FALSE;
	}

	m_hFile = CreateFile(lpszPathName,
		                 GENERIC_READ,
						 FILE_SHARE_READ,
						 NULL,
						 OPEN_EXISTING,
						 FILE_FLAG_RANDOM_ACCESS,
						 0);

	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	try
	{
		DWORD size = GetFileSize(m_hFile, NULL);
		if (size == 0)
		{
			throw Error("file is empty!");
		}

		m_hMap = CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (m_hMap == NULL)
		{
			throw Error("can't map the file!");
		}

		m_pBase = MapViewOfFile(m_hMap, FILE_MAP_READ, 0, 0, 0);
		if (m_pBase == NULL)
		{
			throw Error("can't view the file!");
		}

		CLogLoader loader;
		if( m_pData )
		{
			m_pData = loader.Load((char*)(m_pBase), size, m_pData);
		}
		else
		{
			m_pData = loader.Load((char*)(m_pBase), size);
		}

		CloseHandles();

		return TRUE;
	}
	catch (Error& e)
	{
		AfxMessageBox(e.GetReason().c_str(), MB_OK);

		CloseHandles();

		return FALSE;
	}
}

BOOL CFMonitor2Doc::OnSaveDocument(LPCSTR lpszPathname)
{
	return FALSE;
}

void CFMonitor2Doc::OnCloseDocument()
{
	if (m_pData)
	{
		delete m_pData;
		m_pData = 0;
	}

	CloseHandles();

	CDocument::OnCloseDocument();
}

void CFMonitor2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

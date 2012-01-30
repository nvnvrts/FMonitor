#include "stdafx.h"
#include <utility>
#include <sstream>
#include <boost/foreach.hpp>
#include "FMonitor2.h"
#include "Pipe.h"
#include "LogLoader.h"
#include "FMonitor2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace boost;

IMPLEMENT_DYNCREATE(CFMonitor2Doc, CDocument)

BEGIN_MESSAGE_MAP(CFMonitor2Doc, CDocument)
END_MESSAGE_MAP()

CFMonitor2Doc::CFMonitor2Doc()
{
	m_pData = 0;

	m_config.zoomFit = true;
}

CFMonitor2Doc::~CFMonitor2Doc()
{
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

bool CFMonitor2Doc::LoadFile(LPCTSTR lpszPathName)
{
	HANDLE hFile = CreateFile(lpszPathName,
		                      GENERIC_READ,
							  FILE_SHARE_READ,
							  NULL,
							  OPEN_EXISTING,
							  FILE_FLAG_RANDOM_ACCESS,
							  0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	HANDLE hMap = NULL;
	LPVOID pBase = NULL;

	try
	{
		string filename = lpszPathName;
		string::size_type idx = filename.find_last_of('.');
		if (idx != string::npos)
		{
			string ext = filename.substr(idx + 1);
			if (ext == "log")
			{
				DWORD size = GetFileSize(hFile, NULL);
				if (size == 0)
				{
					ostringstream oss;
					oss << "the file is empty!\n\n"
						<< "path = " << lpszPathName << "\n";

					throw Error(oss.str());
				}
				
				hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
				if (hMap == NULL)
				{
					ostringstream oss;
					oss << "can't map the file!\n\n"
						<< "path = " << lpszPathName << "\n"
						<< "file size = " << size << "\n"
						<< "error = " << ::GetLastError();
					
					throw Error(oss.str());
				}
				
				pBase = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
				if (pBase == NULL)
				{
					ostringstream oss;
					oss << "can't view the file!\n\n"
						<< "path = " << lpszPathName << "\n"
						<< "file size = " << size << "\n"
						<< "error = " << ::GetLastError();
					
					throw Error(oss.str());
				}

				CLogLoader loader;
				m_pData = loader.Load((char*)(pBase), size, m_pData);
			}
			else if (ext == "7z" || ext == "gz")
			{
				Pipe pipe;

				if (!pipe.Open("7z.exe x -so", filename))
				{
					throw Error("can't open pipe!");
				}
				else
				{
					int size = 0;

					typedef pair<char*, int> Buffer;
					typedef vector<Buffer> BufferList;

					BufferList buflist;

					const int BUFSIZE = 4096;

					for (;;)
					{
						char* p = new char[BUFSIZE];

						int read = pipe.Read(p, BUFSIZE);
						if (read > 0)
						{
							buflist.push_back(make_pair(p, read));
							size += read;
						}
						else
						{
							delete [] p;
							break;
						}
					}

					char* data = (char*)(malloc(size));

					int offset = 0;

					BOOST_FOREACH(Buffer buf, buflist)
					{
						memcpy(data + offset, buf.first, buf.second);
						offset += buf.second;
						delete [] buf.first;
					}

					pipe.Close();

					TRACE1("(%d) byte(s) read from the pipe!\n", size);

					CLogLoader loader;
					m_pData = loader.Load(data, size, m_pData);

					free(data);
				}
			}
			else
			{
				ostringstream oss;
				oss << "not supported log format (" << ext << ")\n";
				
				throw Error(oss.str());
			}
		}

		if (pBase) UnmapViewOfFile(pBase);
		if (hMap) CloseHandle(hMap);
		if (hFile) CloseHandle(hFile);

		return true;
	}
	catch (Error& e)
	{
		AfxMessageBox(e.GetReason().c_str(), MB_OK);

		if (pBase) UnmapViewOfFile(pBase);
		if (hMap) CloseHandle(hMap);
		if (hFile) CloseHandle(hFile);

		return false;
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

	if (LoadFile(lpszPathName))
	{
		return TRUE;
	}
	else
	{
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

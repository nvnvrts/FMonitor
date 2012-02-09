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
	ON_COMMAND(ID_FILE_SAVE_AS, &CFMonitor2Doc::OnFileSaveAs)
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
		const string filename = lpszPathName;

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

				m_filepaths.push_back(filename);
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

					m_filepaths.push_back(filename);
				}
			}
			else if (ext == "fml")
			{
				CFile file(hFile);

				hFile = NULL;

				CArchive ar(&file, CArchive::load);

				m_pData = new CFMLogData();
				m_pData->Deserialize(ar);

				ar.Close();

				file.Close();
			}
			else
			{
				ostringstream oss;
				oss << "not supported log format (" << ext << ")\n";
				
				throw Error(oss.str());
			}
		}

		if (pBase)
		{
			UnmapViewOfFile(pBase);
		}
		
		if (hMap)
		{
			CloseHandle(hMap);
		}

		if (hFile)
		{
			CloseHandle(hFile);
		}

		return true;
	}
	catch (Error& e)
	{
		AfxMessageBox(e.GetReason().c_str(), MB_OK);

		if (pBase)
		{
			UnmapViewOfFile(pBase);
		}

		if (hMap)
		{
			CloseHandle(hMap);
		}

		if (hFile)
		{
			CloseHandle(hFile);
		}

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
	return TRUE;
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

void CFMonitor2Doc::OnFileSaveAs()
{
	string filename;

	const CFMLogData::TimeLine& timeline = m_pData->GetTimeLine();
	if (!timeline.empty())
	{
		string prefix = "unknown";

//		const fmlog::Table* config = m_pData->GetConfig();
//		
//		const fmlog::Table* master = config->FindTable("Master");
//		if (master)
//		{
//			const fmlog::Value v = master->FindValue("ID");
//			if (!v.str.empty())
//			{
//				prefix = string("master") + v.str;
//			}
//			else
//			{
//				const fmlog::Table* channel = config->FindTable("Channel");
//				if (channel)
//				{
//					const fmlog::Value v = channel->FindValue("ID");
//					if (!v.str.empty())
//					{
//						prefix = string("channel") + v.str;
//					}
//				}
//			}
//		}

		CTime from(timeline.front());
		CTime to(timeline.back());

		ostringstream oss;

		oss << prefix
			<< "_"
			<< from.Format("%Y%m%d%H%M").GetString()
			<< "_"
			<< to.Format("%Y%m%d%H%M").GetString();

		filename = oss.str();
	}

	CFileDialog dlg(FALSE, _T("fml"), filename.c_str());

	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;

		string pathname = dlg.GetPathName();

		string::size_type idx = pathname.find_last_of('.');
		if (idx == string::npos)
		{
			pathname += ".fml";
		}
		else
		{
			pathname.replace(idx + 1, 3, "fml");
		}
		
		TRACE1("serializing %s...", pathname.c_str());
		
		CFile file;
		
		if (file.Open(pathname.c_str(),CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			CArchive ar(&file, CArchive::store);
			
			m_pData->Serialize(ar);
			
			ar.Close();
			
			file.Close();
		}
		else
		{
		}
	}
}

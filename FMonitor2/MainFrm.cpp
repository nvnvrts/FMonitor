#include "stdafx.h"
#include <boost/foreach.hpp>
#include "FMonitor2.h"
#include "FMonitor2Doc.h"
#include "ChildFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_INITMENU()
	ON_WM_INITMENUPOPUP()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CMainFrame::CMainFrame() : m_config(0)
{
	LoadConfig();

	if (!m_config)
	{
		m_config = new CFMCfgData();

		fmcfg::Preset::List keys;

		keys.push_back(make_pair(string("World"), string("Computers")));
		keys.push_back(make_pair(string("CPU"), string("CPU")));
		keys.push_back(make_pair(string("CPU"), string("CPU0")));
		keys.push_back(make_pair(string("CPU"), string("CPU1")));
		keys.push_back(make_pair(string("CPU"), string("CPU2")));
		keys.push_back(make_pair(string("CPU"), string("CPU3")));
		keys.push_back(make_pair(string("Memory"), string("Memory(Lua)(MB)")));
		keys.push_back(make_pair(string("Memory"), string("Memory(PB)(MB)")));
		AddPreset("server default", keys, "");

		keys.clear();
		keys.push_back(make_pair(string("World"), string("Computers")));
		keys.push_back(make_pair(string("CPU"), string("CPU")));
		keys.push_back(make_pair(string("CPU"), string("CPU0")));
		keys.push_back(make_pair(string("CPU"), string("CPU1")));
		keys.push_back(make_pair(string("CPU"), string("CPU2")));
		keys.push_back(make_pair(string("CPU"), string("CPU3")));
		keys.push_back(make_pair(string("Memory"), string("Memory(Lua)(MB)")));
		keys.push_back(make_pair(string("Memory"), string("Memory(PB)(MB)")));
		keys.push_back(make_pair(string("Tick"), string("NumTicks(/sec)")));
		AddPreset("channel pcu", keys, "");

		keys.clear();
		keys.push_back(make_pair(string("World"), string("Computers")));
		keys.push_back(make_pair(string("Network_Client"), string("KByteRecv")));
		keys.push_back(make_pair(string("Network_Client"), string("KByteSent")));
		AddPreset("channel network", keys, "");
	}
}

CMainFrame::~CMainFrame()
{
	SaveConfig();

	if (m_config)
	{
		delete m_config;
	}
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif

bool CMainFrame::AddPreset(const string& name, const fmcfg::Preset::List& keys, const string& caption)
{
	if (!m_config)
	{
		return false;
	}
	else
	{
		fmcfg::Preset preset;
		preset.name = name;
		preset.keys = keys;
		preset.caption = caption;

		m_config->AddPreset(preset);
	
		return true;
	}
}

const fmcfg::Preset* CMainFrame::GetPreset(UINT nID)
{
	BOOST_FOREACH(const fmcfg::Preset& preset, m_config->GetPreset())
	{
		if (preset.id == nID)
		{
			return &preset;
		}
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CMDIFrameWnd::PreCreateWindow(cs))
	{
		return FALSE;
	}

	return TRUE;
}

CString GetModuleDir()
{
	TCHAR lpszModule[MAX_PATH];

	if (!GetModuleFileName(AfxGetResourceHandle(), lpszModule, MAX_PATH))
	{
		return _T("");
	}

	CString strPath(lpszModule);

	int nIndex = strPath.ReverseFind('\\');

	if (nIndex != -1 && strPath.GetLength() != 3)
	{
		return strPath.Left(nIndex + 1);
	}

	return _T(".\\");
}

CString CMainFrame::DumpConfig()
{
	CString buf;

	buf += _T("{\n");

	BOOST_FOREACH(const fmcfg::Preset& preset, m_config->GetPreset())
	{
		buf.AppendFormat(_T("\t[\"%s\"] = {\n"), preset.name.c_str());

		int i;
		fmcfg::Preset::List::const_iterator it;

		for (i = 1, it = preset.keys.begin(); it != preset.keys.end(); ++i, ++it)
		{
			buf.AppendFormat(_T("\t\t[\"%d\"] = { [\"%s\"] = [[%s]], },\n"),
				i,
				(*it).first.c_str(),
				(*it).second.c_str());
		}

		buf += _T("\t},\n");
	}

	buf += _T("}\n");

	return buf;
}

void CMainFrame::LoadConfig()
{
	CString path = GetModuleDir() + _T("fmonitor2.cfg");

	CFile file;

	if (file.Open(path, CFile::modeRead | CFile::shareDenyNone, 0))
	{
		int len = file.GetLength();
		if (len > 0)
		{
			char* buf = new char[len + 1];

			int count = file.Read(buf, len);

			CFMCfgParser parser(buf, count);

			CFMCfgData* data = parser.ParseData();
			if (data)
			{
				m_config = data;

				OutputDebugString(DumpConfig());
			}

			delete [] buf;
		}

		file.Close();
	}
}

void CMainFrame::SaveConfig()
{
	CString path = GetModuleDir() + _T("fmonitor2.cfg");

	CFile file;

	if (file.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, 0))
	{
		CString dump = DumpConfig();

		file.Write(static_cast<LPCTSTR>(dump), dump.GetLength());

		file.Close();
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;
	}
	else
	{
		m_wndToolBar.SetButtonStyle(6, TBBS_CHECKBOX);
		m_wndToolBar.SetButtonText(6, _T("GRAPH"));
	}

	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

void CMainFrame::OnInitMenu(CMenu* pMenu)
{
	CMDIFrameWnd::OnInitMenu(pMenu);
}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	UINT id = pPopupMenu->GetMenuItemID(0);
	if (id == ID_TOOL_SAVE_PRESET_AS)
	{
		BOOST_FOREACH(const fmcfg::Preset& preset, m_config->GetPreset())
		{
			pPopupMenu->DeleteMenu(2, MF_BYPOSITION);
		}

		int i = 0;

		BOOST_FOREACH(fmcfg::Preset& preset, m_config->GetPreset())
		{
			int id = ID_TOOL_PRESET + i++;

			preset.id = id;

			CString str;
			str.Format("Preset (%s)", preset.name.c_str());

			pPopupMenu->AppendMenu(1, id, str);

			CChildFrame* frame = (CChildFrame*)(GetActiveFrame());
			if (frame)
			{
				if (id == frame->GetPreset())
				{
					pPopupMenu->CheckMenuItem(id, MF_BYCOMMAND | MF_CHECKED);
				}
			}
		}
	}

	CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

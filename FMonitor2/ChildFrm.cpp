#include "stdafx.h"
#include <boost/foreach.hpp>
#include "FMonitor2.h"
#include "FMonitor2Doc.h"
#include "TabView.h"
#include "GraphView.h"
#include "MainFrm.h"
#include "SavePresetDialog.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOOL_ZOOMIN, &CChildFrame::OnToolZoomIn)
	ON_UPDATE_COMMAND_UI(ID_TOOL_ZOOMIN, &CChildFrame::OnUpdateToolZoomIn)
	ON_COMMAND(ID_TOOL_ZOOMOUT, &CChildFrame::OnToolZoomOut)
	ON_UPDATE_COMMAND_UI(ID_TOOL_ZOOMOUT, &CChildFrame::OnUpdateToolZoomOut)
	ON_COMMAND(ID_TOOL_ZOOMFIT, &CChildFrame::OnToolZoomFit)
	ON_UPDATE_COMMAND_UI(ID_TOOL_ZOOMFIT, &CChildFrame::OnUpdateToolZoomFit)
	ON_COMMAND(ID_TOOL_SHOW_LOGTIME, &CChildFrame::OnToolShowLogTime)
	ON_UPDATE_COMMAND_UI(ID_TOOL_SHOW_LOGTIME, &CChildFrame::OnUpdateToolShowLogTime)
	ON_COMMAND(ID_TOOL_SHOW_RULER, &CChildFrame::OnToolShowRuler)
	ON_UPDATE_COMMAND_UI(ID_TOOL_SHOW_RULER, &CChildFrame::OnUpdateToolShowRuler)
	ON_COMMAND(ID_TOOL_SAVE_PRESET_AS, &CChildFrame::OnToolSavePresetAs)
	ON_UPDATE_COMMAND_UI(ID_TOOL_SAVE_PRESET_AS, &CChildFrame::OnUpdateToolSavePresetAs)
	ON_COMMAND_RANGE(ID_TOOL_PRESET, ID_TOOL_PRESET9, &CChildFrame::OnToolPreset)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOL_PRESET, ID_TOOL_PRESET9, &CChildFrame::OnUpdateToolPreset)
END_MESSAGE_MAP()

CChildFrame::CChildFrame() : m_preset(0)
{
}

CChildFrame::~CChildFrame()
{
}

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

void CChildFrame::SetPreset(UINT nID)
{
	m_preset = nID;
}

const UINT CChildFrame::GetPreset()
{
	return m_preset;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CMDIChildWnd::PreCreateWindow(cs))
	{
		return FALSE;
	}

	return TRUE;
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

//	if (!m_status.Create(this))
//	{
//		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
//		return -1;
//	}

	return 0;
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_splitter.CreateStatic(this, 1, 2);

	m_splitter.CreateView(0, 0, RUNTIME_CLASS(CTabView), CSize(200, 10), pContext);
	m_splitter.CreateView(0, 1, RUNTIME_CLASS(CGraphView), CSize(200, 10), pContext);

	return TRUE;
}

void CChildFrame::OnToolZoomIn()
{
	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetActiveDocument());
	if (doc)
	{
		doc->SetConfigZoomFit(false);

		CFMonitor2Doc::Hint hint;
		hint.ratio = doc->GetData()->ZoomIn();

		doc->UpdateAllViews(NULL, CFMonitor2Doc::UPDATE_ZOOM_IN, (CObject*)(&hint));
	}
}

void CChildFrame::OnUpdateToolZoomIn(CCmdUI* pCmdUI)
{
	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetActiveDocument());
	if (!doc)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(doc->GetData()->CanZoomIn());
	}
}

void CChildFrame::OnToolZoomOut()
{
	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetActiveDocument());
	if (doc)
	{
		doc->SetConfigZoomFit(false);

		CFMonitor2Doc::Hint hint;
		hint.ratio = doc->GetData()->ZoomOut();

		doc->UpdateAllViews(NULL, CFMonitor2Doc::UPDATE_ZOOM_OUT, (CObject*)(&hint));
	}
}

void CChildFrame::OnUpdateToolZoomOut(CCmdUI* pCmdUI)
{
	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetActiveDocument());
	if (!doc)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(doc->GetData()->CanZoomOut());
	}
}

void CChildFrame::OnToolZoomFit()
{
	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetActiveDocument());
	if (doc)
	{
		const CFMonitor2Doc::Config* config = doc->GetConfig();

		bool br = config->zoomFit;
		doc->SetConfigZoomFit(!br);

		if (!br)
		{
			doc->UpdateAllViews(NULL, CFMonitor2Doc::UPDATE_ZOOM_FIT, NULL);
		}
	}
}

void CChildFrame::OnUpdateToolZoomFit(CCmdUI* pCmdUI)
{
	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetActiveDocument());
	if (!doc)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(doc->GetData()->CanZoomOut());

		const CFMonitor2Doc::Config* config = doc->GetConfig();
		pCmdUI->SetCheck(config->zoomFit);
	}
}

void CChildFrame::OnToolShowLogTime()
{
	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetActiveDocument());
	if (doc)
	{
		doc->UpdateAllViews(NULL, CFMonitor2Doc::UPDATE_GRAPH_STYLE, NULL);
	}
}

void CChildFrame::OnUpdateToolShowLogTime(CCmdUI* pCmdUI)
{
	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetActiveDocument());
	if (doc)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CChildFrame::OnToolShowRuler()
{
	CMainFrame* frame = (CMainFrame*)(AfxGetMainWnd());
	if (frame)
	{
		frame->ToggleFlag(_T("ruler"));
	}
}

void CChildFrame::OnUpdateToolShowRuler(CCmdUI* pCmdUI)
{
	CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetActiveDocument());
	if (doc)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CChildFrame::OnToolSavePresetAs()
{
	CSavePresetDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetActiveDocument());
		if (doc)
		{
			CFMonitor2Doc::Hint hint;
			hint.str = dlg.m_name;

			doc->UpdateAllViews(NULL, CFMonitor2Doc::UPDATE_SAVE_PRESET, (CObject*)(&hint));
		}
	}
}

void CChildFrame::OnUpdateToolSavePresetAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CChildFrame::OnToolPreset(UINT nID)
{
	CMainFrame* frame = (CMainFrame*)(AfxGetMainWnd());
	if (frame)
	{
		CFMonitor2Doc* doc = (CFMonitor2Doc*)(GetActiveDocument());
		if (doc)
		{
			SetPreset(nID);

			const fmcfg::Preset* preset = frame->GetPreset(m_preset);
			if (preset)
			{
				doc->UpdateAllViews(NULL, CFMonitor2Doc::UPDATE_RESET, (CObject*)(0));

				typedef fmcfg::Preset::List::value_type Value;

				BOOST_FOREACH(const Value& v, preset->keys)
				{
					CFMonitor2Doc::Hint hint;
					hint.id = doc->GetData()->GetIndex(v.first, v.second);
					hint.show = hint.hide = true;

					doc->UpdateAllViews(NULL, CFMonitor2Doc::UPDATE_DATA_SELECTED, (CObject*)(&hint));
				}
			}
		}
	}
}

void CChildFrame::OnUpdateToolPreset(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

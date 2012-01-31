#pragma once

#include <string>
#include <vector>
#include "LogLoader.h"

using namespace std;

class CFMonitor2Doc : public CDocument
{
	vector<string> m_filepaths;
	CFMLogData* m_pData;

protected:
	DECLARE_DYNCREATE(CFMonitor2Doc)

	CFMonitor2Doc();
	virtual ~CFMonitor2Doc();

	enum
	{
		UPDATE_RESET = 1,
		UPDATE_DATA_SELECTED,
		UPDATE_CLOSE,
		UPDATE_ZOOM_IN,
		UPDATE_ZOOM_OUT,
		UPDATE_ZOOM_FIT,
		UPDATE_ITEM_COLOR,
		UPDATE_SAVE_PRESET,
		UPDATE_GRAPH_STYLE,
	};

public:
	struct Hint
	{
		int id;
		bool show;
		bool hide;
		float ratio;
		COLORREF color;
		string str;
	};

	struct Config
	{
		bool zoomFit;
	};

	Config m_config;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	vector<string> GetFilePaths() const
	{
		return m_filepaths;
	}

	CFMLogData* GetData()
	{
		return m_pData;
	}

	const Config* GetConfig() const
	{
		return &m_config;
	}

	void SetConfigZoomFit(bool enable)
	{
		m_config.zoomFit = enable;
	}

protected:
	DECLARE_MESSAGE_MAP()

public:
	bool LoadFile(LPCTSTR lpszPathName);

public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void Serialize(CArchive& ar);
};

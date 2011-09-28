#pragma once

#include "LogLoader.h"

class CFMonitor2Doc : public CDocument
{
	HANDLE m_hFile;
	HANDLE m_hMap;
	LPVOID m_pBase;

	CFMLogData* m_pData;

protected:
	DECLARE_DYNCREATE(CFMonitor2Doc)

	CFMonitor2Doc();
	virtual ~CFMonitor2Doc();

	void CloseHandles();

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
	};

public:
	struct Hint
	{
		int id;
		float ratio;
		COLORREF color;
		string str;
	};

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CFMLogData* GetData()
	{
		return m_pData;
	}

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void Serialize(CArchive& ar);
};

#pragma once

class CSavePresetDialog : public CDialog
{
	DECLARE_DYNAMIC(CSavePresetDialog)

public:
	CString m_name;

public:
	CSavePresetDialog(CWnd* pParent = NULL);
	virtual ~CSavePresetDialog();

	virtual void OnFinalRelease();

	enum { IDD = IDD_SAVE_PRESET_AS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

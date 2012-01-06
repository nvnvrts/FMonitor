#pragma once

#include "afxwin.h"
#include "Filter.h"

class CFilterDialog : public CDialog
{
	DECLARE_DYNAMIC(CFilterDialog)

	CFilter::ActionType m_action;
	CString m_expr;

public:
	CFilterDialog(CWnd* pParent = NULL);
	virtual ~CFilterDialog();

	CFilter::ActionType GetAction() const
	{
		return m_action;
	}

	CString GetExpression() const
	{
		return m_expr;
	}

	virtual void OnFinalRelease();

	enum { IDD = IDD_FILTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	afx_msg void OnBnClickedShowRadio();
	afx_msg void OnBnClickedCloseRadio();
	afx_msg void OnBnClickedOk();
};

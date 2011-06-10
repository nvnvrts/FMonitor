#include "stdafx.h"
#include "FMonitor2.h"
#include "SavePresetDialog.h"

IMPLEMENT_DYNAMIC(CSavePresetDialog, CDialog)

CSavePresetDialog::CSavePresetDialog(CWnd* pParent /*=NULL*/)
	:CDialog(CSavePresetDialog::IDD, pParent)
	, m_name(_T(""))
{
	EnableAutomation();
}

CSavePresetDialog::~CSavePresetDialog()
{
}

void CSavePresetDialog::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDialog::OnFinalRelease();
}

void CSavePresetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PRSET_NAME_EDIT, m_name);
}

BEGIN_MESSAGE_MAP(CSavePresetDialog, CDialog)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSavePresetDialog, CDialog)
END_DISPATCH_MAP()

static const IID IID_ISavePresetDialog = { 0x503A8CDF, 0x61BE, 0x4E6C, { 0x9C, 0x6D, 0xAD, 0xCA, 0x11, 0x1D, 0xDB, 0x56 } };

BEGIN_INTERFACE_MAP(CSavePresetDialog, CDialog)
	INTERFACE_PART(CSavePresetDialog, IID_ISavePresetDialog, Dispatch)
END_INTERFACE_MAP()


#include "stdafx.h"
#include "FMonitor2.h"
#include "Filter.h"
#include "FilterDialog.h"

IMPLEMENT_DYNAMIC(CFilterDialog, CDialog)

CFilterDialog::CFilterDialog(CWnd* pParent /*=NULL*/)
	:CDialog(CFilterDialog::IDD, pParent),
	 m_expr(_T(""))
{
	EnableAutomation();
}

CFilterDialog::~CFilterDialog()
{
}

void CFilterDialog::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDialog::OnFinalRelease();
}

void CFilterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EXPR_EDIT, m_expr);
}

BEGIN_MESSAGE_MAP(CFilterDialog, CDialog)
	ON_BN_CLICKED(IDC_SHOW_RADIO, &CFilterDialog::OnBnClickedShowRadio)
	ON_BN_CLICKED(IDC_CLOSE_RADIO, &CFilterDialog::OnBnClickedCloseRadio)
	ON_BN_CLICKED(IDOK, &CFilterDialog::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFilterDialog, CDialog)
END_DISPATCH_MAP()

static const IID IID_IFilterDialog =
{ 0x54AB9921, 0x5E58, 0x423B, { 0xA5, 0x48, 0x30, 0x23, 0x8, 0x22, 0x3B, 0xC2 } };

BEGIN_INTERFACE_MAP(CFilterDialog, CDialog)
	INTERFACE_PART(CFilterDialog, IID_IFilterDialog, Dispatch)
END_INTERFACE_MAP()

void CFilterDialog::OnBnClickedShowRadio()
{
	m_action = CFilter::SHOW;
}

void CFilterDialog::OnBnClickedCloseRadio()
{
	m_action = CFilter::HIDE;
}

void CFilterDialog::OnBnClickedOk()
{
	CDialog::OnOK();
}

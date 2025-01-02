#include "StdAfx.h"
#include "LeavePartInplaceEditBar.h"

#include "clFrameUIResource.h"
#include "PartInplaceEditManager.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(LeavePartInplaceEditBar, CDialogEx)

LeavePartInplaceEditBar::LeavePartInplaceEditBar(CWnd* pParent /*=NULL*/)
	: CLDialog(LeavePartInplaceEditBar::IDD, pParent)
{
}

LeavePartInplaceEditBar::~LeavePartInplaceEditBar(void)
{
}

void LeavePartInplaceEditBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_CancelEdit, m_btnCancelEdit);
	DDX_Control(pDX, IDC_BTN_FinishEdit, m_btnFinishEdit);
}

BOOL LeavePartInplaceEditBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnCancelEdit.SetImage(IDB_LeavePartInplaceEdit_cancelEdit);
	m_btnCancelEdit.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_LeavePartInplaceEdit_cancelEdit));

	m_btnFinishEdit.SetImage(IDB_LeavePartInplaceEdit_finishEdit);
	m_btnFinishEdit.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_LeavePartInplaceEdit_finishEdit));

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

BEGIN_MESSAGE_MAP(LeavePartInplaceEditBar, CDialogEx)
	ON_WM_CTLCOLOR()

    ON_BN_CLICKED(IDC_BTN_CancelEdit, OnCancelEdit)
	ON_BN_CLICKED(IDC_BTN_FinishEdit, OnFinishEdit)
END_MESSAGE_MAP()

HBRUSH LeavePartInplaceEditBar::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void LeavePartInplaceEditBar::OnCancelEdit()
{
	PartInplaceEditManager::CancelInPlaceEditMode();
}

void LeavePartInplaceEditBar::OnFinishEdit()
{
	PartInplaceEditManager::FinishInPlaceEditMode();
}

END_CUTLEADER_NAMESPACE()

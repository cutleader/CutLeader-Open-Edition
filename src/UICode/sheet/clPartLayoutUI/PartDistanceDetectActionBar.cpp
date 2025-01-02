#include "StdAfx.h"
#include "PartDistanceDetectActionBar.h"

#include "baseConst.h"
#include "DataBaseManager.h"

#include "CLOptionInfo.h"
#include "SheetEditParam.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(PartDistanceDetectActionBar, ActionBar)

PartDistanceDetectActionBar::PartDistanceDetectActionBar(CWnd* pParent)
	: ActionBar(PartDistanceDetectActionBar::IDD, pParent)
{
}

PartDistanceDetectActionBar::~PartDistanceDetectActionBar(void)
{
}

void PartDistanceDetectActionBar::DoDataExchange(CDataExchange* pDX)
{
	ActionBar::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_EnablePartDistanceDetect, m_bEnablePartDistanceDetect);
	DDX_Check(pDX, IDC_CHECK_COMCUT, m_bComCut);
	DDX_Control(pDX, IDC_EDIT_PART_GAP, m_ctrlPartGap);
	DDX_Control(pDX, IDC_EDIT_SHT_MARGIN, m_ctrlShtMargin);
}

BOOL PartDistanceDetectActionBar::OnInitDialog()
{
	__super::OnInitDialog();

	// 创建该对话框时从系统设置获得数据。
	const SheetEditParam* pSheetEditParam = CLOptionInfo::GetInstance()->GetSheetEditParam().get();
	m_bEnablePartDistanceDetect = pSheetEditParam->EnableDistanceDetect();
	m_ctrlPartGap.Init(pSheetEditParam->GetPartDis(), 4, FALSE);
	m_ctrlShtMargin.Init(pSheetEditParam->GetSheetMargin(), 4, FALSE);
	m_bComCut = FALSE;
	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	SetWindowTheme(GetDlgItem(IDC_CHECK_EnablePartDistanceDetect)->GetSafeHwnd(), L"", L"");
	SetWindowTheme( GetDlgItem( IDC_CHECK_COMCUT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_PART_DISTANCE_DETECT )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void PartDistanceDetectActionBar::PostNcDestroy()
{
	ActionBar::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(PartDistanceDetectActionBar, ActionBar)
	ON_BN_CLICKED(IDC_CHECK_EnablePartDistanceDetect, OnCheckPartDistanceDetect)
	ON_BN_CLICKED(IDC_CHECK_COMCUT, OnCheckComcut)
END_MESSAGE_MAP()

void PartDistanceDetectActionBar::OnCheckPartDistanceDetect()
{
	UpdateData(TRUE);

	if (m_bEnablePartDistanceDetect)
	{
		((CWnd*)GetDlgItem(IDC_EDIT_PART_GAP))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_SHT_MARGIN))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_CHECK_COMCUT))->EnableWindow(TRUE);
	}
	else
	{
		((CWnd*)GetDlgItem(IDC_EDIT_PART_GAP))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_SHT_MARGIN))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_CHECK_COMCUT))->EnableWindow(FALSE);
	}
}

void PartDistanceDetectActionBar::OnCheckComcut()
{
	UpdateData(TRUE);

	if (m_bComCut)
		((CWnd*)GetDlgItem(IDC_EDIT_PART_GAP))->EnableWindow(FALSE);
	else
		((CWnd*)GetDlgItem(IDC_EDIT_PART_GAP))->EnableWindow(TRUE);
}

PartAttachConfig PartDistanceDetectActionBar::GetPartAttachConfig()
{
	UpdateData(TRUE);

    PartAttachConfig partAttachConfig(m_bEnablePartDistanceDetect, m_bComCut, m_ctrlPartGap.Get_Number(), m_ctrlShtMargin.Get_Number());
    return partAttachConfig;
}

void PartDistanceDetectActionBar::Show_PartDistanceDetectActionBar()
{
	PositionActionBar();
}

END_CUTLEADER_NAMESPACE()

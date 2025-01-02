#include "StdAfx.h"
#include "LoopPositionDetectActionBar.h"

#include "MultiLanguageMgr.h"

#include "CLOptionInfo.h"
#include "SheetEditParam.h"

#include "clCanvasGeometryUIResource.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(LoopPositionDetectActionBar, ActionBar)

LoopPositionDetectActionBar::LoopPositionDetectActionBar(CWnd* pParent)
	: ActionBar(LoopPositionDetectActionBar::IDD, pParent)
{
}

LoopPositionDetectActionBar::~LoopPositionDetectActionBar(void)
{
}

void LoopPositionDetectActionBar::DoDataExchange(CDataExchange* pDX)
{
	ActionBar::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_EnableLoopDistanceDetect, m_bEnableLoopDistanceDetect);
	DDX_Control(pDX, IDC_EDIT_LoopDistance, m_ctrlLoopDistance);
	DDX_Check(pDX, IDC_CHECK_SupportComCut, m_bComCut);

	DDX_Check(pDX, IDC_CHECK_EnableLoopAlignDetect, m_bEnableLoopAlignmentDetect);
}

BOOL LoopPositionDetectActionBar::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != m_ctrlToolTip.GetSafeHwnd())
	{
		m_ctrlToolTip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL LoopPositionDetectActionBar::OnInitDialog()
{
	__super::OnInitDialog();

	const SheetEditParam* pSheetEditParam = CLOptionInfo::GetInstance()->GetSheetEditParam().get();
	m_bEnableLoopDistanceDetect = pSheetEditParam->EnableDistanceDetect();
	m_ctrlLoopDistance.Init(pSheetEditParam->GetPartDis(), 4, FALSE);
	m_bComCut = FALSE;
	m_bEnableLoopAlignmentDetect = pSheetEditParam->EnableAlignmentDetect();

	// 设置提示信息。
	EnableToolTips(TRUE);
	m_ctrlToolTip.Create(this);
	m_ctrlToolTip.Activate(TRUE);
	m_ctrlToolTip.AddTool(GetDlgItem(IDC_CHECK_SupportComCut), MultiLanguageMgr::GetInstance()->TranslateString(IDS_LoopDistanceDetect_ComCut));

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	SetWindowTheme(GetDlgItem(IDC_CHECK_EnableLoopDistanceDetect)->GetSafeHwnd(), L"", L"");
	SetWindowTheme( GetDlgItem(IDC_CHECK_SupportComCut)->GetSafeHwnd(), L"", L"" );
	SetWindowTheme(GetDlgItem(IDC_CHECK_EnableLoopAlignDetect)->GetSafeHwnd(), L"", L"");
	SetWindowTheme( GetDlgItem(IDC_STATIC_LoopDistanceDetect)->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void LoopPositionDetectActionBar::PostNcDestroy()
{
	ActionBar::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(LoopPositionDetectActionBar, ActionBar)
	ON_BN_CLICKED(IDC_CHECK_EnableLoopDistanceDetect, OnCheckLoopDistanceDetect)
	ON_BN_CLICKED(IDC_CHECK_SupportComCut, OnCheckComCut)
	ON_BN_CLICKED(IDC_CHECK_EnableLoopAlignDetect, OnCheckLoopAlignmentDetect)
END_MESSAGE_MAP()

void LoopPositionDetectActionBar::OnCheckLoopDistanceDetect()
{
	UpdateData(TRUE);

	if (m_bEnableLoopDistanceDetect)
	{
		((CWnd*)GetDlgItem(IDC_EDIT_LoopDistance))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_CHECK_SupportComCut))->EnableWindow(TRUE);
	}
	else
	{
		((CWnd*)GetDlgItem(IDC_EDIT_LoopDistance))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_CHECK_SupportComCut))->EnableWindow(FALSE);
	}
}

void LoopPositionDetectActionBar::OnCheckComCut()
{
	UpdateData(TRUE);

	if (m_bComCut)
		((CWnd*)GetDlgItem(IDC_EDIT_LoopDistance))->EnableWindow(FALSE);
	else
		((CWnd*)GetDlgItem(IDC_EDIT_LoopDistance))->EnableWindow(TRUE);
}

void LoopPositionDetectActionBar::OnCheckLoopAlignmentDetect()
{
	UpdateData(TRUE);
}

LoopPositionConfig LoopPositionDetectActionBar::GetLoopPositionConfig()
{
	UpdateData(TRUE);

	bool bEnableLoopDistanceDetect = m_bEnableLoopDistanceDetect ? true : false;
	double dLoopDistance = m_ctrlLoopDistance.Get_Number();
	bool bEnableComCut = m_bComCut ? true : false;
	bool bEnableLoopAlignmentDetect = m_bEnableLoopAlignmentDetect ? true : false;
	LoopPositionConfig loopPositionConfig(bEnableLoopDistanceDetect, dLoopDistance, bEnableComCut, bEnableLoopAlignmentDetect);
	return loopPositionConfig;
}

void LoopPositionDetectActionBar::Show_LoopPositionDetectActionBar()
{
	PositionActionBar();
}

END_CUTLEADER_NAMESPACE()

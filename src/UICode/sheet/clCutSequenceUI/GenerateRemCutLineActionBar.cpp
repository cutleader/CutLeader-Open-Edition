#include "StdAfx.h"
#include "GenerateRemCutLineActionBar.h"

#include "baseConst.h"
#include "clCutSequenceResource.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(GenerateRemCutLineActionBar, ActionBar)

GenerateRemCutLineActionBar::GenerateRemCutLineActionBar(CWnd* pParent /*=NULL*/)
	: ActionBar(GenerateRemCutLineActionBar::IDD, pParent)
{
	m_iCmdType = GenerateRemCutLineActionBar_noCmd;
}

GenerateRemCutLineActionBar::~GenerateRemCutLineActionBar(void)
{
}

void GenerateRemCutLineActionBar::DoDataExchange(CDataExchange* pDX)
{
	ActionBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_REM_POSITION, m_comboRemnantPosition);
	DDX_Control(pDX, IDC_EDIT_OFFSET_DIS, m_ctrlOffsetDistance);
	DDX_Control(pDX, IDC_EDIT_STEP_WIDTH, m_ctrlStepWidth);
	DDX_Control( pDX, IDC_BTN_PREVIEW, m_btnPreciew );
	DDX_Control( pDX, IDC_BTN_OK, m_btnOk );
	DDX_Control( pDX, IDC_BTN_SAVE_REM_MAT, m_btnSaveRemMat );
}

BOOL GenerateRemCutLineActionBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_ctrlOffsetDistance.Init_WithoutMsg(0.0, 3, FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	SetWindowTheme( GetDlgItem( IDC_GROUP_BORDER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_REM_POSITION )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

void GenerateRemCutLineActionBar::PostNcDestroy()
{
	ActionBar::PostNcDestroy();
}

BEGIN_MESSAGE_MAP(GenerateRemCutLineActionBar, ActionBar)
	ON_BN_CLICKED(IDC_BTN_PREVIEW, OnPreview)
	ON_BN_CLICKED(IDC_BTN_SAVE_REM_MAT, OnSaveRemMat)
	ON_BN_CLICKED(IDC_BTN_OK, OnGenerate)
END_MESSAGE_MAP()

void GenerateRemCutLineActionBar::OnPreview()
{
	m_iCmdType = GenerateRemCutLineActionBar_previewCmd;
	NotifyAllObserver(INVALID_ID, CComVariant());
}

void GenerateRemCutLineActionBar::OnSaveRemMat()
{
	m_iCmdType = GenerateRemCutLineActionBar_saveMatCmd;
	NotifyAllObserver(INVALID_ID, CComVariant());
}

void GenerateRemCutLineActionBar::OnGenerate()
{
	m_iCmdType = GenerateRemCutLineActionBar_generateCmd;
	NotifyAllObserver(INVALID_ID, CComVariant());
}

void GenerateRemCutLineActionBar::Init(RECT_SIDE iRemnantPosition, double dStepWidth)
{
	m_comboRemnantPosition.ResetContent();
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GET_REM_FROM_TOP);
	m_comboRemnantPosition.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GET_REM_FROM_BOTTOM);
	m_comboRemnantPosition.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GET_REM_FROM_LEFT);
	m_comboRemnantPosition.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GET_REM_FROM_RIGHT);
	m_comboRemnantPosition.AddString(str);
	if (iRemnantPosition == RECT_TOP) {
		m_comboRemnantPosition.SetCurSel(0);
	}
	else if (iRemnantPosition == RECT_BOTTOM) {
		m_comboRemnantPosition.SetCurSel(1);
	}
	else if (iRemnantPosition == RECT_LEFT) {
		m_comboRemnantPosition.SetCurSel(2);
	}
	else if (iRemnantPosition == RECT_RIGHT) {
		m_comboRemnantPosition.SetCurSel(3);
	}

	m_ctrlStepWidth.Init(dStepWidth, 2, FALSE);
}

RECT_SIDE GenerateRemCutLineActionBar::GetRemnantPosition() const
{
	RECT_SIDE iRemnantPosition;

	if (m_comboRemnantPosition.GetCurSel() == 0) {
		iRemnantPosition = RECT_TOP;
	}
	else if (m_comboRemnantPosition.GetCurSel() == 1) {
		iRemnantPosition = RECT_BOTTOM;
	}
	else if (m_comboRemnantPosition.GetCurSel() == 2) {
		iRemnantPosition = RECT_LEFT;
	}
	else if (m_comboRemnantPosition.GetCurSel() == 3) {
		iRemnantPosition = RECT_RIGHT;
	}

	return iRemnantPosition;
}

RECT_SIDE GenerateRemCutLineActionBar::GetRayDir() const
{
	RECT_SIDE iRayDir;

	RECT_SIDE iRemnantPosition = GetRemnantPosition();
	if (iRemnantPosition == RECT_TOP) {
		iRayDir = RECT_BOTTOM;
	}
	else if (iRemnantPosition == RECT_BOTTOM) {
		iRayDir = RECT_TOP;
	}
	else if (iRemnantPosition == RECT_LEFT) {
		iRayDir = RECT_RIGHT;
	}
	else if (iRemnantPosition == RECT_RIGHT) {
		iRayDir = RECT_LEFT;
	}

	return iRayDir;
}

void GenerateRemCutLineActionBar::Show_GenerateRemCutLineActionBar()
{
	PositionActionBar();
}

END_CUTLEADER_NAMESPACE()
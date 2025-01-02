#include "stdafx.h"
#include "PartModeEditGeometryToolBar.h"

#include "LogMgr.h"
#include "ClWindowFinder.h"

#include "ClGeometryFeatureResource.h"
#include "PartView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(PartModeEditGeometryToolBar, SubToolBar)

PartModeEditGeometryToolBar::PartModeEditGeometryToolBar() : SubToolBar(PartModeEditGeometryToolBar::IDD)
{
}

PartModeEditGeometryToolBar::~PartModeEditGeometryToolBar()
{
}

void PartModeEditGeometryToolBar::DoDataExchange(CDataExchange* pDX)
{
	SubToolBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_MovePattern, m_btnMovePattern);
	DDX_Control(pDX, IDC_BTN_RotatePattern, m_btnRotatePattern);
}

BEGIN_MESSAGE_MAP(PartModeEditGeometryToolBar, SubToolBar)
	ON_BN_CLICKED(IDC_BTN_MovePattern, OnMovePattern)
	ON_BN_CLICKED(IDC_BTN_RotatePattern, OnRotatePattern)
END_MESSAGE_MAP()

BOOL PartModeEditGeometryToolBar::OnInitDialog()
{
	__super::OnInitDialog();

	m_btnMovePattern.SetImage(IDB_MovePart_32);
	m_btnMovePattern.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MOVE_GEOM_TIP));

	m_btnRotatePattern.SetImage(IDB_RotatePart_32);
	m_btnRotatePattern.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_ROTATE_GEOM_TIP));

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void PartModeEditGeometryToolBar::OnMovePattern()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnMovePattern();
	this->ShowWindow(SW_HIDE);
}

void PartModeEditGeometryToolBar::OnRotatePattern()
{
	((PartView*)ClWindowFinder::GetCurrentView_1())->OnRotatePattern();
	this->ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

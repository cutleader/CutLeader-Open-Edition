#include "stdafx.h"
#include "SheetEditParamPage.h"

#include "baseConst.h"

#include "SheetEditParam.h"
#include "clOptionResource.h"

#include "SkinManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const size_t MAX_TIP_TEXT_LENGTH = 1024;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(SheetEditParamPage, CResizableDialog)

SheetEditParamPage::SheetEditParamPage() : CLResizableDialog( SheetEditParamPage::IDD )
{
}

SheetEditParamPage::~SheetEditParamPage()
{
}

void SheetEditParamPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	// 步进
	DDX_Control(pDX, IDC_EDIT_MOVE_STEP, m_editMoveStep);
	DDX_Control(pDX, IDC_EDIT_ROTATE_STEP, m_editRotStep);

	// 零件距离检测。
	DDX_Check(pDX, IDC_CHECK_DETECT, m_bDetectPartDis);
	DDX_Check(pDX, IDC_CHECK_DetectAlignment, m_bDetectAlignment);
	DDX_Check(pDX, IDC_CHECK_DETECT_COM, m_bDetectCommon);
	DDX_Control(pDX, IDC_EDIT_SHT_MARGIN, m_editSheetMargin);
	DDX_Control(pDX, IDC_EDIT_PART_GAP, m_editPartDis);
	DDX_Control(pDX, IDC_EDIT_STICK_DIS, m_editStickDis);
	DDX_Control(pDX, IDC_EDIT_ESCAPE_DIS, m_editEscapeDis);

	// 光顺控制。
	DDX_Check(pDX, IDC_CHECK_SLEEK, m_bSleekCtrl);
	DDX_Control(pDX, IDC_EDIT_SLEEK_RATE, m_editSleekRate);

	// 工序。
	DDX_Control(pDX, IDC_EDIT_LOOP_DETECT_DIS, m_editLoopDetectDis);
}

BEGIN_MESSAGE_MAP(SheetEditParamPage, CResizableDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL SheetEditParamPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	/************************************************************************/
	// layout controls.

	// 步进。
	AddAnchor(IDC_GROUP_STEP, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_MOVE_STEP, TOP_LEFT);
	AddAnchor(IDC_EDIT_MOVE_STEP, TOP_LEFT);
	AddAnchor(IDC_STATIC_ROTATE_STEP, TOP_LEFT);
	AddAnchor(IDC_EDIT_ROTATE_STEP, TOP_LEFT);

	// 零件距离检测。
	AddAnchor(IDC_GROUP_DETECT, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_CHECK_DETECT, TOP_LEFT);
	AddAnchor(IDC_CHECK_DetectAlignment, TOP_LEFT);
	AddAnchor(IDC_CHECK_DETECT_COM, TOP_LEFT);
	AddAnchor(IDC_TXT_SHT_MARGIN, TOP_LEFT);
	AddAnchor(IDC_EDIT_SHT_MARGIN, TOP_LEFT);
	AddAnchor(IDC_TXT_PART_DIS, TOP_LEFT);
	AddAnchor(IDC_EDIT_PART_GAP, TOP_LEFT);
	AddAnchor(IDC_TXT_STICK_DIS, TOP_LEFT);
	AddAnchor(IDC_EDIT_STICK_DIS, TOP_LEFT);
	AddAnchor(IDC_STATIC_Pixels_1, TOP_LEFT);
	AddAnchor(IDC_TXT_ESCAPE_DIS, TOP_LEFT);
	AddAnchor(IDC_EDIT_ESCAPE_DIS, TOP_LEFT);
	AddAnchor(IDC_STATIC_Pixels_2, TOP_LEFT);

	// 光顺控制。
	AddAnchor(IDC_GROUP_SLEEK, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_CHECK_SLEEK, TOP_LEFT);
	AddAnchor(IDC_TXT_SLEEK_RATE, TOP_LEFT);
	AddAnchor(IDC_EDIT_SLEEK_RATE, TOP_LEFT);

	// 工序。
	AddAnchor(IDC_GROUP_SEQUENCE, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_TXT_LOOP_DETECT_DIS, TOP_LEFT);
	AddAnchor(IDC_EDIT_LOOP_DETECT_DIS, TOP_LEFT);
	/************************************************************************/

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_GROUP_STEP )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_DETECT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_DETECT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme(GetDlgItem(IDC_CHECK_DetectAlignment)->GetSafeHwnd(), L"", L"");
	SetWindowTheme( GetDlgItem( IDC_CHECK_DETECT_COM )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_SLEEK )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SLEEK )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_SEQUENCE )->GetSafeHwnd(), L"", L"" );
	
	return TRUE;
}

HBRUSH SheetEditParamPage::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void SheetEditParamPage::DisplaySheetEditParam(SheetEditParamPtr displayInfoPtr)
{
	m_pSheetEditParam = displayInfoPtr;

	m_editMoveStep.Init(m_pSheetEditParam->GetMoveStep(), DIGITAL_NUM, FALSE);
	m_editRotStep.Init(m_pSheetEditParam->GetRotateStep(), DIGITAL_NUM, FALSE);

	m_bDetectPartDis = m_pSheetEditParam->EnableDistanceDetect();
	m_bDetectAlignment = m_pSheetEditParam->EnableAlignmentDetect();
	m_bDetectCommon = m_pSheetEditParam->EnableComDetect();
	m_editSheetMargin.Init(m_pSheetEditParam->GetSheetMargin(), DIGITAL_NUM, FALSE);
	m_editPartDis.Init(m_pSheetEditParam->GetPartDis(), DIGITAL_NUM, FALSE);
	m_editStickDis.Init(m_pSheetEditParam->GetStickDis(), 0, FALSE);
	m_editEscapeDis.Init(m_pSheetEditParam->GetEscapeDis(), 0, FALSE);

	m_bSleekCtrl = m_pSheetEditParam->EnableSleekCtrl();
	m_editSleekRate.Init(m_pSheetEditParam->GetSleekRate(), DIGITAL_NUM, FALSE);

	m_editLoopDetectDis.Init(m_pSheetEditParam->GetLoopDetectDis(), 0, FALSE);

	UpdateData(FALSE);
}

SheetEditParamPtr SheetEditParamPage::GetSheetEditParam()
{
	UpdateData(TRUE);

	if (m_editMoveStep.Get_Number() != m_pSheetEditParam->GetMoveStep())
	{
		m_pSheetEditParam->SetMoveStep(m_editMoveStep.Get_Number());
		m_pSheetEditParam->SetModified(TRUE);
	}
	if (m_editRotStep.Get_Number() != m_pSheetEditParam->GetRotateStep())
	{
		m_pSheetEditParam->SetRotateStep(m_editRotStep.Get_Number());
		m_pSheetEditParam->SetModified(TRUE);
	}

	if (m_bDetectPartDis != m_pSheetEditParam->EnableDistanceDetect())
	{
		m_pSheetEditParam->EnableDistanceDetect(m_bDetectPartDis);
		m_pSheetEditParam->SetModified(TRUE);
	}
	if (m_bDetectAlignment != m_pSheetEditParam->EnableAlignmentDetect())
	{
		m_pSheetEditParam->EnableAlignmentDetect(m_bDetectAlignment);
		m_pSheetEditParam->SetModified(TRUE);
	}
	if (m_bDetectCommon != m_pSheetEditParam->EnableComDetect())
	{
		m_pSheetEditParam->EnableComDetect(m_bDetectCommon);
		m_pSheetEditParam->SetModified(TRUE);
	}
	if (m_editSheetMargin.Get_Number() != m_pSheetEditParam->GetSheetMargin())
	{
		m_pSheetEditParam->SetSheetMargin(m_editSheetMargin.Get_Number());
		m_pSheetEditParam->SetModified(TRUE);
	}
	if (m_editPartDis.Get_Number() != m_pSheetEditParam->GetPartDis())
	{
		m_pSheetEditParam->SetPartDis(m_editPartDis.Get_Number());
		m_pSheetEditParam->SetModified(TRUE);
	}
	if ((int)m_editStickDis.Get_Number() != m_pSheetEditParam->GetStickDis())
	{
		m_pSheetEditParam->SetStickDis((int)m_editStickDis.Get_Number());
		m_pSheetEditParam->SetModified(TRUE);
	}
	if ((int)m_editEscapeDis.Get_Number() != m_pSheetEditParam->GetEscapeDis())
	{
		m_pSheetEditParam->SetEscapeDis((int)m_editEscapeDis.Get_Number());
		m_pSheetEditParam->SetModified(TRUE);
	}

	if (m_bSleekCtrl != m_pSheetEditParam->EnableSleekCtrl())
	{
		m_pSheetEditParam->EnableSleekCtrl(m_bSleekCtrl);
		m_pSheetEditParam->SetModified(TRUE);
	}
	if (m_editSleekRate.Get_Number() != m_pSheetEditParam->GetSleekRate())
	{
		m_pSheetEditParam->SetSleekRate(m_editSleekRate.Get_Number());
		m_pSheetEditParam->SetModified(TRUE);
	}

	if (m_editLoopDetectDis.Get_Number() != m_pSheetEditParam->GetLoopDetectDis())
	{
		m_pSheetEditParam->SetLoopDetectDis((int)m_editLoopDetectDis.Get_Number());
		m_pSheetEditParam->SetModified(TRUE);
	}

	return m_pSheetEditParam;
}

END_CUTLEADER_NAMESPACE()

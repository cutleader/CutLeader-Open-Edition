#include "StdAfx.h"
#include "CornerAssignPage.h"

#include "baseConst.h"
#include "clExpertLibResource.h"
#include "CornerConfigItem.h"
#include "CornerRangeInfo.h"
#include <math.h>
#include "SkinManager.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CornerAssignPage, CResizableDialog)

CornerAssignPage::CornerAssignPage(void)
					 : CLResizableDialog(CornerAssignPage::IDD)
{
	m_strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CornerAssignRule_2);
}

CornerAssignPage::~CornerAssignPage(void)
{
}

void CornerAssignPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MSG, m_strMsg);

	DDX_Control(pDX, IDC_LIST_RANGETYPE, m_rangeType);
    DDX_Check(pDX, IDC_CHECK_cornerLineLengthControl, m_bEnableCornerLineLengthControl);
    DDX_Control(pDX, IDC_EDIT_cornerLineLength, m_dCornerLineLength);

	DDX_Control(pDX, IDC_STATIC_COR, m_cornerPreview);
	DDX_Control(pDX, IDC_COMBO_CORNERTYPE, m_iCornerType);
	DDX_Control(pDX, IDC_EDIT_CORNERVAL, m_cornerVal);

	DDX_Control(pDX, IDC_COMBO_MANUAL_CORNERTYPE, m_manualCornerType);
	DDX_Control(pDX, IDC_EDIT_MANUAL_CORNERVAL, m_manualCornerVal);
}

BEGIN_MESSAGE_MAP(CornerAssignPage, CResizableDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()

	ON_LBN_SELCHANGE(IDC_LIST_RANGETYPE, OnSelchangeRangeType)
    ON_BN_CLICKED(IDC_CHECK_cornerLineLengthControl, OnCheckCornerLineLength)
    ON_EN_KILLFOCUS(IDC_EDIT_cornerLineLength, OnKillCornerLineLength)

	ON_CBN_SELCHANGE(IDC_COMBO_CORNERTYPE, OnSelchangeCornerType)
	ON_EN_KILLFOCUS(IDC_EDIT_CORNERVAL, OnKillInCornerVal)

	ON_CBN_SELCHANGE(IDC_COMBO_MANUAL_CORNERTYPE, OnSelchangeMCornerType)
	ON_EN_KILLFOCUS(IDC_EDIT_MANUAL_CORNERVAL, OnKillInMCornerVal)
END_MESSAGE_MAP()

BOOL CornerAssignPage::OnInitDialog()
{
	__super::OnInitDialog();

	m_cornerPreview.InitDrawer();

	LayoutCtrl();
	InitRangeList();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_AUTO_ASSIGN )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_EDIT_MSG )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_cornerLineLengthControl )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_cornerFeature_definition )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_CORNERTYPE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_MANUAL_ASSIGN )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_MANUAL_CORNERTYPE )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

void CornerAssignPage::OnPaint()
{
	CPaintDC dc(this);
	m_cornerPreview.OnPaint();
}

HBRUSH CornerAssignPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void CornerAssignPage::OnSelchangeRangeType()
{
	int iIndex = m_rangeType.GetCurSel();
	if (iIndex >=0)
	{
		InitByRange((CORNER_RANGE_TYPE)iIndex);

		// update the preview window
		CornerRangeInfoPtr pRangeCornerInfo = GetCurRangeCorner();
		m_cornerPreview.SetRangeCorner(pRangeCornerInfo);
		m_cornerPreview.OnPaint();

		UpdateData(FALSE);
	}
}

void CornerAssignPage::OnCheckCornerLineLength()
{
    UpdateData(TRUE);

    CornerRangeInfoPtr pRangeCornerInfo = GetCurRangeCorner();
    if (m_bEnableCornerLineLengthControl != pRangeCornerInfo->EnableCornerLineLengthControl())
    {
        pRangeCornerInfo->EnableCornerLineLengthControl(m_bEnableCornerLineLengthControl);
        m_pCornerConfigItem->SetModified(TRUE);
    }
}

void CornerAssignPage::OnSelchangeCornerType()
{
	CornerRangeInfoPtr pRangeCornerInfo = GetCurRangeCorner();

    CornerType iSelectedCornerType = (CornerType)(int)m_iCornerType.GetItemData(m_iCornerType.GetCurSel());
	if (pRangeCornerInfo->GetCornerType() != iSelectedCornerType)
	{
		pRangeCornerInfo->SetCornerType((CornerType)m_iCornerType.GetCurSel());
		m_pCornerConfigItem->SetModified(TRUE);
	}

	// update the preview window
	m_cornerPreview.SetRangeCorner(pRangeCornerInfo);
	m_cornerPreview.OnPaint();
}

void CornerAssignPage::OnKillInCornerVal()
{
	CornerRangeInfoPtr pRangeCornerInfo = GetCurRangeCorner();

	if (pRangeCornerInfo->GetCornerValue() != m_cornerVal.Get_Number())
	{
		pRangeCornerInfo->SetCornerValue(m_cornerVal.Get_Number());
		m_pCornerConfigItem->SetModified(TRUE);
	}

	// update the preview window
	m_cornerPreview.SetRangeCorner(pRangeCornerInfo);
	m_cornerPreview.OnPaint();
}

void CornerAssignPage::OnSelchangeMCornerType()
{
	int iIndex = m_manualCornerType.GetCurSel();
	if (m_pCornerConfigItem->GetDflCornerType() != (CornerType)(iIndex+1))
	{
		m_pCornerConfigItem->SetDflCornerType((CornerType)(iIndex+1));
		m_pCornerConfigItem->SetModified(TRUE);
	}
}

void CornerAssignPage::OnKillInMCornerVal()
{
	if (fabs(m_pCornerConfigItem->GetDflCornerVal() - m_manualCornerVal.Get_Number()) > GEOM_TOLERANCE)
	{
		m_pCornerConfigItem->SetDflCornerVal(m_manualCornerVal.Get_Number());
		m_pCornerConfigItem->SetModified(TRUE);
	}
}

void CornerAssignPage::OnKillCornerLineLength()
{
    CornerRangeInfoPtr pRangeCornerInfo = GetCurRangeCorner();
    if (fabs(pRangeCornerInfo->GetCornerLineLength() - m_dCornerLineLength.Get_Number()) > GEOM_TOLERANCE)
    {
        pRangeCornerInfo->SetCornerLineLength(m_dCornerLineLength.Get_Number());
        m_pCornerConfigItem->SetModified(TRUE);
    }
}

void CornerAssignPage::DisplayCornerConfigItem(CornerConfigItemPtr pCornerConfigItem)
{
	m_pCornerConfigItem = pCornerConfigItem;

	// 初始化自动分配分组。
	m_rangeType.SetCurSel(0);
	InitByRange((CORNER_RANGE_TYPE)0);
	CornerRangeInfoPtr pRangeCornerInfo = GetCurRangeCorner();
	m_cornerPreview.SetRangeCorner(pRangeCornerInfo);
	m_cornerPreview.OnPaint();

	// 初始化手工分配分组。
	InitManualGroup();

    UpdateData(FALSE);
}

void CornerAssignPage::LayoutCtrl()
{
	AddAnchor(IDC_STATIC_AUTO_ASSIGN, TOP_LEFT);

	// "corner range" group.
	AddAnchor(IDC_STATIC_cornerCondition, TOP_LEFT);
	AddAnchor(IDC_STATIC_CORNER_ANGLE_RANGE, TOP_LEFT);
	AddAnchor(IDC_LIST_RANGETYPE, TOP_LEFT);
	AddAnchor(IDC_CHECK_cornerLineLengthControl, TOP_LEFT);
	AddAnchor(IDC_STATIC_cornerLineLength, TOP_LEFT);
    AddAnchor(IDC_EDIT_cornerLineLength, TOP_LEFT);

	// "corner param" group.
    AddAnchor(IDC_STATIC_cornerFeature_definition, TOP_LEFT);
	AddAnchor(IDC_STATIC_TYPE, TOP_LEFT);
	AddAnchor(IDC_COMBO_CORNERTYPE, TOP_LEFT);
	AddAnchor(IDC_STATIC_VAL, TOP_LEFT);
	AddAnchor(IDC_EDIT_CORNERVAL, TOP_LEFT);

	// "corner preview" group.
	AddAnchor(IDC_STATIC_INFO, TOP_LEFT);
	AddAnchor(IDC_EDIT_MSG, TOP_LEFT);
	AddAnchor(IDC_STATIC_COR, TOP_LEFT);

	// 手动分配。
	AddAnchor(IDC_STATIC_MANUAL_ASSIGN, TOP_LEFT);
	AddAnchor(IDC_STATIC_MANUAL_TYPE, TOP_LEFT);
	AddAnchor(IDC_COMBO_MANUAL_CORNERTYPE, TOP_LEFT);
	AddAnchor(IDC_STATIC_MANUAL_VAL, TOP_LEFT);
	AddAnchor(IDC_EDIT_MANUAL_CORNERVAL, TOP_LEFT);
}

void CornerAssignPage::InitRangeList()
{
	m_rangeType.InsertString(0, _T("0 - 90"));
	m_rangeType.InsertString(1, _T("90"));
	m_rangeType.InsertString(2, _T("90 - 180"));
	m_rangeType.InsertString(3, _T("180 - 270"));
	m_rangeType.InsertString(4, _T("270"));
	m_rangeType.InsertString(5, _T("270 - 360"));

	// set focus to the first one.
	m_rangeType.SetCurSel(0);
}

void CornerAssignPage::InitByRange( CORNER_RANGE_TYPE rangeType )
{
	vector<CornerRangeInfoPtr> *pRangeCornerInfoList = m_pCornerConfigItem->GetRangeCornerInfoList();
	CornerRangeInfoPtr pRangeCornerInfo = pRangeCornerInfoList->at( (int)rangeType );

    // 角上线长度相关
	m_bEnableCornerLineLengthControl = pRangeCornerInfo->EnableCornerLineLengthControl();
	m_dCornerLineLength.Init( pRangeCornerInfo->GetCornerLineLength(), DIGITAL_NUM, FALSE );

	// init corner type combo box.
    {
        m_iCornerType.ResetContent();

        //
        CString str = MultiLanguageMgr::GetInstance()->TranslateString( IDS_CORNERTYPE_NONE );
        m_iCornerType.AddString( str );
        m_iCornerType.SetItemData( m_iCornerType.GetCount() - 1, (int)Corner_None );
        //
        str = MultiLanguageMgr::GetInstance()->TranslateString( IDS_CORNERTYPE_CROSS );
        m_iCornerType.AddString(str);
        m_iCornerType.SetItemData( m_iCornerType.GetCount() - 1, (int)Corner_Cross );
        //
        str = MultiLanguageMgr::GetInstance()->TranslateString( IDS_CORNERTYPE_RADIUS );
        m_iCornerType.AddString( str );
        m_iCornerType.SetItemData( m_iCornerType.GetCount() - 1, (int)Corner_Radius );
        //
        str = MultiLanguageMgr::GetInstance()->TranslateString( IDS_CORNERTYPE_TAB );
        m_iCornerType.AddString( str );
        m_iCornerType.SetItemData( m_iCornerType.GetCount() - 1, (int)Corner_Tab );
        //
        str = MultiLanguageMgr::GetInstance()->TranslateString( IDS_CORNERTYPE_LOOP );
        m_iCornerType.AddString( str );
        m_iCornerType.SetItemData( m_iCornerType.GetCount() - 1, (int)Corner_Loop );
        //
        str = MultiLanguageMgr::GetInstance()->TranslateString( IDS_CORNERTYPE_CoolingPoint );
        m_iCornerType.AddString( str );
        m_iCornerType.SetItemData( m_iCornerType.GetCount() - 1, (int)Corner_CoolingPoint );

        // 设置当前选中项
        for (int i = 0; i < m_iCornerType.GetCount(); i++)
		{
            DWORD_PTR pVal = m_iCornerType.GetItemData( i );
            if ( (int)pVal == (int)pRangeCornerInfo->GetCornerType() )
			{
                m_iCornerType.SetCurSel( i );
                break;
            }
        }
    }

	// set the corner value.
	m_cornerVal.Init( pRangeCornerInfo->GetCornerValue(), DIGITAL_NUM, FALSE );

	// show ComboBox
	m_iCornerType.Invalidate( TRUE );
}

CornerRangeInfoPtr CornerAssignPage::GetCurRangeCorner()
{
	vector<CornerRangeInfoPtr>* pRangeCornerInfoList = m_pCornerConfigItem->GetRangeCornerInfoList();

	int iRangeIndex = m_rangeType.GetCurSel();
	CornerRangeInfoPtr pRangeCornerInfo = pRangeCornerInfoList->at((int)iRangeIndex);

	return pRangeCornerInfo;
}

void CornerAssignPage::InitManualGroup()
{
	CString str;

	// init corner type combo box.
	m_manualCornerType.ResetContent();
	str = MultiLanguageMgr::GetInstance()->TranslateString( IDS_CORNERTYPE_CROSS );
	m_manualCornerType.AddString( str );
	str = MultiLanguageMgr::GetInstance()->TranslateString( IDS_CORNERTYPE_RADIUS );
	m_manualCornerType.AddString( str );
	str = MultiLanguageMgr::GetInstance()->TranslateString( IDS_CORNERTYPE_TAB );
	m_manualCornerType.AddString( str );
	str = MultiLanguageMgr::GetInstance()->TranslateString( IDS_CORNERTYPE_LOOP );
	m_manualCornerType.AddString( str );
	CornerType iType = m_pCornerConfigItem->GetDflCornerType();
	m_manualCornerType.SetCurSel( (int)iType - 1 );

	// set the corner value
	m_manualCornerVal.Init( m_pCornerConfigItem->GetDflCornerVal(), DIGITAL_NUM, FALSE );

	// show ComboBox
	m_manualCornerType.Invalidate( TRUE );
}

END_CUTLEADER_NAMESPACE()

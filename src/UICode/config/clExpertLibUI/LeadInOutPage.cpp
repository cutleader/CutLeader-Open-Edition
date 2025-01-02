#include "stdafx.h"
#include "LeadInOutPage.h"

#include "MathEx.h"

#include "CLOptionInfo.h"
#include "ProductParam.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(LeadInOutPage, CResizableDialog)

LeadInOutPage::LeadInOutPage() : CLResizableDialog(LeadInOutPage::IDD)
{
}

LeadInOutPage::~LeadInOutPage()
{
}

void LeadInOutPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB_INEXT, m_tabCtrl);
	DDX_Control(pDX, IDC_STATIC_LEAD, m_leadPreviewWnd);

	// “引入”分组。
	DDX_Control(pDX, IDC_COMBO_IN_TYPE, m_leadInType);
	DDX_Control(pDX, IDC_EDIT_IN_LENGTH, m_dInLength);
	DDX_Control(pDX, IDC_EDIT_IN_ANGLE, m_dInAngle);
	DDX_Control(pDX, IDC_EDIT_IN_ARCR, m_dInArcRadius);
	DDX_Control(pDX, IDC_EDIT_IN_ARCA, m_dInArcAngle);
	DDX_Control(pDX, IDC_EDIT_IN_SPLIT, m_dInLineSplit);

	// “引出”分组。
	DDX_Control(pDX, IDC_COMBO_OUT_TYPE, m_leadOutType);
	DDX_Control(pDX, IDC_EDIT_OUT_LENGTH, m_dOutLength);
	DDX_Control(pDX, IDC_EDIT_OUT_ANGLE, m_dOutAngle);
	DDX_Control(pDX, IDC_EDIT_OUT_ARCR, m_dOutArcRadius);
	DDX_Control(pDX, IDC_EDIT_OUT_ARCA, m_dOutArcAngle);
	DDX_Control(pDX, IDC_EDIT_OUT_CUT, m_dOutOverCut);

	// “角引入引出”分组。
	DDX_Control(pDX, IDC_EDIT_CORNER_LINE_LEN_LEADIN, m_dCornerLineLen_leadIn);
	DDX_Control(pDX, IDC_EDIT_CORNER_ARC_ANG_LEADIN, m_dCornerArcAngle_leadIn);
    DDX_Control(pDX, IDC_EDIT_CORNER_LINE_LEN_LEADOUT, m_dCornerLineLen_leadOut);
    DDX_Control(pDX, IDC_EDIT_CORNER_ARC_ANG_LEADOUT, m_dCornerArcAngle_leadOut);
    DDX_Control(pDX, IDC_EDIT_CORNER_OVERCUT, m_dCornerOverCut_leadOut);
}

BEGIN_MESSAGE_MAP(LeadInOutPage, CResizableDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_INEXT, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_INEXT, OnSelchangingTab)

	// “引入”分组。
	ON_CBN_SELCHANGE(IDC_COMBO_IN_TYPE, OnSelchangeInType)
	ON_EN_KILLFOCUS(IDC_EDIT_IN_LENGTH, OnKillInLength)
	ON_EN_KILLFOCUS(IDC_EDIT_IN_ANGLE, OnKillInAngle)
	ON_EN_KILLFOCUS(IDC_EDIT_IN_ARCR, OnKillInARCR)
	ON_EN_KILLFOCUS(IDC_EDIT_IN_ARCA, OnKillInARCA)
	ON_EN_KILLFOCUS(IDC_EDIT_IN_SPLIT, OnKillInSplit)

	// “引出”分组。
	ON_CBN_SELCHANGE(IDC_COMBO_OUT_TYPE, OnSelchangeOutType)
	ON_EN_KILLFOCUS(IDC_EDIT_OUT_LENGTH, OnKillOutLength)
	ON_EN_KILLFOCUS(IDC_EDIT_OUT_ANGLE, OnKillOutAngle)
	ON_EN_KILLFOCUS(IDC_EDIT_OUT_ARCR, OnKillOutARCR)
	ON_EN_KILLFOCUS(IDC_EDIT_OUT_ARCA, OnKillOutARCA)
	ON_EN_KILLFOCUS(IDC_EDIT_OUT_CUT, OnKillOutCut)

	// “角引入引出”分组。
	ON_EN_KILLFOCUS(IDC_EDIT_CORNER_LINE_LEN_LEADIN, OnKillCornerInLen_leadIn)
	ON_EN_KILLFOCUS(IDC_EDIT_CORNER_ARC_ANG_LEADIN, OnKillCornerArcAng_leadIn)
    ON_EN_KILLFOCUS(IDC_EDIT_CORNER_LINE_LEN_LEADOUT, OnKillCornerInLen_leadOut)
    ON_EN_KILLFOCUS(IDC_EDIT_CORNER_ARC_ANG_LEADOUT, OnKillCornerArcAng_leadOut)
    ON_EN_KILLFOCUS(IDC_EDIT_CORNER_OVERCUT, OnKillCornerOverCut_leadOut)
END_MESSAGE_MAP()

BOOL LeadInOutPage::OnInitDialog()
{
	__super::OnInitDialog();

	m_leadPreviewWnd.InitDrawer();

	// layout controls.
	LayoutCtrl();

	// init something
	Init();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_GROUP_GEOM )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_LEADIN )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_IN_TYPE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_LEADOUT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_OUT_TYPE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_CORNER_LEADIN )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_CORNER_LEADOUT )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

void LeadInOutPage::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_tabCtrl.GetCurSel() == 0)
	{
		/************************************************************************/
		// init lead in group
		LeadInOutDataPtr pInnerLeadIn = m_pLeadSetting->GetInnerLeadInData();
		InitLeadInGroup(pInnerLeadIn);

		// init lead out group
		LeadInOutDataPtr pInnerLeadOut = m_pLeadSetting->GetInnerLeadOutData();
		InitLeadOutGroup(pInnerLeadOut);
		/************************************************************************/
	}
	else if (m_tabCtrl.GetCurSel() == 1)
	{
		/************************************************************************/
		// init lead in group
		LeadInOutDataPtr pOuterLeadIn = m_pLeadSetting->GetOuterLeadInData();
		InitLeadInGroup(pOuterLeadIn);

		// init lead out group
		LeadInOutDataPtr pOutLeadOut = m_pLeadSetting->GetOuterLeadOutData();
		InitLeadOutGroup(pOutLeadOut);
		/************************************************************************/
	}
}

void LeadInOutPage::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetFocus();
}

void LeadInOutPage::OnSelchangeInType()
{
	// get the selected lead type.
	LeadType leadType;
	int iIndex = m_leadInType.GetCurSel();
	if (iIndex == 0)
		leadType = Lead_Line;
	else if (iIndex == 1)
		leadType = Lead_TwoLines;
	else if (iIndex == 2)
		leadType = Lead_LineArc;
	else if (iIndex == 3)
		leadType = Lead_Hook;
	else if (iIndex == 4)
		leadType = Lead_None;

	// update the lead in and the preview window
	int iTabIndex = m_tabCtrl.GetCurSel();
	if (iTabIndex == 0)
	{
		// internal

		LeadInOutDataPtr pInnerLeadIn = m_pLeadSetting->GetInnerLeadInData();
		if (pInnerLeadIn->GetType() != leadType)
		{
			pInnerLeadIn->SetType(leadType);
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		InitLeadInGroup(pInnerLeadIn);
		m_leadPreviewWnd.UpdateLeadIn(pInnerLeadIn);
	}
	else if (iTabIndex == 1)
	{
		// external

		LeadInOutDataPtr pOuterLeadIn = m_pLeadSetting->GetOuterLeadInData();
		if (pOuterLeadIn->GetType() != leadType)
		{
			pOuterLeadIn->SetType(leadType);
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		InitLeadInGroup(pOuterLeadIn);
		m_leadPreviewWnd.UpdateLeadIn(pOuterLeadIn);
	}

	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::OnKillInLength()
{
	if (m_tabCtrl.GetCurSel() == 0)
	{
		LeadInOutDataPtr pInnerLeadIn = m_pLeadSetting->GetInnerLeadInData();
		if (!MathEx::Equal(pInnerLeadIn->GetLength(), m_dInLength.Get_Number()))
		{
			pInnerLeadIn->SetLength(m_dInLength.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}
		
		//
		m_leadPreviewWnd.UpdateLeadIn(pInnerLeadIn);
	}
	else
	{
		LeadInOutDataPtr pOuterLeadIn = m_pLeadSetting->GetOuterLeadInData();
		if (!MathEx::Equal(pOuterLeadIn->GetLength(), m_dInLength.Get_Number()))
		{
			pOuterLeadIn->SetLength(m_dInLength.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}
		
		//
		m_leadPreviewWnd.UpdateLeadIn(pOuterLeadIn);
	}

	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::OnKillInAngle()
{
	if (m_tabCtrl.GetCurSel() == 0)
	{
		LeadInOutDataPtr pInnerLeadIn = m_pLeadSetting->GetInnerLeadInData();
		if (!MathEx::Equal(pInnerLeadIn->GetAngle(), m_dInAngle.Get_Number()))
		{
			pInnerLeadIn->SetAngle(m_dInAngle.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		m_leadPreviewWnd.UpdateLeadIn(pInnerLeadIn);
	}
	else
	{
		LeadInOutDataPtr pOuterLeadIn = m_pLeadSetting->GetOuterLeadInData();
		if (!MathEx::Equal(pOuterLeadIn->GetAngle(), m_dInAngle.Get_Number()))
		{
			pOuterLeadIn->SetAngle(m_dInAngle.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		m_leadPreviewWnd.UpdateLeadIn(pOuterLeadIn);
	}

	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::OnKillInARCR()
{
	if (m_tabCtrl.GetCurSel() == 0)
	{
		LeadInOutDataPtr pInnerLeadIn = m_pLeadSetting->GetInnerLeadInData();
		if (!MathEx::Equal(pInnerLeadIn->GetArcRadius(), m_dInArcRadius.Get_Number()))
		{
			pInnerLeadIn->SetArcRadius(m_dInArcRadius.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		m_leadPreviewWnd.UpdateLeadIn(pInnerLeadIn);
	}
	else
	{
		LeadInOutDataPtr pOuterLeadIn = m_pLeadSetting->GetOuterLeadInData();
		if (!MathEx::Equal(pOuterLeadIn->GetArcRadius(), m_dInArcRadius.Get_Number()))
		{
			pOuterLeadIn->SetArcRadius(m_dInArcRadius.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		m_leadPreviewWnd.UpdateLeadIn(pOuterLeadIn);
	}

	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::OnKillInARCA()
{
	if (m_tabCtrl.GetCurSel() == 0)
	{
		LeadInOutDataPtr pInnerLeadIn = m_pLeadSetting->GetInnerLeadInData();
		if (!MathEx::Equal(pInnerLeadIn->GetArcAngle(), m_dInArcAngle.Get_Number()))
		{
			pInnerLeadIn->SetArcAngle(m_dInArcAngle.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		m_leadPreviewWnd.UpdateLeadIn(pInnerLeadIn);
	}
	else
	{
		LeadInOutDataPtr pOuterLeadIn = m_pLeadSetting->GetOuterLeadInData();
		if (!MathEx::Equal(pOuterLeadIn->GetArcAngle(), m_dInArcAngle.Get_Number()))
		{
			pOuterLeadIn->SetArcAngle(m_dInArcAngle.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		m_leadPreviewWnd.UpdateLeadIn(pOuterLeadIn);
	}

	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::OnKillInSplit()
{
	if (m_tabCtrl.GetCurSel() == 0)
	{
		LeadInOutDataPtr pInnerLeadIn = m_pLeadSetting->GetInnerLeadInData();
		if (!MathEx::Equal(pInnerLeadIn->GetLineSplit(), m_dInLineSplit.Get_Number()))
		{
			pInnerLeadIn->SetLineSplit(m_dInLineSplit.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		m_leadPreviewWnd.UpdateLeadIn(pInnerLeadIn);
	}
	else
	{
		LeadInOutDataPtr pOuterLeadIn = m_pLeadSetting->GetOuterLeadInData();
		if (!MathEx::Equal(pOuterLeadIn->GetLineSplit(), m_dInLineSplit.Get_Number()))
		{
			pOuterLeadIn->SetLineSplit(m_dInLineSplit.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		m_leadPreviewWnd.UpdateLeadIn(pOuterLeadIn);
	}

	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::OnSelchangeOutType()
{
	// get the selected lead type.
	LeadType leadType;
	int iIndex = m_leadOutType.GetCurSel();
	if (iIndex == 0)
		leadType = Lead_Line;
	else if (iIndex == 1)
		leadType = Lead_Arc;
	else if (iIndex == 2)
		leadType = Lead_LineArc;
	else if (iIndex == 3)
		leadType = Lead_None;

	// update the lead out preview window
	int iTabIndex = m_tabCtrl.GetCurSel();
	if (iTabIndex == 0)
	{
		// internal

		LeadInOutDataPtr pInnerLeadOut = m_pLeadSetting->GetInnerLeadOutData();
		if (pInnerLeadOut->GetType() != leadType)
		{
			pInnerLeadOut->SetType(leadType);
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		InitLeadOutGroup(pInnerLeadOut);
		m_leadPreviewWnd.UpdateLeadOut(pInnerLeadOut);
	}
	else if (iTabIndex == 1)
	{
		// external

		LeadInOutDataPtr pOutLeadOut = m_pLeadSetting->GetOuterLeadOutData();
		if (pOutLeadOut->GetType() != leadType)
		{
			pOutLeadOut->SetType(leadType);
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		InitLeadOutGroup(pOutLeadOut);
		m_leadPreviewWnd.UpdateLeadOut(pOutLeadOut);
	}

	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::OnKillOutLength()
{
	if (m_tabCtrl.GetCurSel() == 0)
	{
		LeadInOutDataPtr pInnerLeadOut = m_pLeadSetting->GetInnerLeadOutData();
		if (!MathEx::Equal(pInnerLeadOut->GetLength(), m_dOutLength.Get_Number()))
		{
			pInnerLeadOut->SetLength(m_dOutLength.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		m_leadPreviewWnd.UpdateLeadOut(pInnerLeadOut);
	}
	else
	{
		LeadInOutDataPtr pOutLeadOut = m_pLeadSetting->GetOuterLeadOutData();
		if (!MathEx::Equal(pOutLeadOut->GetLength(), m_dOutLength.Get_Number()))
		{
			pOutLeadOut->SetLength(m_dOutLength.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		m_leadPreviewWnd.UpdateLeadOut(pOutLeadOut);
	}

	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::OnKillOutAngle()
{
	if (m_tabCtrl.GetCurSel() == 0)
	{
		LeadInOutDataPtr pInnerLeadOut = m_pLeadSetting->GetInnerLeadOutData();
		if (!MathEx::Equal(pInnerLeadOut->GetAngle(), m_dOutAngle.Get_Number()))
		{
			pInnerLeadOut->SetAngle(m_dOutAngle.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		m_leadPreviewWnd.UpdateLeadOut(pInnerLeadOut);
	}
	else
	{
		LeadInOutDataPtr pOutLeadOut = m_pLeadSetting->GetOuterLeadOutData();
		if (!MathEx::Equal(pOutLeadOut->GetAngle(), m_dOutAngle.Get_Number()))
		{
			pOutLeadOut->SetAngle(m_dOutAngle.Get_Number());
			m_pLeadSetting->SetModified(TRUE);
		}

		//
		m_leadPreviewWnd.UpdateLeadOut(pOutLeadOut);
	}

	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::OnKillOutARCR()
{
	LeadInOutDataPtr tmpLeadOut;

	if (m_tabCtrl.GetCurSel() == 0)
	{
		tmpLeadOut = m_pLeadSetting->GetInnerLeadOutData();
	}
	else
	{
		tmpLeadOut = m_pLeadSetting->GetOuterLeadOutData();
	}

	if (!MathEx::Equal(tmpLeadOut->GetArcRadius(), m_dOutArcRadius.Get_Number()))
	{
		tmpLeadOut->SetArcRadius(m_dOutArcRadius.Get_Number());
		m_pLeadSetting->SetModified(TRUE);
	}

	m_leadPreviewWnd.UpdateLeadOut(tmpLeadOut);
	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::OnKillOutARCA()
{
	LeadInOutDataPtr tmpLeadOut;

	if (m_tabCtrl.GetCurSel() == 0)
	{
		tmpLeadOut = m_pLeadSetting->GetInnerLeadOutData();
	}
	else
	{
		tmpLeadOut = m_pLeadSetting->GetOuterLeadOutData();
	}

	if (!MathEx::Equal(tmpLeadOut->GetArcAngle(), m_dOutArcAngle.Get_Number()))
	{
		tmpLeadOut->SetArcAngle(m_dOutArcAngle.Get_Number());
		m_pLeadSetting->SetModified(TRUE);
	}

	m_leadPreviewWnd.UpdateLeadOut(tmpLeadOut);
	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::OnKillOutCut()
{
	LeadInOutDataPtr tmpLeadOut;

	if (m_tabCtrl.GetCurSel() == 0)
	{
		tmpLeadOut = m_pLeadSetting->GetInnerLeadOutData();
	}
	else
	{
		tmpLeadOut = m_pLeadSetting->GetOuterLeadOutData();
	}

	if (!MathEx::Equal(tmpLeadOut->GetOverCut(), m_dOutOverCut.Get_Number()))
	{
		tmpLeadOut->SetOverCut(m_dOutOverCut.Get_Number());
		m_pLeadSetting->SetModified(TRUE);
	}

	m_leadPreviewWnd.UpdateLeadOut(tmpLeadOut);
	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::OnKillCornerInLen_leadIn()
{
	if (!MathEx::Equal(m_pLeadSetting->GetCornerLineLen_leadIn(), m_dCornerLineLen_leadIn.Get_Number()))
	{
		m_pLeadSetting->SetCornerLineLen_leadIn(m_dCornerLineLen_leadIn.Get_Number());
		m_pLeadSetting->SetModified(TRUE);
	}
}

void LeadInOutPage::OnKillCornerArcAng_leadIn()
{
	if (!MathEx::Equal(m_pLeadSetting->GetCornerArcAngle_leadIn(), m_dCornerArcAngle_leadIn.Get_Number()))
	{
		m_pLeadSetting->SetCornerArcAngle_leadIn(m_dCornerArcAngle_leadIn.Get_Number());
		m_pLeadSetting->SetModified(TRUE);
	}
}

void LeadInOutPage::OnKillCornerInLen_leadOut()
{
    if (!MathEx::Equal(m_pLeadSetting->GetCornerLineLen_leadOut(), m_dCornerLineLen_leadOut.Get_Number()))
    {
        m_pLeadSetting->SetCornerLineLen_leadOut(m_dCornerLineLen_leadOut.Get_Number());
        m_pLeadSetting->SetModified(TRUE);
    }
}

void LeadInOutPage::OnKillCornerArcAng_leadOut()
{
    if (!MathEx::Equal(m_pLeadSetting->GetCornerArcAngle_leadOut(), m_dCornerArcAngle_leadOut.Get_Number()))
    {
        m_pLeadSetting->SetCornerArcAngle_leadOut(m_dCornerArcAngle_leadOut.Get_Number());
        m_pLeadSetting->SetModified(TRUE);
    }
}

void LeadInOutPage::OnKillCornerOverCut_leadOut()
{
    if (!MathEx::Equal(m_pLeadSetting->GetOverCutAtCorner(), m_dCornerOverCut_leadOut.Get_Number()))
    {
        m_pLeadSetting->SetOverCutAtCorner(m_dCornerOverCut_leadOut.Get_Number());
        m_pLeadSetting->SetModified(TRUE);

        if (fabs(m_dCornerOverCut_leadOut.Get_Number()) < GEOM_TOLERANCE)
        {
            ((CWnd*)GetDlgItem(IDC_EDIT_CORNER_ARC_ANG_LEADOUT))->EnableWindow(TRUE);
        }
        else
        {
            ((CWnd*)GetDlgItem(IDC_EDIT_CORNER_ARC_ANG_LEADOUT))->EnableWindow(FALSE);
        }
    }
}

void LeadInOutPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	m_leadPreviewWnd.OnPaint();
}

HBRUSH LeadInOutPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void LeadInOutPage::LayoutCtrl()
{
	//
	AddAnchor(IDC_GROUP_GEOM, TOP_LEFT);
	AddAnchor(IDC_TAB_INEXT, TOP_LEFT);
	AddAnchor(IDC_STATIC_LEAD, TOP_LEFT);

	/************************************************************************/
	// "lead in" group.

	AddAnchor(IDC_GROUP_LEADIN, TOP_LEFT);

	AddAnchor(IDC_TXT_IN_TYPE, TOP_LEFT);
	AddAnchor(IDC_COMBO_IN_TYPE, TOP_LEFT);

	AddAnchor(IDC_TXT_IN_LENGTH, TOP_LEFT);
	AddAnchor(IDC_EDIT_IN_LENGTH, TOP_LEFT);

	AddAnchor(IDC_TXT_IN_ANGLE, TOP_LEFT);
	AddAnchor(IDC_EDIT_IN_ANGLE, TOP_LEFT);

	AddAnchor(IDC_TXT_IN_ARCR, TOP_LEFT);
	AddAnchor(IDC_EDIT_IN_ARCR, TOP_LEFT);

	AddAnchor(IDC_TXT_IN_ARCA, TOP_LEFT);
	AddAnchor(IDC_EDIT_IN_ARCA, TOP_LEFT);

	AddAnchor(IDC_TXT_IN_SPLIT, TOP_LEFT);
	AddAnchor(IDC_EDIT_IN_SPLIT, TOP_LEFT);
	/************************************************************************/

	/************************************************************************/
	// "lead out" group.

	AddAnchor(IDC_GROUP_LEADOUT, TOP_LEFT);

	AddAnchor(IDC_TXT_OUT_TYPE, TOP_LEFT);
	AddAnchor(IDC_COMBO_OUT_TYPE, TOP_LEFT);

	AddAnchor(IDC_TXT_OUT_LENGTH, TOP_LEFT);
	AddAnchor(IDC_EDIT_OUT_LENGTH, TOP_LEFT);

	AddAnchor(IDC_TXT_OUT_ANGLE, TOP_LEFT);
	AddAnchor(IDC_EDIT_OUT_ANGLE, TOP_LEFT);

	AddAnchor(IDC_TXT_OUT_ARCR, TOP_LEFT);
	AddAnchor(IDC_EDIT_OUT_ARCR, TOP_LEFT);

	AddAnchor(IDC_TXT_OUT_ARCA, TOP_LEFT);
	AddAnchor(IDC_EDIT_OUT_ARCA, TOP_LEFT);

	AddAnchor(IDC_TXT_OUT_CUT, TOP_LEFT);
	AddAnchor(IDC_EDIT_OUT_CUT, TOP_LEFT);
	/************************************************************************/

	/************************************************************************/
	// corner lead group.

    AddAnchor(IDC_GROUP_CORNER_LEADIN, TOP_LEFT);
	AddAnchor(IDC_TXT_CORNER_LINE_LEN_LEADIN, TOP_LEFT);
	AddAnchor(IDC_EDIT_CORNER_LINE_LEN_LEADIN, TOP_LEFT);
	AddAnchor(IDC_TXT_CORNER_ARC_ANG_LEADIN, TOP_LEFT);
	AddAnchor(IDC_EDIT_CORNER_ARC_ANG_LEADIN, TOP_LEFT);

    AddAnchor(IDC_GROUP_CORNER_LEADOUT, TOP_LEFT);
    AddAnchor(IDC_TXT_CORNER_LINE_LEN_LEADOUT, TOP_LEFT);
    AddAnchor(IDC_EDIT_CORNER_LINE_LEN_LEADOUT, TOP_LEFT);
    AddAnchor(IDC_TXT_CORNER_ARC_ANG_LEADOUT, TOP_LEFT);
    AddAnchor(IDC_EDIT_CORNER_ARC_ANG_LEADOUT, TOP_LEFT);
    AddAnchor(IDC_TXT_CORNER_ARC_ANG_LEADOUT2, TOP_LEFT);
    AddAnchor(IDC_EDIT_CORNER_OVERCUT, TOP_LEFT);
	/************************************************************************/
}

void LeadInOutPage::DisplayLeadSetting(LeadConfigItemPtr pLeadConfigItem)
{
	m_pLeadSetting = pLeadConfigItem;

	// init lead in group.
	LeadInOutDataPtr pLeadIn;
	if (m_tabCtrl.GetCurSel() == 0)
		pLeadIn = pLeadConfigItem->GetInnerLeadInData();
	else
		pLeadIn = pLeadConfigItem->GetOuterLeadInData();
	InitLeadInGroup(pLeadIn);

	// init lead out group.
	LeadInOutDataPtr pLeadOut;
	if (m_tabCtrl.GetCurSel() == 0)
		pLeadOut = pLeadConfigItem->GetInnerLeadOutData();
	else
		pLeadOut = pLeadConfigItem->GetOuterLeadOutData();
	InitLeadOutGroup(pLeadOut);

    // 角上引线
	m_dCornerLineLen_leadIn.Init(pLeadConfigItem->GetCornerLineLen_leadIn(), DIGITAL_NUM, FALSE);
	m_dCornerArcAngle_leadIn.Init(pLeadConfigItem->GetCornerArcAngle_leadIn(), DIGITAL_NUM, FALSE);
    m_dCornerLineLen_leadOut.Init(pLeadConfigItem->GetCornerLineLen_leadOut(), DIGITAL_NUM, FALSE);
    m_dCornerArcAngle_leadOut.Init(pLeadConfigItem->GetCornerArcAngle_leadOut(), DIGITAL_NUM, FALSE);
    m_dCornerOverCut_leadOut.Init(pLeadConfigItem->GetOverCutAtCorner(), DIGITAL_NUM, TRUE);
    if (fabs(pLeadConfigItem->GetOverCutAtCorner()) < GEOM_TOLERANCE)
    {
        ((CWnd*)GetDlgItem(IDC_EDIT_CORNER_ARC_ANG_LEADOUT))->EnableWindow(TRUE);
        ((CWnd*)GetDlgItem(IDC_EDIT_CORNER_LINE_LEN_LEADOUT))->EnableWindow(TRUE);
    }
    else
    {
        ((CWnd*)GetDlgItem(IDC_EDIT_CORNER_ARC_ANG_LEADOUT))->EnableWindow(FALSE);
        ((CWnd*)GetDlgItem(IDC_EDIT_CORNER_LINE_LEN_LEADOUT))->EnableWindow(FALSE);
    }

    UpdateData(FALSE);

	// init the lead preview window.
	m_leadPreviewWnd.InitLead(pLeadIn, pLeadOut);
	m_leadPreviewWnd.OnPaint();
}

void LeadInOutPage::InitLeadInGroup(LeadInOutDataPtr pInnerLeadIn)
{
	LeadType iLeadInType = pInnerLeadIn->GetType();

	// enable all edit box at first.
	m_dInLength.EnableWindow(TRUE);
	m_dInAngle.EnableWindow(TRUE);
	m_dInArcRadius.EnableWindow(TRUE);
	m_dInArcAngle.EnableWindow(TRUE);
	m_dInLineSplit.EnableWindow(TRUE);

	// set value of all edit box.
	m_dInLength.Init(pInnerLeadIn->GetLength(), DIGITAL_NUM, FALSE);
	m_dInAngle.Init(pInnerLeadIn->GetAngle(), DIGITAL_NUM, FALSE);
	m_dInArcRadius.Init(pInnerLeadIn->GetArcRadius(), DIGITAL_NUM, FALSE);
	m_dInArcAngle.Init(pInnerLeadIn->GetArcAngle(), DIGITAL_NUM, FALSE);
	m_dInLineSplit.Init(pInnerLeadIn->GetLineSplit(), DIGITAL_NUM, FALSE);

	if (iLeadInType == Lead_Line)
	{
		m_leadInType.SetCurSel(0);

		m_dInArcRadius.EnableWindow(FALSE);
		m_dInArcAngle.EnableWindow(FALSE);
		m_dInLineSplit.EnableWindow(FALSE);
	}
	else if (iLeadInType == Lead_TwoLines)
	{
		m_leadInType.SetCurSel(1);

		m_dInArcRadius.EnableWindow(FALSE);
		m_dInArcAngle.EnableWindow(FALSE);
	}
	else if (iLeadInType == Lead_LineArc)
	{
		m_leadInType.SetCurSel(2);

		m_dInAngle.EnableWindow(FALSE);
		m_dInArcRadius.EnableWindow(FALSE);
		m_dInLineSplit.EnableWindow(FALSE);
	}
	else if (iLeadInType == Lead_Hook)
	{
		m_leadInType.SetCurSel(3);

		m_dInAngle.EnableWindow(FALSE);
		m_dInArcAngle.EnableWindow(FALSE);
		m_dInLineSplit.EnableWindow(FALSE);
	}
	else if (iLeadInType == Lead_None)
	{
		m_leadInType.SetCurSel(4);

		m_dInLength.EnableWindow(FALSE);
		m_dInAngle.EnableWindow(FALSE);
		m_dInArcRadius.EnableWindow(FALSE);
		m_dInArcAngle.EnableWindow(FALSE);
		m_dInLineSplit.EnableWindow(FALSE);
	}

	m_leadInType.Invalidate(TRUE);
}

void LeadInOutPage::InitLeadOutGroup(LeadInOutDataPtr pInnerLeadOut)
{
	LeadType iLeadOutType = pInnerLeadOut->GetType();

	// enable all edit box at first.
	m_dOutLength.EnableWindow(TRUE);
	m_dOutAngle.EnableWindow(TRUE);
	m_dOutArcRadius.EnableWindow(TRUE);
	m_dOutArcAngle.EnableWindow(TRUE);
	m_dOutOverCut.EnableWindow(TRUE);

	// set value of all edit box.
	m_dOutLength.Init(pInnerLeadOut->GetLength(), DIGITAL_NUM, FALSE);
	m_dOutAngle.Init(pInnerLeadOut->GetAngle(), DIGITAL_NUM, FALSE);
	m_dOutArcRadius.Init(pInnerLeadOut->GetArcRadius(), DIGITAL_NUM, FALSE);
	m_dOutArcAngle.Init(pInnerLeadOut->GetArcAngle(), DIGITAL_NUM, FALSE);
	m_dOutOverCut.Init(pInnerLeadOut->GetOverCut(), DIGITAL_NUM, TRUE);

	if (iLeadOutType == Lead_Line)
	{
		m_leadOutType.SetCurSel(0);

		m_dOutArcRadius.EnableWindow(FALSE);
		m_dOutArcAngle.EnableWindow(FALSE);
	}
	else if (iLeadOutType == Lead_Arc)
	{
		m_leadOutType.SetCurSel(1);

		m_dOutLength.EnableWindow(FALSE);
		m_dOutAngle.EnableWindow(FALSE);
	}
	else if (iLeadOutType == Lead_LineArc)
	{
		m_leadOutType.SetCurSel(2);

		m_dOutAngle.EnableWindow(FALSE);
		m_dOutArcRadius.EnableWindow(FALSE);
	}
	else if (iLeadOutType == Lead_None)
	{
		m_leadOutType.SetCurSel(3);

		m_dOutLength.EnableWindow(FALSE);
		m_dOutAngle.EnableWindow(FALSE);
		m_dOutArcRadius.EnableWindow(FALSE);
		m_dOutArcAngle.EnableWindow(FALSE);
	}
	m_leadOutType.Invalidate(TRUE);
}

void LeadInOutPage::Init()
{
	// init the tab control
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INTERNAL_GEOMETRY);
	m_tabCtrl.InsertItem(0, str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXTERNAL_GEOMETRY);
	m_tabCtrl.InsertItem(1, str);

	for (int i = 0; i < m_tabCtrl.GetItemCount(); i++ ) {
		m_tabCtrl.SetTabColor( i, SkinManager::GetBackgroundColor() );
	}

	// init "lead in" type combobox
	StringListPtr leadinTypes = LeadInOutData::GetAllLeadInType();
	for (unsigned int i = 0; i < leadinTypes->size(); i++)
		m_leadInType.AddString(leadinTypes->at(i));

	// init "lead out" type combobox
	StringListPtr leadoutTypes = LeadInOutData::GetAllLeadOutType();
	for (unsigned int i = 0; i < leadoutTypes->size(); i++)
		m_leadOutType.AddString(leadoutTypes->at(i));
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "DisplayParamPage.h"

#include "baseConst.h"
#include "UnitHelper.h"
#include "FontParam.h"
#include "baseConst.h"

#include "DisplayParam.h"
#include "clOptionResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const size_t MAX_TIP_TEXT_LENGTH = 1024;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE( DisplayParamPage, CLResizableDialog )

DisplayParamPage::DisplayParamPage() : CLResizableDialog (DisplayParamPage::IDD )
{
}

DisplayParamPage::~DisplayParamPage()
{
}

void DisplayParamPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_COLOR, m_itemColorList);
	DDX_Control(pDX, IDC_BUTTON_COLORPICKER, m_ctrlColorPicker);

	DDX_Check(pDX, IDC_CHECK_SHOW_STARTCUT, m_bShowStartCut);
	DDX_Check(pDX, IDC_CHECK_SHOW_ENDCUT, m_bShowEndCut);
	DDX_Check(pDX, IDC_CHECK_SHOW_LEAD, m_bShowLead);
	DDX_Check(pDX, IDC_CHECK_SHOWBMJ, m_bShowMicroJoint);
	DDX_Check(pDX, IDC_CHECK_SHOWCORNER, m_bShowCorner);
	DDX_Check(pDX, IDC_CHECK_SHOWOT, m_bShowOT);
	DDX_Check(pDX, IDC_CHECK_FLAG_OPENPATH, m_bFlagOpenPath);
	DDX_Check(pDX, IDC_CHECK_ShowIncorrectTool, m_bShowIncorrectTool);

	DDX_Control(pDX, IDC_SLIDER_SPEED, m_speedSlider);
	DDX_Control( pDX, IDC_BTN_PART_NO_FONT, m_btnPartNoFont );
}

BEGIN_MESSAGE_MAP( DisplayParamPage, CLResizableDialog )
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BUTTON_COLORPICKER, OnBnClickedPickcolor)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOR, OnSelchangeColorItem)
	ON_BN_CLICKED(IDC_BTN_PART_NO_FONT, OnPartNoFont)
END_MESSAGE_MAP()

BOOL DisplayParamPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();


	/************************************************************************/
	// layout controls.

	AddAnchor(IDC_GROUP_COLOR, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_COMBO_COLOR, TOP_LEFT);
	AddAnchor(IDC_BTN_COLOR_PICKER, TOP_LEFT);

	AddAnchor(IDC_GROUP_SHOW_HIDE, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_CHECK_SHOW_STARTCUT, TOP_LEFT);
	AddAnchor(IDC_CHECK_SHOW_ENDCUT, TOP_LEFT);
	AddAnchor(IDC_CHECK_SHOW_LEAD, TOP_LEFT);
	AddAnchor(IDC_CHECK_SHOWBMJ, TOP_LEFT);
	AddAnchor(IDC_CHECK_SHOWCORNER, TOP_LEFT);
	AddAnchor(IDC_CHECK_SHOWOT, TOP_LEFT);
	AddAnchor(IDC_CHECK_FLAG_OPENPATH, TOP_LEFT);
	AddAnchor(IDC_CHECK_ShowIncorrectTool, TOP_LEFT);

	AddAnchor(IDC_GROUP_SIM, TOP_LEFT, TOP_RIGHT);
	AddAnchor( IDC_SLIDER_SPEED, TOP_LEFT, TOP_RIGHT );

	AddAnchor(IDC_GROUP_REPORT, TOP_RIGHT);
	AddAnchor(IDC_BTN_PART_NO_FONT, TOP_RIGHT);
	/************************************************************************/


	// init property of color picker button.
	m_ctrlColorPicker.EnableOtherButton(_T("Other..."));
	m_ctrlColorPicker.SetColumnsNumber(4);

	// set the speed range.
	m_speedSlider.SetRange(1, 8);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	// setup ctrls for don't follow windows setting
	SetWindowTheme( GetDlgItem( IDC_GROUP_SKIN )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_COLOR )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_SHOW_HIDE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_SIM )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_REPORT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_RDB_Light )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_RDB_Dark )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_COLOR )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SHOW_STARTCUT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SHOW_LEAD )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SHOWOT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SHOWBMJ )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SHOWCORNER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_FLAG_OPENPATH )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_SHOW_ENDCUT )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

HBRUSH DisplayParamPage::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void DisplayParamPage::OnBnClickedPickcolor()
{
	m_ctrlColorPicker.SetTextHotColor(RGB(255,0,0));
	int iCurSel = m_itemColorList.GetCurSel();
	if (iCurSel >= 0)
	{
		COLORREF color = m_ctrlColorPicker.GetColor();

		// update the data.
		int index = m_itemColorList.GetCurSel();
		if (index == 0)
			m_pDisplayParam->SetBackgroundColor(color);
		if (index == 1)
			m_pDisplayParam->SetPatColor(color);
		if (index == 2)
			m_pDisplayParam->SetActiveObjColor(color);
		if (index == 3)
			m_pDisplayParam->SetSelectObjColor(color);
		if (index == 4)
			m_pDisplayParam->SetDrawAssistColor(color);
		if (index == 5)
			m_pDisplayParam->SetStartCutPtColor(color);
		if (index == 6)
			m_pDisplayParam->SetEndCutPtColor(color);
		if (index == 7)
			m_pDisplayParam->SetLeadColor(color);
		if (index == 8)
			m_pDisplayParam->SetMicroJointColor(color);
		if (index == 9)
			m_pDisplayParam->SetCornerColor(color);
		if (index == 10)
			m_pDisplayParam->SetSequenceColor(color);
		if (index == 11)
			m_pDisplayParam->SetSeqSimColor(color);
		if (index == 12)
			m_pDisplayParam->SetOTColor(color);
		if (index == 13)
			m_pDisplayParam->SetNoCutLoopColor(color);
		if (index == 14)
		m_pDisplayParam->SetIncorrectToolSizeColor(color);

		m_pDisplayParam->SetModified(TRUE);

		// get a pointer to color
		COLORREF* pChangedColor = (COLORREF*)(m_itemColorList.GetItemData(iCurSel));
		if (pChangedColor != NULL)
		{
			*pChangedColor = color;

			//
			m_itemColorList.Invalidate();
			m_itemColorList.RedrawWindow();
		}
	}
}

void DisplayParamPage::OnSelchangeColorItem()
{
	int index = m_itemColorList.GetCurSel();
	COLORREF* pColor = (COLORREF*)(m_itemColorList.GetItemData(index));
	if (pColor != NULL)
		m_ctrlColorPicker.SetColor(*pColor);
}

void DisplayParamPage::OnPartNoFont()
{
	// init the unit conversion resolution value.
	CClientDC dc(this);
	UnitHelper::Init(dc.GetDeviceCaps(LOGPIXELSX));

	// 字体高度转化为像素单位。
	LOGFONT	logicFont = m_logicFont;
	logicFont.lfHeight = UnitHelper::PointsToPixels(logicFont.lfHeight);

	CFontDialog	dlg(&logicFont);
	dlg.m_cf.rgbColors = m_fontColor;
	dlg.m_cf.Flags |= CF_EFFECTS;
	if( dlg.DoModal() == IDOK )
	{
		dlg.GetCurrentFont(&m_logicFont);
		m_logicFont.lfHeight = dlg.GetSize();
		m_fontColor = dlg.GetColor();
	}
}

void DisplayParamPage::DisplayDisplayParam(DisplayParamPtr colorStyleInfoPtr)
{
	m_pDisplayParam = colorStyleInfoPtr;


	/************************************************************************/
	// 显示颜色。

	CString strItem;
	m_itemColorList.ResetContent();

	// background
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_BACKGROUND);
	m_itemColorList.InsertString(0, strItem);
	m_itemColorList.SetItemData(0, (DWORD_PTR)m_pDisplayParam->GetBackgroundColor());

	// pattern
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_PATTERN);
	m_itemColorList.InsertString(1, strItem);
	m_itemColorList.SetItemData(1, (DWORD_PTR)m_pDisplayParam->GetPatColor());

	// active object color.
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_ACTIVEOBJ);
	m_itemColorList.InsertString(2, strItem);
	m_itemColorList.SetItemData(2, (DWORD_PTR)m_pDisplayParam->GetActiveObjColor());

	// selected object color
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_SELECTOBJ);
	m_itemColorList.InsertString(3, strItem);
	m_itemColorList.SetItemData(3, (DWORD_PTR)m_pDisplayParam->GetSelectObjColor());

	// draw assist color
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_DRAWASSIST);
	m_itemColorList.InsertString(4, strItem);
	m_itemColorList.SetItemData(4, (DWORD_PTR)m_pDisplayParam->GetDrawAssistColor());

	// start cut point
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_STARTCUTPT);
	m_itemColorList.InsertString(5, strItem);
	m_itemColorList.SetItemData(5, (DWORD_PTR)m_pDisplayParam->GetStartCutPtColor());

	// end cut point
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_ENDCUTPT);
	m_itemColorList.InsertString(6, strItem);
	m_itemColorList.SetItemData(6, (DWORD_PTR)m_pDisplayParam->GetEndCutPtColor());

	// lead
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_LEADINOUT);
	m_itemColorList.InsertString(7, strItem);
	m_itemColorList.SetItemData(7, (DWORD_PTR)m_pDisplayParam->GetLeadColor());

	// micro joint
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_MicroJoint);
	m_itemColorList.InsertString(8, strItem);
	m_itemColorList.SetItemData(8, (DWORD_PTR)m_pDisplayParam->GetMicroJointColor());

	// corner
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_CORNER);
	m_itemColorList.InsertString(9, strItem);
	m_itemColorList.SetItemData(9, (DWORD_PTR)m_pDisplayParam->GetCornerColor());

	// sequence
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_SEQUENCE);
	m_itemColorList.InsertString(10, strItem);
	m_itemColorList.SetItemData(10, (DWORD_PTR)m_pDisplayParam->GetSequenceColor());

	// 模拟
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_SEQ_SIM);
	m_itemColorList.InsertString(11, strItem);
	m_itemColorList.SetItemData(11, (DWORD_PTR)m_pDisplayParam->GetSeqSimColor());

	// OT
	strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_OT);
	m_itemColorList.InsertString(12, strItem);
	m_itemColorList.SetItemData(12, (DWORD_PTR)m_pDisplayParam->GetOTColor());

    //
    strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_NoCutLoop);
    m_itemColorList.InsertString(13, strItem);
    m_itemColorList.SetItemData(13, (DWORD_PTR)m_pDisplayParam->GetNoCutLoopColor());

    strItem = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLORITEM_IncorrectToolSize);
    m_itemColorList.InsertString(14, strItem);
    m_itemColorList.SetItemData(14, (DWORD_PTR)m_pDisplayParam->GetIncorrectToolSizeColor());

	// select first one
	m_itemColorList.SetCurSel(0);
	COLORREF* pColor = (COLORREF*)(m_itemColorList.GetItemData(m_itemColorList.GetCurSel()));
	if (pColor != NULL)
		m_ctrlColorPicker.SetColor(*pColor);
	/************************************************************************/


	m_bShowStartCut = m_pDisplayParam->GetShowCutDir();
    m_bShowEndCut = m_pDisplayParam->GetShowEndCutPt();
	m_bShowLead = m_pDisplayParam->GetShowLead();
	m_bShowMicroJoint = m_pDisplayParam->GetShowMicroJoint();
	m_bShowCorner = m_pDisplayParam->GetShowCorner();
	m_bShowOT = m_pDisplayParam->GetShowOT();
	m_bFlagOpenPath = m_pDisplayParam->FlagOpenPath();
	m_bShowIncorrectTool = m_pDisplayParam->GetShowIncorrectToolSize();

	m_speedSlider.SetPos(m_pDisplayParam->GetSpeed());

	// the font info.
	FontParamPtr pFontParam = m_pDisplayParam->GetPartNoFont();
	m_logicFont = pFontParam->GetLogicFont();
	m_fontColor = pFontParam->GetFontColor();

	UpdateData(FALSE);
}

DisplayParamPtr DisplayParamPage::GetDisplayParam()
{
	UpdateData(TRUE);

	// 显示和隐藏。
	if (m_bShowStartCut != m_pDisplayParam->GetShowCutDir())
	{
		m_pDisplayParam->SetShowCutDir(m_bShowStartCut);
		m_pDisplayParam->SetModified(TRUE);
	}
	if (m_bShowEndCut != m_pDisplayParam->GetShowEndCutPt())
	{
		m_pDisplayParam->SetShowEndCutPt(m_bShowEndCut);
		m_pDisplayParam->SetModified(TRUE);
	}
	if (m_bShowLead != m_pDisplayParam->GetShowLead())
	{
		m_pDisplayParam->SetShowLead(m_bShowLead);
		m_pDisplayParam->SetModified(TRUE);
	}
	if (m_bShowMicroJoint != m_pDisplayParam->GetShowMicroJoint())
	{
		m_pDisplayParam->SetShowMicroJoint(m_bShowMicroJoint);
		m_pDisplayParam->SetModified(TRUE);
	}
	if (m_bShowCorner != m_pDisplayParam->GetShowCorner())
	{
		m_pDisplayParam->SetShowCorner(m_bShowCorner);
		m_pDisplayParam->SetModified(TRUE);
	}
	if (m_bShowOT != m_pDisplayParam->GetShowOT())
	{
		m_pDisplayParam->SetShowOT(m_bShowOT);
		m_pDisplayParam->SetModified(TRUE);
	}
	if (m_bFlagOpenPath != m_pDisplayParam->FlagOpenPath())
	{
		m_pDisplayParam->FlagOpenPath(m_bFlagOpenPath);
		m_pDisplayParam->SetModified(TRUE);
	}
	if (m_bShowIncorrectTool != m_pDisplayParam->GetShowIncorrectToolSize())
	{
		m_pDisplayParam->SetShowIncorrectToolSize(m_bShowIncorrectTool);
		m_pDisplayParam->SetModified(TRUE);
	}

	int iSpeed = m_speedSlider.GetPos();
	if (iSpeed != m_pDisplayParam->GetSpeed())
	{
		m_pDisplayParam->SetSpeed(iSpeed);
		m_pDisplayParam->SetModified(TRUE);
	}

	// 字体设置。
	FontParamPtr pFontParam = m_pDisplayParam->GetPartNoFont();
	if (!pFontParam->FitLogicFont(m_logicFont, m_fontColor))
	{
		pFontParam->UpdateFromLogicFont(m_logicFont, m_fontColor);
		m_pDisplayParam->SetModified(TRUE);
	}

	return m_pDisplayParam;
}

END_CUTLEADER_NAMESPACE()

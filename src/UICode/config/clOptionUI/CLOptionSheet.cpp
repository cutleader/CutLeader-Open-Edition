#include "stdafx.h"
#include "CLOptionSheet.h"

#include "baseConst.h"
#include "baseConst.h"
#include "clBaseDataResource.h"
#include "ProductFileDefine.h"
#include "ClFolderHelper.h"
#include "Polygon2DProcessor.h"

#include "ProductParam.h"
#include "ImpExpConfig.h"
#include "DisplayParam.h"
#include "PartEditParam.h"
#include "SheetEditParam.h"
#include "clOptionResource.h"
#include "CLOptionLoader.h"
#include "CLOptionWriter.h"
#include "SnapPtParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

CLOptionSheet::CLOptionSheet(CWnd* pParent)
			: CLResizableDialog(CLOptionSheet::IDD, pParent)
{
	m_pCLOptionInfo.reset();
	m_iConfigType = CONFIG_GENERAL;
}

CLOptionSheet::~CLOptionSheet()
{
}

BEGIN_MESSAGE_MAP(CLOptionSheet, CResizableDialog)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()

	ON_BN_CLICKED(IDC_BTN_GENERAL, OnGeneralConfig)
	ON_BN_CLICKED(IDC_BTN_IMPEXP, OnImpExpConfig)
	ON_BN_CLICKED(IDC_BTN_DISPLAY, OnColorConfig)
	ON_BN_CLICKED(IDC_BTN_PART_EDIT, OnDisplayPartParam)
	ON_BN_CLICKED(IDC_BTN_SHEETEDIT, OnDisplaySheetParam)
	ON_BN_CLICKED(IDC_BTN_SnapPtEdit, OnDisplaySnapPtParam)

	ON_BN_CLICKED(IDC_BTN_SAVE, OnSave)
	ON_BN_CLICKED(IDC_BTN_QUIT, OnQuit)

END_MESSAGE_MAP()

void CLOptionSheet::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_BTN_GENERAL, m_btnGeneral );
	DDX_Control( pDX, IDC_BTN_IMPEXP, m_btnImpExp );
	DDX_Control( pDX, IDC_BTN_DISPLAY, m_btnDisplay );
	DDX_Control( pDX, IDC_BTN_PART_EDIT, m_btnPartEdit );
	DDX_Control( pDX, IDC_BTN_SHEETEDIT, m_btnSheetEdit );
	DDX_Control( pDX, IDC_BTN_SnapPtEdit, m_btnSnapPtEdit );
	DDX_Control( pDX, IDC_BTN_SAVE, m_btnSave );
	DDX_Control( pDX, IDC_BTN_QUIT, m_btnQuit );
	DDX_Control( pDX, IDC_BTN_SEPERATOR, m_btnSeperator );
}

BOOL CLOptionSheet::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	/************************************************************************/
	// layout the controls on the dialog.

	// layout the buttons.
	AddAnchor(IDC_BTN_GENERAL, TOP_LEFT);
	AddAnchor(IDC_BTN_IMPEXP, TOP_LEFT);
	AddAnchor(IDC_BTN_DISPLAY, TOP_LEFT);
	AddAnchor(IDC_BTN_PART_EDIT, TOP_LEFT);
	AddAnchor(IDC_BTN_SHEETEDIT, TOP_LEFT);
	AddAnchor(IDC_BTN_SnapPtEdit, TOP_LEFT);

	AddAnchor(IDC_BTN_SAVE, TOP_RIGHT);
	AddAnchor(IDC_BTN_QUIT, TOP_RIGHT);

	AddAnchor(IDC_BTN_SEPERATOR, TOP_LEFT, TOP_RIGHT);

	// layout the child pages.
	AddAnchor(IDC_STATIC_CHILD_AREA, TOP_LEFT, BOTTOM_RIGHT);
	/************************************************************************/

	// load the option info.
	CString strPath = ClFolderHelper::GetOptionPath();
	strPath += USER_OPTION_FILE;
	m_pCLOptionInfo = CLOptionLoader::LoadCLOption(strPath);

	/************************************************************************/
	// init sub-dialogs.

	InitSubDialog();
	RepositionSubDlg();

	// display the first page.
	if (m_iConfigType == CONFIG_GENERAL)
	{
		m_productParamPage.ShowWindow(SW_SHOW);
		m_productParamPage.DisplayProductParam(m_pCLOptionInfo->GetProductParam());
	}
	else if (m_iConfigType == CONFIG_IMPEXP)
	{
		m_impExpConfigPage.ShowWindow(SW_SHOW);
		m_impExpConfigPage.DisplayImpExpConfig(m_pCLOptionInfo->GetImpExpConfig());
	}
	else if (m_iConfigType == CONFIG_DISPLAY)
	{
		m_colorParamPage.ShowWindow(SW_SHOW);
		m_colorParamPage.DisplayDisplayParam(m_pCLOptionInfo->GetDisplayParam());
	}
	else if (m_iConfigType == CONFIG_PART_EDIT)
	{
		m_partParamPage.ShowWindow(SW_SHOW);
		m_partParamPage.DisplayPartEditParam(m_pCLOptionInfo->GetPartEditParam());
	}
	else if (m_iConfigType == CONFIG_SHEET_EDIT)
	{
		m_sheetParamPage.ShowWindow(SW_SHOW);
		m_sheetParamPage.DisplaySheetEditParam(m_pCLOptionInfo->GetSheetEditParam());
	}
	else if (m_iConfigType == CONFIG_SnapPtParam)
	{
		m_snapPtParamPage.ShowWindow(SW_SHOW);
		m_snapPtParamPage.DisplaySnapPtParam(m_pCLOptionInfo->GetSnapPtParam());
	}
	/************************************************************************/

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	return TRUE;
}

HBRUSH CLOptionSheet::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void CLOptionSheet::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

	// re-position the sub dialog.
	RepositionSubDlg();
}

void CLOptionSheet::OnGeneralConfig()
{
	if (m_iConfigType == CONFIG_GENERAL)
		return;

	// before switch page, check whether we need to save the previous page.
	CheckModification();

	// switch to ProductParamPage.
	m_iConfigType = CONFIG_GENERAL;
	RepositionSubDlg();
	m_productParamPage.DisplayProductParam(m_pCLOptionInfo->GetProductParam());
}

void CLOptionSheet::OnImpExpConfig()
{
	if (m_iConfigType == CONFIG_IMPEXP)
		return;

	// before switch page, check whether we need to save the previous page.
	CheckModification();

	// switch to ImpExpConfigPage.
	m_iConfigType = CONFIG_IMPEXP;
	RepositionSubDlg();
	m_impExpConfigPage.DisplayImpExpConfig(m_pCLOptionInfo->GetImpExpConfig());
}

void CLOptionSheet::OnColorConfig()
{
	if (m_iConfigType == CONFIG_DISPLAY)
		return;

	// before switch page, check whether we need to save the previous page.
	CheckModification();

	// switch to DisplayParamPage.
	m_iConfigType = CONFIG_DISPLAY;
	RepositionSubDlg();
	m_colorParamPage.DisplayDisplayParam(m_pCLOptionInfo->GetDisplayParam());
}

void CLOptionSheet::OnDisplayPartParam()
{
	if (m_iConfigType == CONFIG_PART_EDIT)
		return;

	// before switch page, check whether we need to save the previous page.
	CheckModification();

	// switch to SheetEditParamPage.
	m_iConfigType = CONFIG_PART_EDIT;
	RepositionSubDlg();
	m_partParamPage.DisplayPartEditParam(m_pCLOptionInfo->GetPartEditParam());
}

void CLOptionSheet::OnDisplaySheetParam()
{
	if (m_iConfigType == CONFIG_SHEET_EDIT)
		return;

	// before switch page, check whether we need to save the previous page.
	CheckModification();

	// switch to SheetEditParamPage.
	m_iConfigType = CONFIG_SHEET_EDIT;
	RepositionSubDlg();
	m_sheetParamPage.DisplaySheetEditParam(m_pCLOptionInfo->GetSheetEditParam());
}

void CLOptionSheet::OnDisplaySnapPtParam()
{
	if (m_iConfigType == CONFIG_SnapPtParam)
		return;

	// before switch page, check whether we need to save the previous page.
	CheckModification();

	//
	m_iConfigType = CONFIG_SnapPtParam;
	RepositionSubDlg();
	m_snapPtParamPage.DisplaySnapPtParam(m_pCLOptionInfo->GetSnapPtParam());
}

void CLOptionSheet::OnSave()
{
	/************************************************************************/
	// prepare some data.

	CString strProductName, str;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	/************************************************************************/

	// check whether some param was changed.
	BOOL bChanged = FALSE;
	if (m_iConfigType == CONFIG_GENERAL)
	{
		ProductParamPtr pProductParam = m_productParamPage.GetProductParam();
		if (pProductParam->IsModified())
			bChanged = TRUE;
	}
	else if (m_iConfigType == CONFIG_IMPEXP)
	{
		ImpExpConfigPtr pImpExpConfig = m_impExpConfigPage.GetImpExpConfig();
		if (pImpExpConfig->IsModified())
			bChanged = TRUE;
	}
	else if (m_iConfigType == CONFIG_DISPLAY)
	{
		DisplayParamPtr pDisplayParam = m_colorParamPage.GetDisplayParam();
		if (pDisplayParam->IsModified())
			bChanged = TRUE;
	}
	else if (m_iConfigType == CONFIG_PART_EDIT)
	{
		PartEditParamPtr pPartEditParam = m_partParamPage.GetPartEditParam();
		if (pPartEditParam->IsModified())
			bChanged = TRUE;
	}
	else if (m_iConfigType == CONFIG_SHEET_EDIT)
	{
		SheetEditParamPtr pSheetEditParam = m_sheetParamPage.GetSheetEditParam();
		if (pSheetEditParam->IsModified())
			bChanged = TRUE;
	}
	else if (m_iConfigType == CONFIG_SnapPtParam)
	{
		SnapPtParamPtr pSnapPtParam = m_snapPtParamPage.GetSnapPtParam();
		if (pSnapPtParam->IsModified())
			bChanged = TRUE;
	}

	if (bChanged)
	{
		SaveAndUpdate();

		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CHANGE_SAVED);
		MessageBox(str, strProductName, MB_ICONINFORMATION);
	}
	else
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NO_CHANGE);
		MessageBox(str, strProductName, MB_ICONWARNING);
	}
}

void CLOptionSheet::OnQuit()
{
	// before quit, check whether we need to save the changes.
	CheckModification();

	OnOK();
}

void CLOptionSheet::InitSubDialog()
{
	// create ProductParamPage.
	m_productParamPage.Create(IDD_clOptionUI_ProductOption, this);
	m_productParamPage.ShowWindow(SW_HIDE);

	// create ImpExpConfigPage.
	m_impExpConfigPage.Create(IDD_clOptionUI_ImportExport_Option, this);
	m_impExpConfigPage.ShowWindow(SW_HIDE);

	// create DisplayParamPage.
	m_colorParamPage.Create(IDD_clOptionUI_Display, this);
	m_colorParamPage.ShowWindow(SW_HIDE);

	// create SheetEditParamPage.
	m_partParamPage.Create(IDD_clOptionUI_PartEdit, this);
	m_partParamPage.ShowWindow(SW_HIDE);

	// create SheetEditParamPage.
	m_sheetParamPage.Create(IDD_clOptionUI_SheetEdit, this);
	m_sheetParamPage.ShowWindow(SW_HIDE);

	//
	m_snapPtParamPage.Create(IDD_clOptionUI_SnapPtParam, this);
	m_snapPtParamPage.ShowWindow(SW_HIDE);
}

void CLOptionSheet::HideAllSubDialog()
{
	m_productParamPage.ShowWindow(SW_HIDE);
	m_impExpConfigPage.ShowWindow(SW_HIDE);
	m_colorParamPage.ShowWindow(SW_HIDE);
	m_partParamPage.ShowWindow(SW_HIDE);
	m_sheetParamPage.ShowWindow(SW_HIDE);
	m_snapPtParamPage.ShowWindow(SW_HIDE);
}

void CLOptionSheet::RepositionSubDlg()
{
	if (!m_productParamPage.m_hWnd)
		return;

	// calculate the position of the child page.
	CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILD_AREA);
	CRect pagePos;
	if (pWndPos)
	{
		pWndPos->GetWindowRect(&pagePos);
		ScreenToClient(&pagePos);
	}

	// the layout library will re-layout the controls, so we should re-position the sub dialog.
	HideAllSubDialog();
	if (m_iConfigType == CONFIG_GENERAL)
	{
		m_productParamPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_productParamPage.ShowWindow(SW_SHOW);
	}
	else if (m_iConfigType == CONFIG_IMPEXP)
	{
		m_impExpConfigPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_impExpConfigPage.ShowWindow(SW_SHOW);
	}
	else if (m_iConfigType == CONFIG_DISPLAY)
	{
		m_colorParamPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_colorParamPage.ShowWindow(SW_SHOW);
	}
	else if (m_iConfigType == CONFIG_PART_EDIT)
	{
		m_partParamPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_partParamPage.ShowWindow(SW_SHOW);
	}
	else if (m_iConfigType == CONFIG_SHEET_EDIT)
	{
		m_sheetParamPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_sheetParamPage.ShowWindow(SW_SHOW);
	}
	else if (m_iConfigType == CONFIG_SnapPtParam)
	{
		m_snapPtParamPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_snapPtParamPage.ShowWindow(SW_SHOW);
	}
}

void CLOptionSheet::CheckModification()
{
	/************************************************************************/
	// prepare some data.

	CString strProductName, str;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	CString strPath = ClFolderHelper::GetOptionPath();
	strPath += USER_OPTION_FILE;
	/************************************************************************/

	if (m_iConfigType == CONFIG_GENERAL)
	{
		ProductParamPtr pProductParam = m_productParamPage.GetProductParam();
		if (pProductParam->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_PRODUCT_PARAM);
			if (MessageBox(str, strProductName, MB_YESNO | MB_ICONWARNING) == IDYES)
				SaveAndUpdate();
			else
				m_pCLOptionInfo = CLOptionLoader::LoadCLOption(strPath);
		}
	}
	else if (m_iConfigType == CONFIG_IMPEXP)
	{
		ImpExpConfigPtr pImpExpConfig = m_impExpConfigPage.GetImpExpConfig();
		if (pImpExpConfig->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_IMPEXPT_PARAM);
			if (MessageBox(str, strProductName, MB_YESNO | MB_ICONWARNING) == IDYES)
				SaveAndUpdate();
			else
				m_pCLOptionInfo = CLOptionLoader::LoadCLOption(strPath);
		}
	}
	else if (m_iConfigType == CONFIG_DISPLAY)
	{
		DisplayParamPtr pDisplayParam = m_colorParamPage.GetDisplayParam();
		if (pDisplayParam->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_DISPLAY_PARAM);
			if ( MessageBox( str, strProductName, MB_YESNO | MB_ICONWARNING ) == IDYES ) {
				SaveAndUpdate();
			}
			else {
				m_pCLOptionInfo = CLOptionLoader::LoadCLOption( strPath );
			}
		}
	}
	else if (m_iConfigType == CONFIG_PART_EDIT)
	{
		PartEditParamPtr pPartEditParam = m_partParamPage.GetPartEditParam();
		if (pPartEditParam->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_PARTEDIT_PARAM);
			if (MessageBox(str, strProductName, MB_YESNO | MB_ICONWARNING) == IDYES)
				SaveAndUpdate();
			else
				m_pCLOptionInfo = CLOptionLoader::LoadCLOption(strPath);
		}
	}
	else if (m_iConfigType == CONFIG_SHEET_EDIT)
	{
		SheetEditParamPtr pSheetEditParam = m_sheetParamPage.GetSheetEditParam();
		if (pSheetEditParam->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_SHEETEDIT_PARAM);
			if (MessageBox(str, strProductName, MB_YESNO | MB_ICONWARNING) == IDYES)
				SaveAndUpdate();
			else
				m_pCLOptionInfo = CLOptionLoader::LoadCLOption(strPath);
		}
	}
	else if (m_iConfigType == CONFIG_SnapPtParam)
	{
		SnapPtParamPtr pSnapPtParam = m_snapPtParamPage.GetSnapPtParam();
		if (pSnapPtParam->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_SnapPtParam);
			if (MessageBox(str, strProductName, MB_YESNO | MB_ICONWARNING) == IDYES)
				SaveAndUpdate();
			else
				m_pCLOptionInfo = CLOptionLoader::LoadCLOption(strPath);
		}
	}
}

void CLOptionSheet::SaveAndUpdate()
{
	CString strPath = ClFolderHelper::GetOptionPath();
	strPath += USER_OPTION_FILE;

	// save
	CLOptionWriter::SaveCLOption(m_pCLOptionInfo, strPath);

	// need to update the singleton.
	CLOptionInfoPtr pCLOptionInfo = CLOptionLoader::LoadCLOption(strPath);
	CLOptionInfo::Init(pCLOptionInfo);

	// �初始化Polygon2DProcessor中的变量。
	SheetEditParamPtr pSheetEditParam = pCLOptionInfo->GetSheetEditParam();
	Polygon2DProcessor::Init(pSheetEditParam->EnableSleekCtrl(), pSheetEditParam->GetSleekRate());
}

END_CUTLEADER_NAMESPACE()

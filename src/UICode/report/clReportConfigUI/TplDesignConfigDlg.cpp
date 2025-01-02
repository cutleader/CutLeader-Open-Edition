#include "StdAfx.h"
#include "TplDesignConfigDlg.h"

#include "baseConst.h"
#include "clBaseDataResource.h"

#include "clReportResource.h"
#include "TemplateParam.h"
#include "TplDisplayParam.h"
#include "TplEditParam.h"
#include "TplDesignConfig.h"
#include "TplDesignConfigLoader.h"
#include "TplDesignConfigWriter.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

TplDesignConfigDlg::TplDesignConfigDlg(void)
		: CLResizableDialog(TplDesignConfigDlg::IDD)
{
	m_pTplDesignConfig.reset();
	m_iConfigType = CONFIG_NOT_INIT;
}

TplDesignConfigDlg::~TplDesignConfigDlg(void)
{
}

BEGIN_MESSAGE_MAP(TplDesignConfigDlg, CResizableDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_TEMPLATE, OnTemplateConfig)
	ON_BN_CLICKED(IDC_BTN_DISPLAY, OnDisplayConfig)
	ON_BN_CLICKED(IDC_BTN_DESIGN, OnDesignConfig)

	ON_BN_CLICKED(IDC_BTN_SAVE, OnSave)
	ON_BN_CLICKED(IDC_BTN_QUIT, OnQuit)

END_MESSAGE_MAP()

void TplDesignConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_TEMPLATE, m_btnTemplate);
	DDX_Control(pDX, IDC_BTN_DISPLAY, m_btnDisplay);
	DDX_Control(pDX, IDC_BTN_DESIGN, m_btnDesign);

	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BTN_QUIT, m_btnQuit);
	DDX_Control( pDX, IDC_BTN_SEPERATOR, m_btnSeperator );
}

BOOL TplDesignConfigDlg::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	/************************************************************************/
	// layout the controls on the dialog.

	// layout the buttons.
	AddAnchor(IDC_BTN_TEMPLATE, TOP_LEFT);
	AddAnchor(IDC_BTN_DISPLAY, TOP_LEFT);
	AddAnchor(IDC_BTN_DESIGN, TOP_LEFT);

	AddAnchor(IDC_BTN_SAVE, TOP_RIGHT);
	AddAnchor(IDC_BTN_QUIT, TOP_RIGHT);

	AddAnchor(IDC_BTN_SEPERATOR, TOP_LEFT, TOP_RIGHT);

	// layout the child pages.
	AddAnchor(IDC_STATIC_CHILD_AREA, TOP_LEFT, BOTTOM_RIGHT);
	/************************************************************************/

	// load the preference info.
	m_pTplDesignConfig = TplDesignConfigLoader::LoadTplDesignConfig();

	/************************************************************************/
	// init sub-dialogs.

	m_iConfigType = CONFIG_TEMPLATE;

	InitSubDialog();
	RepositionSubDlg();

	// display the first page.
	m_tplParamPage.ShowWindow(SW_SHOW);
	m_tplParamPage.DisplayTemplateParam(m_pTplDesignConfig->GetTemplateParam(),
										m_pTplDesignConfig->GetTplDisplayParam()->GetTplUnit());
	/************************************************************************/

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	return TRUE;
}

void TplDesignConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

	// re-position the sub dialog.
	RepositionSubDlg();
}

HBRUSH TplDesignConfigDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void TplDesignConfigDlg::OnTemplateConfig()
{
	if (m_iConfigType == CONFIG_TEMPLATE)
		return;

	// before switch page, check whether we need to save the previous page.
	CheckModification();

	// switch to ProductParamPage.
	m_iConfigType = CONFIG_TEMPLATE;
	RepositionSubDlg();
	m_tplParamPage.DisplayTemplateParam(m_pTplDesignConfig->GetTemplateParam(),
										m_pTplDesignConfig->GetTplDisplayParam()->GetTplUnit());
}

void TplDesignConfigDlg::OnDisplayConfig()
{
	if (m_iConfigType == CONFIG_DISPLAY)
		return;

	// before switch page, check whether we need to save the previous page.
	CheckModification();

	// switch to ImpExpConfigPage.
	m_iConfigType = CONFIG_DISPLAY;
	RepositionSubDlg();
	m_TplDisplayParamPage.DisplayDisParam(m_pTplDesignConfig->GetTplDisplayParam());
}

void TplDesignConfigDlg::OnDesignConfig()
{
	if (m_iConfigType == CONFIG_DESIGN)
		return;

	// before switch page, check whether we need to save the previous page.
	CheckModification();

	// switch to ColorParamPage.
	m_iConfigType = CONFIG_DESIGN;
	RepositionSubDlg();
	m_designParamPage.DisplayTplEditParam(m_pTplDesignConfig->GetTplEditParam(),
										  m_pTplDesignConfig->GetTplDisplayParam()->GetTplUnit());
}

void TplDesignConfigDlg::OnSave()
{
	CString strSoluName, str;
	strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// check whether some param was changed.
	BOOL bChanged = FALSE;
	if (m_iConfigType == CONFIG_TEMPLATE)
	{
		TemplateParamPtr pTemplateParam = m_tplParamPage.GetTemplateParam();
		if (pTemplateParam->IsModified())
			bChanged = TRUE;
	}
	else if (m_iConfigType == CONFIG_DISPLAY)
	{
		TplDisplayParamPtr pTplDisplayParam = m_TplDisplayParamPage.GetTplDisplayParam();
		if (pTplDisplayParam->IsModified())
			bChanged = TRUE;
	}
	else if (m_iConfigType == CONFIG_DESIGN)
	{
		TplEditParamPtr pTplEditParam = m_designParamPage.GetTplEditParam();
		if (pTplEditParam->IsModified())
			bChanged = TRUE;
	}

	if (bChanged)
	{
		// save
		TplDesignConfigWriter::SaveTplDesignConfig(m_pTplDesignConfig);

		// need to update the singleton.
		TplDesignConfigPtr pTplDesignConfig = TplDesignConfigLoader::LoadTplDesignConfig();
		TplDesignConfig::Init(pTplDesignConfig);

		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_RPT_CHANGE_SAVED);
		MessageBox(str, strSoluName, MB_ICONINFORMATION);
	}
	else
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_RPT_NO_CHANGE);
		MessageBox(str, strSoluName, MB_ICONWARNING);
	}

	OnOK();
}

void TplDesignConfigDlg::OnQuit()
{
	OnOK();
}

void TplDesignConfigDlg::InitSubDialog()
{
	// create TplParamPage.
	m_tplParamPage.Create(IDD_clReportConfigUI_Template, this);
	m_tplParamPage.ShowWindow(SW_HIDE);

	// create TplDisplayParamPage.
	m_TplDisplayParamPage.Create(IDD_clReportConfigUI_Display, this);
	m_TplDisplayParamPage.ShowWindow(SW_HIDE);

	// create DesignParamPage.
	m_designParamPage.Create(IDD_clReportConfigUI_Design, this);
	m_designParamPage.ShowWindow(SW_HIDE);
}

void TplDesignConfigDlg::HideAllSubDialog()
{
	m_tplParamPage.ShowWindow(SW_HIDE);
	m_TplDisplayParamPage.ShowWindow(SW_HIDE);
	m_designParamPage.ShowWindow(SW_HIDE);
}

void TplDesignConfigDlg::RepositionSubDlg()
{
	if (m_iConfigType == CONFIG_NOT_INIT)
		return;

	/************************************************************************/
	// calculate the position of the child page.

	CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILD_AREA);
	CRect pagePos;
	if (pWndPos)
	{
		pWndPos->GetWindowRect(&pagePos);
		ScreenToClient(&pagePos);
	}
	/************************************************************************/

	/************************************************************************/
	// because the layout library will re-layout the controls, so we should re-position the sub dialog.

	HideAllSubDialog();

	if (m_iConfigType == CONFIG_TEMPLATE)
	{
		m_tplParamPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_tplParamPage.ShowWindow(SW_SHOW);
	}
	else if (m_iConfigType == CONFIG_DISPLAY)
	{
		m_TplDisplayParamPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_TplDisplayParamPage.ShowWindow(SW_SHOW);
	}
	else if (m_iConfigType == CONFIG_DESIGN)
	{
		m_designParamPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_designParamPage.ShowWindow(SW_SHOW);
	}
	/************************************************************************/
}

void TplDesignConfigDlg::CheckModification()
{
	CString strSoluName, str;
	strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_iConfigType == CONFIG_TEMPLATE)
	{
		TemplateParamPtr pTemplateParam = m_tplParamPage.GetTemplateParam();
		if (pTemplateParam->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_TPL_PARAM);
			if (MessageBox(str, strSoluName, MB_YESNO | MB_ICONWARNING) == IDYES)
			{
				// save
				TplDesignConfigWriter::SaveTplDesignConfig(m_pTplDesignConfig);

				// need to update the singleton.
				TplDesignConfigPtr pTplDesignConfig = TplDesignConfigLoader::LoadTplDesignConfig();
				TplDesignConfig::Init(pTplDesignConfig);
			}
			else
				m_pTplDesignConfig = TplDesignConfigLoader::LoadTplDesignConfig();
		}
	}
	else if (m_iConfigType == CONFIG_DISPLAY)
	{
		TplDisplayParamPtr pTplDisplayParam = m_TplDisplayParamPage.GetTplDisplayParam();
		if (pTplDisplayParam->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_RPT_DISPLAY_PARAM);
			if (MessageBox(str, strSoluName, MB_YESNO | MB_ICONWARNING) == IDYES)
			{
				// save
				TplDesignConfigWriter::SaveTplDesignConfig(m_pTplDesignConfig);

				// need to update the singleton.
				TplDesignConfigPtr pTplDesignConfig = TplDesignConfigLoader::LoadTplDesignConfig();
				TplDesignConfig::Init(pTplDesignConfig);
			}
			else
				m_pTplDesignConfig = TplDesignConfigLoader::LoadTplDesignConfig();
		}
	}
	else if (m_iConfigType == CONFIG_DESIGN)
	{
		TplEditParamPtr pTplEditParam = m_designParamPage.GetTplEditParam();
		if (pTplEditParam->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_DESIGN_PARAM);
			if (MessageBox(str, strSoluName, MB_YESNO | MB_ICONWARNING) == IDYES)
			{
				// save
				TplDesignConfigWriter::SaveTplDesignConfig(m_pTplDesignConfig);

				// need to update the singleton.
				TplDesignConfigPtr pTplDesignConfig = TplDesignConfigLoader::LoadTplDesignConfig();
				TplDesignConfig::Init(pTplDesignConfig);
			}
			else
				m_pTplDesignConfig = TplDesignConfigLoader::LoadTplDesignConfig();
		}
	}
}

END_CUTLEADER_NAMESPACE()

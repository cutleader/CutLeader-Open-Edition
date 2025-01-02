#include "StdAfx.h"
#include "RibbonBar.h"

#include "baseConst.h"
#include "MultiLanguageMgr.h"
#include "clUtilityResource.h"
#include "XRibbonBtn.h"
#include "clGeometryBaseResource.h"
#include "clUILibResource.h"
#include "clBaseDataResource.h"
#include "clGeometryFeatureResource.h"
#include "SkinManager.h"
#include "clBaseDataUIResource.h"
#include "LogMgr.h"

#include "CLOptionInfo.h"
#include "ConcealConfig.h"
#include "DebugUIInfo.h"
#include "ProductParam.h"
#include "clExpertLibResource.h"
#include "clOptionResource.h"
#include "clKnowledgeBaseConfigResource.h"
#include "clMaterialLibResource.h"
#include "clReportResource.h"

#include "clPartResource.h"
#include "clPartLayoutResource.h"
#include "clCutFeatureResource.h"
#include "clCutSequenceResource.h"
#include "clDataImportExportResource.h"
#include "clNestResource.h"
#include "clSheetResource.h"
#include "clPostProcessorResource.h"
#include "clDataManagerResource.h"
#include "clFrameResource.h"
#include "taoresource.h"
#include "RecentData.h"
#include "RecentDataList.h"
#include "DataItemTracker.h"
#include "clFrameUIResource.h"
#include "DocManager.h"
#include "PartInplaceEditData.h"
#include "clGeometryFeatureUIResource.h"

BEGIN_CUTLEADER_NAMESPACE()

RibbonBar::RibbonBar(void)
{
}

RibbonBar::~RibbonBar(void)
{
}

void RibbonBar::Init(CWnd* pParentWnd)
{
	CString str;

	// create ribbon bar.
	this->Create(pParentWnd);

	// init main button.
	m_mainButton.SetImage(IDB_MAIN);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FILE);
	m_mainButton.SetToolTipText(str);
	this->SetApplicationButton(&m_mainButton, CSize (45, 45));


	/************************************************************************/
	// 初始化各个类别。

	this->RemoveAllCategories();

	// create main category.
	CreateMainCategory();

	// create "Start" category.
	CreateStartCategory();

	// 创建“几何图形”类别。
	CreateGeomeCategory();

	// 创建“加工工艺”类别。
	CreateCutFeatCategory();

	// 创建“零件布局”类别。
	CreatePartLayoutCategory();

	// 创建“工序”类别。
	CreateSequenceCategory();

	// 创建“配置”类别。
	CreateConfigCategory();

	// create "Help" category.
	CreateHelpCategory();

	// 创建快捷工具条。
	CreateQuickAccess();
	/************************************************************************/


	// 更新工具条的可见性。
	SetCategoryVisibility();

	// 选中默认类别。
	CMFCRibbonCategory* pActiveCategory = this->GetCategory(0);
	this->SetActiveCategory(pActiveCategory);
}

void RibbonBar::SetCategoryVisibility()
{
	if (this->GetCategoryCount() < 7) {
		LogMgr::DebugWarn(_T("452798"));
		return;
	}

	if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp())
	{
		if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) {
			this->ShowCategory(0, TRUE);
			this->ShowCategory(1, TRUE);
			this->ShowCategory(2, TRUE);
			this->ShowCategory(3, FALSE);
			this->ShowCategory(4, FALSE);
			this->ShowCategory(5, FALSE);
			this->ShowCategory(6, FALSE);
		}
		else {
			this->ShowCategory(0, TRUE);
			this->ShowCategory(1, FALSE);
			this->ShowCategory(2, TRUE);
			this->ShowCategory(3, TRUE);
			this->ShowCategory(4, TRUE);
			this->ShowCategory(5, FALSE);
			this->ShowCategory(6, TRUE);
		}
	}
	else
	{
		if (PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) {
			this->ShowCategory(0, TRUE);
			this->ShowCategory(1, TRUE);
			this->ShowCategory(2, TRUE);
			this->ShowCategory(3, FALSE);
			this->ShowCategory(4, FALSE);
			this->ShowCategory(5, FALSE);
			this->ShowCategory(6, FALSE);
		}
		else {
			this->ShowCategory(0, TRUE);
			this->ShowCategory(1, TRUE);
			this->ShowCategory(2, TRUE);
			this->ShowCategory(3, TRUE);
			this->ShowCategory(4, TRUE);
			this->ShowCategory(5, TRUE);
			this->ShowCategory(6, TRUE);
		}
	}
}

void RibbonBar::UpdateRecentDataMenu()
{
	// get the "Recent Data" button.
	CMFCRibbonBaseElement* pElement = this->FindByID(IDC_RECENT_DATA);
	CMFCRibbonButton* pRecentDataBtn = dynamic_cast<CMFCRibbonButton*>(pElement);

	// clear exist sub items.
	pRecentDataBtn->RemoveAllSubItems();

	// append the sub items.
	RecentDataListPtr pRecentDataList = DataItemTracker::GetInstance()->GetRecentData();
	for (unsigned int i = 0; i < pRecentDataList->size(); i++)
	{
		if (i < RECENT_DATA_COUNT)
		{
			RecentDataPtr pRecentData = pRecentDataList->at(i);

			// the string which will display.
			CString strContent;
			strContent.Format(_T("%d: "), i+1);
			strContent += pRecentData->GetDataPath();

			// add the sub menu.
			pRecentDataBtn->AddSubItem(new CMFCRibbonButton(IDC_RECENT_DATA_BASE+i, strContent, 0, -1));
		}
	}
}

void RibbonBar::HighlightBtn(CString strCategoryName, int iBtnID, CString strTips)
{
	// 激活指定的category.
	if (GetCurCategoryName() != strCategoryName)
	{
		int iIndex = GetCategoryIndex(strCategoryName);
		CMFCRibbonCategory* pRibbonCategory = this->GetCategory(iIndex);
		this->SetActiveCategory(pRibbonCategory);
	}

	// 突出按钮显示。
	CMFCRibbonBaseElement* pElement = this->FindByID(iBtnID);
	XRibbonBtn* pXRibbonBtn = (XRibbonBtn*)pElement;
	pXRibbonBtn->EnableTips(strTips);
	this->RecalcLayout();
	pXRibbonBtn->DisableTips();
}

void RibbonBar::CreateMainCategory()
{
	CString str;

	// get main category and clear it.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FILE);
	CMFCRibbonMainPanel* pMainCategory = this->AddMainCategory(str, IDB_MAIN_MENU_SMALL, IDB_MAIN_MENU);

	// the "Data Management" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DATA_MANAGEMENT);
	CMFCRibbonButton* pBtnDataMgr = new CMFCRibbonButton(IDC_DATA_MANAGEMENT, str, 5, 0);
	pMainCategory->Add(pBtnDataMgr);

	pMainCategory->Add(new CMFCRibbonSeparator(TRUE));

	// "Save" menu
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SAVE);
	pMainCategory->Add(new CMFCRibbonButton(IDC_TAO_FILE_SAVE, str, 3, 4));

	// "Save As" menu
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SAVEAS);
	pMainCategory->Add(new CMFCRibbonButton(IDC_TAO_FILE_SAVEAS, str, 4, 5));

	pMainCategory->Add(new CMFCRibbonSeparator(TRUE));

	// add "Recent Data" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RECENT_DATA);
	CMFCRibbonButton* pRecentDataBtn = new CMFCRibbonButton(IDC_RECENT_DATA, str, -1, 6);
	pMainCategory->Add(pRecentDataBtn);


	/************************************************************************/
	// the button on the bottom.

	// "Preference" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OPTIONS);
	pMainCategory->AddToBottom(new CMFCRibbonMainPanelButton(IDC_OPTIONS, str, 1));

	// "Exit" menu
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXIT);
	CMFCRibbonMainPanelButton* pExitButton = new CMFCRibbonMainPanelButton(ID_APP_EXIT, str, 2);
	pMainCategory->AddToBottom(pExitButton);
	/************************************************************************/
}

void RibbonBar::CreateQuickAccess()
{
	CList<UINT, UINT> quickAccessBtns;
	this->SetQuickAccessCommands(quickAccessBtns);

	// 数据管理。
	quickAccessBtns.AddTail(IDC_DATA_MANAGEMENT);

	// save/saveas button.
	quickAccessBtns.AddTail(IDC_TAO_FILE_SAVE);
	quickAccessBtns.AddTail(IDC_TAO_FILE_SAVEAS);

	// undo/redo button.
	quickAccessBtns.AddTail(IDC_TAO_UNDO);
	quickAccessBtns.AddTail(IDC_TAO_REDO);

	this->SetQuickAccessCommands(quickAccessBtns);
}

void RibbonBar::CreateStartCategory()
{
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_QUICK_START);
	CMFCRibbonCategory* pStartCategory = this->AddCategory(str, IDB_QUICK_START_BAR_SMALL, IDB_QUICK_START_BAR);
	AddGroups2StartCategory(pStartCategory);
}

void RibbonBar::AddGroups2StartCategory(CMFCRibbonCategory* pStartCategory)
{
	CString str;

	// “数据中心”按钮。
	{
		CMFCRibbonPanel* pDataCenterPanel = pStartCategory->AddPanel(_T(""));

		// "Data Management" button.
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DATA_MANAGEMENT);
		CMFCRibbonButton* pBtnDataMgr = new CMFCRibbonButton(IDC_DATA_MANAGEMENT, str, -1, 0);
		pBtnDataMgr->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DATA_MANAGEMENT_TIP);
		pBtnDataMgr->SetDescription(str);
		pDataCenterPanel->Add(pBtnDataMgr);
	}

	// “新建任务”面板。
	if (!PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode())
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Create_Task);
		CMFCRibbonPanel* pTaskPanel = pStartCategory->AddPanel(str);

		// 从自动排版创建任务
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FromAutoNesting);
		CMFCRibbonButton* pButton_1 = new CMFCRibbonButton(IDC_NewTaskFromNesting, str, -1, 1);
		pButton_1->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CreateTask_Tip_2);
		pButton_1->SetDescription(str);
		pTaskPanel->Add(pButton_1);

		// 从一个空板材创建任务
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FromEmptyInitalSheet);
		CMFCRibbonButton* pButton_2 = new CMFCRibbonButton(IDC_NewTaskFromEmptySheet, str, -1, 2);
		pButton_2->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CreateTask_Tip_1);
		pButton_2->SetDescription(str);
		pTaskPanel->Add(pButton_2);
	}

	// “新建零件”面板。
	if (!CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp() || PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode())
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Create_Part);
		CMFCRibbonPanel* pPartPanel = pStartCategory->AddPanel(str);

		// 从dxf/dwg新建零件
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FromDxfDwg);
		CMFCRibbonButton* pBtnDxfOpen = new CMFCRibbonButton(IDC_NewPartFromDxfdwg, str, -1, 4);
		pBtnDxfOpen->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OPEN_DXF_TIP);
		pBtnDxfOpen->SetDescription(str);
		pPartPanel->Add(pBtnDxfOpen);

		// 新建空零件
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EmptyPart);
		CMFCRibbonButton* pBtnPartCreate = new CMFCRibbonButton(IDC_NewEmptyPart, str, -1, 3);
		pBtnPartCreate->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CREATE_PART_TIP);
		pBtnPartCreate->SetDescription(str);
		pPartPanel->Add(pBtnPartCreate);
	}

	// 报表面板。
	if (!CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp() && !PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode())
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_REPORT);
		CMFCRibbonPanel* pReportPanel = pStartCategory->AddPanel(str);

		// 任务报表
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRINT_TaskReport);
		CMFCRibbonButton* pPrintTaskRptBtn = new CMFCRibbonButton(IDC_PrintTaskReport, str, -1, 5);
		pPrintTaskRptBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_PreviewTaskReport));
		pPrintTaskRptBtn->SetDefaultCommand(IDC_PrintTaskReport);
		pReportPanel->Add(pPrintTaskRptBtn);
		pStartCategory->AddHidden(new CMFCRibbonUndoButton(IDC_PreviewTaskReport, _T(""), 0));

		// 板材报表
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRINT_SheetReport);
		CMFCRibbonButton* pPrintSheetRptBtn = new CMFCRibbonButton(IDC_PrintSheetReport, str, -1, 6);
		pPrintSheetRptBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_PreviewSheetReport));
		pPrintSheetRptBtn->SetDefaultCommand(IDC_PrintSheetReport);
		pReportPanel->Add(pPrintSheetRptBtn);
		pStartCategory->AddHidden(new CMFCRibbonUndoButton(IDC_PreviewSheetReport, _T(""), 0));

		// 零件标签
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRINT_LABEL);
		CMFCRibbonButton* pPrintPartLabelBtn = new CMFCRibbonButton(IDC_PrintPartLabel, str, -1, 7);
		pPrintPartLabelBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_PreviewPartLabel));
		pPrintPartLabelBtn->SetDefaultCommand(IDC_PrintPartLabel);
		pReportPanel->Add(pPrintPartLabelBtn);
		pStartCategory->AddHidden(new CMFCRibbonUndoButton(IDC_PreviewPartLabel, _T(""), 0));
	}

	// 配置面板
	if (!CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp() && !PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode())
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CONFIG);
		CMFCRibbonPanel* pConfigPanel = pStartCategory->AddPanel(str);

		// “加工参数”按钮。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CAM_PARAM);
		CMFCRibbonButton* pParamConfigBtn = new CMFCRibbonButton(IDC_PARAM_CONFIG, str, -1, 8);
		pParamConfigBtn->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CAM_PARAM_TIP);
		pParamConfigBtn->SetDescription(str);
		pConfigPanel->Add(pParamConfigBtn);

		// “系统参数”按钮。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OPTIONS);
		CMFCRibbonButton* pSystemConfigBtn = new CMFCRibbonButton(IDC_OPTIONS, str, -1, 9);
		pSystemConfigBtn->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OPTIONS_TIP);
		pSystemConfigBtn->SetDescription(str);
		pConfigPanel->Add(pSystemConfigBtn);

		// "default param" button.
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetDefaultParam);
		CMFCRibbonButton* pDflParamBtn = new CMFCRibbonButton(IDC_SetDefaultParam, str, -1, 10);
		pDflParamBtn->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetDefaultParam_Tip);
		pDflParamBtn->SetDescription(str);
		pConfigPanel->Add(pDflParamBtn);
	}
}

void RibbonBar::CreateGeomeCategory()
{
	CString str;

	// add "cad" category
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY);
	CMFCRibbonCategory* pGeomCategory = this->AddCategory(str, IDB_GEOM_BAR, IDB_GEOM_LARGE_BAR);


	/************************************************************************/
	// add "Draw Basic Geometry" panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_BASIC_GEOM);
	CMFCRibbonPanel* pPanelDrawBasic = pGeomCategory->AddPanel(str);

	// "line" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_LINE);
	CMFCRibbonButton* pLineBtn = new CMFCRibbonButton(IDC_GEOM_DRAW_STDLINE, str, 0);
	pLineBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_GEOM_DRAW_LINE));
	pLineBtn->SetDefaultCommand(IDC_GEOM_DRAW_STDLINE);
	pLineBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_LINE_TIP);
	pLineBtn->SetDescription(str);
	pPanelDrawBasic->Add(pLineBtn);
	pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_GEOM_DRAW_MULTILINE, _T(""), 1));
	pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_GEOM_DRAW_RECTLINE, _T(""), 2));

	// "Arc" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_DRAW_3PtArc);
	CMFCRibbonButton* pArcBtn = new CMFCRibbonButton(IDC_GEOM_DRAW_ARC3PT, str, 3);
	pArcBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_GEOM_DRAW_ARC));
	pArcBtn->SetDefaultCommand(IDC_GEOM_DRAW_ARC3PT);
	pArcBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_ARC_TIP);
	pArcBtn->SetDescription(str);
	pPanelDrawBasic->Add(pArcBtn);
	pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_GEOM_DRAW_STDARC, _T(""), 4));

	// "Round Corner line/arcs" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_DRAW_RCLINEARC);
	CMFCRibbonButton* pRCLineArcBtn = new CMFCRibbonButton(IDC_GEOM_DRAW_RCLINEARC, str, 13);
	pRCLineArcBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_DRAW_RCLINEARC_TIP);
	pRCLineArcBtn->SetDescription(str);
	pPanelDrawBasic->Add(pRCLineArcBtn);
	/************************************************************************/


	/************************************************************************/
	// 添加“绘制标准图形”面板。

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_STD_GEOM);
	CMFCRibbonPanel* pPanelDrawStd = pGeomCategory->AddPanel(str);

	// "Round Hole" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_ROUNDHOLE);
	CMFCRibbonButton* pRDHoleBtn = new CMFCRibbonButton(IDC_GEOM_DRAW_ROUNDHOLE, str, 7);
	pRDHoleBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_GEOM_DRAW_RDHOLE));
	pRDHoleBtn->SetDefaultCommand(IDC_GEOM_DRAW_ROUNDHOLE);
	pRDHoleBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_ROUNDHOLE_TIP);
	pRDHoleBtn->SetDescription(str);
	pPanelDrawStd->Add(pRDHoleBtn);
	pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_GEOM_DRAW_ELLIPSE, _T(""), 9));
	pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_GEOM_DRAW_OBROUND, _T(""), 8));

	// "rect" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_RECTHOLE);
	CMFCRibbonButton* pRectHoleBtn = new CMFCRibbonButton(IDC_GEOM_DRAW_RECTHOLE, str, 5);
	pRectHoleBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_GEOM_DRAW_RECTHOLE));
	pRectHoleBtn->SetDefaultCommand(IDC_GEOM_DRAW_RECTHOLE);
	pRectHoleBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_RECTHOLE_TIP);
	pRectHoleBtn->SetDescription(str);
	pPanelDrawStd->Add(pRectHoleBtn);
	pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_GEOM_DRAW_RCHOLE, _T(""), 12));

	// "D Shape" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_SINGLED);
	CMFCRibbonButton* pDShapeBtn = new CMFCRibbonButton(IDC_GEOM_DRAW_SINGLED, str, 10);
	pDShapeBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_GEOM_DRAW_DSHAPE));
	pDShapeBtn->SetDefaultCommand(IDC_GEOM_DRAW_SINGLED);
	pDShapeBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_SINGLED_TIP);
	pDShapeBtn->SetDescription(str);
	pPanelDrawStd->Add(pDShapeBtn);
	pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_GEOM_DRAW_DOUBLED, _T(""), 11));

	// "Triangle Hole" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_TRHOLE);
	CMFCRibbonButton* pTriHoleBtn = new CMFCRibbonButton(IDC_GEOM_DRAW_TRIHOLE, str, 6);
	pTriHoleBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_TRHOLE_TIP);
	pTriHoleBtn->SetDescription(str);
	pPanelDrawStd->Add(pTriHoleBtn);

	// "Radius Slot"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_RADSLOT);
	CMFCRibbonButton* pRSlotBtn = new CMFCRibbonButton(IDC_GEOM_DRAW_RADSLOT, str, 14);
	pRSlotBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_RADSLOT_TIP);
	pRSlotBtn->SetDescription(str);
	pPanelDrawStd->Add(pRSlotBtn);
	/************************************************************************/


	/************************************************************************/
	// add "Draw Array Geometry" panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DRAW_ARRAY_GEOM);
	CMFCRibbonPanel* pPanelDrawArray = pGeomCategory->AddPanel(str);

	// "pattern line" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_PATLINE);
	CMFCRibbonButton* pPatLineBtn = new CMFCRibbonButton(IDC_GEOM_DRAW_PATLINE, str, 15);
	pPatLineBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_PATLINE_TIP);
	pPatLineBtn->SetDescription(str);
	pPanelDrawArray->Add(pPatLineBtn);

	// "pattern arc" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_PATARC);
	CMFCRibbonButton* pPatArcBtn = new CMFCRibbonButton(IDC_GEOM_DRAW_PATARC, str, 17);
	pPatArcBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_PATARC_TIP);
	pPatArcBtn->SetDescription(str);
	pPanelDrawArray->Add(pPatArcBtn);

	// "pattern circle" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_PATCIRCLE);
	CMFCRibbonButton* pPatCircleBtn = new CMFCRibbonButton(IDC_GEOM_DRAW_PATCIRCLE, str, 18);
	pPatCircleBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_PATCIRCLE_TIP);
	pPatCircleBtn->SetDescription(str);
	pPanelDrawArray->Add(pPatCircleBtn);

	// "pattern grid" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_PATGRID);
	CMFCRibbonButton* pPatGridBtn = new CMFCRibbonButton(IDC_GEOM_DRAW_PATGRID, str, 16);
	pPatGridBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY_PATGRID_TIP);
	pPatGridBtn->SetDescription(str);
	pPanelDrawArray->Add(pPatGridBtn);
	/************************************************************************/


	/************************************************************************/
	// 绘制文字。

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DrawText);
	CMFCRibbonPanel* pDrawTextPanel = pGeomCategory->AddPanel(str);
	CMFCRibbonButton* pDrawTextBtn = new CMFCRibbonButton(IDC_DrawText, str, -1, 6);
	pDrawTextBtn->SetToolTipText(str);
	pDrawTextPanel->Add(pDrawTextBtn);
	/************************************************************************/


	/************************************************************************/
	// add "Edit" panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT);
	CMFCRibbonPanel* pPanelEdit = pGeomCategory->AddPanel(str);

	// "Select Pattern"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SELECT_GEOM);
	CMFCRibbonButton* pEditPatBtn = new CMFCRibbonButton(IDC_GEOM_EDIT_SELECTPAT, str, -1, 0);
	pEditPatBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SELECT_GEOM_TIP);
	pEditPatBtn->SetDescription(str);
	pPanelEdit->Add(pEditPatBtn);

	// add a separator.
	pPanelEdit->AddSeparator();

	// "Delete"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE);
	CMFCRibbonButton* pDeletePatBtn = new CMFCRibbonButton(IDC_GEOM_EDIT_DELETE, str, 25);
	pDeletePatBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE_GEOM_TIP);
	pDeletePatBtn->SetDescription(str);
	pPanelEdit->Add(pDeletePatBtn);

	// "Explode"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXPLODE);
	CMFCRibbonButton* pExplodePatBtn = new CMFCRibbonButton(IDC_GEOM_EDIT_EXPLODE, str, 26);
	pExplodePatBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXPLODE_GEOM_TIP);
	pExplodePatBtn->SetDescription(str);
	pPanelEdit->Add(pExplodePatBtn);

	// add a separator.
	pPanelEdit->AddSeparator();

	// "Copy"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COPY);
	CMFCRibbonButton* pCopyBtn = new CMFCRibbonButton(IDC_GEOM_EDIT_COPY, str, 24);
	pCopyBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COPY_GEOM_TIP);
	pCopyBtn->SetDescription(str);
	pPanelEdit->Add(pCopyBtn);

	// "Move"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MOVE);
	CMFCRibbonButton* pMoveBtn = new CMFCRibbonButton(IDC_GEOM_EDIT_MOVE, str, 27);
	pMoveBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MOVE_GEOM_TIP);
	pMoveBtn->SetDescription(str);
	pPanelEdit->Add(pMoveBtn);

	// "Rotate"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ROTATE);
	CMFCRibbonButton* pRotateBtn = new CMFCRibbonButton(IDC_GEOM_EDIT_ROTATE, str, 28);
	pRotateBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ROTATE_GEOM_TIP);
	pRotateBtn->SetDescription(str);
	pPanelEdit->Add(pRotateBtn);

	// 轮廓对齐。
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_AlignLoop_Center);
		CMFCRibbonButton* pAlignBtn = new CMFCRibbonButton(IDC_AlignLoop_Center, str, 35);
		pAlignBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_AlignLoop));
		pAlignBtn->SetDefaultCommand(IDC_AlignLoop_Center);
		pAlignBtn->SetToolTipText(str);
		pPanelEdit->Add(pAlignBtn);
		pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_AlignLoop_Left, _T(""),29));
		pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_AlignLoop_Right, _T(""), 30));
		pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_AlignLoop_HorizontalCenter, _T(""), 34));
		pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_AlignLoop_Top, _T(""), 31));
		pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_AlignLoop_Bottom, _T(""), 32));
		pGeomCategory->AddHidden(new CMFCRibbonUndoButton(IDC_AlignLoop_VerticalCenter, _T(""), 33));
	}

	// add a separator.
	pPanelEdit->AddSeparator();

	// "Optimize" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_OPTIMIZATION);
	CMFCRibbonButton* pOptBtn = new CMFCRibbonButton(IDC_GEOM_EDIT_OPTIMIZE, str, -1, 2);
	pOptBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OPTIMIZE_GEOM_TIP);
	pOptBtn->SetDescription(str);
	pPanelEdit->Add(pOptBtn);

	// “偏移回路”按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_OFFSET_LOOP);
	CMFCRibbonButton* pOffsetLoopBtn = new CMFCRibbonButton(IDC_GEOM_OFFSET_LOOP, str, -1, 1);
	pOffsetLoopBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OFFSET_LOOP_TIP);
	pOffsetLoopBtn->SetDescription(str);
	pPanelEdit->Add(pOffsetLoopBtn);
	/************************************************************************/


	/************************************************************************/
	// “显示比例”面板。

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_VIEW);
	CMFCRibbonPanel* pPanelView = pGeomCategory->AddPanel(str);

	// add "Zoom All" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_VIEW_ZOOMALL);
	CMFCRibbonButton* pZoomAllBtn = new CMFCRibbonButton(IDC_GEOM_VIEW_ZOOMALL, str, -1, 3);
	pZoomAllBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_VIEW_ZOOMALL_TIP);
	pZoomAllBtn->SetDescription(str);
	pPanelView->Add(pZoomAllBtn);

	// add "Zoom In" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_VIEW_ZOOMIN);
	CMFCRibbonButton* pZoomInBtn = new CMFCRibbonButton(IDC_GEOM_VIEW_ZOOMIN, str, -1, 4);
	pZoomInBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_VIEW_ZOOMIN_TIP);
	pZoomInBtn->SetDescription(str);
	pPanelView->Add(pZoomInBtn);

	// add "Zoom Out" button
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_VIEW_ZOOMOUT);
	CMFCRibbonButton* pZoomOutBtn = new CMFCRibbonButton(IDC_GEOM_VIEW_ZOOMOUT, str, -1, 5);
	pZoomOutBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_VIEW_ZOOMOUT_TIP);
	pZoomOutBtn->SetDescription(str);
	pPanelView->Add(pZoomOutBtn);
	/************************************************************************/
}

void RibbonBar::CreateCutFeatCategory()
{
	CString str;

	// add "CAM Feature" category
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE);
	CMFCRibbonCategory* pCutFeatureCategory = this->AddCategory(str, IDB_CUTFEATURE_BAR_SMALL, IDB_CUTFEATURE_BAR_LARGE);


	/************************************************************************/
	// add "Process CAM Feature" panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_AUTO_PROCESS);
	CMFCRibbonPanel* pProcessPartPanel = pCutFeatureCategory->AddPanel(str);

	// add "Assign CAM Feature" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ASSIGN_CUTFEATURE);
	CMFCRibbonButton* pAssignCAMBtn = new CMFCRibbonButton(IDC_CUTFEATURE_ASSIGN, str, -1, 0);
	pAssignCAMBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ASSIGN_CUTFEATURE_TIP);
	pAssignCAMBtn->SetDescription(str);
	pProcessPartPanel->Add(pAssignCAMBtn);

	// add "Reset CAM Feature" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RESET_CUTFEATURE);
	CMFCRibbonButton* pResetCAMBtn = new CMFCRibbonButton(IDC_CUTFEATURE_RESET, str, -1, 1);
	pResetCAMBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RESET_CUTFEATURE_TIP);
	pResetCAMBtn->SetDescription(str);
	pProcessPartPanel->Add(pResetCAMBtn);

	// add "Check Interfere" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CHECK_INTERFERE);
	CMFCRibbonButton* pIntfCheckBtn = new CMFCRibbonButton(IDC_CHECK_INTERFERE, str, -1, 2);
	pIntfCheckBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CHECK_INTERFERE_TIP);
	pIntfCheckBtn->SetDescription(str);
	pProcessPartPanel->Add(pIntfCheckBtn);
	/************************************************************************/


    // "轮廓刀具"面板
    {
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LOOPTOOL_FEATURE);
        CMFCRibbonPanel* pLoopToolPanel = pCutFeatureCategory->AddPanel(str);

        // add "Edit Condition" button.
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT);
        CMFCRibbonButton* pEditLoopToolBtn = new CMFCRibbonButton(IDC_CUTFEATURE_LOOPTOOL_EDIT, str, -1, 13);
        pEditLoopToolBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_ReAssignLoopTool));
        pEditLoopToolBtn->SetDefaultCommand(IDC_CUTFEATURE_LOOPTOOL_EDIT);
        pEditLoopToolBtn->SetToolTipText(str);
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LOOPTOOL_TIP);
        pEditLoopToolBtn->SetDescription(str);
        pLoopToolPanel->Add(pEditLoopToolBtn);
        pCutFeatureCategory->AddHidden(new CMFCRibbonUndoButton(IDC_ReAssignLoopTool, _T(""), 0));
	}


	/************************************************************************/
	// add the start end cut panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_STARTEND_CUT);
	CMFCRibbonPanel* pPanelLead = pCutFeatureCategory->AddPanel(str);

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LOOP_LEAD);
	CMFCRibbonButton* pEditLeadInBtn = new CMFCRibbonButton(IDC_CUTFEATURE_LOOP_LEAD_EDIT, str, -1, 3);
    pEditLeadInBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_ReAssignLeadLine));
    pEditLeadInBtn->SetDefaultCommand(IDC_CUTFEATURE_LOOP_LEAD_EDIT);
	pEditLeadInBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LEAD_TIP);
	pEditLeadInBtn->SetDescription(str);
	pPanelLead->Add(pEditLeadInBtn);
    pCutFeatureCategory->AddHidden(new CMFCRibbonUndoButton(IDC_ReAssignLeadLine, _T(""), 1));

	// 调整起切点。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LOOP_START_PT);
	CMFCRibbonButton* pEditStartPtBtn = new CMFCRibbonButton(IDC_CUTFEATURE_LOOP_START_PT, str, -1, 4);
    pEditStartPtBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_ReAssignStartCutPoint));
    pEditStartPtBtn->SetDefaultCommand(IDC_CUTFEATURE_LOOP_START_PT);
	pEditStartPtBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_START_PT_TIP);
	pEditStartPtBtn->SetDescription(str);
	pPanelLead->Add(pEditStartPtBtn);
    pCutFeatureCategory->AddHidden(new CMFCRibbonUndoButton(IDC_ReAssignStartCutPoint, _T(""), 2));

	// 增加“切换切割方向”按钮。
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SwitchCutDirection);
		CMFCRibbonButton* pChangeCutSideBtn = new CMFCRibbonButton(IDC_SwitchCutDirection, str, 8);
		pChangeCutSideBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_ChangeLoopCutDirection));
		pChangeCutSideBtn->SetDefaultCommand(IDC_SwitchCutDirection);
		pChangeCutSideBtn->SetToolTipText(str);
		pChangeCutSideBtn->SetDescription(MultiLanguageMgr::GetInstance()->TranslateString(IDS_CHANGE_CUTDIR_TIP));
		pPanelLead->Add(pChangeCutSideBtn);
		pCutFeatureCategory->AddHidden(new CMFCRibbonUndoButton(IDC_CcwCutDirection, _T(""), 9));
		pCutFeatureCategory->AddHidden(new CMFCRibbonUndoButton(IDC_CwCutDirection, _T(""), 10));
	}

	// 增加“切换内外切”按钮。
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetAsCutOutside);
		CMFCRibbonButton* pChangeCutSideBtn = new CMFCRibbonButton(IDC_CutOutSide, str, 5);
		pChangeCutSideBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_ChangeLoopCutSide));
		pChangeCutSideBtn->SetDefaultCommand(IDC_CutOutSide);
		pChangeCutSideBtn->SetToolTipText(str);
		pChangeCutSideBtn->SetDescription(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SWITCH_CUTSIDE_TIP));
		pPanelLead->Add(pChangeCutSideBtn);
		pCutFeatureCategory->AddHidden(new CMFCRibbonUndoButton(IDC_CutInside, _T(""), 6));
		pCutFeatureCategory->AddHidden(new CMFCRibbonUndoButton(IDC_CutOn, _T(""), 7));
	}
	/************************************************************************/


	/************************************************************************/
	// add the micro joint panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MICROJOINT);
	CMFCRibbonPanel* pPanelMicroJoint = pCutFeatureCategory->AddPanel(str);

	// "Add Microjoint"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADD);
	CMFCRibbonButton* pAddMicroJointBtn = new CMFCRibbonButton(IDC_CUTFEATURE_MJ_ADD, str, -1, 7);
    pAddMicroJointBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_ReAssignMicroJoint));
    pAddMicroJointBtn->SetDefaultCommand(IDC_CUTFEATURE_MJ_ADD);
	pAddMicroJointBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADD_MJ_TIP);
	pAddMicroJointBtn->SetDescription(str);
	pPanelMicroJoint->Add(pAddMicroJointBtn);
    pCutFeatureCategory->AddHidden(new CMFCRibbonUndoButton(IDC_ReAssignMicroJoint, _T(""), 3));

	// "Edit Microjoint"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT);
	CMFCRibbonButton* pEditMicroJointBtn = new CMFCRibbonButton(IDC_CUTFEATURE_MJ_EDIT, str, -1, 8);
	pEditMicroJointBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_MJ_TIP);
	pEditMicroJointBtn->SetDescription(str);
	pPanelMicroJoint->Add(pEditMicroJointBtn);

	// "Delete Microjoint"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE);
	CMFCRibbonButton* pDeleteMicroJointBtn = new CMFCRibbonButton(IDC_CUTFEATURE_MJ_DELETE, str, -1, 9);
	pDeleteMicroJointBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE_MJ_TIP);
	pDeleteMicroJointBtn->SetDescription(str);
	pPanelMicroJoint->Add(pDeleteMicroJointBtn);
	/************************************************************************/


	/************************************************************************/
	// add the Corner panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CORNER_FEATURE);
	CMFCRibbonPanel* pPanelCorner = pCutFeatureCategory->AddPanel(str);

	// "Add Corner"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADD);
	CMFCRibbonButton* pAddCornerBtn = new CMFCRibbonButton(IDC_CUTFEATURE_CORNER_ADD, str, -1, 10);
    pAddCornerBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_ReAssignCornerFeature));
    pAddCornerBtn->SetDefaultCommand(IDC_CUTFEATURE_CORNER_ADD);
	pAddCornerBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADD_CORNER_TIP);
	pAddCornerBtn->SetDescription(str);
	pPanelCorner->Add(pAddCornerBtn);
    pCutFeatureCategory->AddHidden(new CMFCRibbonUndoButton(IDC_ReAssignCornerFeature, _T(""), 4));

	// "Edit Corner"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT);
	CMFCRibbonButton* pEditCornerBtn = new CMFCRibbonButton(IDC_CUTFEATURE_CORNER_EDIT, str, -1, 11);
	pEditCornerBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_CORNER_TIP);
	pEditCornerBtn->SetDescription(str);
	pPanelCorner->Add(pEditCornerBtn);

	// "Delete Corner"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE);
	CMFCRibbonButton* pDeleteCornerBtn = new CMFCRibbonButton(IDC_CUTFEATURE_CORNER_DELETE, str, -1, 12);
	pDeleteCornerBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE_CORNER_TIP);
	pDeleteCornerBtn->SetDescription(str);
	pPanelCorner->Add(pDeleteCornerBtn);
	/************************************************************************/
}

void RibbonBar::CreatePartLayoutCategory()
{
	CString str;

	// add "Part Layout" category
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_LAYOUT);
	CMFCRibbonCategory* pPartInstCategory = this->AddCategory(str, IDB_ZOOM_BAR, IDB_PART_LAYOUT_BAR);

	/************************************************************************/
	// 添加"增加零件"分组。

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_ADDPART);
	CMFCRibbonPanel* pAddPanel = pPartInstCategory->AddPanel(str);

	// 添加"从数据库"按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_FROM_DB);
	CMFCRibbonButton* pAddPartBtn = new CMFCRibbonButton(IDC_PARTPMT_FROM_DB, str, -1, 0);
	pAddPartBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_FROM_DB_TIP);
	pAddPartBtn->SetDescription(str);
	pAddPanel->Add(pAddPartBtn);

	// 添加"从Dxf/Dwg"按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_FROM_DXF);
	CMFCRibbonButton* pAddDxfBtn = new XRibbonBtn(IDC_PARTPMT_FROM_DXF, str, -1, 1);
	pAddDxfBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_FROM_DXF_TIP);
	pAddDxfBtn->SetDescription(str);
	pAddPanel->Add(pAddDxfBtn);

	// 排任务中的零件
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_AddPartsByNest);
	CMFCRibbonButton* pNestTaskPartsBtn = new XRibbonBtn(IDC_NestTaskParts, str, -1, 2);
	pNestTaskPartsBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NestTaskParts_TIP);
	pNestTaskPartsBtn->SetDescription(str);
	pAddPanel->Add(pNestTaskPartsBtn);
	/************************************************************************/


	/************************************************************************/
	// add "Edit Layout" panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EDIT_LAYOUT);
	CMFCRibbonPanel* pEditPanel = pPartInstCategory->AddPanel(str);

	// add "select Part Pmt" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_SELECT);
	CMFCRibbonButton* pSelectBtn = new CMFCRibbonButton(IDC_PARTPMT_SELECT, str, -1, 3);
	pSelectBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_SELECT_TIP);
	pSelectBtn->SetDescription(str);
	pEditPanel->Add(pSelectBtn);

	// 分隔符
	pEditPanel->Add(new CMFCRibbonSeparator());

	// “创建阵列”按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_CREATE_GRID);
	CMFCRibbonButton* pGridBtn = new CMFCRibbonButton(IDC_PARTPMT_EDIT_GRID, str, -1, 4);
	pGridBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_CREATE_GRID_TIP);
	pGridBtn->SetDescription(str);
	pEditPanel->Add(pGridBtn);

	// “炸开阵列”按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_EXPLODE_GRID);
	CMFCRibbonButton* pExplodeGridBtn = new CMFCRibbonButton(IDC_PARTPMT_EXPLODE_GRID, str, -1, 5);
	pExplodeGridBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_EXPLODE_GRID_TIP);
	pExplodeGridBtn->SetDescription(str);
	pEditPanel->Add(pExplodeGridBtn);

	// 分隔符
	pEditPanel->Add(new CMFCRibbonSeparator());

	// "Delete" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE);
	CMFCRibbonButton* pDeleteBtn = new CMFCRibbonButton(IDC_PARTPMT_EDIT_DELETE, str, -1, 6);
	pDeleteBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_DELETE_TIP);
	pDeleteBtn->SetDescription(str);
	pEditPanel->Add(pDeleteBtn);

	// 分隔符
	pEditPanel->Add(new CMFCRibbonSeparator());

	// "copy" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COPY);
	CMFCRibbonButton* pCopyBtn = new CMFCRibbonButton(IDC_PARTPMT_EDIT_COPY, str, -1, 7);
	pCopyBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_COPY_TIP);
	pCopyBtn->SetDescription(str);
	pEditPanel->Add(pCopyBtn);

	// "Move" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MOVE);
	CMFCRibbonButton* pMoveBtn = new CMFCRibbonButton(IDC_PARTPMT_EDIT_MOVE, str, -1, 8);
	pMoveBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_MOVE_TIP);
	pMoveBtn->SetDescription(str);
	pEditPanel->Add(pMoveBtn);

	// "Rotate" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ROTATE);
	CMFCRibbonButton* pRotateBtn = new CMFCRibbonButton(IDC_PARTPMT_EDIT_ROTATE, str, -1, 9);
	pRotateBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_ROTATE_TIP);
	pRotateBtn->SetDescription(str);
	pEditPanel->Add(pRotateBtn);
	/************************************************************************/


	// 生成余料。
	CMFCRibbonPanel* pRemnantPanel = pPartInstCategory->AddPanel(_T(""));
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEN_SKELE_REMANT);
	CMFCRibbonButton* pGenRemMatBtn = new CMFCRibbonButton(IDC_GEN_SKELE_REMNANT, str, -1, 10);
	pGenRemMatBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEN_SKELE_REMANT_TIP);
	pGenRemMatBtn->SetDescription(str);
	pRemnantPanel->Add(pGenRemMatBtn);

	// 测量距离。
	CMFCRibbonPanel* pMeasureDisPanel = pPartInstCategory->AddPanel(_T(""));
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MEASURE_DISTANCE);
	CMFCRibbonButton* pMeasureDisBtn = new CMFCRibbonButton(IDC_MEASURE_DISTANCE, str, -1, 11);
	pMeasureDisBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MEASURE_DISTANCE_TIP);
	pMeasureDisBtn->SetDescription(str);
	pMeasureDisPanel->Add(pMeasureDisBtn);


	/************************************************************************/
	// “显示比例”面板。不显示这些按钮。

	//str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_VIEW);
	//CMFCRibbonPanel* pPanelView = pPartInstCategory->AddPanel(str);

	//// add "Zoom All" button
	//str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_VIEW_ZOOMALL);
	//CMFCRibbonButton* pZoomAllBtn = new CMFCRibbonButton(IDC_PARTPMT_VIEW_ZOOMALL, str, 0, -1);
	//pZoomAllBtn->SetToolTipText(str);
	//str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_VIEW_ZOOMALL_TIP);
	//pZoomAllBtn->SetDescription(str);
	//pPanelView->Add(pZoomAllBtn);

	//// add "Zoom In" button
	//str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_VIEW_ZOOMIN);
	//CMFCRibbonButton* pZoomInBtn = new CMFCRibbonButton(IDC_PARTPMT_VIEW_ZOOMIN, str, 1, -1);
	//pZoomInBtn->SetToolTipText(str);
	//str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_VIEW_ZOOMIN_TIP);
	//pZoomInBtn->SetDescription(str);
	//pPanelView->Add(pZoomInBtn);

	//// add "Zoom Out" button
	//str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_VIEW_ZOOMOUT);
	//CMFCRibbonButton* pZoomOutBtn = new CMFCRibbonButton(IDC_PARTPMT_VIEW_ZOOMOUT, str, 2, -1);
	//pZoomOutBtn->SetToolTipText(str);
	//str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_VIEW_ZOOMOUT_TIP);
	//pZoomOutBtn->SetDescription(str);
	//pPanelView->Add(pZoomOutBtn);
	/************************************************************************/


	/************************************************************************/
	// "显示/隐藏"面板。

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_WINDOWS);
	CMFCRibbonPanel* pWndPanel = pPartInstCategory->AddPanel(str);

	// the button which can display the part pmt tree.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_VIEW_ALL_PART);
	CMFCRibbonCheckBox* pViewOffsetLoopBtn = new CMFCRibbonCheckBox(IDC_PARTPMT_VIEWBAR_PARTPMT, str);
	pViewOffsetLoopBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_VIEWBAR_PARTPMT_TIP);
	pViewOffsetLoopBtn->SetDescription(str);
	pWndPanel->Add(pViewOffsetLoopBtn);
	/************************************************************************/


	/************************************************************************/
	// 用于调试。

	ConcealConfigPtr pConcealConfig = CLOptionInfo::GetInstance()->GetConcealConfig();

	if (pConcealConfig->GetDebugUIInfo()->IsShowPartPmtOffset())
	{
		CMFCRibbonPanel* pOffsetPanel = pPartInstCategory->AddPanel(_T("Offset"));

		// 增加“显示偏移”按钮。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_VIEW_OFFSET);
		CMFCRibbonCheckBox* pOffsetBtn = new CMFCRibbonCheckBox(IDC_PARTPMT_VIEW_OFFSET, str);
		pOffsetBtn->SetToolTipText(str);
		pOffsetPanel->Add(pOffsetBtn);
	}

	if (pConcealConfig->GetDebugUIInfo()->IsShowPartInstTop())
	{
		CMFCRibbonPanel* pTopologyPanel = pPartInstCategory->AddPanel(_T("Part Topology"));

		// 增加“显示零件拓扑结构”按钮。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_VIEWBAR_PARTTOP);
		CMFCRibbonCheckBox* pTopologyBtn = new CMFCRibbonCheckBox(IDC_PARTPMT_VIEWBAR_PARTTOP, str);
		pTopologyPanel->Add(pTopologyBtn);
	}
	/************************************************************************/
}

void RibbonBar::CreateSequenceCategory()
{
	CString str;

	// add "Sequence" category
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ);
	CMFCRibbonCategory* pSeqNCCategory = this->AddCategory(str, IDB_SQUENCE_BAR_SMALL, IDB_SQUENCE_BAR_LARGE);


	/************************************************************************/
	// 添加“自动处理”分组。

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_AUTO_PROCESS);
	CMFCRibbonPanel* pSeqPanel1 = pSeqNCCategory->AddPanel(str);

	// add "Auto Sequence" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_AUTOSEQUENCE);
	CMFCRibbonButton* pAutoSeq = new XRibbonBtn(IDC_SEQ_AUTOSEQUENCE, str, -1, 0);
    pAutoSeq->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_ReAssignSequence));
    pAutoSeq->SetDefaultCommand(IDC_SEQ_AUTOSEQUENCE);
	pAutoSeq->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_AUTOSEQUENCE_TIP);
	pAutoSeq->SetDescription(str);
	pSeqPanel1->Add(pAutoSeq);
    pSeqNCCategory->AddHidden(new CMFCRibbonUndoButton(IDC_ResetSheetSequence, _T(""), 13));

	// add "Clear All Sequence" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_CLEARALLSEQUENCE);
	CMFCRibbonButton* pClearAllSeq = new CMFCRibbonButton(IDC_SEQ_CLEARALLSEQUENCE, str, -1, 1);
	pClearAllSeq->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_CLEARALLSEQUENCE_TIP);
	pClearAllSeq->SetDescription(str);
	pSeqPanel1->Add(pClearAllSeq);
	/************************************************************************/


	/************************************************************************/
	// add "Edit Sequence" panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_EDIT);
	CMFCRibbonPanel* pSeqPanel2 = pSeqNCCategory->AddPanel(str);

	// add "Select Sequence" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_SELECTSEQUENCE);
	CMFCRibbonButton* pSelSeqBtn = new CMFCRibbonButton(IDC_SEQ_SELECTSEQUENCE, str, -1, 2);
	pSelSeqBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_SELECTSEQUENCE_TIP);
	pSelSeqBtn->SetDescription(str);
	pSeqPanel2->Add(pSelSeqBtn);

	// add "Remove Sequence" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REMOVESEQUENCE);
	CMFCRibbonButton* pRemoveSeqBtn = new CMFCRibbonButton(IDC_SEQ_REMOVESEQUENCE, str, -1, 3);
	pRemoveSeqBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REMOVESEQUENCE_TIP);
	pRemoveSeqBtn->SetDescription(str);
	pSeqPanel2->Add(pRemoveSeqBtn);

	// “调整顺序”按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_SORTSEQUENCE);
	CMFCRibbonButton* pSortSeqBtn = new CMFCRibbonButton(IDC_SEQ_SORTSEQUENCE, str, -1, 4);
	pSortSeqBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ORDER_SEQUENCE_TIP);
	pSortSeqBtn->SetDescription(str);
	pSeqPanel2->Add(pSortSeqBtn);

	// “替换内孔工序”按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REPLACE_HOLE_SEQ);
	CMFCRibbonButton* pReplaceHoleSeqBtn = new CMFCRibbonButton(IDC_SEQ_REPLACE_HOLE_SEQ, str, -1, 7);
	pReplaceHoleSeqBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REPLACE_HOLE_SEQ_TIP);
	pReplaceHoleSeqBtn->SetDescription(str);
	pSeqPanel2->Add(pReplaceHoleSeqBtn);
	/************************************************************************/


	/************************************************************************/
	// 有关轮廓工序。

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LOOP_SEQUNIT);
	CMFCRibbonPanel* pLoopSeqPanel = pSeqNCCategory->AddPanel(str);

	// "添加回路工序"按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADD_LOOP_SEQ);
	CMFCRibbonButton* pAddSeqBtn = new CMFCRibbonButton(IDC_SEQ_ADD_LOOPSEQ, str, -1, 5);
	pAddSeqBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADDSEQUENCE_TIP);
	pAddSeqBtn->SetDescription(str);
	pLoopSeqPanel->Add(pAddSeqBtn);

	// "插入回路工序"按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_INSERT_LOOP_SEQ);
	CMFCRibbonButton* pInsertSeqBtn = new CMFCRibbonButton(IDC_SEQ_INSERT_LOOPSEQ, str, -1, 6);
	pInsertSeqBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_INSERTSEQUENCE_TIP);
	pInsertSeqBtn->SetDescription(str);
	pLoopSeqPanel->Add(pInsertSeqBtn);
	/************************************************************************/


	/************************************************************************/
	// 有关“桥接工序”。

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_BDGE_SEQUNIT);
	CMFCRibbonPanel* pBdgeSeqPanel = pSeqNCCategory->AddPanel(str);

	// "添加桥接"按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADD_BDGE_SEQ);
	CMFCRibbonButton* pAddBdgeSeqBtn = new CMFCRibbonButton(IDC_SEQ_ADD_BDGESEQ, str, -1, 8);
	pAddBdgeSeqBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADD_BDGE_SEQ_TIP);
	pAddBdgeSeqBtn->SetDescription(str);
	pBdgeSeqPanel->Add(pAddBdgeSeqBtn);

	// "断开桥接"按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_BREAK_BDGE_SEQ);
	CMFCRibbonButton* pBreakBdgeSeqBtn = new CMFCRibbonButton(IDC_SEQ_BREAK_BDGESEQ, str, 2);
	pBreakBdgeSeqBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_BREAK_BDGE_SEQ_TIP);
	pBreakBdgeSeqBtn->SetDescription(str);
	pBdgeSeqPanel->Add(pBreakBdgeSeqBtn);

	// "桥接开始轮廓"按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_BDGE_START_LOOP);
	CMFCRibbonButton* pBdgeStartLoopBtn = new CMFCRibbonButton(IDC_SEQ_BDGE_START_LOOP, str, 3);
	pBdgeStartLoopBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_BDGE_SEQ_START_TIP);
	pBdgeStartLoopBtn->SetDescription(str);
	pBdgeSeqPanel->Add(pBdgeStartLoopBtn);
	/************************************************************************/


	/************************************************************************/
	// 有关“连割工序”。

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CHAIN_SEQUNIT);
	CMFCRibbonPanel* pChainSeqPanel = pSeqNCCategory->AddPanel(str);

	// "添加连割"按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADD_CHAIN_SEQ);
	CMFCRibbonButton* pAddChainSeqBtn = new CMFCRibbonButton(IDC_SEQ_ADD_CHAINSEQ, str, -1, 13);
	pAddChainSeqBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADD_CHAIN_SEQ_TIP);
	pAddChainSeqBtn->SetDescription(str);
	pChainSeqPanel->Add(pAddChainSeqBtn);

	// “插入连割节点”按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_INSERT_CHAIN_NODE);
	CMFCRibbonButton* pAddChainNodeBtn = new CMFCRibbonButton(IDC_SEQ_INSERT_CHAIN_NODE, str, 0);
	pAddChainNodeBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADD_CHAIN_NODE_TIP);
	pAddChainNodeBtn->SetDescription(str);
	pChainSeqPanel->Add(pAddChainNodeBtn);

	// “删除连割节点”按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_DEL_CHAIN_NODE);
	CMFCRibbonButton* pDelChainNodeBtn = new CMFCRibbonButton(IDC_SEQ_DEL_CHAIN_NODE, str, 1);
	pDelChainNodeBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_DEL_CHAIN_NODE_TIP);
	pDelChainNodeBtn->SetDescription(str);
	pChainSeqPanel->Add(pDelChainNodeBtn);

    if (CLOptionInfo::GetInstance()->GetProductParam()->IsEnableReset4ChainSeq()) // 客户定制功能
    {
        // 分隔符
        pChainSeqPanel->Add(new CMFCRibbonSeparator());

        // “插入重置指令”按钮。
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_INSERT_CHAIN_RESET);
        CMFCRibbonButton* pAddResetInstructionBtn = new CMFCRibbonButton(IDC_SEQ_INSERT_RESET_INSTRUCTION, str, 9);
        pAddResetInstructionBtn->SetToolTipText(str);
        pChainSeqPanel->Add(pAddResetInstructionBtn);

        // “删除重置指令”按钮。
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_DEL_CHAIN_RESET);
        CMFCRibbonButton* pDelResetInstructionBtn = new CMFCRibbonButton(IDC_SEQ_DEL_RESET_INSTRUCTION, str, 10);
        pDelResetInstructionBtn->SetToolTipText(str);
        pChainSeqPanel->Add(pDelResetInstructionBtn);
    }
	/************************************************************************/


	/************************************************************************/
	// 有关“田字工序”。

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GRID_SEQUNIT);
	CMFCRibbonPanel* pGridSeqPanel = pSeqNCCategory->AddPanel(str);

	// "添加田字工序"按钮。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADD_GRID_SEQ);
	CMFCRibbonButton* pAddGridSeqBtn = new CMFCRibbonButton(IDC_SEQ_ADD_GRIDSEQ, str, -1, 9);
	pAddGridSeqBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADD_GRID_SEQ_TIP);
	pAddGridSeqBtn->SetDescription(str);
	pGridSeqPanel->Add(pAddGridSeqBtn);
	/************************************************************************/


	// 有关“飞行切割工序”。
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FLYCUT_SEQUNIT);
		CMFCRibbonPanel* pFlyCutSeqPanel = pSeqNCCategory->AddPanel(str);

		// "添加飞行切割"按钮。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADD);
		CMFCRibbonButton* pAddFlyCutSeqBtn = new CMFCRibbonButton(IDC_SEQ_ADD_FLYCUTSEQ, str, -1, 10);
		pAddFlyCutSeqBtn->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_AddFlyCut_TIP);
		pAddFlyCutSeqBtn->SetDescription(str);
		pFlyCutSeqPanel->Add(pAddFlyCutSeqBtn);
	}

	// 有关“运动点”。
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MOVE_PT);
		CMFCRibbonPanel* pMovePtPanel = pSeqNCCategory->AddPanel(str);

		// "插入移动点"按钮。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADD_MOV_PT);
		CMFCRibbonButton* pAddMovPtBtn = new CMFCRibbonButton(IDC_SEQ_ADD_MOV_PT, str, 7);
		pAddMovPtBtn->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADD_MOV_PT_TIP);
		pAddMovPtBtn->SetDescription(str);
		pMovePtPanel->Add(pAddMovPtBtn);

		// "删除移动点"按钮。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_DEL_MOV_PT);
		CMFCRibbonButton* pDelMovPtBtn = new CMFCRibbonButton(IDC_SEQ_DEL_MOV_PT, str, 8);
		pDelMovPtBtn->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_DEL_MOV_PT_TIP);
		pDelMovPtBtn->SetDescription(str);
		pMovePtPanel->Add(pDelMovPtBtn);
	}

	// 有关余料切割线
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_REMNANT_CUTLINE);
		CMFCRibbonPanel* pPanel_remCutLine = pSeqNCCategory->AddPanel(str);

		//
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_AUTO_REM_CUTLINE);
		CMFCRibbonButton* pAutoRemCutLineBtn = new CMFCRibbonButton(IDC_SEQ_AUTO_REM_CUTLINE, str, 4);
		pAutoRemCutLineBtn->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_AUTO_REM_CUTLINE_TIP);
		pAutoRemCutLineBtn->SetDescription(str);
		pPanel_remCutLine->Add(pAutoRemCutLineBtn);

		// “创建”按钮。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_CREATE_REM_CUTLINE);
		CMFCRibbonButton* pAddRemCutLineBtn = new CMFCRibbonButton(IDC_SEQ_CREATE_REM_CUTLINE, str, 5);
		pAddRemCutLineBtn->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_ADD_REM_CUTLINE_TIP);
		pAddRemCutLineBtn->SetDescription(str);
		pPanel_remCutLine->Add(pAddRemCutLineBtn);

		// “删除”按钮。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_DELETE_REM_CUTLINE);
		CMFCRibbonButton* pDelRemCutLineBtn = new CMFCRibbonButton(IDC_SEQ_DELETE_REM_CUTLINE, str, 6);
		pDelRemCutLineBtn->SetToolTipText(str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_DEL_REM_CUTLINE_TIP);
		pDelRemCutLineBtn->SetDescription(str);
		pPanel_remCutLine->Add(pDelRemCutLineBtn);
	}

	// “工序模拟”按钮。
	CMFCRibbonPanel* pSeqSimPanel = pSeqNCCategory->AddPanel(_T(""));
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_VIEWSEQUENCE);
	CMFCRibbonButton* pViewSeqBtn = new XRibbonBtn(IDC_SEQ_VIEWSEQUENCE, str, -1, 14);
	pViewSeqBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_VIEWSEQUENCE_TIP);
	pViewSeqBtn->SetDescription(str);
	pSeqSimPanel->Add(pViewSeqBtn);

	// “生成NC”按钮。
	CMFCRibbonPanel* pGenNCPanel = pSeqNCCategory->AddPanel(_T(""));
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MACROUTER_VIEW_NC);
	CMFCRibbonButton* pViewNCBtn = new XRibbonBtn(IDC_MACROUTER_VIEW_NC, str, -1, 15);
    pViewNCBtn->SetMenu(MultiLanguageMgr::GetInstance()->CreateTranslatedMenu(IDR_ConfigNcParam));
    pViewNCBtn->SetDefaultCommand(IDC_MACROUTER_VIEW_NC);
	pViewNCBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MACROUTER_VIEW_NC_TIP);
	pViewNCBtn->SetDescription(str);
	pGenNCPanel->Add(pViewNCBtn);
    pSeqNCCategory->AddHidden(new CMFCRibbonUndoButton(IDC_ConfigNcParam, _T(""), 12));


	/************************************************************************/
	// "显示/隐藏"面板。

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_WINDOWS);
	CMFCRibbonPanel* pWndPanel = pSeqNCCategory->AddPanel(str);

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_LIST_SEQ);
	CMFCRibbonCheckBox* pSeqUnitViewBarBtn = new CMFCRibbonCheckBox(IDC_SEQ_VIEWBAR_SEQUNIT, str);
	pSeqUnitViewBarBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_VIEW_CUTSEQ_TIP);
	pSeqUnitViewBarBtn->SetDescription(str);
	pWndPanel->Add(pSeqUnitViewBarBtn);
	/************************************************************************/


    // sample functions, to be commented.
    {
        //str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SAMPLE_FUNCTIONS);
        //CMFCRibbonPanel* pSampleFunctionPanel = pSeqNCCategory->AddPanel(str);

        //// the button which calculate the part instance quantity on current sheet.
        //str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Delete_LoopSequence);
        //CMFCRibbonButton* pDeleteLoopSequenceBtn = new CMFCRibbonButton(IDC_Delete_LoopSequence, str, -1, 16);
        //pDeleteLoopSequenceBtn->SetToolTipText(str);
        //str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Delete_LoopSequence_TIP);
        //pDeleteLoopSequenceBtn->SetDescription(str);
        //pSampleFunctionPanel->Add(pDeleteLoopSequenceBtn);
    }
}

void RibbonBar::CreateConfigCategory()
{
	CString str;

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CONFIG);
	CMFCRibbonCategory* pConfigCategory = this->AddCategory(str, IDB_CONFIG_BAR, IDB_CONFIG_BAR);

	// “加工参数”按钮。
	CMFCRibbonPanel* pParamConfigPanel = pConfigCategory->AddPanel(_T(""));
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CAM_PARAM);
	CMFCRibbonButton* pParamConfigBtn = new XRibbonBtn(IDC_PARAM_CONFIG, str, -1, 0);
	pParamConfigBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CAM_PARAM_TIP);
	pParamConfigBtn->SetDescription(str);
	pParamConfigPanel->Add(pParamConfigBtn);

	// “系统参数”按钮。
	CMFCRibbonPanel* pSystemConfigPanel = pConfigCategory->AddPanel(_T(""));
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OPTIONS);
	CMFCRibbonButton* pSystemConfigBtn = new XRibbonBtn(IDC_OPTIONS, str, -1, 1);
	pSystemConfigBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OPTIONS_TIP);
	pSystemConfigBtn->SetDescription(str);
	pSystemConfigPanel->Add(pSystemConfigBtn);

	/************************************************************************/
	// add "CAM Library" panel.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CAM_LIBRARY);
	CMFCRibbonPanel* pCamLibPanel = pConfigCategory->AddPanel(str);

	// add "Machine Library" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAC_MGR);
	CMFCRibbonButton* pMacMgrBtn = new CMFCRibbonButton(IDC_MAC_MGR, str, -1, 2);
	pMacMgrBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAC_MGR_TIP);
	pMacMgrBtn->SetDescription(str);
	pCamLibPanel->Add(pMacMgrBtn);

	// add "Expert Library" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXPERTLIB_MGR);
	CMFCRibbonButton* pExpertLibBtn = new CMFCRibbonButton(IDC_EXPERTLIB_MGR, str, -1, 3);
	pExpertLibBtn->SetToolTipText(str);
	pExpertLibBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXPERTLIB_MGR_TIP);
	pExpertLibBtn->SetDescription(str);
	pCamLibPanel->Add(pExpertLibBtn);

	// add "Material Library" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MATLIB);
	CMFCRibbonButton* pMatLibBtn = new CMFCRibbonButton(IDC_MATLIB_MGR, str, -1, 4);
	pMatLibBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MATLIB_TIP);
	pMatLibBtn->SetDescription(str);
	pCamLibPanel->Add(pMatLibBtn);
	/************************************************************************/

	/************************************************************************/
	// “报表模板”面板。

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_REPORT_TPL);
	CMFCRibbonPanel* pReportTplPanel = pConfigCategory->AddPanel(str);

	// 默认模板。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DFL_REPORT_TPL);
	CMFCRibbonButton* pDflTplBtn = new CMFCRibbonButton(IDC_DFL_REPORT_TPL, str, -1, 5);
	pReportTplPanel->Add(pDflTplBtn);

	// 模板设计。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DESIGN_REPORT_TPL);
	CMFCRibbonButton* pDesignTplBtn = new CMFCRibbonButton(IDC_DESIGN_REPORT_TPL, str, -1, 6);
	pReportTplPanel->Add(pDesignTplBtn);
	/************************************************************************/
}

void RibbonBar::CreateHelpCategory()
{
	CString str;

	// add "help" category
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_HELP);
	CMFCRibbonCategory* pHelpCategory = this->AddCategory(str, IDB_UNDO_REDO, IDB_HELP_BAR);
	CMFCRibbonPanel* pHelpPanel = pHelpCategory->AddPanel(_T(""));

	// add "About" button.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_HELP_ABOUT);
	CMFCRibbonButton* pAboutBtn = new CMFCRibbonButton(IDC_HELP_ABOUT, str, -1, 4);
	pAboutBtn->SetToolTipText(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_HELP_ABOUT_TIP);
	pAboutBtn->SetDescription(str);
	pHelpPanel->Add(pAboutBtn);

	// add hidden buttons for UNDO/REDO
	pHelpCategory->AddHidden(new CMFCRibbonUndoButton(IDC_TAO_UNDO, _T("Undo"), 0));
	pHelpCategory->AddHidden(new CMFCRibbonUndoButton(IDC_TAO_REDO, _T("Redo"), 1));
}

int RibbonBar::GetCategoryIndex( CString strName )
{
	int iIndex = INVALID_IDX;

	for (int i = 0; i < this->GetCategoryCount(); i++)
	{
		CMFCRibbonCategory* pCategory = this->GetCategory(i);
		CString str = pCategory->GetName();
		if (str == strName)
			return i;
	}

	return iIndex;
}

CString RibbonBar::GetCurCategoryName()
{
	CMFCRibbonCategory* pCategory = this->GetActiveCategory();
	CString str = pCategory->GetName();

	return str;
}

END_CUTLEADER_NAMESPACE()
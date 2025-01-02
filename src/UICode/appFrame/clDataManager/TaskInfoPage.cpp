#include "stdafx.h"
#include "TaskInfoPage.h"

#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"
#include "IntegerList.h"
#include "ClUtilityResource.h"
#include "DateHelper.h"
#include "ImgData.h"
#include "LogMessage.h"
#include "LogMessageList.h"
#include "LogMgr.h"
#include "baseConst.h"
#include "ClBaseDataResource.h"
#include "LineArc2DList.h"
#include "ClBaseDataResource.h"
#include "DataBaseManager.h"
#include "StringUtil.h"
#include "DataCenterItemLoader.h"
#include "DataCenterNode.h"

#include "ParamConfig.h"
#include "ParamConfigLoader.h"
#include "ExpertLibItemLoader.h"
#include "ClExpertLibResource.h"
#include "CamConfigSelectDlg.h"
#include "Material.h"
#include "MaterialThickness.h"
#include "IMaterialSize.h"
#include "MaterialLibLoader.h"
#include "MaterialSizeSelectDlg.h"
#include "MaterialProcessor.h"
#include "ClMaterialLibResource.h"

#include "Part.h"
#include "Sheet.h"
#include "SheetList.h"
#include "SheetProcessor.h"
#include "SheetLoader.h"
#include "TaskItem.h"
#include "TaskItemManager.h"
#include "TaskPart.h"
#include "TaskPartList.h"
#include "ClTaskResource.h"
#include "ClDataManagerResource.h"
#include "DataItemTracker.h"
#include "ClDataImportExportResource.h"
#include "ExportManager.h"
#include "SheetHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const size_t MAX_TIP_TEXT_LENGTH = 1024;

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TaskInfoPage, CLResizableDialog)

TaskInfoPage::TaskInfoPage() : CLResizableDialog(TaskInfoPage::IDD)
{
    m_bDisableSelectChangeMessageOfGrid = false;
}

TaskInfoPage::~TaskInfoPage()
{
}

void TaskInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CLResizableDialog::DoDataExchange(pDX);

	// basic info group.
	DDX_Text(pDX, IDC_EDIT_TASK_NAME, m_strName);
	DDX_Control(pDX, IDC_EDIT_TASK_NAME, m_ctrlNameEditBox);
	DDX_Text(pDX, IDC_EDIT_TASK_COMMENT, m_strComment);
	DDX_Control(pDX, IDC_EDIT_TASK_COMMENT, m_ctrlCommentEditBox);
	DDX_Text(pDX, IDC_EDIT_CREATE_DATE, m_strCreateDate);
	DDX_Text(pDX, IDC_EDIT_MODIFY_DATE, m_strModifyDate);

	DDX_Control(pDX, IDC_LIST_TaskPart, m_partGrid);
	DDX_Control(pDX, IDC_LIST_TaskSheet, m_sheetGrid);

	DDX_Control(pDX, IDC_BTN_ViewSelectedPart, m_ctrlViewSelectedPartButton);
    DDX_Control(pDX, IDC_BTN_ExportSheetAsDxf, m_ctrlExportSheetButton);
    DDX_Control(pDX, IDC_BTN_ExportAllSheetsAsDxf, m_ctrlExportAllSheetsButton);
	DDX_Control(pDX, IDC_BTN_ChangeMaterial, m_ctrlChangeMaterialButton);
	DDX_Control(pDX, IDC_BTN_ChangeCamParam, m_ctrlSwitchCAMParamButton);
}

BEGIN_MESSAGE_MAP(TaskInfoPage, CLResizableDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()

	ON_EN_KILLFOCUS(IDC_EDIT_TASK_NAME, OnKillTaskName)
	ON_EN_KILLFOCUS(IDC_EDIT_TASK_COMMENT, OnKillTaskComment)

    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TaskPart, OnSelchangedPart)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TaskSheet, OnSelchangedSheet)

	ON_BN_CLICKED(IDC_BTN_ViewSelectedPart, OnViewSelectedPart)
	ON_BN_CLICKED(IDC_BTN_ChangeMaterial, OnChangeMaterial)
	ON_BN_CLICKED(IDC_BTN_ChangeCamParam, OnChangeCamParam)
    ON_BN_CLICKED(IDC_BTN_ExportSheetAsDxf, OnExportSheetAsDxf)
	ON_BN_CLICKED(IDC_BTN_ExportAllSheetsAsDxf, OnExportAllSheetsAsDxf)
END_MESSAGE_MAP()


BOOL TaskInfoPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// 布局控件
    {
        // 基本信息分组
        AddAnchor(IDC_STATIC_BASE_INFO, TOP_LEFT, TOP_RIGHT);
        AddAnchor(IDC_EDIT_TASK_NAME, TOP_LEFT);
        AddAnchor(IDC_STATIC_TASK_NAME, TOP_LEFT);
        AddAnchor(IDC_EDIT_TASK_COMMENT, TOP_LEFT);
        AddAnchor(IDC_STATIC_TASK_COMMENT, TOP_LEFT);
        AddAnchor(IDC_STATIC_CREATE_DATE, TOP_LEFT);
        AddAnchor(IDC_EDIT_CREATE_DATE, TOP_LEFT);
        AddAnchor(IDC_STATIC_MODIFY_DATE, TOP_LEFT);
        AddAnchor(IDC_EDIT_MODIFY_DATE, TOP_LEFT);

        // 零件分组
        AddAnchor(IDC_STATIC_TaskPart, TOP_LEFT, BOTTOM_RIGHT);
        AddAnchor(IDC_LIST_TaskPart, TOP_LEFT, BOTTOM_RIGHT);
        AddAnchor(IDC_BTN_ViewSelectedPart, BOTTOM_RIGHT);
        AddAnchor(IDC_GROUP_PartPreview, TOP_RIGHT);
        AddAnchor(IDC_STATIC_PartPreview, TOP_RIGHT);

        // 板材分组
        AddAnchor(IDC_STATIC_TaskSheet, BOTTOM_LEFT, BOTTOM_RIGHT);
        AddAnchor(IDC_LIST_TaskSheet, BOTTOM_LEFT, BOTTOM_RIGHT);
        AddAnchor(IDC_BTN_ChangeMaterial, BOTTOM_RIGHT);
        AddAnchor(IDC_BTN_ChangeCamParam, BOTTOM_RIGHT);
        AddAnchor(IDC_GROUP_SheetPreview, BOTTOM_RIGHT);
        AddAnchor(IDC_STATIC_SheetPreview, BOTTOM_RIGHT);
        AddAnchor(IDC_BTN_ExportSheetAsDxf, BOTTOM_LEFT);
		AddAnchor(IDC_BTN_ExportAllSheetsAsDxf, BOTTOM_LEFT);
    }

	//
	m_partPreviewWnd.SubclassDlgItem(IDC_STATIC_PartPreview, this);
    m_sheetPreviewWnd.SubclassDlgItem(IDC_STATIC_SheetPreview, this);

	InitGridStyle();

	SetBackgroundColor(SkinManager::GetBackgroundColor());

	SetWindowTheme(GetDlgItem(IDC_STATIC_BASE_INFO)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_STATIC_TaskPart)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_GROUP_PartPreview)->GetSafeHwnd(), L"", L"");
    SetWindowTheme(GetDlgItem(IDC_STATIC_TaskSheet)->GetSafeHwnd(), L"", L"");
    SetWindowTheme(GetDlgItem(IDC_GROUP_SheetPreview)->GetSafeHwnd(), L"", L"");

	return TRUE;
}

void TaskInfoPage::OnDestroy()
{
	ReleaseGridItemData();
	CLResizableDialog::OnDestroy();
}

BOOL TaskInfoPage::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	GetClientRect(&r);
	if ((HBRUSH)m_brush == NULL)
		m_brush.CreateSolidBrush(SkinManager::GetBackgroundColor());
	pDC->FillRect(&r, &m_brush);

	// 绘制预览。
	ImgData::DrawImgData(m_pPartImgData.get(), &m_partPreviewWnd, SkinManager::GetBackgroundColor());
	ImgData::DrawImgData(m_pSheetImgData.get(), &m_sheetPreviewWnd, SkinManager::GetBackgroundColor());

	return TRUE;
}

void TaskInfoPage::OnPaint()
{
	CLResizableDialog::OnPaint();

    // 绘制预览。
    ImgData::DrawImgData(m_pPartImgData.get(), &m_partPreviewWnd, SkinManager::GetBackgroundColor());
    ImgData::DrawImgData(m_pSheetImgData.get(), &m_sheetPreviewWnd, SkinManager::GetBackgroundColor());
}

HBRUSH TaskInfoPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_PartPreview)
    {
		ImgData::DrawImgData(m_pPartImgData.get(), &m_partPreviewWnd, SkinManager::GetBackgroundColor());
		return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC_SheetPreview)
	{
		ImgData::DrawImgData(m_pSheetImgData.get(), &m_sheetPreviewWnd, SkinManager::GetBackgroundColor());
		return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void TaskInfoPage::OnKillTaskName()
{
	UpdateData(TRUE);

	if (m_pTaskItem->GetName() != m_strName)
	{
		m_pTaskItem->SetName(m_strName);
		m_pTaskItem->SetModified(TRUE);
	}
}

void TaskInfoPage::OnKillTaskComment()
{
	UpdateData(TRUE);

	if (m_pTaskItem->GetComment() != m_strComment)
	{
		m_pTaskItem->SetComment(m_strComment);
		m_pTaskItem->SetModified(TRUE);
	}
}

TaskItemPtr TaskInfoPage::GetTaskItem()
{
	SetFocus();
	UpdateData(TRUE);

	return m_pTaskItem;
}

void TaskInfoPage::OnSelchangedPart(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if (!m_bDisableSelectChangeMessageOfGrid)
    {
        LONGLONG* pTaskPartID = (LONGLONG*)m_partGrid.GetItemData(pNMListView->iItem);
        const TaskPart* pTaskPart = m_pTaskItem->GetTaskPartByID(*pTaskPartID).get();
        m_pPartImgData = pTaskPart->GetPart()->GetPreviewData();
		ImgData::DrawImgData(m_pPartImgData.get(), &m_partPreviewWnd, SkinManager::GetBackgroundColor());
    }

    *pResult = 0;
}

void TaskInfoPage::OnSelchangedSheet(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if (!m_bDisableSelectChangeMessageOfGrid)
    {
        LONGLONG* pSheetID = (LONGLONG*)m_sheetGrid.GetItemData(pNMListView->iItem);
        const Sheet* pSheet = m_pTaskItem->GetSheetList()->GetSheetByID(*pSheetID).get();
        m_pSheetImgData = pSheet->GetPreviewData();
		ImgData::DrawImgData(m_pSheetImgData.get(), &m_sheetPreviewWnd, SkinManager::GetBackgroundColor());
    }

    *pResult = 0;
}

void TaskInfoPage::OnViewSelectedPart()
{
    // 得到选中的零件
    LONGLONG iTaskPartID = INVALID_ID;
    for (int i = 0; i < m_partGrid.GetItemCount(); i++)
    {
        if (m_partGrid.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
        {
            LONGLONG* pTaskPartID = (LONGLONG*)m_partGrid.GetItemData(i);
            iTaskPartID = *pTaskPartID;
            break;
        }
    }
    if (iTaskPartID == INVALID_ID)
    {
        LogMgr::DebugWarn(_T("78473"));
        return;
    }

    // 发送消息让父窗口在数据中心中定位该节点。
    DataProperty dataProp;
    DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
    TaskPartPtr pTaskPart = m_pTaskItem->GetTaskPartByID(iTaskPartID);
    BOOL bRet = DataCenterItemLoader::GetDataProp(pMainDBCon, pTaskPart->GetPart()->GetID(), dataProp);
    if (!bRet)
    {
        LogMgr::DebugWarn(_T("86539"));
        return;
    }
    DataCenterNode productNodeData(DataCenterNodeType_Part, dataProp.GetItem(4).GetValue().llVal);
    GetParent()->SendMessage(WM_DATA_ITEM_DB_CLICK, (WPARAM)&productNodeData, 0);
}

void TaskInfoPage::OnChangeMaterial()
{
    CString str;
    CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

    // 要是当前任务是打开状态，不允许更改板材的材料。
    if (DataItemTracker::GetInstance()->IsDataItemOpened(m_pTaskItem->GetID()))
    {
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CannotChangeMat);
        str.Replace(_T("WILLBEREPLACED1"), m_pTaskItem->GetName());
        MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
        return;
    }

    // 得到选中的板材。
    SheetPtr pSelectedSheet;
    int iSelectedRowIndex = -1;
    for (int i = 0; i < m_sheetGrid.GetItemCount(); i++)
    {
        if (m_sheetGrid.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
        {
            LONGLONG* pSheetID = (LONGLONG*)m_sheetGrid.GetItemData(i);
            pSelectedSheet = m_pTaskItem->GetSheetList()->GetSheetByID(*pSheetID);
            iSelectedRowIndex = i;
            break;
        }
    }
    if (!pSelectedSheet)
    {
        LogMgr::DebugWarn(_T("67132"));
        return;
    }

    // 更改板材的材料。
    MaterialSizeSelectDlg matSizeSelDlg;
    if (matSizeSelDlg.DoModal() == IDOK)
    {
        IMaterialSizePtr pMaterialSize = matSizeSelDlg.GetSelMatSize();
        if (MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CHANGE_MAT), strProductName, MB_YESNO | MB_ICONWARNING) == IDYES)
        {
            // change the material of the sheet.
            SheetProcessor::ChangeMatSize(pSelectedSheet, pMaterialSize);

            // 更新界面
            {
                m_strModifyDate = DateHelper::GetStrFromTimeStamp(DateHelper::GetCurrentTime());
                UpdateData(FALSE);

                DataProperty matLibProp;
                DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
                MaterialLibLoader::GetMatSizeProp(pMatLibDBCon, pMaterialSize->GetID(), matLibProp);
                m_sheetGrid.SetItemText(iSelectedRowIndex, 4, matLibProp.GetItem(0).GetValue().bstrVal);
                str.Format(FLOAT_FORMAT_2, matLibProp.GetItem(1).GetValue().dblVal);
                m_sheetGrid.SetItemText(iSelectedRowIndex, 5, str);
                m_sheetGrid.SetItemText(iSelectedRowIndex, 6, matLibProp.GetItem(6).GetValue().bstrVal);
            }
        }
    }
}

void TaskInfoPage::OnChangeCamParam()
{
    CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

    // 要是当前任务是打开状态，不允许更改板材的材料。
    if (DataItemTracker::GetInstance()->IsDataItemOpened(m_pTaskItem->GetID()))
    {
        CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CannotChangeCamParam);
        str.Replace(_T("WILLBEREPLACED1"), m_pTaskItem->GetName());
        MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
        return;
    }

    // 得到选中的板材。
    SheetPtr pSelectedSheet;
    int iSelectedRowIndex = -1;
    for (int i = 0; i < m_sheetGrid.GetItemCount(); i++)
    {
        if (m_sheetGrid.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
        {
            LONGLONG* pSheetID = (LONGLONG*)m_sheetGrid.GetItemData(i);
            pSelectedSheet = m_pTaskItem->GetSheetList()->GetSheetByID(*pSheetID);
            iSelectedRowIndex = i;
            break;
        }
    }
    if (!pSelectedSheet)
    {
        LogMgr::DebugWarn(_T("87693"));
        return;
    }

    // 更换板材的加工参数。
    CamConfigSelectDlg dlg;
    dlg.InitParamConfigID(pSelectedSheet->GetParamConfigID());
    if (dlg.DoModal() == IDOK)
    {
        LONGLONG iParamConfigID = dlg.GetParamConfigID();
        if (MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CHANGE_SHEETSCHEME), strProductName, MB_YESNO | MB_ICONWARNING) == IDYES)
        {
            // change and save the scheme of the sheet.
            SheetProcessor::ChangeSheetConfig(pSelectedSheet, iParamConfigID);

            // 更新界面
            {
                m_strModifyDate = DateHelper::GetStrFromTimeStamp(DateHelper::GetCurrentTime());
                UpdateData(FALSE);

                DataProperty expLibProp;
                DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
                ParamConfigLoader::GetParamConfigInfo(pExpLibDBCon, iParamConfigID, expLibProp);
                CString str = StringUtil::ConnectStrings(expLibProp.GetItem(2).GetValue().bstrVal, _T("("), expLibProp.GetItem(3).GetValue().bstrVal, _T(")"));
                m_sheetGrid.SetItemText(iSelectedRowIndex, 7, str);
            }
        }
    }
}

void TaskInfoPage::OnExportSheetAsDxf()
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

    // 得到选中的板材。
    LONGLONG iSelectedSheetID = INVALID_ID;
    for (int i = 0; i < m_sheetGrid.GetItemCount(); i++)
    {
        if (m_sheetGrid.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
        {
            LONGLONG* pSheetID = (LONGLONG*)m_sheetGrid.GetItemData(i);
            iSelectedSheetID = *pSheetID;
            break;
        }
    }
    if (iSelectedSheetID == INVALID_ID)
    {
		MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SelectSheet2Export), strProductName, MB_OK | MB_ICONWARNING);
        return;
    }

    // 加载板材并保存
    CString strFilter = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXP_DXFDWG_FILEEXT);
    CString strDflExt = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DXF_EXT);
    CFileDialog exportDataDlg(FALSE, strDflExt, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter);
    if (exportDataDlg.DoModal() == IDOK)
    {
        CString strFullFilePath = exportDataDlg.GetPathName();
        SheetPtr pSheet = SheetLoader::LoadSheetFromDb(iSelectedSheetID, SHEETLOAD_ALL);
        if (SheetHelper::ExportSheetAsDxfDwg(pSheet.get(), strFullFilePath))
        {
            MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXP_SHEET_SUCCESS), strProductName, MB_OK | MB_ICONINFORMATION);
        }
    }
}

void TaskInfoPage::OnExportAllSheetsAsDxf()
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// 选择导出位置
	CString strFolder;
	{
		BROWSEINFO bi;
		memset(&bi, 0, sizeof(BROWSEINFO));
		bi.hwndOwner = NULL;
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
		CString strTitle;
		strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_Sheet_EXPORT_FOLDER);
		bi.lpszTitle = strTitle;
		LPITEMIDLIST idl = SHBrowseForFolder(&bi);
		if (idl == NULL)
			return;
		SHGetPathFromIDList(idl, strFolder.GetBuffer(MAX_PATH));
		strFolder.ReleaseBuffer();
	}

	// 导出所有板材
	bool bSucceed = true;
	for (int i = 0; i < m_sheetGrid.GetItemCount(); i++)
	{
		LONGLONG* pSheetID = (LONGLONG*)m_sheetGrid.GetItemData(i);
		SheetPtr pSheet = SheetLoader::LoadSheetFromDb(*pSheetID, SHEETLOAD_ALL);
		CString strDxfPath = strFolder + _T("\\") + pSheet->GetName() + _T(".dxf");
		if (!SheetHelper::ExportSheetAsDxfDwg(pSheet.get(), strDxfPath))
		{
			bSucceed = false;
			break;
		}
	}

	if (bSucceed)
	{
		MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXP_SHEET_SUCCESS), strProductName, MB_OK | MB_ICONINFORMATION);
		ShellExecute(NULL, _T("open"), NULL, NULL, strFolder, SW_SHOWNORMAL);
	}
	else
	{
		MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXP_SHEET_Fail), strProductName, MB_OK | MB_ICONINFORMATION);
	}
}

void TaskInfoPage::DisplayTaskItem(TaskItemPtr pTaskItem) 
{
	m_pTaskItem = pTaskItem;

	// init the basic and scheme data.
	m_strName = m_pTaskItem->GetName();
	m_strComment = m_pTaskItem->GetComment();
	m_strCreateDate = DateHelper::GetStrFromTimeStamp(m_pTaskItem->GetCreateTime());
	m_strModifyDate = DateHelper::GetStrFromTimeStamp(m_pTaskItem->GetModifyTime());

    // 在表格中显示内容。
    InitGridData();

    // 更新预览数据。
    {
        if (m_partGrid.GetItemCount() > 0)
        {
            LONGLONG* pTaskPartID = (LONGLONG*)m_partGrid.GetItemData(0);
            const TaskPart* pTaskPart = m_pTaskItem->GetTaskPartByID(*pTaskPartID).get();
            m_pPartImgData = pTaskPart->GetPart()->GetPreviewData();
        }
        else // 清空预览。
        {
            m_pPartImgData.reset();
        }

        if (m_sheetGrid.GetItemCount() > 0)
        {
            LONGLONG* pSheetID = (LONGLONG*)m_sheetGrid.GetItemData(0);
            const Sheet* pSheet = m_pTaskItem->GetSheetList()->GetSheetByID(*pSheetID).get();
            m_pSheetImgData = pSheet->GetPreviewData();
        }
        else // 清空预览。
        {
            m_pSheetImgData.reset();
        }
    }

	UpdateData(FALSE);
    Invalidate();
}

void TaskInfoPage::UpdateModifyDate()
{
	m_strModifyDate = DateHelper::GetStrFromTimeStamp(m_pTaskItem->GetModifyTime());
	UpdateData(FALSE);
}

void TaskInfoPage::InitGridStyle()
{
	CString str;

	// 初始化零件列表的表头
    {
        // Requires one never uses column 0
        m_partGrid.InsertHiddenLabelColumn();

        // "num" column
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
        CGridColumnTrait* pNUMCol = new CGridColumnTraitText;
        m_partGrid.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNUMCol);

        // "name" column
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
        CGridColumnTrait* pNameCol = new CGridColumnTraitText;
        m_partGrid.InsertColumnTrait(2, str, LVCFMT_LEFT, 120, 0, pNameCol);

        // “已排数量”列。
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PartPlacedCount);
        CGridColumnTrait* pPlacedCountCol = new CGridColumnTraitText;
        m_partGrid.InsertColumnTrait(3, str, LVCFMT_LEFT, 100, 0, pPlacedCountCol);

        // “初始数量”列。
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PartPlanningCount);
        CGridColumnTrait* pInitialCountCol = new CGridColumnTraitText;
        m_partGrid.InsertColumnTrait(4, str, LVCFMT_LEFT, 100, 0, pInitialCountCol);
    }

	// 初始化板材列表的表头
    {
        // Requires one never uses column 0
        m_sheetGrid.InsertHiddenLabelColumn();

        // "num" column
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
        CGridColumnTrait* pNUMCol = new CGridColumnTraitText;
        m_sheetGrid.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNUMCol);

        // "name" column
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
        CGridColumnTrait* pNameCol = new CGridColumnTraitText;
        m_sheetGrid.InsertColumnTrait(2, str, LVCFMT_LEFT, 80, 0, pNameCol);

        // “数量”列。
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COUNT);
        CGridColumnTrait* pCountCol = new CGridColumnTraitText;
        m_sheetGrid.InsertColumnTrait(3, str, LVCFMT_LEFT, 50, 0, pCountCol);

        // "材料类型"列
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAT_Type);
        CGridColumnTrait* pMaterialTypeCol = new CGridColumnTraitText;
        m_sheetGrid.InsertColumnTrait(4, str, LVCFMT_LEFT, 90, 0, pMaterialTypeCol);

        // "材料厚度"列
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAT_THICKNESS);
        CGridColumnTrait* pMaterialThicknessCol = new CGridColumnTraitText;
        m_sheetGrid.InsertColumnTrait(5, str, LVCFMT_LEFT, 100, 0, pMaterialThicknessCol);

        // "材料尺寸"列
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAT_Size);
        CGridColumnTrait* pMaterialSizeCol = new CGridColumnTraitText;
        m_sheetGrid.InsertColumnTrait(6, str, LVCFMT_LEFT, 90, 0, pMaterialSizeCol);

        // "加工参数"列
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CAM_PARAM);
        CGridColumnTrait* pCamParamCol = new CGridColumnTraitText;
        m_sheetGrid.InsertColumnTrait(7, str, LVCFMT_LEFT, 140, 0, pCamParamCol);
    }
}

void TaskInfoPage::InitGridData()
{
    CString str;

    m_bDisableSelectChangeMessageOfGrid = true;

    // 清除现有表格数据。
    ReleaseGridItemData();
    m_partGrid.DeleteAllItems();
    m_sheetGrid.DeleteAllItems();

    // 填充零件表格
    {
        for (unsigned int i = 0; i < m_pTaskItem->GetTaskPartList()->size(); i++)
        {
            const TaskPart* pTaskPart = m_pTaskItem->GetTaskPartList()->operator [](i).get();

            // insert an item.
            int iRowCount = m_partGrid.GetItemCount();
            m_partGrid.InsertItem(iRowCount, _T(""));
            m_partGrid.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pTaskPart->GetID())));

            // 序号
            str.Format(_T("%d"), iRowCount + 1);
            m_partGrid.SetItemText(iRowCount, 1, str);

            // 零件名称
            m_partGrid.SetItemText(iRowCount, 2, pTaskPart->GetPart()->GetName());

            // 已排数量
            str.Format(_T("%d"), pTaskPart->GetNestedCount());
            m_partGrid.SetItemText(iRowCount, 3, str);

            // 初始数量
            str.Format(_T("%d"), pTaskPart->GetPlanningCount());
            m_partGrid.SetItemText(iRowCount, 4, str);
        }

        // 选择第一行。
        if (m_partGrid.GetItemCount() > 0)
        {
            m_partGrid.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
        }

        m_partGrid.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    }

    // 填充板材表格
    {
        for (unsigned int i = 0; i < m_pTaskItem->GetSheetList()->size(); i++)
        {
            const Sheet* pSheet = m_pTaskItem->GetSheetList()->operator [](i).get();

            // insert an item.
            int iRowCount = m_sheetGrid.GetItemCount();
            m_sheetGrid.InsertItem(iRowCount, _T(""));
            m_sheetGrid.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pSheet->GetID())));

            // 序号
            str.Format(_T("%d"), iRowCount + 1);
            m_sheetGrid.SetItemText(iRowCount, 1, str);

            // 板材名称
            m_sheetGrid.SetItemText(iRowCount, 2, pSheet->GetName());

            // 板材数量
            str.Format(_T("%d"), pSheet->GetSheetCount());
            m_sheetGrid.SetItemText(iRowCount, 3, str);

            // 材料信息
            {
                DataProperty matLibProp;
                DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
                if (MaterialLibLoader::GetMatSizeProp(pMatLibDBCon, pSheet->GetMaterialSizeID(), matLibProp))
                {
                    m_sheetGrid.SetItemText(iRowCount, 4, matLibProp.GetItem(0).GetValue().bstrVal);
                    str.Format(FLOAT_FORMAT_2, matLibProp.GetItem(1).GetValue().dblVal);
                    m_sheetGrid.SetItemText(iRowCount, 5, str);
                    m_sheetGrid.SetItemText(iRowCount, 6, matLibProp.GetItem(6).GetValue().bstrVal);
                }
                else
                {
                    str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INVALID_DATA);
                    m_sheetGrid.SetItemText(iRowCount, 4, str);
                    m_sheetGrid.SetItemText(iRowCount, 5, str);
                    m_sheetGrid.SetItemText(iRowCount, 6, str);
                }
            }

            // 加工参数信息
            {
                DataProperty expLibProp;
                DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
                if (ParamConfigLoader::GetParamConfigInfo(pExpLibDBCon, pSheet->GetParamConfigID(), expLibProp))
                {
                    CString strExpLibItem = expLibProp.GetItem(3).GetValue().bstrVal;
                    str = expLibProp.GetItem(2).GetValue().bstrVal;
                    str += _T("(");
                    str += strExpLibItem;
                    str += _T(")");
                    m_sheetGrid.SetItemText(iRowCount, 7, str);
                }
                else
                {
                    m_sheetGrid.SetItemText(iRowCount, 7, MultiLanguageMgr::GetInstance()->TranslateString(IDS_INVALID_DATA));
                }
            }
        }

        // 选择第一行。
        if (m_sheetGrid.GetItemCount() > 0)
        {
            m_sheetGrid.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
        }

        m_sheetGrid.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    }

    m_bDisableSelectChangeMessageOfGrid = false;
}

void TaskInfoPage::ReleaseGridItemData()
{
	for (int i = 0; i < m_partGrid.GetItemCount(); i++)
	{
		DWORD_PTR pItemData = m_partGrid.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}

    for (int i = 0; i < m_sheetGrid.GetItemCount(); i++)
    {
        DWORD_PTR pItemData = m_sheetGrid.GetItemData(i);
        delete (LONGLONG*)pItemData;
    }
}

END_CUTLEADER_NAMESPACE()

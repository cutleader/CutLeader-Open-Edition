#include "stdafx.h"
#include "NestTaskPartsDlg.h"

#include "baseConst.h"
#include "DBConnect.h"
#include "DataBaseManager.h"
#include "ClUtilityResource.h"
#include "LogMgr.h"
#include "ClBaseDataResource.h"
#include "StringUtil.h"
#include "CGridColumnTraitCombo.h"
#include <tuple>

#include "DefaultParamConfig.h"
#include "ParamConfigLoader.h"
#include "MachineItemLoader.h"
#include "ConfigItemLoader.h"
#include "ExpertLibItemLoader.h"
#include "MaterialLibLoader.h"
#include "MaterialSizeList.h"
#include "MaterialProcessor.h"
#include "ClMaterialLibResource.h"
#include "ConfigNestParamDlg.h"
#include "MachineParam.h"
#include "CLOptionInfo.h"
#include "ImpExpConfig.h"

#include "ClNestResource.h"
#include "TaskPartsNestResultDlg.h"
#include "NestPartList.h"
#include "NestPart.h"
#include "TaskItem.h"
#include "TaskPartList.h"
#include "SheetList.h"
#include "TaskPart.h"
#include "Part.h"
#include "PartLoader.h"
#include "PartManager.h"
#include "PartWriter.h"
#include "PartCamData.h"
#include "PartBuilder.h"
#include "NestMaterialList.h"
#include "NestTask.h"
#include "AreaInfo.h"
#include "PartItem.h"
#include "LibraryData.h"
#include "NestMaterial.h"
#include "NestProcessor.h"
#include "Sheet.h"
#include "SheetProcessor.h"
#include "SheetMaterial.h"
#include "SheetMaterialList.h"
#include "NestMatSelectDlg.h"
#include "ClGeometryFeatureResource.h"
#include "ClTaskResource.h"
#include "PartPlacementManager.h"
#include "Helper_4_NestingTaskParts.h"
#include "PolygonMaterial.h"
#include "SheetCache.h"
#include "BatchEditTaskPartDlg_2.h"
#include "clTaskUIResource.h"
#include "PartCadData.h"
#include "LoopTopologyItemList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(NestTaskPartsDlg, CDialogEx)

BEGIN_MESSAGE_MAP(NestTaskPartsDlg, CDialogEx)
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()

    ON_BN_CLICKED(IDC_RADIO_NestAllParts, OnNestAllParts)
    ON_BN_CLICKED(IDC_RADIO_NestSelectedParts, OnNestSelectedParts)
	ON_BN_CLICKED(IDC_BTN_BatchSetting, OnBatchSetting)

    ON_BN_CLICKED(IDC_RADIO_NestOnCurrentSheet, OnNestOnCurrentSheet)
    ON_BN_CLICKED(IDC_RADIO_NestOnNewMaterial, OnNestOnNewMaterial)
    ON_BN_CLICKED(IDC_BTN_AddMaterial, OnAddMat)
    ON_BN_CLICKED(IDC_BTN_DeleteMaterial, OnDeleteMat)

	ON_BN_CLICKED(IDC_BTN_NestParam, OnEditNestParam)
	ON_BN_CLICKED(IDC_BTN_StartNesting, OnStartNesting)
    ON_BN_CLICKED(IDC_BTN_CancelNesting, OnCancelNesting)
END_MESSAGE_MAP()

void NestTaskPartsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

    DDX_Radio(pDX, IDC_RADIO_NestAllParts, m_iNestedPartType);
    DDX_Control(pDX, IDC_LIST_NEST_PART, m_partGrid);

    DDX_Radio(pDX, IDC_RADIO_NestOnCurrentSheet, m_iUseMaterialType);
    DDX_Control(pDX, IDC_LIST_SheetsInTask, m_grid_sheetsInTask);
    DDX_Control(pDX, IDC_LIST_NewMaterial, m_grid_newMaterial);


    DDX_Control(pDX, IDC_BTN_BatchSetting, m_btnBatchSetPart);
    DDX_Control(pDX, IDC_BTN_AddMaterial, m_btnAddMaterial);
    DDX_Control(pDX, IDC_BTN_DeleteMaterial, m_btnDeleteMaterial);
    DDX_Control(pDX, IDC_BTN_NestParam, m_btnNestParam);
    DDX_Control(pDX, IDC_BTN_StartNesting, m_btnStartNest);
    DDX_Control(pDX, IDC_BTN_CancelNesting, m_btnCancel);
}

NestTaskPartsDlg::NestTaskPartsDlg(CWnd* pParent /*=NULL*/)
				: CLDialog(NestTaskPartsDlg::IDD, pParent)
{
	m_iNestedPartType = 0;
	m_iUseMaterialType = 0;
}

NestTaskPartsDlg::~NestTaskPartsDlg()
{
}

BOOL NestTaskPartsDlg::OnInitDialog()
{
	__super::OnInitDialog();

    // 初始化表格样式
    InitGridStyle();

    // 显示零件
    FillPartGrid_withAllPartsInTask();

    // 显示任务中所有板材
    {
        DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
        for (unsigned int i = 0; i < m_pTaskItem->GetSheetList()->size(); i++)
        {
            const Sheet* pSheet = m_pTaskItem->GetSheetList()->operator [](i).get();

			if (i == 0) // 底图上不能排版。
				continue;

            // 得到材料尺寸名称
            CString strMaterialSizeName;
            {
                DataProperty dataProp;
                if (MaterialLibLoader::GetMatSizeProp(pMatLibDBCon, pSheet->GetMaterialSizeID(), dataProp))
                {
                    strMaterialSizeName = dataProp.GetItem(6).GetValue().bstrVal;
                }
                else
                {
                    LogMgr::DebugWarn(_T("54681"));
                }
            }

            int iRowCount = m_grid_sheetsInTask.GetItemCount();
            m_grid_sheetsInTask.InsertItem(iRowCount, _T(""));
            m_grid_sheetsInTask.SetItemText(iRowCount, 1, StringUtil::Integer2String(iRowCount + 1));
            m_grid_sheetsInTask.SetItemText(iRowCount, 2, pSheet->GetName());
            m_grid_sheetsInTask.SetItemText(iRowCount, 3, strMaterialSizeName);
            m_grid_sheetsInTask.SetItemText(iRowCount, 4, StringUtil::Integer2String(pSheet->GetSheetCount()));
            m_grid_sheetsInTask.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pSheet->GetID())));
        }
        ((CWnd*)GetDlgItem(IDC_LIST_SheetsInTask))->EnableWindow(FALSE);
    }

	SetBackgroundColor(SkinManager::GetBackgroundColor());

	SetWindowTheme(GetDlgItem(IDC_STATIC_Parts)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_STATIC_Materials)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_STATIC_AllSheetsInTask)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_STATIC_MaterialFromLibrary)->GetSafeHwnd(), L"", L"");

	return TRUE;
}

void NestTaskPartsDlg::FillPartGrid_withAllPartsInTask()
{
    ReleaseData_inPartGrid();
    m_partGrid.DeleteAllItems();
	vector<CString> invalidPartNames;
    for (unsigned int i = 0; i < m_pTaskItem->GetTaskPartList()->size(); i++)
    {
        const TaskPart* pTaskPart = m_pTaskItem->GetTaskPartList()->operator [](i).get();
        const Part* pPart = pTaskPart->GetPart();

		// 检查选中的零件是否有闭合的外边框。
		DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(pPart->GetID());
		PartCadDataPtr pPartCadData = PartLoader::LoadPartCadData(pPartDBCon);
		if (pPartCadData->GetLoopTopologyItems()->GetBoundaryLoop(pPartCadData->GetPatternLoopList().get()) == INVALID_IDX)
		{
			invalidPartNames.push_back(pPart->GetName());
			continue;
		}

        // 计算零件剩余个数
        int iRemainingCount = pTaskPart->GetPlanningCount() - m_pTaskItem->GetSheetList()->GetPartNestedCount(pPart->GetID());
        if (iRemainingCount > 0)
        {
            int iRowCount = m_partGrid.GetItemCount();
            m_partGrid.InsertItem(iRowCount, _T(""));
            m_partGrid.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pPart->GetID())));
            m_partGrid.SetItemText(iRowCount, 0, StringUtil::Integer2String(iRowCount + 1));
            m_partGrid.SetItemText(iRowCount, 1, pPart->GetName());
            m_partGrid.SetItemText(iRowCount, 2, StringUtil::Integer2String(iRemainingCount));
            m_partGrid.SetItemText(iRowCount, 3, StringUtil::Integer2String(pTaskPart->GetNestPriority().GetVal()));
            m_partGrid.SetItemText(iRowCount, 4, NestPart::GetName_ofPartRotateStyle(pTaskPart->GetPartRotateType()));
        }
    }

	if (invalidPartNames.size() > 0)
		DisplayInvalidParts(invalidPartNames);
}

void  NestTaskPartsDlg::DisplayInvalidParts(const std::vector<CString>& openBoundaryPartNames)
{
	CString strMsg;
	strMsg.Format(_T("% s % s"), MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NestOpenBoundary), _T("\n"));
	for (int i = 0; i < openBoundaryPartNames.size(); i++)
	{
		CString szInvalidPartInfo;
		szInvalidPartInfo.Format(_T("% i % s % s % s"), i + 1, _T(")"), openBoundaryPartNames[i], _T("\n"));
		strMsg += szInvalidPartInfo;
	}
	MessageBox(strMsg, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_ICONWARNING);
}

void NestTaskPartsDlg::FillPartGrid_withSelectedPartsInTask()
{
    ReleaseData_inPartGrid();
    m_partGrid.DeleteAllItems();
	vector<CString> invalidPartNames;
    for (unsigned int i = 0; i < m_pSelectedTaskParts->size(); i++)
    {
        const TaskPart* pTaskPart = m_pSelectedTaskParts->operator [](i).get();
        const Part* pPart = pTaskPart->GetPart();

		// 检查选中的零件是否有闭合的外边框。
		DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(pPart->GetID());
		PartCadDataPtr pPartCadData = PartLoader::LoadPartCadData(pPartDBCon);
		if (pPartCadData->GetLoopTopologyItems()->GetBoundaryLoop(pPartCadData->GetPatternLoopList().get()) == INVALID_IDX)
		{
			invalidPartNames.push_back(pPart->GetName());
			continue;
		}

        // 计算零件剩余个数
        int iRemainingCount = pTaskPart->GetPlanningCount() - m_pTaskItem->GetSheetList()->GetPartNestedCount(pPart->GetID());
        if (iRemainingCount > 0)
        {
            int iRowCount = m_partGrid.GetItemCount();
            m_partGrid.InsertItem(iRowCount, _T(""));
            m_partGrid.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pPart->GetID())));
            m_partGrid.SetItemText(iRowCount, 0, StringUtil::Integer2String(iRowCount + 1));
            m_partGrid.SetItemText(iRowCount, 1, pPart->GetName());
            m_partGrid.SetItemText(iRowCount, 2, StringUtil::Integer2String(iRemainingCount));
            m_partGrid.SetItemText(iRowCount, 3, StringUtil::Integer2String(pTaskPart->GetNestPriority().GetVal()));
            m_partGrid.SetItemText(iRowCount, 4, NestPart::GetName_ofPartRotateStyle(pTaskPart->GetPartRotateType()));
        }
    }

	if (invalidPartNames.size() > 0)
		DisplayInvalidParts(invalidPartNames);
}

void NestTaskPartsDlg::OnNestAllParts()
{
    UpdateData(TRUE);
    FillPartGrid_withAllPartsInTask();
}

void NestTaskPartsDlg::OnNestSelectedParts()
{
    // 选中的零件中需要有未排完的零件
    bool bHaveRemainingParts = false;
    for (unsigned int i = 0; i < m_pSelectedTaskParts->size(); i++)
    {
        const TaskPart* pTaskPart = m_pSelectedTaskParts->operator [](i).get();
        int iRemainingCount = pTaskPart->GetPlanningCount() - m_pTaskItem->GetSheetList()->GetPartNestedCount(pTaskPart->GetPart()->GetID());
        if (iRemainingCount > 0)
        {
            bHaveRemainingParts = true;
            break;
        }
    }
    if (!bHaveRemainingParts)
    {
        CString strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
        CString strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PleaseSelectUnfinishedParts);
        MessageBox(strMsg, strTitle, MB_ICONWARNING);
        m_iNestedPartType = 0;
        UpdateData(FALSE);
        return;
    }

    FillPartGrid_withSelectedPartsInTask();
    UpdateData(TRUE);
}

void NestTaskPartsDlg::OnBatchSetting()
{
	// 检查有没有选中零件。
	int iSelCount = 0;
	for (int i = 0; i < m_partGrid.GetItemCount(); i++)
	{
		if (m_partGrid.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			iSelCount++;
		}
	}
	if (iSelCount == 0)
	{
		CString strProductName, str;
		strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NoPartIsSelectedToEdit);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// 批量设置任务零件的信息。
	BatchEditTaskPartDlg_2 dlg;
	if (dlg.DoModal() == IDOK)
	{
		// 自动排版旋转角度。
		if (dlg.IsEnableNestingRotateAngle())
		{
			PartRotateType emNestingRotateAngle = dlg.GetNestingRotateAngle();
			for (int i = 0; i < m_partGrid.GetItemCount(); i++)
			{
				if (m_partGrid.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
				{
					m_partGrid.SetItemText(i, 4, NestPart::GetName_ofPartRotateStyle(emNestingRotateAngle));
				}
			}
		}
	}
}

void NestTaskPartsDlg::OnNestOnCurrentSheet()
{
    UpdateData(TRUE);
    ((CWnd*)GetDlgItem(IDC_LIST_SheetsInTask))->EnableWindow(FALSE);
    ((CWnd*)GetDlgItem(IDC_LIST_NewMaterial))->EnableWindow(FALSE);
}

void NestTaskPartsDlg::OnNestOnNewMaterial()
{
    UpdateData(TRUE);
    ((CWnd*)GetDlgItem(IDC_LIST_SheetsInTask))->EnableWindow(TRUE);
    ((CWnd*)GetDlgItem(IDC_LIST_NewMaterial))->EnableWindow(TRUE);
}

void NestTaskPartsDlg::OnAddMat()
{
    NestMatSelectDlg dlg(this);
    if (dlg.DoModal() == IDOK)
    {
        // 添加到材料表格。
        MaterialSizeListPtr pMatSizeList = dlg.GetSelectedMaterialSizes();
        for (unsigned int i = 0; i < pMatSizeList->size(); i++)
        {
#if TRIAL_VERSION == 1
            if (m_grid_newMaterial.GetItemCount() == 1)
            {
                CString strTitle, strMsg;
                strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
                strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_TRIAL_MAT_COUNT_1);
                ClMessageBox(strMsg, strTitle, MB_ICONWARNING);
                return;
            }
#endif

            // check whether this material is added.
            IMaterialSizePtr pMaterialSize = pMatSizeList->at(i);
            if (MatAdded(pMaterialSize->GetID()))
            {
                continue;
            }

            // insert a new row.
            AddNestMaterial(pMaterialSize.get());
        }

        // select the last row.
        {
            int iCount = m_grid_newMaterial.GetItemCount();

            // de-select the selected rows.
            for (int i = 0; i < iCount; i++)
            {
                if (m_grid_newMaterial.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
                    m_grid_newMaterial.SetItemState(i, 0, LVIS_SELECTED);
            }

            // select the last row.
            m_grid_newMaterial.EnsureVisible(iCount-1, FALSE);
            m_grid_newMaterial.SetItemState(iCount-1, LVIS_SELECTED, LVIS_SELECTED);
            m_grid_newMaterial.SetFocus();
        }
    }
}

void NestTaskPartsDlg::OnDeleteMat()
{
    for (int i = 0; i < m_grid_newMaterial.GetItemCount(); i++)
    {
        if (m_grid_newMaterial.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
        {
            // release item data.
            DWORD_PTR pItemData = m_grid_newMaterial.GetItemData(i);
            delete (LONGLONG*)pItemData;

            // delete the item.
            m_grid_newMaterial.DeleteItem(i);
            i--;
        }
    }

    // re-index the first column.
    for (int i = 0; i < m_grid_newMaterial.GetItemCount(); i++)
    {
        CString strNum;
        if ((i+1) < 10)
            strNum.Format(_T("0%d"), i+1);
        else
            strNum.Format(_T("%d"), i+1);
        m_grid_newMaterial.SetItemText(i, 1, strNum);
    }

    // select the last row.
    int iCount = m_grid_newMaterial.GetItemCount();
    if (iCount > 0)
    {
        m_grid_newMaterial.EnsureVisible(iCount-1, FALSE);
        m_grid_newMaterial.SetItemState(iCount-1, LVIS_SELECTED, LVIS_SELECTED);
        m_grid_newMaterial.SetFocus();
    }
}

void NestTaskPartsDlg::OnEditNestParam()
{
    DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
    DefaultParamConfigPtr pDefaultParamConfig = ParamConfigLoader::LoadDflParamConfig(pExpLibDBCon);

	ConfigNestParamDlg dlg;
	dlg.Init(pDefaultParamConfig->GetParamConfigID());
	dlg.DoModal();
}

void NestTaskPartsDlg::OnStartNesting()
{
	UpdateData(TRUE);

	CString strTitle, strMsg;
	strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// 创建排版任务。
	NestTaskPtr pNestTask(new NestTask);
	SheetMaterialListPtr pSheetMaterials(new SheetMaterialList);
	{
		// 加工参数
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		DefaultParamConfigPtr pDefaultParamConfig = ParamConfigLoader::LoadDflParamConfig(pExpLibDBCon);
		LONGLONG iParamConfigID = pDefaultParamConfig->GetParamConfigID();
		pNestTask->SetParamConfigID(iParamConfigID);

		// 从列表得到排版零件信息。
		NestPartListPtr pNestPartList = GetNestPartList(iParamConfigID);
		pNestTask->SetNestPartList(pNestPartList);

		// 从列表得到排版材料信息。
		NestMaterialListPtr pNestMatList = GetNestMatList(pSheetMaterials);
		if (pNestMatList->size() == 0)
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NO_NESTMAT);
			MessageBox(strMsg, strTitle, MB_ICONWARNING);
			return;
		}
		pNestTask->SetNestMatList(pNestMatList);
	}

	// 计算零件的面积信息。
	AreaInfo areaInfo;
	{
		map<__int64, double> areaData;
		map<__int64, Polygon2DPtr> partOutPolyData;
		double dAllArea = .0;
		NestPartListPtr pNestPartList = pNestTask->GetNestPartList();
		for (unsigned int i = 0; i < pNestPartList->size(); i++)
		{
			NestPartPtr pNestPart = pNestPartList->at(i);
			pair<double, Polygon2DPtr> area = PartManager::GetPartAreaEx(pNestPart->GetPartItem()->GetPart()->GetCadData());
			areaData[pNestPart->GetPartItem()->GetPart()->GetID()] = area.first;
			partOutPolyData[pNestPart->GetPartItem()->GetPart()->GetID()] = area.second;
			dAllArea += area.first*pNestPart->GetNestCount();
		}
		areaInfo.partAreaData = areaData;
		areaInfo.partOutPolyData = partOutPolyData;
		areaInfo.dAllPartArea = dAllArea;
	}

	// 准备库信息。
	LibraryDataPtr pLibraryData;
	{
		// 机器信息
		DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pExpLibDBCon, pNestTask->GetParamConfigID());
		LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pExpLibDBCon, iExpLibItemID);
		MachineParamPtr pMachineParam = MachineItemLoader::LoadMacParam(pMacLibDBConnect, iMacItemID);

		// 参数配置。
		NestConfigItemPtr pNestingRule = ConfigItemLoader::LoadNestConfigItem(pExpLibDBCon, pNestTask->GetParamConfigID());
		SequenceConfigItemPtr pCutSequenceConfigItem = ConfigItemLoader::LoadSeqConfigItem(pExpLibDBCon, pNestTask->GetParamConfigID());

		// 材料
		MaterialSizeListPtr pMatSizeList(new MaterialSizeList);
		for (unsigned int i = 0; i < pNestTask->GetNestMatList()->size(); i++)
		{
			pMatSizeList->push_back(pNestTask->GetNestMatList()->at(i)->GetMatSize());
		}

		pLibraryData.reset(new LibraryData(pMachineParam, pMatSizeList, pNestingRule, pCutSequenceConfigItem));
	}

	// 启动排版。
	NestProcessorPtr pNestProcessor(new NestProcessor(pNestTask, areaInfo, pLibraryData));
	pNestProcessor->StartNest();

	// 显示排版过程。
	TaskPartsNestResultDlg nestResultDlg;
	nestResultDlg.Init(m_pTaskItem, pNestTask, pNestProcessor, pLibraryData, pSheetMaterials);
	if (nestResultDlg.DoModal() == IDOK)
    {
        m_pTaskPartsNestResult = nestResultDlg.GetTaskPartsNestResult();
        EndDialog(IDC_BTN_StartNesting);
    }
}

void NestTaskPartsDlg::OnCancelNesting()
{
    OnCancel();
}

void NestTaskPartsDlg::OnDestroy()
{
    ReleaseData_inPartGrid();
    ReleaseData_inSheetsInTaskGrid();
    ReleaseData_inNewMaterialGrid();
    CDialogEx::OnDestroy();
}

void NestTaskPartsDlg::ReleaseData_inPartGrid()
{
    for (int i = 0; i < m_partGrid.GetItemCount(); i++)
    {
        DWORD_PTR pItemData = m_partGrid.GetItemData(i);
        delete (LONGLONG*)pItemData;
    }
}

void NestTaskPartsDlg::ReleaseData_inSheetsInTaskGrid()
{
    for (int i = 0; i < m_grid_sheetsInTask.GetItemCount(); i++)
    {
        DWORD_PTR pItemData = m_grid_sheetsInTask.GetItemData(i);
        delete (LONGLONG*)pItemData;
    }
}

void NestTaskPartsDlg::ReleaseData_inNewMaterialGrid()
{
    for (int i = 0; i < m_grid_newMaterial.GetItemCount(); i++)
    {
        DWORD_PTR pItemData = m_grid_newMaterial.GetItemData(i);
        delete (LONGLONG*)pItemData;
    }
}

BOOL NestTaskPartsDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect r;
    GetClientRect(&r);
    if ((HBRUSH)m_brush == NULL)
        m_brush.CreateSolidBrush(SkinManager::GetBackgroundColor());
    pDC->FillRect(&r, &m_brush);

    return TRUE;
}

HBRUSH NestTaskPartsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void NestTaskPartsDlg::InitGridStyle()
{
    CString str;

    // 零件
    {
        m_partGrid.InsertColumn(0, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM), LVCFMT_LEFT, 40);
        m_partGrid.InsertColumn(1, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME), LVCFMT_LEFT, 80);
        m_partGrid.InsertColumn(2, MultiLanguageMgr::GetInstance()->TranslateString(IDS_RemainingQuantity), LVCFMT_LEFT, 120);

        // 排版优先级
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NESTMGR_NEST_PRIORITY);
        CGridColumnTraitCombo* pNameCo3 = new CGridColumnTraitCombo;
        pNameCo3->SetStyle(pNameCo3->GetStyle() | CBS_DROPDOWNLIST);
        StringListPtr pList1 = NestPriority::GetAllPriority();
        for (unsigned int i = 0; i < pList1->size(); i++)
        {
            pNameCo3->AddItem(i, pList1->at(i));
        }
        m_partGrid.InsertColumnTrait(3, str, LVCFMT_LEFT, 70, 0, pNameCo3);

        // 旋转角度
        CGridColumnTraitCombo* pRotAngCol = new CGridColumnTraitCombo;
        pRotAngCol->SetStyle(pRotAngCol->GetStyle() | CBS_DROPDOWNLIST);
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_FREE);
        pRotAngCol->AddItem(0, str);
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_90_INCREMENT);
        pRotAngCol->AddItem(1, str);
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_0_FIXED);
        pRotAngCol->AddItem(2, str);
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_90_FIXED);
        pRotAngCol->AddItem(3, str);
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_180_FIXED);
        pRotAngCol->AddItem(4, str);
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_270_FIXED);
        pRotAngCol->AddItem(5, str);
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_HORIZONTAL);
        pRotAngCol->AddItem(6, str);
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_VERTICAL);
        pRotAngCol->AddItem(7, str);
        str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEST_ROT_ANG);
        m_partGrid.InsertColumnTrait(4, str, LVCFMT_LEFT, 130, 0, pRotAngCol);

        m_partGrid.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    }

    // 任务中的板材
    {
        m_grid_sheetsInTask.InsertColumn(0, _T(""), LVCFMT_LEFT, 25);
        m_grid_sheetsInTask.InsertColumn(1, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM), LVCFMT_LEFT, 40);
        m_grid_sheetsInTask.InsertColumn(2, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME), LVCFMT_LEFT, 80);
        m_grid_sheetsInTask.InsertColumn(3, MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAT_Size), LVCFMT_LEFT, 100);
        m_grid_sheetsInTask.InsertColumn(4, MultiLanguageMgr::GetInstance()->TranslateString(IDS_COUNT), LVCFMT_LEFT, 60);
        m_grid_sheetsInTask.SetExtendedStyle(LVS_EX_CHECKBOXES | m_grid_sheetsInTask.GetExtendedStyle());
        m_grid_sheetsInTask.EnableVisualStyles(TRUE);
    }

    // 材料库的材料
    {
        m_grid_newMaterial.InsertColumn(0, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM), LVCFMT_LEFT, 40);
        m_grid_newMaterial.InsertColumn(1, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME), LVCFMT_LEFT, 80);
        m_grid_newMaterial.InsertColumn(2, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_WIDTH), LVCFMT_LEFT, 60);
        m_grid_newMaterial.InsertColumn(3, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_HEIGHT), LVCFMT_LEFT, 60);
        m_grid_newMaterial.InsertColumn(4, MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAT_AVAILABLE_NUM), LVCFMT_LEFT, 80);
        m_grid_newMaterial.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    }
}

void NestTaskPartsDlg::Init(TaskItemPtr pTaskItem, TaskPartListPtr pSelectedTaskParts, SheetPtr pActiveSheet)
{
	m_pTaskItem = pTaskItem;
	m_pSelectedTaskParts = pSelectedTaskParts;
	m_pActiveSheet = pActiveSheet;
}

NestPartListPtr NestTaskPartsDlg::GetNestPartList(LONGLONG iParamConfigID) const
{
	NestPartListPtr pNestPartList(new NestPartList);

	for (int i = 0; i < m_partGrid.GetItemCount(); i++)
	{
        LONGLONG* pPartID = (LONGLONG*)m_partGrid.GetItemData(i);

        NestPartPtr pNestPart(new NestPart);
        pNestPartList->push_back(pNestPart);

        // 加载零件项
        PartItemPtr pPartItem;
        {
            LONGLONG iCamDataID = CheckPart(iParamConfigID, *pPartID);
            DBConnectPtr pPartDBConnect = DataBaseManager::ConnectPartDB(*pPartID);
            pPartItem = PartLoader::LoadPartItem(pPartDBConnect, *pPartID, iCamDataID);
        }
        pNestPart->SetPartItem(pPartItem);

        // 排版个数。
        pNestPart->SetNestCount(_wtoi(m_partGrid.GetItemText(i, 2)));

        // 排版优先级
        pNestPart->SetPriority(NestPriority(_wtoi(m_partGrid.GetItemText(i, 3))));

        // 旋转角度
        {
            CString str1, str2, str3, str4, str5, str6, str7, str8;
            str1 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_FREE);
            str2 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_90_INCREMENT);
            str3 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_0_FIXED);
            str4 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_90_FIXED);
            str5 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_180_FIXED);
            str6 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_270_FIXED);
            str7 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_HORIZONTAL);
            str8 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_VERTICAL);
            PartRotateType iPartRotAng = PartRotate_Free;
            CString str = m_partGrid.GetItemText(i, 4);
            if (str == str1)
                iPartRotAng = PartRotate_Free;
            else if (str == str2)
                iPartRotAng = PartRotate_90Increment;
            else if (str == str3)
                iPartRotAng = PartRotate_0Fixed;
            else if (str == str4)
                iPartRotAng = PartRotate_90Fixed;
            else if (str == str5)
                iPartRotAng = PartRotate_180Fixed;
            else if (str == str6)
                iPartRotAng = PartRotate_270Fixed;
            else if (str == str7)
            {
                iPartRotAng = PartRotate_0And180Fixed;
            }
            else if (str == str8)
            {
                iPartRotAng = PartRotate_90And270Fixed;
            }
            pNestPart->SetRotStyle(iPartRotAng);
        }
	}

	return pNestPartList;
}

NestMaterialListPtr NestTaskPartsDlg::GetNestMatList(SheetMaterialListPtr pSheetMaterials) const
{
	NestMaterialListPtr pNestMatList(new NestMaterialList);

    DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
	if (m_iUseMaterialType == 0) // 在当前材料的剩余区域上排版
	{
        // 板材上除掉“不动的零件放置”留出来的余料区域。
        vector<PolygonMaterialPtr> polyMaterials = Helper_4_NestingTaskParts::CreatePolyMaterials_fromRemnantRegions(m_pActiveSheet.get());
        for (unsigned int i = 0; i < polyMaterials.size(); i++)
        {
            PolygonMaterialPtr pPolyMat = polyMaterials[i];
            NestMaterialPtr pNestMat(new NestMaterial(pPolyMat));
            if (pPolyMat->GetAvailNum() > 1)
            {
                pNestMat->SetIsEachMaterialMustHasSameLayout(true);
            }
            pNestMatList->push_back(pNestMat);
        }

        IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(pMatLibDBCon, m_pActiveSheet->GetMaterialSizeID());
		SheetMaterialPtr pSheetMaterial(new SheetMaterial(m_pActiveSheet, pMaterialSize, polyMaterials, m_pActiveSheet->Calculate2DLineArcs()));
		pSheetMaterials->push_back(pSheetMaterial);
	}
	else // 在任务中的板材上排版，以及在新材料上排版。
	{
        // 来自现有板材的材料
        for (int i = 0; i < m_grid_sheetsInTask.GetItemCount(); i++)
        {
            BOOL bChecked = m_grid_sheetsInTask.GetCheck(i);
            if (bChecked)
            {
                LONGLONG* pSheetID = (LONGLONG*)m_grid_sheetsInTask.GetItemData(i);
                SheetPtr pSheet = m_pTaskItem->GetSheetList()->GetSheetByID(*pSheetID);

                // 板材上除掉“不动的零件放置”留出来的余料区域。
                vector<PolygonMaterialPtr> polyMaterials = Helper_4_NestingTaskParts::CreatePolyMaterials_fromRemnantRegions(pSheet.get());
                for (unsigned int i = 0; i < polyMaterials.size(); i++)
                {
                    PolygonMaterialPtr pPolyMat = polyMaterials[i];
                    NestMaterialPtr pNestMat(new NestMaterial(pPolyMat));
                    if (pPolyMat->GetAvailNum() > 1)
                    {
                        pNestMat->SetIsEachMaterialMustHasSameLayout(true);
                    }
                    pNestMatList->push_back(pNestMat);
                }

                IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(pMatLibDBCon, pSheet->GetMaterialSizeID());
                SheetMaterialPtr pSheetMaterial(new SheetMaterial(pSheet, pMaterialSize, polyMaterials, pSheet->Calculate2DLineArcs()));
                pSheetMaterials->push_back(pSheetMaterial);
            }
        }

        // 来自材料库的新材料
        {
            // 得到默认机器的参数
            DBConnectPtr pExpLibConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
            DBConnectPtr pMacLibConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
            DefaultParamConfigPtr pDefaultParamConfig = ParamConfigLoader::LoadDflParamConfig(pExpLibConnect);
            LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pExpLibConnect, pDefaultParamConfig->GetExpLibItemID());
            MachineParamPtr pMachineParam = MachineItemLoader::LoadMacParam(pMacLibConnect, iMacItemID);

            //
            for (int i = 0; i < m_grid_newMaterial.GetItemCount(); i++)
            {
                LONGLONG* pMatSizeID = (LONGLONG*)m_grid_newMaterial.GetItemData(i);

                // 从数据库加载材料。
                DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
                IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(pMatLibDBCon, *pMatSizeID);

                // 创建“排版材料”。
                NestMaterialPtr pNestMat(new NestMaterial(pMaterialSize));
                pNestMatList->push_back(pNestMat);
            }
        }
	}

	return pNestMatList;
}

LONGLONG NestTaskPartsDlg::CheckPart(LONGLONG iParamConfigID, LONGLONG iPartID) const
{
	// 检查零件是不是有和当前选中的“参数配置”配套的工艺数据。
	LONGLONG iCamDataID, iExpLibItemID;
	if (!PartManager::HasCamDatasetA(iPartID, iParamConfigID, iCamDataID, iExpLibItemID))
	{
		// 创建新的工艺数据并保存。
		DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(iPartID);
		PartCadDataPtr pPartCadData = PartLoader::LoadPartCadData(pPartDBCon);
		UINT iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
		PartCamDataPtr pNewPartCamData = PartBuilder::BuildPartCamData(pPartCadData, iParamConfigID, iAssignLimit, FALSE);
		PartWriter::SaveCamData(pPartDBCon, pNewPartCamData);
		iCamDataID = pNewPartCamData->GetID();
	}

	return iCamDataID;
}

BOOL NestTaskPartsDlg::MatAdded(LONGLONG iMatSizeID)
{
    for (int i = 0; i < m_grid_newMaterial.GetItemCount(); i++)
    {
        LONGLONG* pMatSizeID = (LONGLONG*)m_grid_newMaterial.GetItemData(i);
        if (*pMatSizeID == iMatSizeID)
        {
            return TRUE;
        }
    }

    return FALSE;
}

void NestTaskPartsDlg::AddNestMaterial(const IMaterialSize* pMaterialSize)
{
    int iRowCount = m_grid_newMaterial.GetItemCount();
    LVITEM lvItem;
    lvItem.mask = LVIF_TEXT | LVIF_PARAM;
    lvItem.iItem = iRowCount;
    lvItem.iSubItem = 0;
    lvItem.pszText = _T("");
    lvItem.lParam = (DWORD_PTR)(new LONGLONG(pMaterialSize->GetID()));
    m_grid_newMaterial.InsertItem(&lvItem);
    m_grid_newMaterial.SetItemText(iRowCount, 0, StringUtil::Integer2String(iRowCount + 1));
    m_grid_newMaterial.SetItemText(iRowCount, 1, pMaterialSize->GetName());
    m_grid_newMaterial.SetItemText(iRowCount, 2, StringUtil::Double2String(pMaterialSize->GetWidth(), FLOAT_FORMAT_2));
    m_grid_newMaterial.SetItemText(iRowCount, 3, StringUtil::Double2String(pMaterialSize->GetHeight(), FLOAT_FORMAT_2));
    m_grid_newMaterial.SetItemText(iRowCount, 4, StringUtil::Integer2String(pMaterialSize->GetAvailNum()));
}

END_CUTLEADER_NAMESPACE()

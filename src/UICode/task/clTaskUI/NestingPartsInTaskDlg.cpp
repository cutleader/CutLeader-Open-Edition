#include "stdafx.h"
#include "NestingPartsInTaskDlg.h"

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

using namespace std;


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(NestingPartsInTaskDlg, CLResizableDialog)

BEGIN_MESSAGE_MAP(NestingPartsInTaskDlg, CLResizableDialog)
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()

    ON_BN_CLICKED(IDC_RADIO_NestAllParts, OnNestAllParts)
    ON_BN_CLICKED(IDC_RADIO_NestSelectedParts, OnNestSelectedParts)
END_MESSAGE_MAP()

void NestingPartsInTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CLResizableDialog::DoDataExchange(pDX);

    DDX_Radio(pDX, IDC_RADIO_NestAllParts, m_iNestedPartType);
    DDX_Control(pDX, IDC_LIST_NEST_PART, m_partGrid);
}

NestingPartsInTaskDlg::NestingPartsInTaskDlg(CWnd* pParent /*=NULL*/)
				: CLResizableDialog(NestingPartsInTaskDlg::IDD, pParent)
{
	m_iNestedPartType = 0;
}

NestingPartsInTaskDlg::~NestingPartsInTaskDlg()
{
}

BOOL NestingPartsInTaskDlg::OnInitDialog()
{
	__super::OnInitDialog();

    // 布局控件
    AddAnchor(IDC_STATIC_12, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_RADIO_NestAllParts, TOP_LEFT);
    AddAnchor(IDC_RADIO_NestSelectedParts, TOP_LEFT);
    AddAnchor(IDC_LIST_NEST_PART, TOP_LEFT, BOTTOM_RIGHT);

    // 初始化表格样式
    InitGridStyle();

    // 显示零件
    FillPartGrid_withAllPartsInTask();

	SetBackgroundColor(UI_COLOR);
	return TRUE;
}

void NestingPartsInTaskDlg::FillPartGrid_withAllPartsInTask()
{
    ReleaseData_inPartGrid();
    m_partGrid.DeleteAllItems();
    for (unsigned int i = 0; i < m_pTaskItem->GetTaskPartList()->size(); i++)
    {
        const TaskPart* pTaskPart = m_pTaskItem->GetTaskPartList()->operator [](i).get();
        const Part* pPart = pTaskPart->GetPart();

        // 计算零件剩余个数
        int iRemainingCount = pTaskPart->GetPlanningCount() - m_pTaskItem->GetSheetList()->GetPartNestedCount(pPart->GetID());
        if (iRemainingCount > 0)
        {
            int iRowCount = m_partGrid.GetItemCount();
            m_partGrid.InsertItem(iRowCount, _T(""));
            m_partGrid.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pPart->GetID())));
            m_partGrid.SetItemText(iRowCount, 0, StringUtil::Integer2String(i + 1));
            m_partGrid.SetItemText(iRowCount, 1, pPart->GetName());
            m_partGrid.SetItemText(iRowCount, 2, StringUtil::Integer2String(iRemainingCount));
            m_partGrid.SetItemText(iRowCount, 3, StringUtil::Integer2String(pTaskPart->GetNestPriority().GetVal()));
			m_partGrid.SetItemText(iRowCount, 4, NestPart::GetName_ofPartRotateStyle(pTaskPart->GetPartRotateType()));
        }
    }
}

void NestingPartsInTaskDlg::FillPartGrid_withSelectedPartsInTask()
{
    ReleaseData_inPartGrid();
    m_partGrid.DeleteAllItems();
    for (unsigned int i = 0; i < m_pSelectedTaskParts->size(); i++)
    {
        const TaskPart* pTaskPart = m_pSelectedTaskParts->operator [](i).get();
        const Part* pPart = pTaskPart->GetPart();

        // 计算零件剩余个数
        int iRemainingCount = pTaskPart->GetPlanningCount() - m_pTaskItem->GetSheetList()->GetPartNestedCount(pPart->GetID());
        if (iRemainingCount > 0)
        {
            int iRowCount = m_partGrid.GetItemCount();
            m_partGrid.InsertItem(iRowCount, _T(""));
            m_partGrid.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pPart->GetID())));
            m_partGrid.SetItemText(iRowCount, 0, StringUtil::Integer2String(i + 1));
            m_partGrid.SetItemText(iRowCount, 1, pPart->GetName());
            m_partGrid.SetItemText(iRowCount, 2, StringUtil::Integer2String(iRemainingCount));
            m_partGrid.SetItemText(iRowCount, 3, StringUtil::Integer2String(pTaskPart->GetNestPriority().GetVal()));
            m_partGrid.SetItemText(iRowCount, 4, NestPart::GetName_ofPartRotateStyle(pTaskPart->GetPartRotateType()));
        }
    }
}

void NestingPartsInTaskDlg::OnNestAllParts()
{
    UpdateData(TRUE);
    FillPartGrid_withAllPartsInTask();
}

void NestingPartsInTaskDlg::OnNestSelectedParts()
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

void NestingPartsInTaskDlg::OnDestroy()
{
    ReleaseData_inPartGrid();
    CLResizableDialog::OnDestroy();
}

void NestingPartsInTaskDlg::ReleaseData_inPartGrid()
{
    for (int i = 0; i < m_partGrid.GetItemCount(); i++)
    {
        DWORD_PTR pItemData = m_partGrid.GetItemData(i);
        delete (LONGLONG*)pItemData;
    }
}

BOOL NestingPartsInTaskDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect r;
    GetClientRect(&r);
    if ((HBRUSH)m_brush == NULL)
        m_brush.CreateSolidBrush(UI_COLOR);
    pDC->FillRect(&r, &m_brush);

    return TRUE;
}

HBRUSH NestingPartsInTaskDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CLResizableDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    TCHAR classname[MAX_PATH];
    if (::GetClassName(pWnd->m_hWnd, classname, MAX_PATH) == 0)
        return hbr;

    if (_tcsicmp(classname, _T("EDIT")) == 0)
        return hbr;

    if (_tcsicmp(classname, _T("LISTBOX")) == 0)
        return hbr;

    if (_tcsicmp(classname, WC_TREEVIEW) == 0)
        return hbr;

    pDC->SetBkColor(UI_COLOR);

    if ((HBRUSH)m_brush == NULL)
        m_brush.CreateSolidBrush(UI_COLOR);

    return (HBRUSH) m_brush;
}

void NestingPartsInTaskDlg::InitGridStyle()
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
}

void NestingPartsInTaskDlg::Init(TaskItemPtr pTaskItem, TaskPartListPtr pSelectedTaskParts)
{
	m_pTaskItem = pTaskItem;
	m_pSelectedTaskParts = pSelectedTaskParts;
}

NestPartListPtr NestingPartsInTaskDlg::GetNestPartList(LONGLONG iParamConfigID) const
{
	NestPartListPtr pNestPartList(new NestPartList);

	for (unsigned int i = 0; i < m_partGrid.GetItemCount(); i++)
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
		PartRotateType emPartRotate = NestPart::GetPartRotateStyle_ofName(m_partGrid.GetItemText(i, 4));
        pNestPart->SetRotStyle(emPartRotate);
	}

	return pNestPartList;
}

LONGLONG NestingPartsInTaskDlg::CheckPart(LONGLONG iParamConfigID, LONGLONG iPartID) const
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

END_CUTLEADER_NAMESPACE()

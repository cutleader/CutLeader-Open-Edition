#include "stdafx.h"
#include "ResetLoopToolDlg.h"

#include "baseConst.h"
#include "DBConnect.h"
#include "DataBaseManager.h"
#include "DateHelper.h"
#include "clBaseDataResource.h"

#include "ParamConfigLoader.h"
#include "MachineItemLoader.h"
#include "ConfigItemLoader.h"
#include "ExpertLibItemLoader.h"
#include "ToolAssignConfigItem.h"
#include "ConfigItemWriter.h"
#include "ExpertLibItemWriter.h"
#include "clExpertLibResource.h"
#include "ToolInstanceList.h"
#include "ToolInstance.h"

#include "clCutFeatureResource.h"
#include "PartItem.h"
#include "PartCamData.h"
#include "LoopCutFeature.h"
#include "LoopCutFeatureList.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "LoopToolManager.h"
#include "Sheet.h"
#include "CutSequenceManager.h"
#include "clPartLayoutUIResource.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

ResetLoopToolDlg::ResetLoopToolDlg(CWnd* pParent)
			: CLResizableDialog(ResetLoopToolDlg::IDD, pParent)
{
}

ResetLoopToolDlg::~ResetLoopToolDlg()
{
}

BEGIN_MESSAGE_MAP(ResetLoopToolDlg, CResizableDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

void ResetLoopToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDOK, m_btnOK );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL ResetLoopToolDlg::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// layout the controls on the dialog.
	AddAnchor(IDC_STATIC_CHILDAREA, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	// 创建子对话框。
	m_toolAssignPage.Create(IDD_clExpertLibUI_ToolAssign, this);
    PlaceSubDlg();

	// 显示子对话框数据。
    {
        DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
        DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();

        // 加载所有条件类型。
        LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, m_pSheet->GetParamConfigID());
        LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pDBConnect, iExpLibItemID);
        ConditionTypeListPtr pAllCondTypeList = MachineItemLoader::LoadMacConds(pMacLibDBConnect, iMacItemID);

        ToolAssignConfigItemPtr pToolAssignConfigItem = ConfigItemLoader::LoadToolAssignConfigItem(pDBConnect, m_pSheet->GetParamConfigID(), pAllCondTypeList);
        m_toolAssignPage.DisplayToolAssignInfo(pToolAssignConfigItem, pAllCondTypeList);
    }

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

void ResetLoopToolDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

    // position the sub dialog.
    PlaceSubDlg();
}

void ResetLoopToolDlg::OnBnClickedOk()
{
    // save the modification.
    ToolAssignConfigItemPtr pToolAssignConfigItem = m_toolAssignPage.GetToolAssignConfigItem();
    if (pToolAssignConfigItem->IsModified())
    {
        SaveParam();
    }

    // reset tool info of all loops.
    PartPlacementListPtr pPartPlacements = m_pSheet->GetPartPlacements();
    for (unsigned int i = 0; i < pPartPlacements->size(); i++)
    {
        PartPlacementPtr pPartPlacement = pPartPlacements->operator [](i);
        PartCamDataPtr pPartCamData = pPartPlacement->GetCamData();
        LoopCutFeatureListPtr pLoopCutFeatures = pPartCamData->GetLoopFeatureList();
        for (unsigned int j = 0; j < pLoopCutFeatures->size(); j++)
        {
            LoopCutFeaturePtr pLoopCutFeature = pLoopCutFeatures->operator [](j);
            LoopToolManager::AssignLoopTool(pLoopCutFeature->GetLoopTool(), pPartPlacement->GetLoopTopologyItems(), pToolAssignConfigItem.get());
        }
    }

    // update the cache data of sequences.
    CutSequenceListPtr pCutSequences = m_pSheet->GetCutSequences();
    CutSequenceManager::UpdateCache(pCutSequences);

    // 提示信息。
    CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AssignedLoopTool);
    CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
    MessageBox(str, strProductName, MB_OK | MB_ICONINFORMATION);

	CResizableDialog::OnOK();
}

void ResetLoopToolDlg::OnBnClickedCancel()
{
    ToolAssignConfigItemPtr pToolAssignConfigItem = m_toolAssignPage.GetToolAssignConfigItem();
    if (pToolAssignConfigItem->IsModified() && MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_TOOL_CONFIG), NULL, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        SaveParam();
    }

	CResizableDialog::OnCancel();
}

void ResetLoopToolDlg::Init(SheetPtr pSheet)
{
    m_pSheet = pSheet;
}

void ResetLoopToolDlg::SaveParam()
{
    ToolAssignConfigItemPtr pToolAssignConfigItem = m_toolAssignPage.GetToolAssignConfigItem();

    DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
    pDBConnect->BeginTransaction();

	// this function will clear isModified flag
    ConfigItemWriter::SaveToolAssignConfigItem(pDBConnect, pToolAssignConfigItem, FALSE);
    LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pToolAssignConfigItem->GetParentID());
    ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
    pDBConnect->CommitTransaction();
}

void ResetLoopToolDlg::PlaceSubDlg()
{
    CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILDAREA);
    if (pWndPos)
    {
        CRect pagePos;
        pWndPos->GetWindowRect(&pagePos);
        ScreenToClient(&pagePos);
        m_toolAssignPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
        m_toolAssignPage.ShowWindow(SW_SHOW);
    }
}
HBRUSH ResetLoopToolDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

END_CUTLEADER_NAMESPACE()

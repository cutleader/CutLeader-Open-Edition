#include "stdafx.h"
#include "ResetStartCutPointDlg.h"

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
#include "ToolInstance.h"
#include "ToolInstanceList.h"
#include "PierceConfigItem.h"

#include "clCutFeatureResource.h"
#include "PartItem.h"
#include "PartCamData.h"
#include "LoopToolData.h"
#include "LoopStartCutProcessor.h"
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

ResetStartCutPointDlg::ResetStartCutPointDlg(CWnd* pParent)
			: CLResizableDialog(ResetStartCutPointDlg::IDD, pParent)
{
}

ResetStartCutPointDlg::~ResetStartCutPointDlg()
{
}

BEGIN_MESSAGE_MAP(ResetStartCutPointDlg, CResizableDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

void ResetStartCutPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDOK, m_btnOK );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL ResetStartCutPointDlg::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// layout the controls on the dialog.
	AddAnchor(IDC_STATIC_CHILDAREA, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	// 创建子对话框。
	m_pierceLocPage.Create(IDD_clExpertLibUI_PierceLocation, this);
    PlaceSubDlg();

	// 显示子对话框数据。
    {
        DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
        PierceConfigItemPtr pPierceConfigItem = ConfigItemLoader::LoadPierceSetting(pDBConnect, m_pSheet->GetParamConfigID());
        m_pierceLocPage.DisplayPierceSetting(pPierceConfigItem);
    }

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

void ResetStartCutPointDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

    // position the sub dialog.
    PlaceSubDlg();
}

void ResetStartCutPointDlg::OnBnClickedOk()
{
    // save the modification.
    PierceConfigItemPtr pPierceConfigItem = m_pierceLocPage.GetPierceConfigItem();
    if (pPierceConfigItem->IsModified())
    {
        SaveParam();
    }

    DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
    DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();

    // 加载所有条件类型。
    LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, m_pSheet->GetParamConfigID());
    LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pDBConnect, iExpLibItemID);
    ConditionTypeListPtr pAllCondTypeList = MachineItemLoader::LoadMacConds(pMacLibDBConnect, iMacItemID);

    ToolAssignConfigItemPtr pToolAssignConfigItem = ConfigItemLoader::LoadToolAssignConfigItem(pDBConnect, m_pSheet->GetParamConfigID(), pAllCondTypeList);
    LeadConfigItemPtr pLeadConfigItem = ConfigItemLoader::LoadLeadSetting(pDBConnect, m_pSheet->GetParamConfigID());

    // 重置每个轮廓的起切点。
    PartPlacementListPtr pPartPlacements = m_pSheet->GetPartPlacements();
    for (unsigned int i = 0; i < pPartPlacements->size(); i++)
    {
        PartPlacementPtr pPartPlacement = pPartPlacements->operator [](i);
        const Matrix2D& transformMatInSheet = pPartPlacement->GetMatrix();
        LoopCutFeatureListPtr pLoopCutFeatures = pPartPlacement->GetCamData()->GetLoopFeatureList();
        for (unsigned int j = 0; j < pLoopCutFeatures->size(); j++)
        {
            LoopCutFeaturePtr pLoopCutFeature = pLoopCutFeatures->operator [](j);
			LoopStartCutProcessor::ResetStartCutPtOnPatternLoop(pLoopCutFeature->GetLoopStartCutData(), pPartPlacement->GetLoopTopologyItems(), pLeadConfigItem.get(),
				pPierceConfigItem.get(), transformMatInSheet);
        }
    }

    // update the cache data of sequences.
    CutSequenceListPtr pCutSequences = m_pSheet->GetCutSequences();
    CutSequenceManager::UpdateCache(pCutSequences);

    // 提示信息。
    CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AssignedStartCutPtAndLeadData);
    CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
    MessageBox(str, strProductName, MB_OK | MB_ICONINFORMATION);

	CResizableDialog::OnOK();
}

void ResetStartCutPointDlg::OnBnClickedCancel()
{
    PierceConfigItemPtr pPierceConfigItem = m_pierceLocPage.GetPierceConfigItem();
    if (pPierceConfigItem->IsModified() && MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_PIERCE), NULL, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        SaveParam();
    }

	CResizableDialog::OnCancel();
}

void ResetStartCutPointDlg::Init(SheetPtr pSheet)
{
    m_pSheet = pSheet;
}

void ResetStartCutPointDlg::SaveParam()
{
    DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
    PierceConfigItemPtr pPierceConfigItem = m_pierceLocPage.GetPierceConfigItem();
    LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pPierceConfigItem->GetParentID());

    pDBConnect->BeginTransaction();
    ConfigItemWriter::SavePierceConfigItem(pDBConnect, pPierceConfigItem, FALSE);
    ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
    pDBConnect->CommitTransaction();
}

void ResetStartCutPointDlg::PlaceSubDlg()
{
    CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILDAREA);
    if (pWndPos)
    {
        CRect pagePos;
        pWndPos->GetWindowRect(&pagePos);
        ScreenToClient(&pagePos);
        m_pierceLocPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
        m_pierceLocPage.ShowWindow(SW_SHOW);
    }
}

HBRUSH ResetStartCutPointDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

END_CUTLEADER_NAMESPACE()

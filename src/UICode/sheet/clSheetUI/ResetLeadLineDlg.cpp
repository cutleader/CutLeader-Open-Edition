#include "stdafx.h"
#include "ResetLeadLineDlg.h"

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

ResetLeadLineDlg::ResetLeadLineDlg(CWnd* pParent)
			: CLResizableDialog(ResetLeadLineDlg::IDD, pParent)
{
}

ResetLeadLineDlg::~ResetLeadLineDlg()
{
}

BEGIN_MESSAGE_MAP(ResetLeadLineDlg, CResizableDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

void ResetLeadLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDOK, m_btnOK );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL ResetLeadLineDlg::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// layout the controls on the dialog.
	AddAnchor(IDC_STATIC_CHILDAREA, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	// 创建子对话框。
	m_leadInOutPage.Create(IDD_clExpertLibUI_LeadInOut, this);
    PlaceSubDlg();

	// 显示子对话框数据。
    {
        DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
        LeadConfigItemPtr pLeadConfigItem = ConfigItemLoader::LoadLeadSetting(pDBConnect, m_pSheet->GetParamConfigID());
        m_leadInOutPage.DisplayLeadSetting(pLeadConfigItem);
    }

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

void ResetLeadLineDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

    // position the sub dialog.
    PlaceSubDlg();
}

void ResetLeadLineDlg::OnBnClickedOk()
{
    // save the modification.
    LeadConfigItemPtr pLeadConfigItem = m_leadInOutPage.GetLeadSetting();
    if (pLeadConfigItem->IsModified()) {
        SaveParam();
    }

    PartPlacementListPtr pPartPlacements = m_pSheet->GetPartPlacements();
    for (unsigned int i = 0; i < pPartPlacements->size(); i++)
    {
        PartPlacementPtr pPartPlacement = pPartPlacements->operator [](i);
        PartCamDataPtr pPartCamData = pPartPlacement->GetCamData();
        LoopCutFeatureListPtr pLoopCutFeatures = pPartCamData->GetLoopFeatureList();
        for (unsigned int j = 0; j < pLoopCutFeatures->size(); j++) {
            LoopCutFeaturePtr pLoopCutFeature = pLoopCutFeatures->operator [](j);
			LoopStartCutProcessor::ResetLeadInfoOnPatternLoop(pLoopCutFeature->GetLoopStartCutData(), pPartPlacement->GetLoopTopologyItems(), pLeadConfigItem.get());
        }
    }

    // update the cache data of sequences.
    CutSequenceListPtr pCutSequences = m_pSheet->GetCutSequences();
    CutSequenceManager::UpdateCache(pCutSequences);

    // 提示信息。
    CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AssignedStartCutPtAndLeadData);
    MessageBox(str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONINFORMATION);

	CResizableDialog::OnOK();
}

void ResetLeadLineDlg::OnBnClickedCancel()
{
    LeadConfigItemPtr pLeadConfigItem = m_leadInOutPage.GetLeadSetting();
    if (pLeadConfigItem->IsModified() && MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_LEAD), NULL, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        SaveParam();
    }

	CResizableDialog::OnCancel();
}

void ResetLeadLineDlg::Init(SheetPtr pSheet)
{
    m_pSheet = pSheet;
}

void ResetLeadLineDlg::SaveParam()
{
    DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
    LeadConfigItemPtr pLeadConfigItem = m_leadInOutPage.GetLeadSetting();
    LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pLeadConfigItem->GetParentID());

    pDBConnect->BeginTransaction();
    ConfigItemWriter::SaveLeadSetting(pDBConnect, pLeadConfigItem, FALSE);
    ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
    pDBConnect->CommitTransaction();
}

void ResetLeadLineDlg::PlaceSubDlg()
{
    CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILDAREA);
    if (pWndPos)
    {
        CRect pagePos;
        pWndPos->GetWindowRect(&pagePos);
        ScreenToClient(&pagePos);
        m_leadInOutPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
        m_leadInOutPage.ShowWindow(SW_SHOW);
    }
}
HBRUSH ResetLeadLineDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

END_CUTLEADER_NAMESPACE()

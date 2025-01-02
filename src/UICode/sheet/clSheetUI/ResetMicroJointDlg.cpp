#include "stdafx.h"
#include "ResetMicroJointDlg.h"

#include "baseConst.h"
#include "DBConnect.h"
#include "DataBaseManager.h"
#include "DateHelper.h"
#include "clBaseDataResource.h"

#include "ParamConfigLoader.h"
#include "MachineItemLoader.h"
#include "ConfigItemLoader.h"
#include "ExpertLibItemLoader.h"
#include "MicroJointConfigItem.h"
#include "ConfigItemWriter.h"
#include "ExpertLibItemWriter.h"
#include "clExpertLibResource.h"

#include "LoopMicroJoint.h"
#include "MicroJointList.h"
#include "IPatternLoop.h"
#include "clCutFeatureResource.h"
#include "PartItem.h"
#include "PartCamData.h"
#include "LoopCutFeature.h"
#include "LoopCutFeatureList.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "MicroJointManager.h"
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

ResetMicroJointDlg::ResetMicroJointDlg(CWnd* pParent)
			: CLResizableDialog(ResetMicroJointDlg::IDD, pParent)
{
}

ResetMicroJointDlg::~ResetMicroJointDlg()
{
}

BEGIN_MESSAGE_MAP(ResetMicroJointDlg, CResizableDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

void ResetMicroJointDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDOK, m_btnOK );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL ResetMicroJointDlg::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// layout the controls on the dialog.
	AddAnchor(IDC_STATIC_CHILDAREA, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	// 创建子对话框。
	m_microJointAssignmentPage.Create(IDD_clExpertLibUI_MicroJoint_Assignment, this);
    PlaceSubDlg();

	// 显示子对话框数据。
    {
        DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
        MicroJointConfigItemPtr pMicroJointConfigItem = ConfigItemLoader::LoadMicroJointSetting(pDBConnect, m_pSheet->GetParamConfigID());
        m_microJointAssignmentPage.DisplayMicroJointSetting(pMicroJointConfigItem);
    }

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

void ResetMicroJointDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

    // position the sub dialog.
    PlaceSubDlg();
}

void ResetMicroJointDlg::OnBnClickedOk()
{
    // save the modification.
    MicroJointConfigItemPtr pMicroJointConfigItem = m_microJointAssignmentPage.GetMicroJointSetting();
    if (pMicroJointConfigItem->IsModified())
    {
        SaveParam();
    }

	// must be modified after clicking ok to prevent from sheet changed even no param changed
	m_pSheet->SetModified( TRUE );

    // reset micro joint info of all loops.
    PartPlacementListPtr pPartPlacements = m_pSheet->GetPartPlacements();
    for (unsigned int i = 0; i < pPartPlacements->size(); i++)
    {
        PartPlacementPtr pPartPlacement = pPartPlacements->operator [](i);
        PartCamDataPtr pPartCamData = pPartPlacement->GetCamData();
        LoopCutFeatureListPtr pLoopCutFeatures = pPartCamData->GetLoopFeatureList();
        for (unsigned int j = 0; j < pLoopCutFeatures->size(); j++)
        {
            LoopCutFeaturePtr pLoopCutFeature = pLoopCutFeatures->operator [](j);
            if (!pLoopCutFeature->GetPatternLoop()->IsOpenPath())
            {
                // clear micro joint and assign again.
                pLoopCutFeature->GetLoopMicroJoint()->GetMicroJointList()->clear();
                MicroJointManager::AssignLoopMicroJoint(pLoopCutFeature->GetLoopMicroJoint(), pMicroJointConfigItem.get(), pPartPlacement->GetLoopTopologyItems());
            }
        }
    }

    // update the cache data of sequences.
    CutSequenceListPtr pCutSequenceList = m_pSheet->GetCutSequences();
    CutSequenceManager::UpdateCache(pCutSequenceList);

    // 提示信息。
    CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AssignedMicroJoint);
    CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
    MessageBox(str, strProductName, MB_OK | MB_ICONINFORMATION);

	CResizableDialog::OnOK();
}

void ResetMicroJointDlg::OnBnClickedCancel()
{
    MicroJointConfigItemPtr pMicroJointConfigItem = m_microJointAssignmentPage.GetMicroJointSetting();
    if (pMicroJointConfigItem->IsModified() && MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_MJ_ASSIGN), NULL, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        SaveParam();
    }

	CResizableDialog::OnCancel();
}

void ResetMicroJointDlg::Init(SheetPtr pSheet)
{
    m_pSheet = pSheet;
}

void ResetMicroJointDlg::SaveParam()
{
    MicroJointConfigItemPtr pMicroJointConfigItem = m_microJointAssignmentPage.GetMicroJointSetting();

    DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
    pDBConnect->BeginTransaction();
    ConfigItemWriter::SaveMicroJointConfigItem(pDBConnect, pMicroJointConfigItem, FALSE);
    LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pMicroJointConfigItem->GetParentID());
    ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
    pDBConnect->CommitTransaction();
}

void ResetMicroJointDlg::PlaceSubDlg()
{
    CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILDAREA);
    if (pWndPos)
    {
        CRect pagePos;
        pWndPos->GetWindowRect(&pagePos);
        ScreenToClient(&pagePos);
        m_microJointAssignmentPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
        m_microJointAssignmentPage.ShowWindow(SW_SHOW);
    }
}
HBRUSH ResetMicroJointDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

END_CUTLEADER_NAMESPACE()

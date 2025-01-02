#include "stdafx.h"
#include "ResetCornerFeatureDlg.h"

#include "baseConst.h"
#include "DBConnect.h"
#include "DataBaseManager.h"
#include "DateHelper.h"
#include "clBaseDataResource.h"

#include "ParamConfigLoader.h"
#include "MachineItemLoader.h"
#include "ConfigItemLoader.h"
#include "ExpertLibItemLoader.h"
#include "CornerConfigItem.h"
#include "ConfigItemWriter.h"
#include "ExpertLibItemWriter.h"
#include "clExpertLibResource.h"

#include "IPatternLoop.h"
#include "clCutFeatureResource.h"
#include "PartItem.h"
#include "PartCamData.h"
#include "LoopCutFeature.h"
#include "LoopCutFeatureList.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "LoopCornerData.h"
#include "CornerList.h"
#include "CornerManager.h"
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

ResetCornerFeatureDlg::ResetCornerFeatureDlg(CWnd* pParent)
			: CLResizableDialog(ResetCornerFeatureDlg::IDD, pParent)
{
}

ResetCornerFeatureDlg::~ResetCornerFeatureDlg()
{
}

BEGIN_MESSAGE_MAP(ResetCornerFeatureDlg, CResizableDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

void ResetCornerFeatureDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDOK, m_btnOK );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL ResetCornerFeatureDlg::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// layout the controls on the dialog.
	AddAnchor(IDC_STATIC_CHILDAREA, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	// 创建子对话框。
	m_cornerAssignPage.Create(IDD_clExpertLibUI_Corner, this);
    PlaceSubDlg();

	// 显示子对话框数据。
    {
        DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
        CornerConfigItemPtr pCornerConfigItem = ConfigItemLoader::LoadCornerConfigItem(pDBConnect, m_pSheet->GetParamConfigID());
        m_cornerAssignPage.DisplayCornerConfigItem(pCornerConfigItem);
    }

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

void ResetCornerFeatureDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

    // position the sub dialog.
    PlaceSubDlg();
}

void ResetCornerFeatureDlg::OnBnClickedOk()
{
    // save the modification.
    CornerConfigItemPtr pCornerConfigItem = m_cornerAssignPage.GetCornerConfigItem();
    if (pCornerConfigItem->IsModified())
    {
        SaveParam();
    }

	// must be modified after clicking ok to prevent from sheet changed even no param changed
	m_pSheet->SetModified( TRUE );

    // reset corner info of all loops.
    PartPlacementListPtr pPartPlacements = m_pSheet->GetPartPlacements();
    for (unsigned int i = 0; i < pPartPlacements->size(); i++)
    {
        PartPlacementPtr pPartPlacement = pPartPlacements->operator [](i);
		const LoopTopologyItemList* pLoopTopologyItems = pPartPlacement->GetLoopTopologyItems();
        PartCamDataPtr pPartCamData = pPartPlacement->GetCamData();
        LoopCutFeatureListPtr pLoopCutFeatures = pPartCamData->GetLoopFeatureList();
        for (unsigned int j = 0; j < pLoopCutFeatures->size(); j++)
        {
            LoopCutFeaturePtr pLoopCutFeature = pLoopCutFeatures->operator [](j);
            if (pLoopCutFeature->GetPatternLoop()->GetPatternLoopType() == PatternLoop_Polygon && !pLoopCutFeature->GetPatternLoop()->IsOpenPath())
            {
                // clear corner feature and assign again.
                pLoopCutFeature->GetLoopCorner()->GetCornerList()->clear();
                CornerManager::AssignLoopCorner(pLoopCutFeature->GetLoopCorner(), pLoopTopologyItems, pCornerConfigItem.get());
            }
        }
    }

    // update the cache data of sequences.
    CutSequenceListPtr pCutSequences = m_pSheet->GetCutSequences();
    CutSequenceManager::UpdateCache(pCutSequences);

    // 提示信息。
    CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AssignedCornerFeature);
    CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
    MessageBox(str, strProductName, MB_OK | MB_ICONINFORMATION);

	CResizableDialog::OnOK();
}

void ResetCornerFeatureDlg::OnBnClickedCancel()
{
    CornerConfigItemPtr pCornerConfigItem = m_cornerAssignPage.GetCornerConfigItem();
    if (pCornerConfigItem->IsModified() && MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_CORNER_ASSIGN), NULL, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        SaveParam();
    }

	CResizableDialog::OnCancel();
}

void ResetCornerFeatureDlg::Init(SheetPtr pSheet)
{
    m_pSheet = pSheet;
}

void ResetCornerFeatureDlg::SaveParam()
{
    CornerConfigItemPtr pCornerConfigItem = m_cornerAssignPage.GetCornerConfigItem();

    DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
    pDBConnect->BeginTransaction();
    ConfigItemWriter::SaveCornerConfigItem(pDBConnect, pCornerConfigItem, FALSE);
    LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pCornerConfigItem->GetParentID());
    ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
    pDBConnect->CommitTransaction();
}

void ResetCornerFeatureDlg::PlaceSubDlg()
{
    CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILDAREA);
    if (pWndPos)
    {
        CRect pagePos;
        pWndPos->GetWindowRect(&pagePos);
        ScreenToClient(&pagePos);
        m_cornerAssignPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
        m_cornerAssignPage.ShowWindow(SW_SHOW);
    }
}
HBRUSH ResetCornerFeatureDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "ResetSequenceDlg.h"

#include "baseConst.h"
#include "DBConnect.h"
#include "DataBaseManager.h"
#include "DateHelper.h"
#include "clBaseDataResource.h"
#include "ProgressDlg.h"

#include "ParamConfigLoader.h"
#include "MachineItemLoader.h"
#include "ConfigItemLoader.h"
#include "ExpertLibItemLoader.h"
#include "SequenceConfigItem.h"
#include "ConfigItemWriter.h"
#include "ExpertLibItemWriter.h"
#include "clExpertLibResource.h"

#include "clCutSequenceResource.h"
#include "PartItem.h"
#include "PartCamData.h"
#include "LoopCutFeature.h"
#include "LoopCutFeatureList.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "Sheet.h"
#include "CutSequenceManager.h"
#include "AutoSequenceTask.h"
#include "clSheetResource.h"
#include "CutSequenceList.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

ResetSequenceDlg::ResetSequenceDlg(CWnd* pParent)
			: CLResizableDialog(ResetSequenceDlg::IDD, pParent)
{
}

ResetSequenceDlg::~ResetSequenceDlg()
{
}

BEGIN_MESSAGE_MAP(ResetSequenceDlg, CResizableDialog)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

void ResetSequenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDOK, m_btnOK );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL ResetSequenceDlg::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// layout the controls on the dialog.
	AddAnchor(IDC_STATIC_CHILDAREA, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	// 创建子对话框。
	m_sequenceSetPage.Create(IDD_clExpertLibUI_Sequence, this);
	PlaceSubDlg();

	// 显示子对话框数据。
	{
		DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		SequenceConfigItemPtr pSequenceConfigItem = ConfigItemLoader::LoadSeqConfigItem(pDBConnect, m_pSheet->GetParamConfigID());
		m_sequenceSetPage.DisplaySeqConfigItem(pSequenceConfigItem);
	}

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

void ResetSequenceDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

	// position the sub dialog.
	PlaceSubDlg();
}

void ResetSequenceDlg::OnBnClickedOk()
{
	// save the modification.
	SequenceConfigItemPtr pSequenceConfigItem = m_sequenceSetPage.GetSeqConfigItem();
	if (pSequenceConfigItem->IsModified()) {
		SaveParam();
	}

	// 启动任务来分配工序。
	AutoSequenceTaskPtr pAutoSeqTask(new AutoSequenceTask(m_pSheet, true));
	ProgressDlg progressDlg;
	progressDlg.Init(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AUTO_ASSIGN_SEQ), pAutoSeqTask);
	progressDlg.DoModal();

	CResizableDialog::OnOK();
}

void ResetSequenceDlg::OnBnClickedCancel()
{
	SequenceConfigItemPtr pSequenceConfigItem = m_sequenceSetPage.GetSeqConfigItem();
	if (pSequenceConfigItem->IsModified() && MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_SEQ_CONFIG), NULL, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		SaveParam();
	}

	CResizableDialog::OnCancel();
}

void ResetSequenceDlg::Init(SheetPtr pSheet)
{
	m_pSheet = pSheet;
}

void ResetSequenceDlg::SaveParam()
{
	SequenceConfigItemPtr pSequenceConfigItem = m_sequenceSetPage.GetSeqConfigItem();

	DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	pDBConnect->BeginTransaction();
	ConfigItemWriter::SaveSeqConfigItem(pDBConnect, pSequenceConfigItem, FALSE);
	LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pSequenceConfigItem->GetParentID());
	ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
	pDBConnect->CommitTransaction();
}

void ResetSequenceDlg::PlaceSubDlg()
{
	CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILDAREA);
	if (pWndPos)
	{
		CRect pagePos;
		pWndPos->GetWindowRect(&pagePos);
		ScreenToClient(&pagePos);
		m_sequenceSetPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_sequenceSetPage.ShowWindow(SW_SHOW);
	}
}
HBRUSH ResetSequenceDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

END_CUTLEADER_NAMESPACE()

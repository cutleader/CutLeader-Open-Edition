#include "stdafx.h"
#include "ConfigNestParamDlg.h"

#include "baseConst.h"
#include "DBConnect.h"
#include "DataBaseManager.h"
#include "DateHelper.h"
#include "ClBaseDataResource.h"
#include "baseConst.h"

#include "ParamConfigLoader.h"
#include "ConfigItemLoader.h"
#include "ExpertLibItemLoader.h"
#include "NestConfigItem.h"
#include "ConfigItemWriter.h"
#include "ExpertLibItemWriter.h"
#include "ClExpertLibResource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

ConfigNestParamDlg::ConfigNestParamDlg(CWnd* pParent)
			: CLResizableDialog(ConfigNestParamDlg::IDD, pParent)
{
}

ConfigNestParamDlg::~ConfigNestParamDlg()
{
}

BEGIN_MESSAGE_MAP(ConfigNestParamDlg, CLResizableDialog)
	ON_WM_SIZE()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

void ConfigNestParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CLResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL ConfigNestParamDlg::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// layout the controls on the dialog.
	AddAnchor(IDC_STATIC_CHILDAREA, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	// 创建子对话框。
	m_nestingRulePage.Create(IDD_clExpertLibUI_Nest, this);
    PlaceSubDlg();

	// 显示子对话框数据。
    {
        DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
        NestConfigItemPtr pNestingRule = ConfigItemLoader::LoadNestConfigItem(pDBConnect, m_iParamConfigID);
        m_nestingRulePage.DisplayNestConfigItem(pNestingRule);
    }

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void ConfigNestParamDlg::OnSize(UINT nType, int cx, int cy)
{
	CLResizableDialog::OnSize(nType, cx, cy);

    // position the sub dialog.
    PlaceSubDlg();
}

void ConfigNestParamDlg::OnBnClickedOk()
{
    // save the modification.
    NestConfigItemPtr pNestingRule = m_nestingRulePage.GetNestConfigItem();
    if (pNestingRule->IsModified())
    {
        SaveParam();
    }

	CLResizableDialog::OnOK();
}

void ConfigNestParamDlg::OnBnClickedCancel()
{
    NestConfigItemPtr pNestingRule = m_nestingRulePage.GetNestConfigItem();
    if (pNestingRule->IsModified() && MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_NESTPARAM), NULL, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        SaveParam();
    }

	CLResizableDialog::OnCancel();
}

void ConfigNestParamDlg::Init(LONGLONG iParamConfigID)
{
    m_iParamConfigID = iParamConfigID;
}

void ConfigNestParamDlg::SaveParam()
{
    NestConfigItemPtr pNestingRule = m_nestingRulePage.GetNestConfigItem();

    DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
    pDBConnect->BeginTransaction();
    ConfigItemWriter::SaveNestConfigItem(pDBConnect, pNestingRule, FALSE);
    LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pNestingRule->GetParentID());
    ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
    pDBConnect->CommitTransaction();
}

void ConfigNestParamDlg::PlaceSubDlg()
{
    CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILDAREA);
    if (pWndPos)
    {
        CRect pagePos;
        pWndPos->GetWindowRect(&pagePos);
        ScreenToClient(&pagePos);
        m_nestingRulePage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
        m_nestingRulePage.ShowWindow(SW_SHOW);
    }
}

END_CUTLEADER_NAMESPACE()

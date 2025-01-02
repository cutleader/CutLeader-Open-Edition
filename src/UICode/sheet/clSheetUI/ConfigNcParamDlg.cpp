#include "stdafx.h"
#include "ConfigNcParamDlg.h"

#include "baseConst.h"
#include "DBConnect.h"
#include "DataBaseManager.h"
#include "DateHelper.h"
#include "clBaseDataResource.h"

#include "ParamConfigLoader.h"
#include "ConfigItemLoader.h"
#include "ExpertLibItemLoader.h"
#include "NCConfigItem.h"
#include "ConfigItemWriter.h"
#include "ExpertLibItemWriter.h"
#include "clExpertLibResource.h"
#include "MachineItemLoader.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

ConfigNcParamDlg::ConfigNcParamDlg(CWnd* pParent)
			: CLResizableDialog(ConfigNcParamDlg::IDD, pParent)
{
}

ConfigNcParamDlg::~ConfigNcParamDlg()
{
}

BEGIN_MESSAGE_MAP(ConfigNcParamDlg, CResizableDialog)
	ON_WM_SIZE()
    ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

void ConfigNcParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL ConfigNcParamDlg::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// layout the controls on the dialog.
	AddAnchor(IDC_STATIC_CHILDAREA, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	// 创建子对话框。
	m_ncConfigPage.Create(IDD_clExpertLibUI_NC, this);
    PlaceSubDlg();

	// 显示子对话框数据。
    {
        DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
        DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
        LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, m_iParamConfigID);
        LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pDBConnect, iExpLibItemID);
        NCConfigTypeListPtr pNCConfigTypeList = MachineItemLoader::LoadMacNCConfigType(pMacLibDBConnect, iMacItemID);
        NCConfigItemPtr pNCConfigItem = ConfigItemLoader::LoadNCConfigItem(pDBConnect, m_iParamConfigID, pNCConfigTypeList);
        m_ncConfigPage.DisplayNCConfig(pNCConfigItem);
    }

    SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void ConfigNcParamDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

    // position the sub dialog.
    PlaceSubDlg();
}

HBRUSH ConfigNcParamDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void ConfigNcParamDlg::OnBnClickedOk()
{
    // save the modification.
    NCConfigItemPtr pNCConfigItem = m_ncConfigPage.GetNCConfig();
    if (pNCConfigItem->IsModified())
    {
        SaveParam();
    }

	CResizableDialog::OnOK();
}

void ConfigNcParamDlg::OnBnClickedCancel()
{
    NCConfigItemPtr pNCConfigItem = m_ncConfigPage.GetNCConfig();
    if (pNCConfigItem->IsModified() && MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_NC_CONFIG), NULL, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        SaveParam();
    }

	CResizableDialog::OnCancel();
}

void ConfigNcParamDlg::Init(LONGLONG iParamConfigID)
{
    m_iParamConfigID = iParamConfigID;
}

void ConfigNcParamDlg::SaveParam()
{
    NCConfigItemPtr pNCConfigItem = m_ncConfigPage.GetNCConfig();

    DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
    pDBConnect->BeginTransaction();
    ConfigItemWriter::SaveNCConfigItem(pDBConnect, pNCConfigItem, FALSE);
    LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pNCConfigItem->GetParentID());
    ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
    pDBConnect->CommitTransaction();
}

void ConfigNcParamDlg::PlaceSubDlg()
{
    CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILDAREA);
    if (pWndPos)
    {
        CRect pagePos;
        pWndPos->GetWindowRect(&pagePos);
        ScreenToClient(&pagePos);
        m_ncConfigPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
        m_ncConfigPage.ShowWindow(SW_SHOW);
    }
}

END_CUTLEADER_NAMESPACE()

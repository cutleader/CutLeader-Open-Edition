#include "StdAfx.h"
#include "CamConfigSelectDlg.h"

#include "baseConst.h"
#include "IntegerList.h"
#include "DataBaseManager.h"
#include "ClBaseDataResource.h"
#include "LogMgr.h"

#include "IExpertLibNode.h"
#include "MachineItemLoader.h"
#include "ExpertLibItemLoader.h"
#include "ParamConfigLoader.h"
#include "ClExpertLibResource.h"
#include "ProductParam.h"
#include "CLOptionInfo.h"


using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CamConfigSelectDlg, CDialogEx)

CamConfigSelectDlg::CamConfigSelectDlg()
				: CLDialog(CamConfigSelectDlg::IDD)
{
    m_iParamConfigID = -1;
}

CamConfigSelectDlg::~CamConfigSelectDlg(void)
{
}

void CamConfigSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_TREE_EXPLIB, m_expLibTree);

	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CamConfigSelectDlg, CDialogEx)
    ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CamConfigSelectDlg::OnInitDialog()
{
	__super::OnInitDialog();

    // 初始化专家库树。
    {
        // 是否内部使用。
        ProductParamPtr pProductParam = CLOptionInfo::GetInstance()->GetProductParam();
        BOOL bInternal = pProductParam->IsUseInside();

        DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
        ExpLibNodePtr pExpLibRootNode;
        if (bInternal)
        {
            pExpLibRootNode = ExpertLibItemLoader::LoadExpLibTree(pExpLibDBCon, FALSE, FALSE);
        }
        else
        {
            pExpLibRootNode = ExpertLibItemLoader::LoadExpLibTree(pExpLibDBCon);
        }
        m_expLibTree.Init(pExpLibRootNode, m_iParamConfigID);
    }

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

HBRUSH CamConfigSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void CamConfigSelectDlg::OnBnClickedOk()
{
    HTREEITEM hItem = m_expLibTree.GetFirstSelectedItem();
    IExpertLibNode* pNodeData = (IExpertLibNode*)m_expLibTree.GetItemData(hItem);
    if (pNodeData->GetNodeType() == EXPLIB_NODE_EXPLIBITEM)
    {
        CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
        CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SelectParamConfigSubNode);
        MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
        return;
    }
    else if (pNodeData->GetNodeType() == EXPLIB_NODE_CONFIGURATION)
    {
        m_iParamConfigID = pNodeData->GetDataID();
    }
    else
    {
        LogMgr::DebugWarn(_T("18776"));
    }
    
	OnOK();
}

END_CUTLEADER_NAMESPACE()

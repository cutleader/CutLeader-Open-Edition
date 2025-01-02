#include "stdafx.h"
#include "TaskInfoDlg.h"

#include "baseConst.h"
#include "baseConst.h"
#include "DataBaseManager.h"
#include "ClBaseDataResource.h"
#include "LogMgr.h"
#include "StringUtil.h"

#include "CamConfigSelectDlg.h"
#include "ParamConfigLoader.h"
#include "ParamConfig.h"
#include "ParamConfigLoader.h"
#include "DefaultParamConfig.h"
#include "ExpertLibItemLoader.h"
#include "Material.h"
#include "MaterialThickness.h"
#include "IMaterialSize.h"
#include "MaterialLibLoader.h"
#include "MaterialSizeSelectDlg.h"
#include "ClMaterialLibResource.h"

#include "Sheet.h"
#include "SheetList.h"
#include "ClSheetResource.h"
#include "SheetManager.h"
#include "SheetProcessor.h"
#include "TaskItem.h"
#include "ClTaskResource.h"
#include "clCanvasGeometryUIResource.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(TaskInfoDlg, CDialogEx)

BEGIN_MESSAGE_MAP(TaskInfoDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SelectCamParam, OnSelectCamParam)
	ON_BN_CLICKED(IDC_BTN_SEL_MAT, OnSelectMat)
END_MESSAGE_MAP()

void TaskInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_NAME, m_strTaskName);

	// Material Info group.
	DDX_Text(pDX, IDC_EDIT_MAT_TYPE, m_strMat);
	DDX_Text(pDX, IDC_EDIT_MAT_NAME, m_strMatThick);
	DDX_Text(pDX, IDC_EDIT_MAT_SIZE, m_strMatSize);

	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

TaskInfoDlg::TaskInfoDlg(CWnd* pParent /*=NULL*/)
				: CLDialog(TaskInfoDlg::IDD, pParent)
{
	m_iParamConfigID = INVALID_ID;
	m_iMatSizeID = INVALID_ID;
}

TaskInfoDlg::~TaskInfoDlg()
{
}

BOOL TaskInfoDlg::OnInitDialog()
{
	__super::OnInitDialog();

	m_strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEW_TASK);

	// 默认材料
    {
        DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
        m_iMatSizeID = MaterialLibLoader::GetDflMatSize(pMatLibDBCon);
        DataProperty dataProp;
        MaterialLibLoader::GetMatSizeProp(pMatLibDBCon, m_iMatSizeID, dataProp);
        m_strMat = dataProp.GetItem(0).GetValue().bstrVal;
        m_strMatThick.Format(FLOAT_FORMAT_4, dataProp.GetItem(1).GetValue().dblVal);
        m_strMatSize = dataProp.GetItem(6).GetValue().bstrVal;
    }

    // 加工参数
    {
        DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
        DefaultParamConfigPtr pDefaultParamConfig = ParamConfigLoader::LoadDflParamConfig(pExpLibDBCon);
        m_iParamConfigID = pDefaultParamConfig->GetParamConfigID();
        DataProperty expLibProp;
        if (ParamConfigLoader::GetParamConfigInfo(pExpLibDBCon, m_iParamConfigID, expLibProp))
        {
            CString str = StringUtil::ConnectStrings(expLibProp.GetItem(2).GetValue().bstrVal, _T("("), expLibProp.GetItem(3).GetValue().bstrVal, _T(")"));
            GetDlgItem(IDC_EDIT_CamParam)->SetWindowText(str);
        }
        else
        {
            LogMgr::DebugWarn(_T("98986"));
        }
    }

	UpdateData(FALSE);

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	SetWindowTheme(GetDlgItem(IDC_STATIC_Material)->GetSafeHwnd(), L"", L"");

	return TRUE;
}

HBRUSH TaskInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void TaskInfoDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	// check the available material count.
	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
	IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(pMatLibDBCon, m_iMatSizeID);
	if (pMaterialSize->GetAvailNum() < 1)
	{
		CString strProductName, strMsgContent;
		strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAT_SIZE_NONE);
		MessageBox(strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
		return;
	}

	OnOK();
}

void TaskInfoDlg::OnSelectCamParam()
{
    // 更换板材的加工参数。
	CamConfigSelectDlg dlg;
	dlg.InitParamConfigID(m_iParamConfigID);
	if (dlg.DoModal() == IDOK)
	{
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		DataProperty expLibProp;
		if (ParamConfigLoader::GetParamConfigInfo(pExpLibDBCon, dlg.GetParamConfigID(), expLibProp))
		{
			CString str = StringUtil::ConnectStrings(expLibProp.GetItem(2).GetValue().bstrVal, _T("("), expLibProp.GetItem(3).GetValue().bstrVal, _T(")"));
			GetDlgItem(IDC_EDIT_CamParam)->SetWindowText(str);
			m_iParamConfigID = expLibProp.GetItem(0).GetValue().llVal;
		}
		else
		{
			LogMgr::DebugWarn(_T("47124"));
		}
	}
}

void TaskInfoDlg::OnSelectMat()
{
	MaterialSizeSelectDlg matSizeSelDlg;
	if (matSizeSelDlg.DoModal() == IDOK)
	{
		IMaterialSizePtr pMaterialSize = matSizeSelDlg.GetSelMatSize();
		m_iMatSizeID = pMaterialSize->GetID();

		DataProperty dataProp;
		DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
		MaterialLibLoader::GetMatSizeProp(pMatLibDBCon, m_iMatSizeID, dataProp);
		m_strMat = dataProp.GetItem(0).GetValue().bstrVal;
		m_strMatThick.Format(FLOAT_FORMAT_4, dataProp.GetItem(1).GetValue().dblVal);
		m_strMatSize = pMaterialSize->GetName();

		UpdateData(FALSE);
	}
}

TaskItemPtr TaskInfoDlg::GetTaskItem()
{
    TaskItemPtr pTaskItem(new TaskItem);
    pTaskItem->SetName(m_strTaskName);

	// 创建底图板材。
	{
		DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
		LONGLONG iMaterialThicknessID = MaterialLibLoader::GetMatThickID(pMatLibDBCon, m_iMatSizeID);
		LONGLONG iCanvasMaterialSizeID = MaterialLibLoader::GetCanvasMaterialSizeID(pMatLibDBCon, iMaterialThicknessID);
		CString strCanvasSheetName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Canvas);
		SheetPtr pSheet = SheetManager::CreateCanvasSheet(strCanvasSheetName, iCanvasMaterialSizeID, m_iParamConfigID);
		SheetProcessor::UpdateSheetCache(pSheet);
		pTaskItem->GetSheetList()->push_back(pSheet);
	}

	// 创建初始板材。
	{
		CString strInitialSheetName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SHEET);
		strInitialSheetName += _T("-1");
		SheetPtr pSheet = SheetManager::CreateNormalSheet(strInitialSheetName, m_iMatSizeID, m_iParamConfigID);
		SheetProcessor::UpdateSheetCache(pSheet);
		pTaskItem->GetSheetList()->push_back(pSheet);
	}

    return pTaskItem;
}

END_CUTLEADER_NAMESPACE()

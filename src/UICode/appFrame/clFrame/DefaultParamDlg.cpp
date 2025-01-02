#include "stdafx.h"
#include "DefaultParamDlg.h"

#include "baseConst.h"
#include "baseConst.h"
#include "DataBaseManager.h"
#include "ClBaseDataResource.h"
#include "LogMgr.h"
#include "StringUtil.h"
#include "ClFolderHelper.h"
#include "ProductFileDefine.h"

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
#include "CLOptionInfo.h"
#include "ImpExpConfig.h"
#include "CLOptionWriter.h"
#include "clReportResource.h"
#include "ReportTpl.h"
#include "ReportTplList.h"
#include "ReportTplLoader.h"
#include "MaterialLibWriter.h"
#include "ParamConfigWriter.h"


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(DefaultParamDlg, CDialogEx)

BEGIN_MESSAGE_MAP(DefaultParamDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SelectCamParam, OnSelectCamParam)
	ON_BN_CLICKED(IDC_BTN_SEL_MAT, OnSelectMat)
END_MESSAGE_MAP()

void DefaultParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_CamParam, m_strCamParam);

	// Material Info group.
	DDX_Text(pDX, IDC_EDIT_MAT_TYPE, m_strMat);
	DDX_Text(pDX, IDC_EDIT_MAT_NAME, m_strMatThick);
	DDX_Text(pDX, IDC_EDIT_MAT_SIZE, m_strMatSize);

	DDX_Control(pDX, IDC_COMBO_SHT_TPL, m_ctrlShtTpl);
	DDX_Control(pDX, IDC_COMBO_NEST_TPL, m_ctrlNestTpl);
	DDX_Control(pDX, IDC_COMBO_LABEL_TPL, m_ctrlLabelTpl);

	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

DefaultParamDlg::DefaultParamDlg(CWnd* pParent /*=NULL*/)
				: CLDialog(DefaultParamDlg::IDD, pParent)
{
	m_iParamConfigID = INVALID_ID;
	m_iMatSizeID = INVALID_ID;
}

DefaultParamDlg::~DefaultParamDlg()
{
}

BOOL DefaultParamDlg::OnInitDialog()
{
	__super::OnInitDialog();

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
			m_strCamParam = str;
        }
        else
        {
            LogMgr::DebugWarn(_T("98986"));
        }
    }

	// 报表模板相关。
	{
		// 默认报表模板。
		ImpExpConfigPtr pImpExpConfig = CLOptionInfo::GetInstance()->GetImpExpConfig();
		LONGLONG iShtRptTplID = pImpExpConfig->GetShtRptTplID();
		LONGLONG iNestRptTplID = pImpExpConfig->GetNestRptTplID();
		LONGLONG iLabelRptTplID = pImpExpConfig->GetLabelRptTplID();

		// 所有报表模板。
		ReportTplListPtr pAllReportTpls = ReportTplLoader::LoadAllReportTpl(FALSE);

		// 板材。
		ReportTplListPtr pShtReportTpls = pAllReportTpls->GetTplByType(SHEET_REPORT);
		if (pShtReportTpls->size() > 0)
		{
			int iSelIndex = 0;
			for (unsigned int i = 0; i < pShtReportTpls->size(); i++)
			{
				ReportTplPtr pReportTpl = pShtReportTpls->at(i);
				m_ctrlShtTpl.AddString(pReportTpl->GetName());
				m_ctrlShtTpl.SetItemData(i, (DWORD_PTR)(new LONGLONG(pReportTpl->GetID())));
				if (pReportTpl->GetID() == iShtRptTplID)
				{
					iSelIndex = i;
				}
			}
			m_ctrlShtTpl.SetCurSel(iSelIndex);
		}

		// 排版任务。
		ReportTplListPtr pNestReportTpls = pAllReportTpls->GetTplByType(TASK_REPORT);
		if (pNestReportTpls->size() > 0)
		{
			int iSelIndex = 0;
			for (unsigned int i = 0; i < pNestReportTpls->size(); i++)
			{
				ReportTplPtr pReportTpl = pNestReportTpls->at(i);
				m_ctrlNestTpl.AddString(pReportTpl->GetName());
				m_ctrlNestTpl.SetItemData(i, (DWORD_PTR)(new LONGLONG(pReportTpl->GetID())));
				if (pReportTpl->GetID() == iNestRptTplID)
				{
					iSelIndex = i;
				}
			}
			m_ctrlNestTpl.SetCurSel(iSelIndex);
		}

		// 标签。
		ReportTplListPtr pLabelReportTpls = pAllReportTpls->GetTplByType(LABEL_REPORT);
		if (pLabelReportTpls->size() > 0)
		{
			int iSelIndex = 0;
			for (unsigned int i = 0; i < pLabelReportTpls->size(); i++)
			{
				ReportTplPtr pReportTpl = pLabelReportTpls->at(i);
				m_ctrlLabelTpl.AddString(pReportTpl->GetName());
				m_ctrlLabelTpl.SetItemData(i, (DWORD_PTR)(new LONGLONG(pReportTpl->GetID())));
				if (pReportTpl->GetID() == iLabelRptTplID)
				{
					iSelIndex = i;
				}
			}
			m_ctrlLabelTpl.SetCurSel(iSelIndex);
		}
	}

	UpdateData(FALSE);

	SetBackgroundColor(SkinManager::GetBackgroundColor());

	SetWindowTheme(GetDlgItem(IDC_COMBO_SHT_TPL)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_COMBO_NEST_TPL)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_COMBO_LABEL_TPL)->GetSafeHwnd(), L"", L"");

	SetWindowTheme(GetDlgItem(IDC_STATIC_Material)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_STATIC_ReportTemplate)->GetSafeHwnd(), L"", L"");

	return TRUE;
}

HBRUSH DefaultParamDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void DefaultParamDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	// 保存默认的加工参数设置。
	{
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		DataProperty expLibProp;
		if (ParamConfigLoader::GetParamConfigInfo(pExpLibDBCon, m_iParamConfigID, expLibProp))
		{
			LONGLONG iExpLibItemID = expLibProp.GetItem(1).GetValue().llVal;
			DefaultParamConfigPtr pDflParamConfig(new DefaultParamConfig(iExpLibItemID, m_iParamConfigID));
			ParamConfigWriter::SaveDflParamConfig(pExpLibDBCon, pDflParamConfig);
		}
		else
		{
			LogMgr::DebugWarn(_T("532144"));
			return;
		}
	}

	// 保存默认的材料设置。
	{
		DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
		MaterialLibWriter::SaveDflMatSize(pMatLibDBCon, m_iMatSizeID);
	}

	// 保存默认的报表模板设置。
	{
		CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
		ImpExpConfigPtr pImpExpConfig = pCLOptionInfo->GetImpExpConfig();

		// 得到新的默认模板。
		int iIndex = m_ctrlShtTpl.GetCurSel();
		if (iIndex >= 0)
		{
			LONGLONG* pTplID = (LONGLONG*)m_ctrlShtTpl.GetItemData(iIndex);
			pImpExpConfig->SetShtRptTplID(*pTplID);
		}
		iIndex = m_ctrlNestTpl.GetCurSel();
		if (iIndex >= 0)
		{
			LONGLONG* pTplID = (LONGLONG*)m_ctrlNestTpl.GetItemData(iIndex);
			pImpExpConfig->SetNestRptTplID(*pTplID);
		}
		iIndex = m_ctrlLabelTpl.GetCurSel();
		if (iIndex >= 0)
		{
			LONGLONG* pTplID = (LONGLONG*)m_ctrlLabelTpl.GetItemData(iIndex);
			pImpExpConfig->SetLabelRptTplID(*pTplID);
		}

		// 保存。
		CString strPath = ClFolderHelper::GetOptionPath();
		strPath += USER_OPTION_FILE;
		CLOptionWriter::SaveCLOption(pCLOptionInfo, strPath);
	}

	OnOK();
}

void DefaultParamDlg::OnSelectCamParam()
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

void DefaultParamDlg::OnSelectMat()
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

END_CUTLEADER_NAMESPACE()

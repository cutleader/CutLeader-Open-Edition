#include "StdAfx.h"
#include "CamDataAddDlg.h"

#include "baseConst.h"
#include "IntegerList.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"

#include "clPartResource.h"
#include "PartManager.h"

#include "MachineItemLoader.h"
#include "clExpertLibResource.h"
#include "ExpertLibItemLoader.h"
#include "ParamConfigLoader.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CamDataAddDlg, CDialogEx)

CamDataAddDlg::CamDataAddDlg()
				: CLDialog(CamDataAddDlg::IDD)
{
	m_iPartID = INVALID_ID;
	m_strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SCHEME_GEN_CAMDATA);
}

CamDataAddDlg::~CamDataAddDlg(void)
{
}

void CamDataAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MSG, m_strMsg);
	DDX_Control(pDX, IDC_COMBO_EXPLIB_ITEM, m_expLibItemCBX);
	DDX_Control(pDX, IDC_COMBO_CONFIG, m_partSchemeCBX);
	DDX_Control( pDX, IDOK, m_btnOK );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(CamDataAddDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_EXPLIB_ITEM, OnSelchangeExpLibItem)

	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CamDataAddDlg::OnDestroy()
{
	RlsExpLibItemData();
	RlsPartSchemeItemData();
}

HBRUSH CamDataAddDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

BOOL CamDataAddDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// set style
	m_expLibItemCBX.SetColumnCount(1);
	m_partSchemeCBX.SetColumnCount(1);

	// init the dialog
	Init();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_CONFIG )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_EXPLIB_ITEM )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_CONFIG )->GetSafeHwnd(), L"", L"" );
	
	return TRUE;
}

void CamDataAddDlg::OnSelchangeExpLibItem()
{
	int index = m_expLibItemCBX.GetCurSel();
	if (index != -1)
	{
		InitParamConfigCBX();
	}
}

void CamDataAddDlg::OnBnClickedOk()
{
	CString strProductName, str;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// 得到选中的“专家库项”。
	int iIndex = m_expLibItemCBX.GetCurSel();
	if (iIndex == -1)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SEL_EXPLIB_ITEM);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		LONGLONG* pData = (LONGLONG*)m_expLibItemCBX.GetItemData(iIndex);
		m_iExpLibItemID = *pData;
	}

	// 得到选中的“参数配置”。
	iIndex = m_partSchemeCBX.GetCurSel();
	if (iIndex == -1)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SEL_PARAM_CONFIG);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		LONGLONG* pData = (LONGLONG*)m_partSchemeCBX.GetItemData(iIndex);
		m_iParamConfigID = *pData;
	}

	// 检查零件的某套“工艺数据”是否已经用了这个“参数配置”。
	LONGLONG iTmpCamDataID, iTmpExpLibItemID;
	if (PartManager::HasCamDatasetA(m_iPartID, m_iParamConfigID, iTmpCamDataID, iTmpExpLibItemID))
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CONFIG_PARAM_USED);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	OnOK();
}

void CamDataAddDlg::InitParamConfigCBX()
{
	// at first, clear the combo box.
	m_partSchemeCBX.ResetContent();
	RlsPartSchemeItemData();
	m_paramConfigList.clear();

	// 选中的“专家库项”。
	int iIndex = m_expLibItemCBX.GetCurSel();
	LONGLONG* pExpLibItemID = (LONGLONG*)m_expLibItemCBX.GetItemData(iIndex);

	// 得到“专家库项”下所有“参数配置”信息。
	DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	m_paramConfigList = ParamConfigLoader::GetParamConfigList(pDBConnect, *pExpLibItemID);
	for (unsigned int i = 0; i < m_paramConfigList.size(); i++)
	{
		DataProperty prop = m_paramConfigList[i];

		// the 参数配置名称.
		DataPropItem propItem = prop.GetItem(2);
		const CComVariant& v = propItem.GetValue();
		m_partSchemeCBX.AddString((LPCTSTR)v.bstrVal);

		// set the item data.
		LONGLONG iParamConfigID = prop.GetItem(0).GetValue().llVal;
		m_partSchemeCBX.SetItemData(i, (DWORD_PTR)(new LONGLONG(iParamConfigID)));

		// whether set it to text color.
		LONGLONG iCamDataID, iExpLibItemID;
		if (PartManager::HasCamDatasetA(m_iPartID, iParamConfigID, iCamDataID, iExpLibItemID))
			m_partSchemeCBX.SetItemColor(i, SkinManager::GetTextColor() );
	}
}

void CamDataAddDlg::Init()
{
	DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	m_expLibItemList = ExpertLibItemLoader::GetExpLibItemInfoList(pDBConnect);

	// 可见的机器。
	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	IntegerListPtr pVisibleMacList = MachineItemLoader::GetVisibleMacList(pMacLibDBConnect);

	// 显示所有“专家库项”。
	for (unsigned int i = 0; i < m_expLibItemList.size(); i++)
	{
		DataProperty prop = m_expLibItemList[i];

		// 检查对应的机器是否可见。
		if (!pVisibleMacList->Include(prop.GetItem(3).GetValue().llVal))
			continue;

		m_expLibItemCBX.AddString((LPCTSTR)prop.GetItem(1).GetValue().bstrVal);
		LONGLONG iExpLibItemID = prop.GetItem(0).GetValue().llVal;
		m_expLibItemCBX.SetItemData(m_expLibItemCBX.GetCount()-1, (DWORD_PTR)(new LONGLONG(iExpLibItemID)));

		// whether set it to text color.
		for (int n = 0; n < m_expLibItemCBX.GetCount(); n++) {
			m_expLibItemCBX.SetItemColor( 0, SkinManager::GetTextColor() );
		}
	}
}

void CamDataAddDlg::RlsExpLibItemData()
{
	for (int i = 0; i < m_expLibItemCBX.GetCount(); i++)
	{
		DWORD_PTR pItemData = m_expLibItemCBX.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}
}

void CamDataAddDlg::RlsPartSchemeItemData()
{
	for (int i = 0; i < m_partSchemeCBX.GetCount(); i++)
	{
		DWORD_PTR pItemData = m_partSchemeCBX.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}
}

END_CUTLEADER_NAMESPACE()

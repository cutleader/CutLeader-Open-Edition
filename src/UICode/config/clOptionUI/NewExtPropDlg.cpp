#include "stdafx.h"
#include "NewExtPropDlg.h"

#include "baseConst.h"
#include "clUILibResource.h"
#include "clBaseDataResource.h"
#include "ExtPropType.h"
#include "ExtPropTypeList.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(NewExtPropDlg, CDialogEx)

NewExtPropDlg::NewExtPropDlg()
						 : CLDialog(NewExtPropDlg::IDD)
{
	m_strName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEW_EXT_PROP);
}

NewExtPropDlg::~NewExtPropDlg()
{
}

void NewExtPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_PROP_NAME, m_strName);
	DDX_Control(pDX, IDC_COMBO_DATA_TYPE, m_ctrlDataType);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(NewExtPropDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL NewExtPropDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// 初始化数据类型。
	StringListPtr pDataTypes = ExtPropType::GetAllDataType();
	for (unsigned int i = 0; i < pDataTypes->size(); i++)
		m_ctrlDataType.InsertString(i, pDataTypes->at(i));
	m_ctrlDataType.SetCurSel(0);

	UpdateData(FALSE);
	
	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_COMBO_DATA_TYPE )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

HBRUSH NewExtPropDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void NewExtPropDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	// 先检查有无同名的扩展属性。
	if (m_pExtPropTypes->GetItemByName(m_strName))
	{
		CString strProductName, str;
		strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXT_PROP_SAME_NAME);
		MessageBox(str, strProductName, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		m_pExtPropType.reset(new ExtPropType(m_strName, (BASIC_DATA_TYPE)m_ctrlDataType.GetCurSel()));
		OnOK();
	}
}

END_CUTLEADER_NAMESPACE()

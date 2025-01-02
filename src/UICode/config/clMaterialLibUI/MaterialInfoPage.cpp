#include "stdafx.h"
#include "MaterialInfoPage.h"

#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"
#include "DateHelper.h"
#include "clUtilityResource.h"
#include "baseConst.h"
#include "baseConst.h"

#include "clMaterialLibResource.h"
#include "Material.h"
#include "MaterialThickness.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

const size_t MAX_TIP_TEXT_LENGTH = 1024;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(MaterialInfoPage, CResizableDialog)

MaterialInfoPage::MaterialInfoPage() : CLResizableDialog(MaterialInfoPage::IDD)
{
}

MaterialInfoPage::~MaterialInfoPage()
{
}

void MaterialInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MATTYPE_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_MATTYPE_COMMENT, m_strComment);
	DDX_Control(pDX, IDC_EDIT_GRAVITY, m_dGravity);
	DDX_Text(pDX, IDC_EDIT_CREATE_DATE, m_strCreateDate);
	DDX_Text(pDX, IDC_EDIT_MODIFY_DATE, m_strModifyDate);

	DDX_Control(pDX, IDC_LIST_MATNAME, m_ctrlMatNameList);

	DDX_Control(pDX, IDC_EDIT_DUMMY, m_dDummy);
}

BEGIN_MESSAGE_MAP(MaterialInfoPage, CResizableDialog)
	ON_WM_CTLCOLOR()

	ON_EN_KILLFOCUS(IDC_EDIT_MATTYPE_NAME, OnKillName)
	ON_EN_KILLFOCUS(IDC_EDIT_MATTYPE_COMMENT, OnKillComment)
	ON_EN_KILLFOCUS(IDC_EDIT_GRAVITY, OnKillGravity)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MATNAME, OnDblclkList)
END_MESSAGE_MAP()

BOOL MaterialInfoPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	/************************************************************************/
	// layout controls.

	// "basic info" group
	AddAnchor(IDC_STATIC_BASE_INFO, TOP_LEFT, TOP_RIGHT);
	// name
	AddAnchor(IDC_EDIT_MATTYPE_NAME, TOP_LEFT);
	AddAnchor(IDC_STATIC_NAME, TOP_LEFT);
	// gravity
	AddAnchor(IDC_EDIT_GRAVITY, TOP_LEFT);
	AddAnchor(IDC_STATIC_GRAVITY, TOP_LEFT);
	// comment
	AddAnchor(IDC_EDIT_MATTYPE_COMMENT, TOP_LEFT);
	AddAnchor(IDC_STATIC_COMMENT, TOP_LEFT);
	// date
	AddAnchor(IDC_STATIC_CREATE_DATE, TOP_LEFT);
	AddAnchor(IDC_EDIT_CREATE_DATE, TOP_LEFT);
	AddAnchor(IDC_STATIC_MODIFY_DATE, TOP_LEFT);
	AddAnchor(IDC_EDIT_MODIFY_DATE, TOP_LEFT);

	// "matName list" group
	AddAnchor(IDC_STATIC_MATNAME, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_LIST_MATNAME, TOP_LEFT, BOTTOM_RIGHT);
	/************************************************************************/

	InitGridStyle();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_BASE_INFO )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_MATNAME )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}


HBRUSH MaterialInfoPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void MaterialInfoPage::OnKillName()
{
	UpdateData(TRUE);

	if (m_matProp.GetItem(1).GetValue().bstrVal != m_strName)
	{
		m_matProp.SetItem(1, DataPropItem(CComVariant(m_strName.AllocSysString())));
		m_bModified = TRUE;
	}
}

void MaterialInfoPage::OnKillComment()
{
	UpdateData(TRUE);

	if (m_matProp.GetItem(3).GetValue().bstrVal != m_strComment)
	{
		m_matProp.SetItem(3, DataPropItem(CComVariant(m_strComment.AllocSysString())));
		m_bModified = TRUE;
	}
}

void MaterialInfoPage::OnKillGravity()
{
	UpdateData(TRUE);

	if (m_matProp.GetItem(2).GetValue().dblVal != m_dGravity.Get_Number())
	{
		m_matProp.SetItem(2, DataPropItem(CComVariant(m_dGravity.Get_Number())));
		m_bModified = TRUE;
	}
}

void MaterialInfoPage::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
}

void MaterialInfoPage::DisplayMatType(DataProperty matProp, vector<DataProperty> matThickProps)
{
	m_matProp = matProp;
	m_bModified = FALSE;

	// 材料的信息。
	m_strName = matProp.GetItem(1).GetValue().bstrVal;
	m_strComment = matProp.GetItem(3).GetValue().bstrVal;
	m_dGravity.Init(matProp.GetItem(2).GetValue().dblVal, DIGITAL_NUM, FALSE);
	m_strCreateDate = DateHelper::GetStrFromTimeStamp(matProp.GetItem(4).GetValue().llVal);
	m_strModifyDate = DateHelper::GetStrFromTimeStamp(matProp.GetItem(5).GetValue().llVal);
	UpdateData(FALSE);

	// 这个材料下的“材料厚度”。
	m_ctrlMatNameList.DeleteAllItems();
	for (unsigned int i = 0; i < matThickProps.size(); i++)
	{
		DataProperty dataProp = matThickProps.at(i);

		// insert a new row.
		m_ctrlMatNameList.InsertItem(i, _T(""));
		m_ctrlMatNameList.SetItemData(i, (DWORD_PTR)(new LONGLONG(matProp.GetItem(0).GetValue().llVal)));

		// "num" column
		CString strNum;
		strNum.Format(INT_FORMAT, i+1);
		m_ctrlMatNameList.SetItemText(i, 1, strNum);

		// the thickness.
		CString strThickness;
		strThickness.Format(FLOAT_FORMAT_4, dataProp.GetItem(3).GetValue().dblVal);
		m_ctrlMatNameList.SetItemText(i, 2, strThickness);

		// 备注
		m_ctrlMatNameList.SetItemText(i, 3, dataProp.GetItem(4).GetValue().bstrVal);
	}

	m_dDummy.Init(0.001, DIGITAL_NUM, FALSE);
}

void MaterialInfoPage::InitGridStyle()
{
	// Requires one never uses column 0
	m_ctrlMatNameList.InsertHiddenLabelColumn();

	CString str;

	// "num" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
	CGridColumnTrait* pNUMCol = new CGridColumnTraitText;
	m_ctrlMatNameList.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNUMCol);

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAT_THICKNESS);
	CGridColumnTrait* pCommentCo3 = new CGridColumnTraitText;
	m_ctrlMatNameList.InsertColumnTrait(2, str, LVCFMT_LEFT, 140, 0, pCommentCo3);

	// "comment" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMENT);
	CGridColumnTrait* pCommentCol = new CGridColumnTraitText;
	m_ctrlMatNameList.InsertColumnTrait(3, str, LVCFMT_LEFT, 300, 0, pCommentCol);

	m_ctrlMatNameList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

END_CUTLEADER_NAMESPACE()

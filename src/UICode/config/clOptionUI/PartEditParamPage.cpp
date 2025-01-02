#include "stdafx.h"
#include "PartEditParamPage.h"

#include "baseConst.h"
#include "clUtilityResource.h"
#include "baseConst.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "ExtPropType.h"
#include "ExtPropTypeList.h"
#include "Text2ContourHelper.h"
#include "clBaseDataResource.h"

#include "PartEditParam.h"
#include "NewExtPropDlg.h"
#include "clOptionResource.h"
#include "TextDefaultFontDlg.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const size_t MAX_TIP_TEXT_LENGTH = 1024;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(PartEditParamPage, CResizableDialog)

PartEditParamPage::PartEditParamPage() : CLResizableDialog( PartEditParamPage::IDD )
{
}

PartEditParamPage::~PartEditParamPage()
{
}

void PartEditParamPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_DefaultFontName, m_strDefaultFontName);
	DDX_Text(pDX, IDC_EDIT_DefaultFontFileName, m_strDefaultFontFileName);
	DDX_Control(pDX, IDC_BTN_ChangeDefaultFont, m_btnChange);

	DDX_Control(pDX, IDC_LIST_EXT_PROP, m_extPropList);
	DDX_Control( pDX, IDC_BTN_ADD, m_btnAdd );
	DDX_Control( pDX, IDC_BTN_DEL, m_btnDelete );
}

BEGIN_MESSAGE_MAP(PartEditParamPage, CResizableDialog)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_ChangeDefaultFont, OnChangeDefaultFont)

	ON_BN_CLICKED(IDC_BTN_ADD, AddExtProp)
	ON_BN_CLICKED(IDC_BTN_DEL, DelExtProp)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_EXT_PROP, OnEndLabelEdit)
END_MESSAGE_MAP()

BOOL PartEditParamPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	/************************************************************************/
	// layout controls.

	AddAnchor(IDC_STATIC_TextDefaultFont, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_DefaultFontName, TOP_LEFT);
	AddAnchor(IDC_EDIT_DefaultFontName, TOP_LEFT);
	AddAnchor(IDC_STATIC_DefaultFontFileName, TOP_LEFT);
	AddAnchor(IDC_EDIT_DefaultFontFileName, TOP_LEFT);
	AddAnchor(IDC_BTN_ChangeDefaultFont, TOP_LEFT);

	// 扩展属性。
	AddAnchor(IDC_STATIC_EXT_PROP, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_LIST_EXT_PROP, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_BTN_ADD, BOTTOM_RIGHT);
	AddAnchor(IDC_BTN_DEL, BOTTOM_RIGHT);
	/************************************************************************/

	/************************************************************************/
	// 初始化列表风格。

	CString str;

	// Requires one never uses column 0
	m_extPropList.InsertHiddenLabelColumn();

	// "num" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
	CGridColumnTrait* pNumCol = new CGridColumnTraitText;
	m_extPropList.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNumCol);

	// "name" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
	CGridColumnTrait* pNameCol = new CGridColumnTraitText;
	m_extPropList.InsertColumnTrait(2, str, LVCFMT_LEFT, 90, 0, pNameCol);

	// "data type" column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DATATYPE);
	CGridColumnTraitCombo* pTypeCol = new CGridColumnTraitCombo;
	pTypeCol->SetStyle(pTypeCol->GetStyle() | CBS_DROPDOWNLIST);
	StringListPtr pList2 = ExtPropType::GetAllDataType();
	for (unsigned int j = 0; j < pList2->size(); j++)
		pTypeCol->AddItem(j, pList2->at(j));
	m_extPropList.InsertColumnTrait(3, str, LVCFMT_LEFT, 130, 0, pTypeCol);

	m_extPropList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	/************************************************************************/

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	
	SetWindowTheme( GetDlgItem( IDC_EDIT_MSG )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_EXT_PROP )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

HBRUSH PartEditParamPage::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void PartEditParamPage::OnChangeDefaultFont()
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	TextDefaultFontDlg dlg;
	dlg.Init(m_pPartEditParam->GetDefaultFontName(), m_pPartEditParam->GetDefaultFontFileName());
	if (dlg.DoModal() == IDOK)
	{
		const CString& strFontName = dlg.GetFontName();
		const CString& strFontFileName = dlg.GetFontFileName();

		// 检查字体文件是否存在。
		bool bFontFileExist = true;
		CString strFontFilePath = Text2ContourHelper::GetFontFilePathByFontFileName_1(strFontFileName);
		CFileFind finder;
		if (!finder.FindFile(strFontFilePath))
			bFontFileExist = false;
		finder.Close();
		if (!bFontFileExist)
		{
			MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_FontFileNotExist), strProductName, MB_OK | MB_ICONWARNING);
			return;
		}

		if (strFontName != m_pPartEditParam->GetDefaultFontName())
		{
			m_pPartEditParam->SetDefaultFontName(strFontName);
			m_pPartEditParam->SetDefaultFontFileName(strFontFileName);
			m_pPartEditParam->SetModified(TRUE);

			m_strDefaultFontName = m_pPartEditParam->GetDefaultFontName();
			m_strDefaultFontFileName = m_pPartEditParam->GetDefaultFontFileName();
			UpdateData(FALSE);

			// 提示需要重启ProCAM。
			MessageBox(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CHANGE_NEEDTO_RESTART), strProductName, MB_ICONINFORMATION);
		}
	}
}

void PartEditParamPage::AddExtProp()
{
	NewExtPropDlg dlg;
	dlg.Init(m_pPartEditParam->GetExtPropTypeList());
	if (dlg.DoModal() == IDOK)
	{
		ExtPropTypePtr pExtPropType = dlg.GetExtPropType();
		m_pPartEditParam->GetExtPropTypeList()->push_back(pExtPropType);
		m_pPartEditParam->SetModified(TRUE);

		// 添加一行。
		int iRowCount = m_extPropList.GetItemCount();
		CString strNum;
		if ((iRowCount+1) < 10)
			strNum.Format(_T("0%d"), iRowCount+1);
		else
			strNum.Format(_T("%d"), iRowCount+1);
		iRowCount = m_extPropList.InsertItem(iRowCount, _T(""));
		m_extPropList.SetItemText(iRowCount, 1, strNum);
		m_extPropList.SetItemText(iRowCount, 2, pExtPropType->GetName());
		m_extPropList.SetItemText(iRowCount, 3, pExtPropType->GetDataTypeName());

		// 选中最后一行。
		iRowCount = m_extPropList.GetItemCount();
		for (int i = 0; i < iRowCount; i++)
		{
			if (m_extPropList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
				m_extPropList.SetItemState(i, 0, LVIS_SELECTED);
		}
		m_extPropList.EnsureVisible(iRowCount-1, FALSE);
		m_extPropList.SetItemState(iRowCount-1, LVIS_SELECTED, LVIS_SELECTED);
		m_extPropList.SetFocus();
	}
}

void PartEditParamPage::DelExtProp()
{
	for (int i = 0; i < m_extPropList.GetItemCount(); i++)
	{
		if (m_extPropList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			// delete the item.
			CString strName = m_extPropList.GetItemText(i, 2);
			m_pPartEditParam->GetExtPropTypeList()->DeleteItemByName(strName);
			m_pPartEditParam->SetModified(TRUE);
			m_extPropList.DeleteItem(i);
			i--;
		}
	}

	// re-index the first column.
	for (int i = 0; i < m_extPropList.GetItemCount(); i++)
	{
		CString strNum;
		if ((i+1) < 10)
			strNum.Format(_T("0%d"), i+1);
		else
			strNum.Format(_T("%d"), i+1);
		m_extPropList.SetItemText(i, 1, strNum);
	}

	// 选中最后一行。
	int iCount = m_extPropList.GetItemCount();
	if (iCount > 0)
	{
		m_extPropList.EnsureVisible(iCount-1, FALSE);
		m_extPropList.SetItemState(iCount-1, LVIS_SELECTED, LVIS_SELECTED);
		m_extPropList.SetFocus();
	}
}

void PartEditParamPage::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);

	// 得到扩展属性。
	int iItem = pDispInfo->item.iItem;
	CString strName = m_extPropList.GetItemText(iItem, 2);
	ExtPropTypePtr pExtPropType = m_pPartEditParam->GetExtPropTypeList()->GetItemByName(strName);

	// 修改。
	BOOL bModified = FALSE;
	int iSubitem = pDispInfo->item.iSubItem;
	CString str = pDispInfo->item.pszText;
	if (iSubitem == 3) // data type
	{
		if (pExtPropType->GetDataTypeName() != str)
		{
			pExtPropType->SetDataType(str);
			bModified = TRUE;
		}
	}
	if (bModified)
		m_pPartEditParam->SetModified(TRUE);
}

void PartEditParamPage::DisplayPartEditParam(PartEditParamPtr pPartEditParam)
{
	m_pPartEditParam = pPartEditParam;

	m_strDefaultFontName = m_pPartEditParam->GetDefaultFontName();
	m_strDefaultFontFileName = m_pPartEditParam->GetDefaultFontFileName();

	// 显示扩展属性。
	int nItem = 0;
	ExtPropTypeListPtr pExtPropTypes = m_pPartEditParam->GetExtPropTypeList();
	m_extPropList.DeleteAllItems();
	for (unsigned int i = 0; i < pExtPropTypes->size(); i++)
	{
		ExtPropTypePtr pExtPropType = pExtPropTypes->at(i);

		// init a row.
		CString strNum;
		if ((i+1) < 10)
			strNum.Format(_T("0%d"), i+1);
		else
			strNum.Format(_T("%d"), i+1);
		nItem = m_extPropList.InsertItem(++nItem, _T(""));
		m_extPropList.SetItemText(nItem, 1, strNum);
		m_extPropList.SetItemText(nItem, 2, pExtPropType->GetName());
		m_extPropList.SetItemText(nItem, 3, pExtPropType->GetDataTypeName());
	}

	UpdateData(FALSE);
}

PartEditParamPtr PartEditParamPage::GetPartEditParam()
{
	UpdateData(TRUE);

	if (m_pPartEditParam->GetDefaultFontName() != m_strDefaultFontName)
	{
		m_pPartEditParam->SetDefaultFontName(m_strDefaultFontName);
		m_pPartEditParam->SetModified(TRUE);
	}

	if (m_pPartEditParam->GetDefaultFontFileName() != m_strDefaultFontFileName)
	{
		m_pPartEditParam->SetDefaultFontFileName(m_strDefaultFontFileName);
		m_pPartEditParam->SetModified(TRUE);
	}

	return m_pPartEditParam;
}

END_CUTLEADER_NAMESPACE()

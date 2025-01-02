#include "StdAfx.h"
#include "PatternPropertyBar.h"

#include "clUtilityResource.h"
#include "clUILibResource.h"
#include "DataProperty.h"
#include "NumberProp.h"
#include "IntegerList.h"
#include "CommandManager.h"
#include "clBaseDataResource.h"
#include "StringUtil.h"

#include "PatternPropertyEditCommand.h"
#include "IPattern.h"
#include "PatternList.h"
#include "clGeometryFeatureResource.h"
#include "SkinManager.h"
#include "TextStructList.h"
#include "clGeometryFeatureUIResource.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PatternPropertyBar* PatternPropertyBar::m_pPatPropBar = NULL;

PatternPropertyBar::PatternPropertyBar(void)
{
	m_pPatList.reset();
}

PatternPropertyBar::~PatternPropertyBar(void)
{
}

EditorData PatternPropertyBar::GetEditorData() const
{
    CString str;
    str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY);

    EditorData editorData(GetRelativeView(), str);
    return editorData;
}

// get the singleton object
PatternPropertyBar* PatternPropertyBar::GetInstance(CWnd* pParent)
{
	if(m_pPatPropBar == NULL)
	{
		m_pPatPropBar = new PatternPropertyBar();
		m_pPatPropBar->m_pMainWnd = pParent;

		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROPBAR_PATPROP);

		if (!m_pPatPropBar->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200),
								   TRUE, IDC_PROPBAR_PATPROP, WS_CHILD | WS_CLIPSIBLINGS | 
								   WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
			return NULL;
	}

	return m_pPatPropBar;
}

BEGIN_MESSAGE_MAP(PatternPropertyBar, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()

	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChange)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void PatternPropertyBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient, rectCombo;
	GetClientRect(rectClient);

	rectCombo.SetRectEmpty();
	int cyCmb = rectCombo.Size().cy;

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, 
							   rectClient.Width(), rectClient.Height() -cyCmb, 
							   SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetCustomColors( SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(), SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(),
		SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(), SkinManager::GetTextColor() );
}

int PatternPropertyBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
		return -1;

	AdjustLayout();

	return 0;
}

void PatternPropertyBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void PatternPropertyBar::InitPropGroup(int iGroupName, vector<DataPropItem>& propItems)
{
	CString strGroupName, propName;
	strGroupName = MultiLanguageMgr::GetInstance()->TranslateString(iGroupName);

	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(strGroupName);

	// go through each data prop item
	for (unsigned int i = 0; i < propItems.size(); i++)
	{
		CMFCPropertyGridProperty* pProp = NULL;

		//
		DataPropItem dataPropItem = propItems[i];
		propName = MultiLanguageMgr::GetInstance()->TranslateString(dataPropItem.GetItemName());

		// create CMFCPropertyGridProperty object.
		if (dataPropItem.IsOption())
		{
			StringListPtr optListPtr = dataPropItem.GetOptions();
			CString str = (*optListPtr)[dataPropItem.GetValue().lVal];
			pProp = new CMFCPropertyGridProperty(propName, str, _T(""));

			for (unsigned int i = 0; i < optListPtr->size(); i++)
				pProp->AddOption((*optListPtr)[i]);
			pProp->AllowEdit(FALSE);
		}
		else if (dataPropItem.IsNumber())
		{
			// this item is number-item
			pProp = new NumberProp(propName, dataPropItem.GetValue(), dataPropItem.GetFormat(), NULL, 0, 
								   dataPropItem.HasButton(), dataPropItem.CanNegative());

			// whether read only.
			if (dataPropItem.IsReadOnly())
				pProp->AllowEdit(FALSE);
		}
		else
		{
			pProp = new CMFCPropertyGridProperty(propName, dataPropItem.GetValue(), _T(""));

			// whether read only.
			if (dataPropItem.IsReadOnly())
				pProp->AllowEdit(FALSE);
		}

		// add the data prop item to the group.
		pGroup->AddSubItem(pProp);

		// keep track the prop info
		m_propInfo[(CObject*)pProp] = dataPropItem;
	}

	m_wndPropList.AddProperty(pGroup);
}

void PatternPropertyBar::DisplayPatProp(PatternLoopListPtr pPatternLoopList, PatternListPtr pPatList, LoopTopologyItemListPtr pLoopTopologyItems, TextStructListPtr pTextStructs)
{
	m_pPatternLoopList = pPatternLoopList;
	m_pPatList = pPatList;
	m_pLoopTopologyItems = pLoopTopologyItems;
	m_pTextStructs = pTextStructs;

	// reset something for the CMFCPropertyGridCtrl.
	m_wndPropList.RemoveAll();
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();

	if (m_pPatList->size() == 1)
	{
		// get the property of the pattern
		IPatternPtr pPattern = m_pPatList->at(0);
		DataProperty patProp = pPattern->GetProperty();

		//
		m_wndPropList.EnableHeaderCtrl(FALSE);
		m_wndPropList.MarkModifiedProperties();

		// init general category
		vector<DataPropItem> genPropItems;
		patProp.GetItemsByType(IDS_CATEGORY_GENERAL, genPropItems);
		InitPropGroup(IDS_CATEGORY_GENERAL, genPropItems);

		// init geometry category
		vector<DataPropItem> geomPropItems;
		patProp.GetItemsByType(IDS_CATEGORY_GEOMETRY, geomPropItems);
		InitPropGroup(IDS_CATEGORY_GEOMETRY, geomPropItems);
	}
	else
	{
		// init the header for CMFCPropertyGridCtrl.
		CString strCol1, strCol2;
		strCol1 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOM_TYPE);
		strCol2 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COUNT);
		m_wndPropList.EnableHeaderCtrl(TRUE, strCol1, strCol2);

		// get the pattern types within pattern list.
		IntegerListPtr pPatTypeList = m_pPatList->GetPatternTypeList();

		// init the CMFCPropertyGridCtrl.
		for (unsigned int i = 0; i < pPatTypeList->size(); i++)
		{
			PatternType emPatternType = (PatternType)pPatTypeList->at(i);

			// get the patterns for this type.
			PatternListPtr pTmpPatList = m_pPatList->GetPatternByType(emPatternType);

			/************************************************************************/
			// add a CMFCPropertyGridProperty item.

			// get the type name for the patterns in "pTmpPatList".
			DataProperty patProp = pTmpPatList->at(0)->GetProperty();
			DataPropItem dataPropItem = patProp.GetItemByName(IDS_GEOM_TYPE);
			const CComVariant& varGeomTypeName = dataPropItem.GetValue();
			CString strPatName = varGeomTypeName.bstrVal;

			// the count of the pattern which the type is "emPatternType".
			CString strPatCount;
			strPatCount.Format(_T("%d"), pTmpPatList->size());

			CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(strPatName, strPatCount);
			pItem->AllowEdit(FALSE);
			m_wndPropList.AddProperty(pItem);
			/************************************************************************/
		}

		// 显示文字个数。
		if (m_pTextStructs->size() > 0)
		{
			CString strPropName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Text);
			CString strPropValue = StringUtil::Integer2String(m_pTextStructs->size());
			CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(strPropName, strPropValue);
			pItem->AllowEdit(FALSE);
			m_wndPropList.AddProperty(pItem);
		}
	}

	if (!IsVisible())
		ShowPane(TRUE, FALSE, TRUE);
}

void PatternPropertyBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void PatternPropertyBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
}

LRESULT PatternPropertyBar::OnPropertyChange(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* prop = (CMFCPropertyGridProperty *)lParam;
	COleVariant var = prop->GetValue();

	DataPropItem item = m_propInfo[(CObject*)prop];
	if (item.GetItemName() == IDS_PROP_REPEAT_NUM || item.GetItemName() == IDS_PROP_REPEAT_NUM_X ||
		item.GetItemName() == IDS_PROP_REPEAT_NUM_Y)
	{
		if (var.intVal < 2)
		{
			CString str, strProductName;
			strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_REPEAT_NUM_SMALL);
			::MessageBox(m_pMainWnd->m_hWnd, str, strProductName, MB_OK | MB_ICONWARNING);

			// refresh pattern data.
			DisplayPatProp(m_pPatternLoopList, m_pPatList, m_pLoopTopologyItems, m_pTextStructs);

			return S_OK;
		}
	}

	// 修改图形属性。
	DataPropItem* pNewPropItem = new DataPropItem(item.GetItemName(), var);
	DataPropItem* pOldPropItem = new DataPropItem(item.GetItemName(), item.GetValue());
	ICommandPtr pCommand(new PatternPropertyEditCommand(GetEditorData(), m_pPatternLoopList, m_pLoopTopologyItems, m_pPatList->at(0), pNewPropItem, pOldPropItem));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	// 刷新界面。
	GetRelativeView()->Invalidate();

	return S_OK;
}

END_CUTLEADER_NAMESPACE()

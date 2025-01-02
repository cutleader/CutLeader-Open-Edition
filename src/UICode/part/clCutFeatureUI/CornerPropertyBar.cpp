#include "StdAfx.h"
#include "CornerPropertyBar.h"

#include "NumberProp.h"
#include "baseConst.h"
#include "CommandManager.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"
#include "clCutFeatureResource.h"
#include "LoopCornerData.h"
#include "ICorner.h"
#include "CornerList.h"
#include "LoopCutFeatureList.h"
#include "CornerModifyCommand.h"
#include "CornerManager.h"

#include "CornerConfigItem.h"
#include "CornerRangeInfo.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

CornerPropertyBar* CornerPropertyBar::m_pCornerPropBar = NULL;

CornerPropertyBar::CornerPropertyBar(void)
{
	m_bIgnore_OnPropertyChangeMsg = false;
}

CornerPropertyBar::~CornerPropertyBar(void)
{
}

CornerPropertyBar* CornerPropertyBar::GetInstance(CWnd* pParent)
{
	if (m_pCornerPropBar == NULL)
	{
		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROPBAR_CORNERPROP);

		m_pCornerPropBar = new CornerPropertyBar();
		m_pCornerPropBar->m_pMainWnd = pParent;
		if (!m_pCornerPropBar->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200), TRUE, IDC_PROPBAR_CORNERPROP,
									  WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
        {
			return NULL;
        }
	}

	return m_pCornerPropBar;
}

BEGIN_MESSAGE_MAP(CornerPropertyBar, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChange)
END_MESSAGE_MAP()

void CornerPropertyBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient, rectCombo;
	GetClientRect(rectClient);

	rectCombo.SetRectEmpty();
	int cyCmb = rectCombo.Size().cy;
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), rectClient.Height() - cyCmb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetCustomColors( SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(), SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(),
		SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(), SkinManager::GetTextColor() );
}

int CornerPropertyBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void CornerPropertyBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
}

void CornerPropertyBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

LRESULT CornerPropertyBar::OnPropertyChange(WPARAM wParam, LPARAM lParam)
{
	if (m_bIgnore_OnPropertyChangeMsg)
	{
		return S_OK;
	}

	CMFCPropertyGridProperty* prop = (CMFCPropertyGridProperty *)lParam;
	COleVariant var = prop->GetValue();

	CString strType = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_CORNER_TYPE);
	CString strValue = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_CORNER_VALUE);
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_bCreate) // 在创建模式下
	{
		if (prop->GetName() == strType) // 修改了角特征类型
        {
			m_iCornerType_4_create = CornerRangeInfo::GetCornerTypeByStr(var.bstrVal);

            // 刷新界面。
            m_bIgnore_OnPropertyChangeMsg = true;
            DisplayCornerData_4_create(m_iCornerType_4_create, m_dCornerValue_4_create);
            m_bIgnore_OnPropertyChangeMsg = false;
        }
        else if (prop->GetName() == strValue)
        {
            m_dCornerValue_4_create = var.dblVal;
        }
	}
	else // 在编辑模式下
	{
		if (prop->GetName() == strType) // 修改了角特征类型
		{
            // 角特征类型和值。
            CornerType iCornerType = CornerRangeInfo::GetCornerTypeByStr(var.bstrVal);

			// 获取角特征值。
            double dCornerValue = -1;
            if (CornerManager::WhetherHaveCornerValue(iCornerType))
            {
                CMFCPropertyGridProperty* pGroup = m_wndPropList.GetProperty(0);
                if (pGroup->GetSubItemsCount() == 1) // 修改前没有“值输入框”
                {
                    dCornerValue = m_pCornerConfigItem->GetDflCornerVal();
                }
                else
                {
                    COleVariant vVal = pGroup->GetSubItem(1)->GetValue();
                    dCornerValue = vVal.dblVal;
                }
            }

			// 创建新的角特征对象。
			CornerListPtr pNewCornerList(new CornerList);
			for (unsigned int i = 0; i < m_pCornerList->size(); i++)
			{
				const ICorner* pOldCorner = m_pCornerList->at(i).get();
				ICornerPtr pNewCorner = CornerManager::BuildCorner(pOldCorner->GetParentID(), iCornerType, dCornerValue,
																  pOldCorner->GetFirstPatternID(), pOldCorner->GetSecondPatternID());
				pNewCornerList->push_back(pNewCorner);
			}

			// 检查这些角特征的“角特征值”能不能被接受。
			if (CornerManager::WhetherHaveCornerValue(iCornerType) && !CornerManager::CheckCornerValue(m_pLoopCornerList.get(), pNewCornerList.get()))
			{
				CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_CORNER_WRONG_VAL);
				::MessageBox(m_pMainWnd->m_hWnd, str, strProductName, MB_OK | MB_ICONWARNING);

                // 要是切换类型前没有“值输入框”，这里需要更新属性列表控件以加上“值输入框”。
                if (m_wndPropList.GetProperty(0)->GetSubItemsCount() == 1)
                {
                    m_bIgnore_OnPropertyChangeMsg = true;

                    // 更新属性列表控件
                    {
                        m_wndPropList.RemoveAll();
                        CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_CORNERINFO));
                        AddTypeControl(pGroup, iCornerType);
                        _variant_t v(m_pCornerConfigItem->GetDflCornerVal(), VT_R8);
                        NumberProp* pValProp = new NumberProp(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_CORNER_VALUE), v, DIGITAL_NUM, NULL, 0, FALSE);
                        pGroup->AddSubItem(pValProp);
                        m_wndPropList.AddProperty(pGroup);
                    }

                    m_bIgnore_OnPropertyChangeMsg = false;
                }

				return S_OK;
			}

			// 修改角特征。
			ICommandPtr pCommand(new CornerModifyCommand(GetEditorData(), m_pLoopCornerList, m_pCornerList, pNewCornerList));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 更新当前编辑的角特征。
			m_pCornerList = pNewCornerList;
		}
		else if (prop->GetName() == strValue) // 修改了角特征值
		{
            // 此时的类型
            CMFCPropertyGridProperty* pGroup = m_wndPropList.GetProperty(0);
            COleVariant typeVal = pGroup->GetSubItem(0)->GetValue();
            CornerType iCornerType = CornerRangeInfo::GetCornerTypeByStr(typeVal.bstrVal);

			// 创建新的角特征对象。
			CornerListPtr pNewCornerList(new CornerList);
			for (unsigned int i = 0; i < m_pCornerList->size(); i++)
			{
				const ICorner* pOldCorner = m_pCornerList->at(i).get();
                ICornerPtr pNewCorner;
                if (pOldCorner->HasCornerValue())
                {
                    pNewCorner = CornerManager::BuildCorner_byChangeCornerValue(pOldCorner, var.dblVal);
                }
				else // 这时其实类型也被改了。
                {
                    pNewCorner = CornerManager::BuildCorner(pOldCorner->GetParentID(), iCornerType, var.dblVal,
                        pOldCorner->GetFirstPatternID(), pOldCorner->GetSecondPatternID());
                }
				pNewCornerList->push_back(pNewCorner);
			}

			// 检查这些角特征的“角特征值”能不能被接受。
			if (!CornerManager::CheckCornerValue(m_pLoopCornerList.get(), pNewCornerList.get()))
			{
				CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_CORNER_WRONG_VAL);
				::MessageBox(m_pMainWnd->m_hWnd, str, strProductName, MB_OK | MB_ICONWARNING);
				return S_OK;
			}

			// 修改角特征。
			ICommandPtr pCommand(new CornerModifyCommand(GetEditorData(), m_pLoopCornerList, m_pCornerList, pNewCornerList));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 更新当前编辑的角特征。
			m_pCornerList = pNewCornerList;
		}

		// 刷新界面。
		GetRelativeView()->Invalidate();
		m_bIgnore_OnPropertyChangeMsg = true;
		DisplayCornerData_4_edit(m_editorData, m_pLoopCornerList, m_pCornerList, m_pCornerConfigItem);
		m_bIgnore_OnPropertyChangeMsg = false;
	}

	return S_OK;
}

XGridProperty* CornerPropertyBar::AddTypeControl(CMFCPropertyGridProperty* pGroup, CornerType iCurrentCornerType)
{
    StringListPtr pOptList = CornerRangeInfo::GetAllCornerType(FALSE);
    CString str1 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_CORNER_TYPE);;
    XGridProperty* pTypeProp = new XGridProperty(str1, CornerRangeInfo::GetNameByType(iCurrentCornerType), _T(""));
    for (unsigned int i = 0; i < pOptList->size(); i++)
    {
        pTypeProp->AddOption((*pOptList)[i]);
    }
    pTypeProp->AllowEdit(FALSE);
    pGroup->AddSubItem(pTypeProp);

    return pTypeProp;
}

void CornerPropertyBar::DisplayCornerData_4_create(CornerType iCornerType_4_create, double dCornerValue_4_create)
{
	// init value.
	m_bCreate = TRUE;
    m_iCornerType_4_create = iCornerType_4_create;
    m_dCornerValue_4_create = dCornerValue_4_create;

	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CString strGroupName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_CORNERINFO);
	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(strGroupName);

	// 添加“类型”属性项。
    AddTypeControl(pGroup, iCornerType_4_create);

	// 添加“值”属性项。
    if (CornerManager::WhetherHaveCornerValue(iCornerType_4_create))
    {
        _variant_t v(dCornerValue_4_create, VT_R8);
        NumberProp* pValProp = new NumberProp(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_CORNER_VALUE), v, DIGITAL_NUM, NULL, 0, FALSE);
        pGroup->AddSubItem(pValProp);
    }

	m_wndPropList.AddProperty(pGroup);

	if (!IsVisible())
    {
		ShowPane(TRUE, FALSE, TRUE);
    }
}

void CornerPropertyBar::DisplayCornerData_4_edit(const EditorData& editorData, LoopCornerDataListPtr pLoopCornerList, CornerListPtr pCornerList,
                                                 CornerConfigItemPtr pCornerConfigItem)
{
	m_editorData = editorData;
	m_bCreate = FALSE;
	m_pLoopCornerList = pLoopCornerList;
	m_pCornerList = pCornerList;
    m_pCornerConfigItem = pCornerConfigItem;

	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	// 创建分组。
	CString strGroupName;
	strGroupName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_CORNERINFO);
	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(strGroupName);

	const ICorner* pCorner = m_pCornerList->at(0).get();

	// 添加“类型”属性项。
    XGridProperty* pTypeProp = AddTypeControl(pGroup, pCorner->GetCornerType());
	if (m_pCornerList->size() > 1 && !m_pCornerList->SameCornerType())
	{
		pTypeProp->HighlightText(TRUE);
	}

    // 添加“值”属性项。
    if (m_pCornerList->size() == 1) // 只显示一个角特征的信息
    {
        if (pCorner->HasCornerValue()) // 这个角特征有值。
        {
            _variant_t v(CornerManager::GetCornerValue(pCorner), VT_R8);
            NumberProp* pValProp = new NumberProp(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_CORNER_VALUE), v, DIGITAL_NUM, NULL, 0, FALSE);
            pGroup->AddSubItem(pValProp);
        }
    }
    else // 需要显示多个角特征的信息
    {
        if (m_pCornerList->SameCornerType()) // 这多个角特征是同样的类型
        {
            if (pCorner->HasCornerValue()) // 这些角特征都有值。
            {
                _variant_t v(CornerManager::GetCornerValue(pCorner), VT_R8);
                NumberProp* pValProp = new NumberProp(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_CORNER_VALUE), v, DIGITAL_NUM, NULL, 0, FALSE);
                if (!CornerManager::SameCornerValue(m_pCornerList.get())) // 值要是不全相同，则高亮值。
                {
                    pValProp->HighlightText(TRUE);
                }
                pGroup->AddSubItem(pValProp);
            }
        }
        else // 这多个角特征不是同一类型，值的位置就显示第一个角特征的值，要是第一个角特征没有值，那就不加值输入框。
        {
            if (pCorner->HasCornerValue()) // 第一个角特征有值。
            {
                _variant_t v(CornerManager::GetCornerValue(pCorner), VT_R8);
                NumberProp* pValProp = new NumberProp(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_CORNER_VALUE), v, DIGITAL_NUM, NULL, 0, FALSE);
                pValProp->Enable(FALSE);
                pGroup->AddSubItem(pValProp);
            }
        }
    }

	m_wndPropList.AddProperty(pGroup);

	if (!IsVisible())
    {
		ShowPane(TRUE, FALSE, TRUE);
    }
}

END_CUTLEADER_NAMESPACE()

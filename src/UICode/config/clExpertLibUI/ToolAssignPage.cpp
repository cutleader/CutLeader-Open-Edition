#include "stdafx.h"
#include "ToolAssignPage.h"

#include <math.h>
#include "baseConst.h"
#include "clUtilityResource.h"
#include "clBaseDataResource.h"
#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"
#include "clExpertLibResource.h"

#include "ToolManagementDlg.h"
#include "ToolAssignConfigItem.h"
#include "ToolAssignRule.h"
#include "ToolAssignRuleList.h"
#include "ToolInstance.h"
#include "ToolInstanceList.h"
#include "ToolDxfMapDlg.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ToolAssignPage, CResizableDialog)

ToolAssignPage::ToolAssignPage() : CLResizableDialog(ToolAssignPage::IDD)
{
}

ToolAssignPage::~ToolAssignPage()
{
}

void ToolAssignPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB_INNER_OR_OUTER, m_tabCtrl);
	DDX_Control(pDX, IDC_COMBO_DEFAULT_TOOL, m_defaultTool_comboBox);
	DDX_Control(pDX, IDC_LIST_TOOL_ASSIGN_RULE, m_toolAssignRule_gridList);
	DDX_Control( pDX, IDC_BTN_MANAGE_TOOLS, m_btnManageTools );
	DDX_Control( pDX, IDC_BTN_DXF_LAYER_MAPPING, m_btnDXFLayerMapping );
	DDX_Control( pDX, IDC_BTN_ADD_RULE, m_btnAddRule );
	DDX_Control( pDX, IDC_BTN_DELETE_RULE, m_btnDeleteRule );
}

BEGIN_MESSAGE_MAP(ToolAssignPage, CResizableDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_INNER_OR_OUTER, OnSelchangeTab)
    ON_CBN_SELCHANGE(IDC_COMBO_DEFAULT_TOOL, OnSelchangeDefaultTool)
    ON_MESSAGE(WM_GET_CELL_EDITOR_DATA, GetCellEditorData)
    ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_TOOL_ASSIGN_RULE, OnEndLabelEdit)

	ON_BN_CLICKED(IDC_BTN_MANAGE_TOOLS, OnToolManagement)
	ON_BN_CLICKED(IDC_BTN_DXF_LAYER_MAPPING, OnDxfToolMapping)
	ON_BN_CLICKED(IDC_BTN_ADD_RULE, OnAddRule)
	ON_BN_CLICKED(IDC_BTN_DELETE_RULE, OnDeleteRule)

END_MESSAGE_MAP()

BOOL ToolAssignPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

    // 布局控件
    AddAnchor(IDC_TAB_INNER_OR_OUTER, TOP_LEFT);
    AddAnchor(IDC_STATIC_TOOL_ASSIGN, TOP_LEFT);
    AddAnchor(IDC_STATIC_DEFAULT_TOOL, TOP_LEFT);
    AddAnchor(IDC_COMBO_DEFAULT_TOOL, TOP_LEFT);
    AddAnchor(IDC_LIST_TOOL_ASSIGN_RULE, TOP_LEFT);
    AddAnchor(IDC_BTN_MANAGE_TOOLS, TOP_LEFT);
    AddAnchor(IDC_BTN_ADD_RULE, TOP_LEFT);
    AddAnchor(IDC_BTN_DELETE_RULE, TOP_LEFT);
	AddAnchor(IDC_BTN_DXF_LAYER_MAPPING, TOP_LEFT);

	// init the tab control
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INTERNAL_GEOMETRY);
	m_tabCtrl.InsertItem(0, str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXTERNAL_GEOMETRY);
	m_tabCtrl.InsertItem(1, str);

	for (int i = 0; i < m_tabCtrl.GetItemCount(); i++ ) {
		m_tabCtrl.SetTabColor( i, SkinManager::GetBackgroundColor() );
	}

    // grid的样式
    InitGridStyle();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_TOOL_ASSIGN )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_DEFAULT_TOOL )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void ToolAssignPage::OnDestroy()
{
    // 释放grid中资源
    for (int i = 0; i < m_toolAssignRule_gridList.GetItemCount(); i++)
    {
        LONGLONG* pToolAssignRuleID = (LONGLONG*)m_toolAssignRule_gridList.GetItemData(i);
        delete pToolAssignRuleID;
    }

	CResizableDialog::OnDestroy();
}

HBRUSH ToolAssignPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ToolAssignPage::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_tabCtrl.GetCurSel() == 0)
	{
        InitDefaultTool(true);
        InitGridData(true);
	}
	else if (m_tabCtrl.GetCurSel() == 1)
	{
        InitDefaultTool(false);
        InitGridData(false);
	}
}

void ToolAssignPage::OnSelchangeDefaultTool()
{
    int iIndex = m_defaultTool_comboBox.GetCurSel();
    if (iIndex != INVALID_IDX)
    {
        LONGLONG* pToolID = (LONGLONG*)m_defaultTool_comboBox.GetItemData(iIndex);
        ToolInstancePtr pTool = m_pToolAssignConfigItem->GetToolInstanceList()->GetToolByID(*pToolID);
        if (m_tabCtrl.GetCurSel() == 0)
        {
            m_pToolAssignConfigItem->SetDefaultTool_4_innerLoop(pTool);
        }
        else if (m_tabCtrl.GetCurSel() == 1)
        {
            m_pToolAssignConfigItem->SetDefaultTool_4_outerLoop(pTool);
        }
        m_pToolAssignConfigItem->SetModified(TRUE);
    }
}

LRESULT ToolAssignPage::GetCellEditorData(WPARAM wParam, LPARAM lParam)
{
    CellEditorData* pCellEditorData = reinterpret_cast<CellEditorData*>(wParam);
    if (pCellEditorData->hwnd == m_toolAssignRule_gridList.m_hWnd)
    {
        pCellEditorData->m_cellEditorType = COMBOBOX;
        pCellEditorData->m_optionValList.clear();
        ToolInstanceListPtr pToolInstanceList = m_pToolAssignConfigItem->GetToolInstanceList();
        for (unsigned int i = 0; i < pToolInstanceList->size(); i++)
        {
            pCellEditorData->m_optionValList.push_back(pToolInstanceList->at(i)->GetToolName());
        }
    }

    return true;
}

void ToolAssignPage::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);
    int iRowIndex = pDispInfo->item.iItem;
    CString str = pDispInfo->item.pszText;

    // 当前编辑的规则
    ToolAssignRulePtr pToolAssignRule_current;
    LONGLONG* pToolAssignRuleID = (LONGLONG*)m_toolAssignRule_gridList.GetItemData(iRowIndex);
    if (m_tabCtrl.GetCurSel() == 0)
    {
        pToolAssignRule_current = m_pToolAssignConfigItem->GetToolAssignRules_4_innerLoop()->GetToolAssignRule_byID(*pToolAssignRuleID);
    }
    else if (m_tabCtrl.GetCurSel() == 1)
    {
        pToolAssignRule_current = m_pToolAssignConfigItem->GetToolAssignRules_4_outerLoop()->GetToolAssignRule_byID(*pToolAssignRuleID);
    }

    int iColumnIndex = pDispInfo->item.iSubItem;
    if (iColumnIndex == 2)
    {
        _bstr_t bstr = str;
        const char* pStrVal = (const char*)bstr;
        double dWidthRange_new = atof(pStrVal);
        if (fabs(dWidthRange_new - pToolAssignRule_current->GetWidthRange_of_loop()) > GEOM_TOLERANCE)
        {
            pToolAssignRule_current->SetWidthRange_of_loop(dWidthRange_new);
            pToolAssignRule_current->SetModified(TRUE);
        }
    }
    else if (iColumnIndex == 3)
    {
        _bstr_t bstr = str;
        const char* pStrVal = (const char*)bstr;
        double dHeightRange_new = atof(pStrVal);
        if (fabs(dHeightRange_new - pToolAssignRule_current->GetHeightRange_of_loop()) > GEOM_TOLERANCE)
        {
            pToolAssignRule_current->SetHeightRange_of_loop(dHeightRange_new);
            pToolAssignRule_current->SetModified(TRUE);
        }
    }
    else if (iColumnIndex == 4)
    {
        if (pToolAssignRule_current->GetToolInstance()->GetToolName() != str)
        {
            ToolInstancePtr pToolInstance = m_pToolAssignConfigItem->GetToolInstanceList()->GetToolByName(str);
            pToolAssignRule_current->SetToolInstance(pToolInstance);
            pToolAssignRule_current->SetModified(TRUE);
        }
    }
}

void ToolAssignPage::OnToolManagement()
{
	ToolManagementDlg dlg(m_pToolAssignConfigItem, m_pCondTypeList);
	dlg.DoModal();

    // 刷新界面数据
    if (m_tabCtrl.GetCurSel() == 0)
    {
        InitDefaultTool(true);
        InitGridData(true);
    }
    else if (m_tabCtrl.GetCurSel() == 1)
    {
        InitDefaultTool(false);
        InitGridData(false);
    }
}

void ToolAssignPage::OnDxfToolMapping()
{
	ToolDxfMapDlg dlg;
	dlg.InitDlg(m_pToolAssignConfigItem);
	dlg.DoModal();
}

void ToolAssignPage::OnAddRule()
{
    if (m_tabCtrl.GetCurSel() == 0)
    {
        ToolAssignRulePtr pToolAssignRule(new ToolAssignRule(true, 100.00, 100.00, m_pToolAssignConfigItem->GetDefaultTool_4_innerLoop()));
        m_pToolAssignConfigItem->GetToolAssignRules_4_innerLoop()->push_back(pToolAssignRule);
        AddRuleToGrid(pToolAssignRule);
    }
    else if (m_tabCtrl.GetCurSel() == 1)
    {
        ToolAssignRulePtr pToolAssignRule(new ToolAssignRule(false, 100.00, 100.00, m_pToolAssignConfigItem->GetDefaultTool_4_outerLoop()));
        m_pToolAssignConfigItem->GetToolAssignRules_4_outerLoop()->push_back(pToolAssignRule);
        AddRuleToGrid(pToolAssignRule);
    }
}

void ToolAssignPage::OnDeleteRule()
{
    int iIndex_selectedRule = INVALID_IDX;
    for (int i = 0; i < m_toolAssignRule_gridList.GetItemCount(); i++)
    {
        if (m_toolAssignRule_gridList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
            iIndex_selectedRule = i;
    }

    CString strMsg, strProductName;
    strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
    strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DELETE_TOOL_ASSIGN_RULE);
    if (iIndex_selectedRule != INVALID_IDX && MessageBox(strMsg, strProductName, MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
        LONGLONG* pToolAssignRuleID = (LONGLONG*)m_toolAssignRule_gridList.GetItemData(iIndex_selectedRule);
        if (m_tabCtrl.GetCurSel() == 0)
        {
            m_pToolAssignConfigItem->GetToolAssignRules_4_innerLoop()->DeleteToolAssignRule_byID(*pToolAssignRuleID);
			m_pToolAssignConfigItem->GetToolAssignRules_4_innerLoop()->SetModified(TRUE);
        }
        else if (m_tabCtrl.GetCurSel() == 1)
        {
            m_pToolAssignConfigItem->GetToolAssignRules_4_outerLoop()->DeleteToolAssignRule_byID(*pToolAssignRuleID);
			m_pToolAssignConfigItem->GetToolAssignRules_4_outerLoop()->SetModified(TRUE);
        }

        // 更新grid
        delete pToolAssignRuleID;
        m_toolAssignRule_gridList.DeleteItem(iIndex_selectedRule);
        for (int i = 0; i < m_toolAssignRule_gridList.GetItemCount(); i++)
        {
            CString strNum;
            strNum.Format(_T("%d"), i+1);
            m_toolAssignRule_gridList.SetItemText(i, 1, strNum);
        }
    }
}

void ToolAssignPage::DisplayToolAssignInfo(ToolAssignConfigItemPtr pToolAssignConfigItem, ConditionTypeListPtr pCondTypeList)
{
	m_pToolAssignConfigItem = pToolAssignConfigItem;
	m_pCondTypeList = pCondTypeList;

    if (m_tabCtrl.GetCurSel() == 0)
    {
        InitDefaultTool(true);
        InitGridData(true);
    }
    else if (m_tabCtrl.GetCurSel() == 1)
    {
        InitDefaultTool(false);
        InitGridData(false);
    }
}

void ToolAssignPage::InitGridStyle()
{
    CString str;

    // Requires one never uses column 0
    m_toolAssignRule_gridList.InsertHiddenLabelColumn();

    // 序号
    str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
    CGridColumnTrait* pNumCol = new CGridColumnTraitText;
    m_toolAssignRule_gridList.InsertColumnTrait(1, str, LVCFMT_LEFT, 50, 0, pNumCol);

    // 轮廓宽度范围
    str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LOOP_WIDTH_SMALLER_THAN);
    CGridColumnTrait* pLoopWidthRangeCol = new CGridColumnTraitEdit(FLOATEDITBOX);
    m_toolAssignRule_gridList.InsertColumnTrait(2, str, LVCFMT_LEFT, 120, 0, pLoopWidthRangeCol);

    // 轮廓高度范围
    str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LOOP_HEIGHT_SMALLER_THAN);
    CGridColumnTrait* pLoopHeightRangeCol = new CGridColumnTraitEdit(FLOATEDITBOX);
    m_toolAssignRule_gridList.InsertColumnTrait(3, str, LVCFMT_LEFT, 120, 0, pLoopHeightRangeCol);

    // 分配的刀具
    str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TOOL);
    CGridColumnTrait* pToolCol = new CGridColumnTraitText;
    m_toolAssignRule_gridList.InsertColumnTrait(4, str, LVCFMT_LEFT, 100, 0, pToolCol);
    m_toolAssignRule_gridList.AddDynamicCol(4);

    // set other style.
    m_toolAssignRule_gridList.SetExtendedStyle(LVS_EX_CHECKBOXES | m_toolAssignRule_gridList.GetExtendedStyle());
    m_toolAssignRule_gridList.EnableVisualStyles(TRUE);
}

void ToolAssignPage::InitGridData(bool bForInnerLoop)
{
    ToolAssignRuleListPtr pToolAssignRuleList;
    if (bForInnerLoop)
    {
        pToolAssignRuleList = m_pToolAssignConfigItem->GetToolAssignRules_4_innerLoop();
    }
    else
    {
        pToolAssignRuleList = m_pToolAssignConfigItem->GetToolAssignRules_4_outerLoop();
    }

    // 清空grid中现有数据
    for (int i = 0; i < m_toolAssignRule_gridList.GetItemCount(); i++)
    {
        LONGLONG* pToolAssignRuleID = (LONGLONG*)m_toolAssignRule_gridList.GetItemData(i);
        delete pToolAssignRuleID;
    }
    m_toolAssignRule_gridList.DeleteAllItems();

    for (unsigned int i = 0; i < pToolAssignRuleList->size(); i++)
    {
        AddRuleToGrid(pToolAssignRuleList->at(i));
    }
}

void ToolAssignPage::AddRuleToGrid(ToolAssignRulePtr pToolAssignRule)
{
    int iRowCount = m_toolAssignRule_gridList.GetItemCount();

    // insert an item.
    m_toolAssignRule_gridList.InsertItem(iRowCount, _T(""));
    m_toolAssignRule_gridList.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pToolAssignRule->GetID())));

    // 序号
    CString str;
    str.Format(_T("%d"), iRowCount+1);
    m_toolAssignRule_gridList.SetItemText(iRowCount, 1, str);

    // 轮廓范围
    CString strWidthRange, strHeightRange;
    strWidthRange.Format(_T("%.3f"), pToolAssignRule->GetWidthRange_of_loop());
    strHeightRange.Format(_T("%.3f"), pToolAssignRule->GetHeightRange_of_loop());
    m_toolAssignRule_gridList.SetItemText(iRowCount, 2, strWidthRange);
    m_toolAssignRule_gridList.SetItemText(iRowCount, 3, strHeightRange);

    // 刀具
    m_toolAssignRule_gridList.SetItemText(iRowCount, 4, pToolAssignRule->GetToolInstance()->GetToolName());
}

void ToolAssignPage::InitDefaultTool(bool bForInnerLoop)
{
    ToolInstancePtr pDefaultTool;
    if (bForInnerLoop)
    {
        pDefaultTool = m_pToolAssignConfigItem->GetDefaultTool_4_innerLoop();
    }
    else
    {
        pDefaultTool = m_pToolAssignConfigItem->GetDefaultTool_4_outerLoop();
    }

    // 清空combobox中现有数据
    for (int i = 0; i < m_defaultTool_comboBox.GetCount(); i++)
    {
        LONGLONG* pToolID = (LONGLONG*)m_defaultTool_comboBox.GetItemData(i);
        delete pToolID;
    }

    int iCurrentIndex = 0;
    ToolInstanceListPtr pToolInstanceList = m_pToolAssignConfigItem->GetToolInstanceList();
    m_defaultTool_comboBox.ResetContent();
    for (unsigned int i = 0; i < pToolInstanceList->size(); i++)
    {
        ToolInstancePtr pToolInstance = pToolInstanceList->at(i);
        m_defaultTool_comboBox.AddString(pToolInstance->GetToolName());
        m_defaultTool_comboBox.SetItemData(i, (DWORD_PTR)(new LONGLONG(pToolInstance->GetID())));
        if (pToolInstance->GetID() == pDefaultTool->GetID())
            iCurrentIndex = i;
    }
    m_defaultTool_comboBox.SetCurSel(iCurrentIndex);
}

END_CUTLEADER_NAMESPACE()

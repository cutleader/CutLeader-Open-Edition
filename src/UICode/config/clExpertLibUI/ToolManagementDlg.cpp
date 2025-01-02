#pragma once
#include "StdAfx.h"

#include "math.h"
#include "baseConst.h"
#include "clBaseDataResource.h"
#include "clUtilityResource.h"
#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"
#include "clExpertLibResource.h"
#include "DataHelper.h"

#include "ConditionTypeList.h"
#include "ConditionValue.h"
#include "ConditionValueList.h"
#include "ToolInstance.h"
#include "ToolInstanceList.h"
#include "ToolAssignRuleList.h"
#include "ToolAssignConfigItem.h"
#include "DxfLayerToolMap.h"
#include "ToolManagementDlg.h"


using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

bool ToolGridListCtrl::OnDisplayCellColor(int nRow, int nCol, COLORREF& textColor, COLORREF& backColor)
{
    // 太早调用这个函数的话，不做什么。
    if (!m_pToolInstanceList)
    {
        return false;
    }

    if (nCol == 5)
    {
        LONGLONG* pToolID = (LONGLONG*)GetItemData(nRow);
        ToolInstancePtr pToolInstance = m_pToolInstanceList->GetToolByID(*pToolID);
        backColor = pToolInstance->GetToolColor();

        return true; // override the color of this cell.
    }

    // use default color.
    return false;
}

IMPLEMENT_DYNCREATE(ToolManagementDlg, CDialogEx)

ToolManagementDlg::ToolManagementDlg()
	: CLDialog(ToolManagementDlg::IDD)
{
}

ToolManagementDlg::ToolManagementDlg(ToolAssignConfigItemPtr pToolAssignConfigItem, ConditionTypeListPtr pCondTypeList)
				: CLDialog(ToolManagementDlg::IDD)
{
	m_pToolAssignConfigItem = pToolAssignConfigItem;
	m_pCondTypeList = pCondTypeList;
	m_bDisableSelectChangeEvent_of_toolGrid = false;
    m_toolsGridList.InitToolList(pToolAssignConfigItem->GetToolInstanceList());
}

ToolManagementDlg::~ToolManagementDlg(void)
{
}

void ToolManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_TOOLS, m_toolsGridList);
	DDX_Control(pDX, IDC_LIST_COND_VAL, m_conditionValGridList);
}

BEGIN_MESSAGE_MAP(ToolManagementDlg, CDialogEx)
    ON_WM_DESTROY()

	ON_MESSAGE(WM_GET_CELL_EDITOR_DATA, GetCellEditorData)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TOOLS, OnToolGrid_selectChanged)
    ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_TOOLS, OnToolGrid_endLabelEdit)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COND_VAL, OnConditionValGrid_itemchanged)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_COND_VAL, OnConditionValGrid_endLabelEdit)

    ON_BN_CLICKED(IDC_BTN_ADD_TOOL, OnAddTool)
    ON_BN_CLICKED(IDC_BTN_DELETE_TOOL, OnDeleteTool)
	ON_BN_CLICKED(IDC_BTN_QUIT, OnQuit)
END_MESSAGE_MAP()

BOOL ToolManagementDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitGridStyle();
	m_bDisableSelectChangeEvent_of_toolGrid = true;
    ToolInstanceListPtr pToolInstanceList = m_pToolAssignConfigItem->GetToolInstanceList();
    for (unsigned int i = 0; i < pToolInstanceList->size(); i++)
    {
        ToolInstancePtr pToolInstance = pToolInstanceList->at(i);
        AddToolToGrid(pToolInstance);
    }
	m_bDisableSelectChangeEvent_of_toolGrid = false;

	// 选中第一把刀具并显示其条件值。
	if (m_toolsGridList.GetItemCount() > 0) // 必须的
	{
		m_toolsGridList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

	SetBackgroundColor(UI_COLOR);
	return TRUE;
}

void ToolManagementDlg::OnDestroy()
{
    // 释放grid中资源
    for (int i = 0; i < m_toolsGridList.GetItemCount(); i++)
    {
        LONGLONG* pToolID = (LONGLONG*)m_toolsGridList.GetItemData(i);
        delete pToolID;
    }
    for (int i = 0; i < m_conditionValGridList.GetItemCount(); i++)
    {
        LONGLONG* pCondTypeID = (LONGLONG*)m_conditionValGridList.GetItemData(i);
        delete pCondTypeID;
    }

    CDialogEx::OnDestroy();
}

BOOL ToolManagementDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    if (wParam == NM_CLICK)
    {
        NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)lParam;
        if (pNMListView && pNMListView->hdr.idFrom == IDC_LIST_TOOLS) // 这是来自刀具表格的点击事件
        {
            int iRowIndex = pNMListView->iItem;
            int iColumnIndex = pNMListView->iSubItem;
            if (iColumnIndex == 5)
            {
                LONGLONG* pToolID = (LONGLONG*)m_toolsGridList.GetItemData(iRowIndex);
                ToolInstancePtr pToolInstance = m_pToolAssignConfigItem->GetToolInstanceList()->GetToolByID(*pToolID);
                CColorDialog dlg(pToolInstance->GetToolColor());
                if (dlg.DoModal() == IDOK)
                {
                    COLORREF iColor = dlg.GetColor();
                    pToolInstance->SetToolColor(iColor);
                    pToolInstance->SetModified(TRUE);
                }
            }
        }

        return TRUE;
    }

    return CDialogEx::OnNotify(wParam, lParam, pResult);
}

void ToolManagementDlg::OnToolGrid_selectChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_bDisableSelectChangeEvent_of_toolGrid)
	{
		return;
	}

	// 显示选中刀具的条件值
    ToolInstancePtr pToolInstance = GetSelectedTool();
	if (pToolInstance)
	{
		InitGrid_of_conditionVal(pToolInstance);
	}
}

void ToolManagementDlg::OnToolGrid_endLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);
    int iRowIndex = pDispInfo->item.iItem;
    CString str = pDispInfo->item.pszText;

    // 当前编辑的刀具
    LONGLONG* pToolID = (LONGLONG*)m_toolsGridList.GetItemData(iRowIndex);
    ToolInstancePtr pTool_current = m_pToolAssignConfigItem->GetToolInstanceList()->GetToolByID(*pToolID);

    int iColumnIndex = pDispInfo->item.iSubItem;
    if (iColumnIndex == 2)
    {
        if (pTool_current->GetToolName() != str)
        {
            pTool_current->SetToolName(str);
            pTool_current->SetModified(TRUE);
        }
    }
    else if (iColumnIndex == 3)
    {
        _bstr_t bstr = str;
        const char* pStrVal = (const char*)bstr;
        double dToolWidth_new = atof(pStrVal);
        if (fabs(dToolWidth_new - pTool_current->GetToolWidth()) > GEOM_TOLERANCE)
        {
            pTool_current->SetToolWidth(dToolWidth_new);
            pTool_current->SetModified(TRUE);
        }
    }
	else if (iColumnIndex == 4)
	{
		// 用户选的结果
		CutOffsetType iCutOffsetType;
		CString strAuto, strCutOn;
		strAuto = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TOOL_CUTSIDE_AUTO);
		strCutOn = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TOOL_CUTSIDE_CUTON);
		if (str == strAuto)
		{
			iCutOffsetType = CutOffset_Auto;
		}
		else if (str == strCutOn)
		{
			iCutOffsetType = CutOffset_CutOn;
		}

		if (iCutOffsetType != pTool_current->GetCutOffsetType())
		{
			pTool_current->SetCutOffsetType(iCutOffsetType);
			pTool_current->SetModified(TRUE);
		}
	}
}

LRESULT ToolManagementDlg::GetCellEditorData(WPARAM wParam, LPARAM lParam)
{
	CellEditorData* pCellEditorData = reinterpret_cast<CellEditorData*>(wParam);
	if (pCellEditorData->hwnd == m_conditionValGridList.m_hWnd)
	{
		// 这个条件是不是enable了
		LONGLONG* pCondTypeId = (LONGLONG*)m_conditionValGridList.GetItemData(pCellEditorData->iRow);
		ToolInstancePtr pToolInstance = GetSelectedTool();
		if (!pToolInstance)
		{
			return true;
		}
		CondValInfoPtr pCondValInfo = pToolInstance->GetCondValList()->GetItemByCondType(*pCondTypeId);
		if (!pCondValInfo)
		{
			pCellEditorData->m_cellEditorType = READEDITBOX; // make the cell readonly.
			return true;
		}

		// 确定这个格子里面的控件类型
		ConditionTypePtr pCondType = m_pCondTypeList->GetItemByID(*pCondTypeId);
		if (pCondType->GetCondDataType() == BASIC_DATA_STRING)
		{
			pCellEditorData->m_cellEditorType = STREDITBOX;
		}
		else if (pCondType->GetCondDataType() == BASIC_DATA_INT)
		{
			pCellEditorData->m_cellEditorType = INTEDITBOX;
		}
		else if (pCondType->GetCondDataType() == BASIC_DATA_FLOAT)
		{
			pCellEditorData->m_cellEditorType = FLOATEDITBOX;
		}
		else if (pCondType->GetCondDataType() == BASIC_DATA_BOOL)
		{
			pCellEditorData->m_cellEditorType = COMBOBOX;

			// prepare the options for combo box.
			CString str1, str2;
			str1 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_YES);
			str2 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NO);
			pCellEditorData->m_optionValList.push_back(str1);
			pCellEditorData->m_optionValList.push_back(str2);
		}
	}

	return true;
}

void ToolManagementDlg::OnConditionValGrid_itemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// 选中行
	int iItem = pNMLV->iItem;
	if (iItem == -1)
		return;

	ToolInstancePtr pToolInstance = GetSelectedTool();
	if (!pToolInstance)
	{
		return;
	}
	LONGLONG* pCondTypeID = (LONGLONG*)m_conditionValGridList.GetItemData(iItem);
	if (!pCondTypeID)
	{
		return;
	}

	// if we un-check the check box in the first column, we should delete the CondValInfo, and vise versa.
	if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2))) // old state : unchecked, new state : checked
	{
		ConditionTypePtr pCondType = m_pCondTypeList->GetItemByID(*pCondTypeID);
		if (pCondType)
		{
			// create a CondValInfo object.
			CondValInfoPtr pCondValInfo(new CondValInfo());
			pCondValInfo->SetCondType(pCondType);
			_bstr_t bstr = m_conditionValGridList.GetItemText(iItem, 2);
			pCondValInfo->SetCondValue(bstr, TRUE);
			pToolInstance->GetCondValList()->push_back(pCondValInfo);
			pToolInstance->SetModified(TRUE);
		}
	} 
	else if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1))) // old state: checked, new state: unchecked
	{
		CondValInfoPtr pCondValInfo = pToolInstance->GetCondValList()->GetItemByCondType(*pCondTypeID);
		if (pCondValInfo)
		{
			pToolInstance->GetCondValList()->DeleteCondVal(pCondValInfo);
			pToolInstance->SetModified(TRUE);
		}
	}

	*pResult = 0;
}

void ToolManagementDlg::OnConditionValGrid_endLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);

	int iItem = pDispInfo->item.iItem;
	CString str = pDispInfo->item.pszText;

	// 得到编辑的“条件值对象”
	LONGLONG* pCondTypeID = (LONGLONG*)m_conditionValGridList.GetItemData(iItem);
	ToolInstancePtr pToolInstance = GetSelectedTool();
	CondValInfoPtr pCondValInfo = pToolInstance->GetCondValList()->GetItemByCondType(*pCondTypeID);

	// 更新条件值
	if (pCondValInfo->GetStrValue(TRUE) != str)
	{
		_bstr_t bstr = str;
		pCondValInfo->SetCondValue(bstr, TRUE);
		pToolInstance->SetModified(TRUE);
	}
}

void ToolManagementDlg::OnAddTool()
{
    // 要是重名，提示一下
    CString strToolName;
    strToolName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEW_TOOL);
    if (m_pToolAssignConfigItem->GetToolInstanceList()->GetToolByName(strToolName))
    {
        CString strMsg, strProductName;
        strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
        strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_TOOL_RENAME);
        MessageBox(strMsg, strProductName, MB_OK | MB_ICONWARNING);
    }

    // 新建刀具对象
    CondValInfoListPtr pCondValList(new CondValInfoList);
    ToolInstancePtr pToolInstance_new(new ToolInstance(strToolName, TOOL_CUT, 0.1, RGB(255,0,0), pCondValList, CutOffset_Auto));
    m_pToolAssignConfigItem->GetToolInstanceList()->push_back(pToolInstance_new);

    // 加到表格中
    AddToolToGrid(pToolInstance_new);
    m_toolsGridList.SetItemState(m_toolsGridList.GetItemCount()-1, LVIS_SELECTED, LVIS_SELECTED); // 选中这个
}

void ToolManagementDlg::OnDeleteTool()
{
    ToolInstancePtr pToolInstance = GetSelectedTool();
    if (pToolInstance)
    {
        CString strMsg, strProductName;
        strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

        // 看看是不是默认的刀具
        if (pToolInstance->GetID() == m_pToolAssignConfigItem->GetDefaultTool_4_innerLoop()->GetID() ||
            pToolInstance->GetID() == m_pToolAssignConfigItem->GetDefaultTool_4_outerLoop()->GetID())
        {
            strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DELETE_DEFAULT_TOOL);
            MessageBox(strMsg, strProductName, MB_OK | MB_ICONWARNING);
            return;
        }

		// 看看这个刀具是不是被规则占用了
		if (m_pToolAssignConfigItem->GetToolAssignRules_4_innerLoop()->GetRules_byTool(pToolInstance)->size() > 0 ||
			m_pToolAssignConfigItem->GetToolAssignRules_4_outerLoop()->GetRules_byTool(pToolInstance)->size() > 0 ||
			m_pToolAssignConfigItem->GetDxfLayerToolMap()->GetLayerMapInfos_byTool(pToolInstance).size() > 0 ||
			m_pToolAssignConfigItem->GetDxfLayerToolMap()->GetColorMapInfos_byTool(pToolInstance).size() > 0)
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DELETE_USED_TOOL);
			MessageBox(strMsg, strProductName, MB_OK | MB_ICONWARNING);
			return;
		}

        // 删除
        strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DELETE_TOOL);
        if (MessageBox(strMsg, strProductName, MB_YESNO|MB_ICONQUESTION) == IDYES)
        {
            m_pToolAssignConfigItem->GetToolInstanceList()->DeleteToolByID(pToolInstance->GetID());
			m_pToolAssignConfigItem->GetToolInstanceList()->SetModified(TRUE);

            int iIndex_selectedTool = INVALID_IDX;
            for (int i = 0; i < m_toolsGridList.GetItemCount(); i++)
            {
                if (m_toolsGridList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
                    iIndex_selectedTool = i;
            }

            // 更新grid
            {
                // 删掉这一行
                LONGLONG* pToolID = (LONGLONG*)m_toolsGridList.GetItemData(iIndex_selectedTool);
                delete pToolID;
                m_toolsGridList.DeleteItem(iIndex_selectedTool);

                // 重新编号
                for (int i = 0; i < m_toolsGridList.GetItemCount(); i++)
                {
                    CString strNum;
                    strNum.Format(_T("%d"), i+1);
                    m_toolsGridList.SetItemText(i, 1, strNum);
                }
            }
        }
    }
}

void ToolManagementDlg::OnQuit()
{
	OnOK();
}

void ToolManagementDlg::InitGridStyle()
{
	// 刀具列表
	{
		CString str;

		// Requires one never uses column 0
		m_toolsGridList.InsertHiddenLabelColumn();

		// 序号
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
		CGridColumnTrait* pNumCol = new CGridColumnTraitText;
		m_toolsGridList.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNumCol);

		// 名称
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
		CGridColumnTrait* pNameCol = new CGridColumnTraitEdit(STREDITBOX);
		m_toolsGridList.InsertColumnTrait(2, str, LVCFMT_LEFT, 80, 0, pNameCol);

		// 尺寸
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SIZE);
		CGridColumnTrait* pSizeCol = new CGridColumnTraitEdit(FLOATEDITBOX);
		m_toolsGridList.InsertColumnTrait(3, str, LVCFMT_LEFT, 75, 0, pSizeCol);

		// 偏移类型
		CGridColumnTraitCombo* pOffsetType = new CGridColumnTraitCombo;
		pOffsetType->SetStyle(pOffsetType->GetStyle() | CBS_DROPDOWNLIST);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TOOL_CUTSIDE_AUTO);
		pOffsetType->AddItem(0, str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TOOL_CUTSIDE_CUTON);
		pOffsetType->AddItem(1, str);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TOOL_CUTSIDE);
		m_toolsGridList.InsertColumnTrait(4, str, LVCFMT_LEFT, 70, 0, pOffsetType);

		// 颜色
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLOR);
		CGridColumnTrait* pColorCol = new CGridColumnTraitText;
		m_toolsGridList.InsertColumnTrait(5, str, LVCFMT_LEFT, 75, 0, pColorCol);

		// set other style.
		m_toolsGridList.EnableVisualStyles(TRUE);
        m_toolsGridList.IsSendItemClickEventToParent(true);
	}

	// 条件值
	{
		CString str;

		// place check box at first column.
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENABLE);
		CGridColumnTrait* pStatusCol = new CGridColumnTraitText;
		m_conditionValGridList.InsertColumnTrait(0, str, LVCFMT_LEFT, 50, 0, pStatusCol);

		// "Name" column
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
		CGridColumnTrait* pNUMCol = new CGridColumnTraitText;
		m_conditionValGridList.InsertColumnTrait(1, str, LVCFMT_LEFT, 300, 0, pNUMCol);

		// "Value" column
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_VALUE);
		CGridColumnTrait* pNameCol = new CGridColumnTraitText;
		m_conditionValGridList.InsertColumnTrait(2, str, LVCFMT_LEFT, 80, 0, pNameCol);
		m_conditionValGridList.AddDynamicCol(2);

		// set other style.
		m_conditionValGridList.SetExtendedStyle(LVS_EX_CHECKBOXES | m_conditionValGridList.GetExtendedStyle());
		m_conditionValGridList.EnableVisualStyles(TRUE);
	}
}

void ToolManagementDlg::AddToolToGrid(ToolInstancePtr pToolInstance)
{
    int iRowCount = m_toolsGridList.GetItemCount();

	// insert an item.
	m_toolsGridList.InsertItem(iRowCount, _T(""));
	m_toolsGridList.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pToolInstance->GetID())));

	// 序号
	CString str;
	str.Format(_T("%d"), iRowCount+1);
	m_toolsGridList.SetItemText(iRowCount, 1, str);

	// 名称
	m_toolsGridList.SetItemText(iRowCount, 2, pToolInstance->GetToolName());

	// 刀具尺寸
	CString strSize;
	strSize.Format(_T("%.3f"), pToolInstance->GetToolWidth());
	m_toolsGridList.SetItemText(iRowCount, 3, strSize);

	// 刀具偏移类型
	CString strOffsetType;
	CutOffsetType iCutOffsetType = pToolInstance->GetCutOffsetType();
	if (iCutOffsetType == CutOffset_Auto)
	{
		strOffsetType = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TOOL_CUTSIDE_AUTO);
	}
	else if (iCutOffsetType == CutOffset_CutOn)
	{
		strOffsetType = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TOOL_CUTSIDE_CUTON);
	}
	m_toolsGridList.SetItemText(iRowCount, 4, strOffsetType);
}

void ToolManagementDlg::InitGrid_of_conditionVal(ToolInstancePtr pToolInstance)
{
    // 清空grid中现有数据
    for (int i = 0; i < m_conditionValGridList.GetItemCount(); i++)
    {
        LONGLONG* pCondTypeID = (LONGLONG*)m_conditionValGridList.GetItemData(i);
        delete pCondTypeID;
    }
    m_conditionValGridList.DeleteAllItems();

	for (unsigned int i = 0; i < m_pCondTypeList->size(); i++)
	{
		ConditionTypePtr pCondType = m_pCondTypeList->at(i);

		// 得到条件值
		CondValInfoPtr pCondValInfo = pToolInstance->GetCondValList()->GetItemByCondType(pCondType->GetID());

		m_conditionValGridList.InsertItem(i, _T(""));
		m_conditionValGridList.SetItemText(i, 1, pCondType->GetCondName());
		if (pCondValInfo)
		{
			m_conditionValGridList.SetItemText(i, 2, pCondValInfo->GetStrValue(TRUE));
			m_conditionValGridList.SetCheck(i, TRUE);
		}
		else
		{
			CString strDefaultVal = DataHelper::GetDefaultValue_4_display(pCondType->GetCondDataType());
			m_conditionValGridList.SetItemText(i, 2, strDefaultVal);
		}

		// we put the condition type id into grid item.
		m_conditionValGridList.SetItemData(i, (DWORD_PTR)(new LONGLONG(pCondType->GetID())));
	}
}

ToolInstancePtr ToolManagementDlg::GetSelectedTool()
{
	ToolInstancePtr pToolInstance;

	int iIndex_selectedTool = INVALID_IDX;
	for (int i = 0; i < m_toolsGridList.GetItemCount(); i++)
	{
		if (m_toolsGridList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
			iIndex_selectedTool = i;
	}

	if (iIndex_selectedTool != INVALID_IDX)
	{
		pToolInstance = GetTargetTool(iIndex_selectedTool);
	}

	return pToolInstance;
}

ToolInstancePtr ToolManagementDlg::GetTargetTool(int nToolIndex)
{
	ToolInstancePtr pToolInstance;

	LONGLONG* pToolID = (LONGLONG*)m_toolsGridList.GetItemData(nToolIndex);
	pToolInstance = m_pToolAssignConfigItem->GetToolInstanceList()->GetToolByID(*pToolID);

	return pToolInstance;
}

CComVariant ToolManagementDlg::GetToolConditionVariant(ToolInstancePtr pToolInstance, int nToolConditionIndex)
{
	CComVariant Value;
	LONGLONG* pCondTypeID = (LONGLONG*)m_conditionValGridList.GetItemData(nToolConditionIndex);
	CondValInfoPtr pCondValInfo = pToolInstance->GetCondValList()->GetItemByCondType(*pCondTypeID);
	if (pCondValInfo)
	{
		Value = pCondValInfo->GetCondVal();
	}else{
		Value.dblVal = 0;
	}

	return Value;
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "LoopToolEditDlg.h"

#include "math.h"
#include "clUtilityResource.h"
#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"
#include "baseConst.h"
#include "CommandManager.h"
#include "DataHelper.h"

#include "ConditionType.h"
#include "ConditionTypeList.h"
#include "ToolAssignConfigItem.h"
#include "ToolInstance.h"
#include "ToolInstanceList.h"

#include "LoopToolData.h"
#include "LoopToolDataList.h"
#include "LoopToolManager.h"
#include "ConditionNode.h"
#include "ConditionNodeList.h"
#include "ConditionNodeAddCommand.h"
#include "ConditionNodeDeleteCommand.h"
#include "ConditionNodeModifyCommand.h"
#include "ToolSizeModifyCommand.h"
#include "ToolColorModifyCommand.h"
#include "LoopToolChangeCommand.h"
#include "clCutFeatureResource.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(LoopToolEditDlg, CLResizableDialog)

LoopToolEditDlg::LoopToolEditDlg() : CLResizableDialog(LoopToolEditDlg::IDD)
{
}

LoopToolEditDlg::~LoopToolEditDlg()
{
}

void LoopToolEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CLResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_TOOL_NAME, m_toolName_comboBox);
	DDX_Control(pDX, IDC_EDIT_TOOL_SIZE, m_toolSize_editBox);
	DDX_Control(pDX, IDC_BTN_TOOL_COLOR, m_toolColor_btn);
	DDX_Control(pDX, IDC_LIST_DOPIERCE_CONDITION, m_doPierceCondition_gridList);
	DDX_Control(pDX, IDC_LIST_UNPIERCE_CONDITION, m_unPierceCondition_gridList);
	DDX_Control(pDX, IDC_LIST_CUT_CONDITION, m_cutCondition_gridList);
}

BEGIN_MESSAGE_MAP(LoopToolEditDlg, CLResizableDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_WM_SIZE()

	ON_CBN_SELCHANGE(IDC_COMBO_TOOL_NAME, OnToolSelectChanged)
	ON_BN_CLICKED(IDC_BTN_TOOL_COLOR, OnColorClicked)

    ON_MESSAGE(WM_GET_CELL_EDITOR_DATA, GetCellEditorData)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DOPIERCE_CONDITION, OnGridItemchanged_doPierceCondition)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_UNPIERCE_CONDITION, OnGridItemchanged_unPierceCondition)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CUT_CONDITION, OnGridItemchanged_cutCondition)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_DOPIERCE_CONDITION, OnGridEndEdit_doPierceCondition)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_UNPIERCE_CONDITION, OnGridEndEdit_unPierceCondition)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_CUT_CONDITION, OnGridEndEdit_cutCondition)
END_MESSAGE_MAP()

BOOL LoopToolEditDlg::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// about scroll bar.
	GetWindowRect(m_rcOriginalRect); // get the original size
	m_nScrollPos = 0; // initial scroll position

	// 颜色按钮
	m_toolColor_btn.EnableOtherButton(_T("Other..."));
	m_toolColor_btn.SetColumnsNumber(4);

	// 初始化3个表格的风格
	InitGridStyle(&m_doPierceCondition_gridList);
	InitGridStyle(&m_unPierceCondition_gridList);
	InitGridStyle(&m_cutCondition_gridList);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_COMBO_TOOL_NAME )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_AVAIL_PART2 )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_AVAIL_PART6 )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_AVAIL_PART7 )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void LoopToolEditDlg::OnOK()
{
	if (GetDlgItem(IDC_EDIT_TOOL_SIZE) == GetFocus())
	{
		m_toolSize_editBox.UpdateNumEdit_fromUI();
		LoopToolDataPtr pLoopTool = m_pLoopToolList->at(0);
        double dOldToolSize = pLoopTool->GetToolWidth();
		if (fabs(dOldToolSize - m_toolSize_editBox.Get_Number()) > GEOM_TOLERANCE)
		{
			ICommandPtr pCommand(new ToolSizeModifyCommand(m_editorData, m_pLoopToolList, m_toolSize_editBox.Get_Number()));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

            // 通知观察者
            {
                // 构造消息体
                TwoToolSizes* pTwoToolSizes = new TwoToolSizes();
                pTwoToolSizes->m_dOldToolSize = dOldToolSize;
                pTwoToolSizes->m_dNewToolSize = m_toolSize_editBox.Get_Number();

                // 发送通知
				CComVariant varEventContent;
                varEventContent.vt = VT_BYREF;
                varEventContent.byref = pTwoToolSizes;
                this->NotifyAllObserver(ID_EVENT_ChangedToolSizeOfSelectedLoops, varEventContent);

                // 释放资源
                delete pTwoToolSizes;
            }
		}
	}

	return;
}

void LoopToolEditDlg::OnCancel()
{ 
	return;
}

void LoopToolEditDlg::OnDestroy()
{
	// release resource.
	ReleaseComboBoxData();
	ReleaseGridData();

	CLResizableDialog::OnDestroy();
}

HBRUSH LoopToolEditDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void LoopToolEditDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = m_rcOriginalRect.Height() - m_nCurHeight;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;

		nDelta = min(max(nMaxPos/20,5), nMaxPos-m_nScrollPos);
		break;

	case SB_LINEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos/20,5), m_nScrollPos);
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos/10,5), nMaxPos-m_nScrollPos);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPos;
		break;

	case SB_PAGEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos/10,5), m_nScrollPos);
		break;

	default:
		return;
	}
	m_nScrollPos += nDelta;
	SetScrollPos(SB_VERT, m_nScrollPos, TRUE);
	ScrollWindow(0, -nDelta);

	CLResizableDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void LoopToolEditDlg::OnSize(UINT nType, int cx, int cy) 
{
	m_nCurHeight = cy;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL; 
	si.nMin = 0;
	si.nMax = m_rcOriginalRect.Height();
	si.nPage = cy;
	si.nPos = m_nScrollPos;
	SetScrollInfo(SB_VERT, &si, TRUE);

	CLResizableDialog::OnSize(nType, cx, cy);
}

void LoopToolEditDlg::OnToolSelectChanged() 
{
	int iIndex = m_toolName_comboBox.GetCurSel();
	if (iIndex != INVALID_IDX)
	{
		LONGLONG* pToolID = (LONGLONG*)m_toolName_comboBox.GetItemData(iIndex);
		ToolInstancePtr pToolInstance = m_pToolAssignConfigItem->GetToolInstanceList()->GetToolByID(*pToolID);

		// 更改刀具
		ICommandPtr pCommand(new LoopToolChangeCommand(m_editorData, m_loopTopologyItemData, m_pLoopToolList, pToolInstance, m_pLoopStaEndCutList, m_pLeadConfigItem));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 刀具尺寸和颜色
		m_toolSize_editBox.Init(pToolInstance->GetToolWidth(), 4, FALSE);
		m_toolColor_btn.SetColor(pToolInstance->GetToolColor());

		// 初始化条件grid
		{
			ReleaseGridData();
			m_doPierceCondition_gridList.DeleteAllItems();
			m_unPierceCondition_gridList.DeleteAllItems();
			m_cutCondition_gridList.DeleteAllItems();

			//
			InitGridData(&m_doPierceCondition_gridList, m_pCondTypeList->GetCondTypeList(COND_CATEGORY_DOPIERCE));
			InitGridData(&m_unPierceCondition_gridList, m_pCondTypeList->GetCondTypeList(COND_CATEGORY_UNPIERCE));
			InitGridData(&m_cutCondition_gridList, m_pCondTypeList->GetCondTypeList(COND_CATEGORY_CUT));
		}

		// 更新视图显示
		CWnd::FromHandle(m_editorData.GetView()->m_hWnd)->Invalidate();
	}
}

void LoopToolEditDlg::OnColorClicked() 
{
	COLORREF iColor = m_toolColor_btn.GetColor();
	if (iColor != m_pLoopToolList->at(0)->GetToolColor())
	{
		ICommandPtr pCommand(new ToolColorModifyCommand(m_editorData, m_pLoopToolList, iColor));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}
}

LRESULT LoopToolEditDlg::GetCellEditorData(WPARAM wParam, LPARAM lParam)
{
    LoopToolDataPtr pLoopTool = m_pLoopToolList->at(0);

    // 当前编辑的格子是不是只读。
    LONGLONG* pCondTypeID = 0;
    CellEditorData* pCellEditorData = reinterpret_cast<CellEditorData*>(wParam);
    if (pCellEditorData->hwnd == m_doPierceCondition_gridList.m_hWnd)
    {
        pCondTypeID = (LONGLONG*)m_doPierceCondition_gridList.GetItemData(pCellEditorData->iRow);
    }
    else if (pCellEditorData->hwnd == m_unPierceCondition_gridList.m_hWnd)
    {
        pCondTypeID = (LONGLONG*)m_unPierceCondition_gridList.GetItemData(pCellEditorData->iRow);
    }
    else if (pCellEditorData->hwnd == m_cutCondition_gridList.m_hWnd)
    {
        pCondTypeID = (LONGLONG*)m_cutCondition_gridList.GetItemData(pCellEditorData->iRow);
    }
    ConditionNodePtr pCondNode = pLoopTool->GetConditionNodeList()->GetByCondTypeID(*pCondTypeID);
    if (!pCondNode)
    {
        pCellEditorData->m_cellEditorType = READEDITBOX; // make the cell readonly.
        return true;
    }

    // 确定这个格子里面的控件类型
    ConditionTypePtr pCondType = m_pCondTypeList->GetItemByID(*pCondTypeID);
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

    return true;
}

void LoopToolEditDlg::OnGridItemchanged_doPierceCondition(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    // 选中行
    int iItem = pNMLV->iItem;
    if (iItem == -1)
        return;

    LONGLONG* pCondTypeID = (LONGLONG*)m_doPierceCondition_gridList.GetItemData(iItem);
    if (!pCondTypeID)
    {
        return;
    }

    // if we un-check the check box in the first column, we should delete the CondValInfo, and vise versa.
    if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2))) // old state : unchecked, new state : checked
    {
		CString strCondVal_4_display = m_doPierceCondition_gridList.GetItemText(iItem, 2);
		AddConditionNode(*pCondTypeID, strCondVal_4_display);
    } 
    else if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1))) // old state: checked, new state: unchecked
    {
		DeleteConditionNode(*pCondTypeID); // 删除切割条件
    }

    *pResult = 0;
}

void LoopToolEditDlg::OnGridItemchanged_unPierceCondition(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// 选中行
	int iItem = pNMLV->iItem;
	if (iItem == -1)
		return;

	LONGLONG* pCondTypeID = (LONGLONG*)m_unPierceCondition_gridList.GetItemData(iItem);
	if (!pCondTypeID)
	{
		return;
	}

	// if we un-check the check box in the first column, we should delete the CondValInfo, and vise versa.
	if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2))) // old state : unchecked, new state : checked
	{
		CString strCondVal_4_display = m_unPierceCondition_gridList.GetItemText(iItem, 2);
		AddConditionNode(*pCondTypeID, strCondVal_4_display);
	} 
	else if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1))) // old state: checked, new state: unchecked
	{
		DeleteConditionNode(*pCondTypeID); // 删除切割条件
	}

	*pResult = 0;
}

void LoopToolEditDlg::OnGridItemchanged_cutCondition(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// 选中行
	int iItem = pNMLV->iItem;
	if (iItem == -1)
		return;

	LONGLONG* pCondTypeID = (LONGLONG*)m_cutCondition_gridList.GetItemData(iItem);
	if (!pCondTypeID)
	{
		return;
	}

	// if we un-check the check box in the first column, we should delete the CondValInfo, and vise versa.
	if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2))) // old state : unchecked, new state : checked
	{
		CString strCondVal_4_display = m_cutCondition_gridList.GetItemText(iItem, 2);
		AddConditionNode(*pCondTypeID, strCondVal_4_display);
	} 
	else if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1))) // old state: checked, new state: unchecked
	{
		DeleteConditionNode(*pCondTypeID); // 删除切割条件
	}

	*pResult = 0;
}

void LoopToolEditDlg::OnGridEndEdit_doPierceCondition(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);

	int iItem = pDispInfo->item.iItem;
	CString str = pDispInfo->item.pszText;

	LONGLONG* pCondTypeID = (LONGLONG*)m_doPierceCondition_gridList.GetItemData(iItem);
	LoopToolDataPtr pLoopTool = m_pLoopToolList->at(0);

	// 更新条件值
	ConditionNodePtr pCondNode = pLoopTool->GetConditionNodeList()->GetByCondTypeID(*pCondTypeID);
	if (pCondNode->GetCondVal() != str)
	{
		ConditionTypePtr pCondType = m_pCondTypeList->GetItemByID(*pCondTypeID);
		CString strVal_real = DataHelper::DisplayVal_2_realVal(str, pCondType->GetCondDataType());
		ICommandPtr pCommand(new ConditionNodeModifyCommand(m_editorData, m_pLoopToolList, *pCondTypeID, strVal_real));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}
}

void LoopToolEditDlg::OnGridEndEdit_unPierceCondition(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);

	int iItem = pDispInfo->item.iItem;
	CString str = pDispInfo->item.pszText;

	LONGLONG* pCondTypeID = (LONGLONG*)m_unPierceCondition_gridList.GetItemData(iItem);
	LoopToolDataPtr pLoopTool = m_pLoopToolList->at(0);

	// 更新条件值
	ConditionNodePtr pCondNode = pLoopTool->GetConditionNodeList()->GetByCondTypeID(*pCondTypeID);
	if (pCondNode->GetCondVal() != str)
	{
		ConditionTypePtr pCondType = m_pCondTypeList->GetItemByID(*pCondTypeID);
		CString strVal_real = DataHelper::DisplayVal_2_realVal(str, pCondType->GetCondDataType());
		ICommandPtr pCommand(new ConditionNodeModifyCommand(m_editorData, m_pLoopToolList, *pCondTypeID, strVal_real));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}
}

void LoopToolEditDlg::OnGridEndEdit_cutCondition(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);

	int iItem = pDispInfo->item.iItem;
	CString str = pDispInfo->item.pszText;

	LONGLONG* pCondTypeID = (LONGLONG*)m_cutCondition_gridList.GetItemData(iItem);
	LoopToolDataPtr pLoopTool = m_pLoopToolList->at(0);

	// 更新条件值
	ConditionNodePtr pCondNode = pLoopTool->GetConditionNodeList()->GetByCondTypeID(*pCondTypeID);
	if (pCondNode->GetCondVal() != str)
	{
		ConditionTypePtr pCondType = m_pCondTypeList->GetItemByID(*pCondTypeID);
		CString strVal_real = DataHelper::DisplayVal_2_realVal(str, pCondType->GetCondDataType());
		ICommandPtr pCommand(new ConditionNodeModifyCommand(m_editorData, m_pLoopToolList, *pCondTypeID, strVal_real));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}
}

void LoopToolEditDlg::DisplayLoopToolInfo(const vector<LoopTopologyItemListPtr>& loopTopologyItemData, LoopToolDataListPtr pLoopToolList, LoopStartCutDataListPtr pLoopStaEndCutList,
	LeadConfigItemPtr pLeadConfigItem, ToolAssignConfigItemPtr pToolAssignConfigItem, ConditionTypeListPtr pCondTypeList, EditorData editorData)
{
	m_loopTopologyItemData = loopTopologyItemData;
	m_pLoopToolList = pLoopToolList;
	m_pLoopStaEndCutList = pLoopStaEndCutList;
	m_pToolAssignConfigItem = pToolAssignConfigItem;
	m_pLeadConfigItem = pLeadConfigItem;
    m_pCondTypeList = pCondTypeList;
    m_editorData = editorData;

    LoopToolDataPtr pLoopTool = pLoopToolList->at(0);

    // 显示刀具名称
    {
        // 清理combobox
        ReleaseComboBoxData();
        m_toolName_comboBox.ResetContent();

        // 初始化combobox
        int iCurrentIndex = 0;
        ToolInstanceListPtr pToolInstanceList = m_pToolAssignConfigItem->GetToolInstanceList();
        for (unsigned int i = 0; i < pToolInstanceList->size(); i++)
        {
            ToolInstancePtr pToolInstance = pToolInstanceList->at(i);
            m_toolName_comboBox.AddString(pToolInstance->GetToolName());
            m_toolName_comboBox.SetItemData(i, (DWORD_PTR)(new LONGLONG(pToolInstance->GetID())));
            if (pToolInstance->GetToolName() == pLoopTool->GetToolName())
                iCurrentIndex = i;
        }
        m_toolName_comboBox.SetCurSel(iCurrentIndex);
    }

    // 刀具尺寸和颜色
    m_toolSize_editBox.Init(pLoopTool->GetToolWidth(), 4, FALSE);
    m_toolColor_btn.SetColor(pLoopTool->GetToolColor());

    // 条件
    {
        ReleaseGridData();
        m_doPierceCondition_gridList.DeleteAllItems();
        m_unPierceCondition_gridList.DeleteAllItems();
        m_cutCondition_gridList.DeleteAllItems();

        //
		InitGridData(&m_doPierceCondition_gridList, m_pCondTypeList->GetCondTypeList(COND_CATEGORY_DOPIERCE));
		InitGridData(&m_unPierceCondition_gridList, m_pCondTypeList->GetCondTypeList(COND_CATEGORY_UNPIERCE));
		InitGridData(&m_cutCondition_gridList, m_pCondTypeList->GetCondTypeList(COND_CATEGORY_CUT));
    }
}

void LoopToolEditDlg::InitGridStyle(SkinGridListCtrlGroups* pConditionGridList)
{
	CString str;

	// place check box at first column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENABLE);
	CGridColumnTrait* pStatusCol = new CGridColumnTraitText;
	pConditionGridList->InsertColumnTrait(0, str, LVCFMT_LEFT, 50, 0, pStatusCol);

	// "Name" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
	CGridColumnTrait* pNUMCol = new CGridColumnTraitText;
	pConditionGridList->InsertColumnTrait(1, str, LVCFMT_LEFT, 60, 0, pNUMCol);

	// "Value" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_VALUE);
	CGridColumnTrait* pNameCol = new CGridColumnTraitText;
	pConditionGridList->InsertColumnTrait(2, str, LVCFMT_LEFT, 60, 0, pNameCol);
	pConditionGridList->AddDynamicCol(2);

	// set other style.
	pConditionGridList->SetExtendedStyle(LVS_EX_CHECKBOXES | pConditionGridList->GetExtendedStyle());
	pConditionGridList->EnableVisualStyles(TRUE);
}

void LoopToolEditDlg::InitGridData(SkinGridListCtrlGroups* pConditionGridList, ConditionTypeListPtr pCondTypeList)
{
	LoopToolDataPtr pLoopTool = m_pLoopToolList->at(0);

	for (unsigned int i = 0; i < pCondTypeList->size(); i++)
	{
		ConditionTypePtr pCondType = pCondTypeList->at(i);
		ConditionNodePtr pCondNode = pLoopTool->GetConditionNodeList()->GetByCondTypeID(pCondType->GetID());
		pConditionGridList->InsertItem(i, _T(""));
		pConditionGridList->SetItemText(i, 1, pCondType->GetCondName());
		if (pCondNode)
		{
			pConditionGridList->SetItemText(i, 2, LoopToolManager::GetConditionValue_4_display(pCondNode, pCondTypeList));
			pConditionGridList->SetCheck(i, TRUE);
		}
		else
		{
			CString strDefaultVal = DataHelper::GetDefaultValue_4_display(pCondType->GetCondDataType());
			pConditionGridList->SetItemText(i, 2, strDefaultVal);
		}
		pConditionGridList->SetItemData(i, (DWORD_PTR)(new LONGLONG(pCondType->GetID())));
	}
}

void LoopToolEditDlg::ReleaseGridData()
{
	for (int i = 0; i < m_doPierceCondition_gridList.GetItemCount(); i++)
	{
		LONGLONG* pCondTypeID = (LONGLONG*)m_doPierceCondition_gridList.GetItemData(i);
		delete pCondTypeID;
	}

	for (int i = 0; i < m_unPierceCondition_gridList.GetItemCount(); i++)
	{
		LONGLONG* pCondTypeID = (LONGLONG*)m_unPierceCondition_gridList.GetItemData(i);
		delete pCondTypeID;
	}

	for (int i = 0; i < m_cutCondition_gridList.GetItemCount(); i++)
	{
		LONGLONG* pCondTypeID = (LONGLONG*)m_cutCondition_gridList.GetItemData(i);
		delete pCondTypeID;
	}
}

void LoopToolEditDlg::ReleaseComboBoxData()
{
	for (int i = 0; i < m_toolName_comboBox.GetCount(); i++)
	{
		LONGLONG* pToolID = (LONGLONG*)m_toolName_comboBox.GetItemData(i);
		delete pToolID;
	}
}

void LoopToolEditDlg::AddConditionNode(LONGLONG iCondTypeID, CString strCondVal_4_display)
{
	ConditionNodeListPtr pNewCondNodes(new ConditionNodeList);
	for (unsigned int i = 0; i < m_pLoopToolList->size(); i++)
	{
		pNewCondNodes->push_back(LoopToolManager::BuildCondNode(m_pCondTypeList, iCondTypeID, strCondVal_4_display));
	}
	ICommandPtr pCommand(new ConditionNodeAddCommand(m_editorData, m_pLoopToolList, pNewCondNodes));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);
}

void LoopToolEditDlg::DeleteConditionNode(LONGLONG iCondTypeID)
{
	ICommandPtr pCommand(new ConditionNodeDeleteCommand(m_editorData, m_pLoopToolList, iCondTypeID));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);
}

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clExpertLibUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "PierceDrawStatic.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(ToolAssignRule)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLibUI_Export ToolAssignPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(ToolAssignPage)

public:
	ToolAssignPage();
	~ToolAssignPage();

	enum { IDD = IDD_clExpertLibUI_ToolAssign };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected:
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangeDefaultTool();
    afx_msg LRESULT GetCellEditorData(WPARAM wParam, LPARAM lParam);
    afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnToolManagement();
	afx_msg void OnDxfToolMapping();
	afx_msg void OnAddRule();
	afx_msg void OnDeleteRule();

	DECLARE_MESSAGE_MAP()

public:
	void DisplayToolAssignInfo(ToolAssignConfigItemPtr pToolAssignConfigItem, ConditionTypeListPtr pCondTypeList);
    ToolAssignConfigItemPtr GetToolAssignConfigItem() const { return m_pToolAssignConfigItem; }

private:
	void InitGridStyle();

    // 初始化grid的数据
	void InitGridData(bool bForInnerLoop);
    void AddRuleToGrid(ToolAssignRulePtr pToolAssignRule);

    // 初始化默认刀具信息
    void InitDefaultTool(bool bForInnerLoop);

private: // controls
	SkinTabCtrl m_tabCtrl;
	CComboBox m_defaultTool_comboBox;
	SkinGridListCtrlGroups m_toolAssignRule_gridList;

	SkinMFCButton m_btnManageTools;
	SkinMFCButton m_btnDXFLayerMapping;
	SkinMFCButton m_btnAddRule;
	SkinMFCButton m_btnDeleteRule;

private: // for data.
    ToolAssignConfigItemPtr m_pToolAssignConfigItem;
	ConditionTypeListPtr m_pCondTypeList;
};

END_CUTLEADER_NAMESPACE()

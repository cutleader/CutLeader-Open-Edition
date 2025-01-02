#pragma once

#include "clMachineLibUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(ConditionType)

BEGIN_CUTLEADER_NAMESPACE()

// we can edit the mac's condition in this page.
// notes:
//   1) rules for popup menu.
//      a) if there is item under the mouse, select it.
//      b) if no item under the mouse.
//         i) no item selected currently, select the first item.
//         ii) there are item(s) selected currently, handle them.
class ClMachineLibUI_Export ConditionEditPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(ConditionEditPage)

public:
	ConditionEditPage(void);
	~ConditionEditPage(void);

	enum { IDD = IDD_clMachineLibUI_ConditionEdit };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected: // message mapping.
	// for the popup menu.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnCreateMacCond();
	afx_msg void OnDelMacCond();

	// cell editor will call this function(by sending msg) to notify the dlg that the editing is finished.
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnRClickList(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

public:
	// display the conditions of the machine.
	void DisplayMacConds(ConditionTypeListPtr pMacConds);
	
	// get the updated conditions.
	ConditionTypeListPtr GetMacConds() { return m_pMacConds; }

private:
	// init the column header of the list ctrl.
	void InitGridStyle();

	// init the content of the list ctrl.
	void InitGridData();

	// release the data which stored in grid.
	void ReleaseGridItemData();

	// add a row.
	void AddNewRow(ConditionTypePtr pCondType);

private: // controls
	SkinGridListCtrlGroups m_condList;

	// the popup.
	CMenu m_menu;

private:
	ConditionTypeListPtr m_pMacConds;
};

END_CUTLEADER_NAMESPACE()

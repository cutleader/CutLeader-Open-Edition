#pragma once

#include "clExpertLibUICommon.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(NCConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

// we can edit the sequence configuration.
class ClExpertLibUI_Export NCConfigPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(NCConfigPage)

public:
	NCConfigPage(void);
	~NCConfigPage(void);

	enum { IDD = IDD_clExpertLibUI_NC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnDestroy();

	// grid ctrl will call this function(by sending msg) to get the cell editor info.
	afx_msg LRESULT GetCellEditorData(WPARAM wParam, LPARAM lParam);

	// cell editor will call this function(by sending msg) to notify the dlg that the editing is finished.
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

public:
	void DisplayNCConfig(NCConfigItemPtr pNCConfigItem);
	NCConfigItemPtr GetNCConfig();

private:
	// init the style of the list ctrl
	void InitListStyle();

	// init the data of the list ctrl
	void InitListData();

	// release the data which stored in grid.
	void ReleaseGridItemData();

private: // controls
	SkinGridListCtrlGroups m_ncConfigValList;

private:
	NCConfigItemPtr m_pNCConfigItem;
};

END_CUTLEADER_NAMESPACE()

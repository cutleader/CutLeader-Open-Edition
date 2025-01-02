#pragma once

#include "clMachineLibUICommon.h"
#include "CLResizableDialog.h"
#include "taoresource.h"
#include "NCConfigType.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(NCConfigTypeList)

BEGIN_CUTLEADER_NAMESPACE()

// we can edit the mac's condition in this page.
class ClMachineLibUI_Export NCConfigTypeEditPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(NCConfigTypeEditPage)

public:
	NCConfigTypeEditPage();
	NCConfigTypeEditPage(NCConfigTypeListPtr pNCConfigTypeList);
	~NCConfigTypeEditPage(void);

	enum { IDD = IDD_clMachineLibUI_NcConfigType };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnDestroy();

	afx_msg void OnNewNCConfig();
	afx_msg void OnDelNCConfig();

	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickList(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

public:
	void DisplayNCConfigTypeList(NCConfigTypeListPtr pNCConfigTypeList);
	NCConfigTypeListPtr GetNCConfigTypeList() const { return m_pNCConfigTypeList; }

private:
	// init the column header of the list ctrl.
	void InitGridStyle();

	// init the content of the list ctrl.
	void InitGridData();

	// release the data which stored in grid.
	void ReleaseGridItemData();

	// add a row.
	void AddNewRow(NCConfigTypePtr pNCConfigType);

	CString GetDataTypeName(BASIC_DATA_TYPE ncConfigDataType) const;

private: // controls
	SkinGridListCtrlGroups m_ncConfigList;

	// the popup.
	CMenu m_menu;

private:
	NCConfigTypeListPtr m_pNCConfigTypeList;
};

END_CUTLEADER_NAMESPACE()

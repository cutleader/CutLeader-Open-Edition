#pragma once

#include "ClDataManagerCommon.h"
#include "DataProperty.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(DataCenterItemList)

BEGIN_CUTLEADER_NAMESPACE()

// this page will display the items.
class ClDataManager_Export ItemListPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(ItemListPage)

public:
	ItemListPage();
	~ItemListPage();

	enum { IDD = IDD_clDataManagerUI_ItemList };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

public:
	// display node list.
	void DisplayItemList(DataCenterItemListPtr pDataCenterItemList);

private:
	// init the column header of the list ctrl.
	void InitGridStyle();

	// init the content of the list ctrl.
	void InitGridData();

private: // controls
	SkinGridListCtrlGroups m_dataCenterItemGrid;

private: // data member.
	DataCenterItemListPtr m_pDataCenterItemList;
};

END_CUTLEADER_NAMESPACE()

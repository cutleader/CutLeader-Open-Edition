#pragma once

#include "clDataManagerCommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(Part)

BEGIN_CUTLEADER_NAMESPACE()

// this page will display the part info.
// notes:
//   1) if we changed some props in "Basic info" group, we can get the changes from "m_pPart", so we can save these props outside the dialog.
class ClDataManager_Export PartInfoPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(PartInfoPage)

public:
	PartInfoPage();
	~PartInfoPage();

	enum { IDD = IDD_clDataManagerUI_PartInfo };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected: // message mapping.
	afx_msg void OnKillPartName();
	afx_msg void OnKillPartComment();

	afx_msg void OnAddCamData();
	afx_msg void OnRepairCamData();
	afx_msg void OnDeleteCamData();

	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg LRESULT GetCellEditorData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

public:
	// display the part that will be edited.
	void DisplayPartInfo(PartPtr pPart);

	// not all data of this part has been loaded.
	PartPtr GetPart() { return m_pPart; }

	// update cam data grid.
	void UpdateCamDataGrid() { InitCAMGridData(); }

	// update the control status.
	void UpdateCtrlStatus();

	// update the modify date.
	// notes:
	//   1) get the modify date from part object.
	void UpdateModifyDate();

	// release the data which stored in grid.
	void ReleaseGridItemData();

private:
	// init the column header of the list ctrl.
	void InitGridStyle();

	// init the content of the list ctrl.
	void InitCAMGridData();
	void InitTaskGridData();
	void InitExtPropGridData();

	// preview part
	void PreviewPart();

private: // controls
	// basic info group.
	CString m_strPartName;
	CString m_strComment;
	CString m_strCreateDate;
	CString m_strModifyDate;

	// "cam data" group.
	SkinInvCellGrid m_partCamDataGrid;

	// window for preview.
	CStatic m_previewWnd;

	SkinGridListCtrlGroups m_taskList;
	SkinGridListCtrlGroups m_extPropList;

	SkinMFCButton m_btnAdd;
	SkinMFCButton m_btnRepair;
	SkinMFCButton m_btnDelete;

	// use this brush to paint the background.
	CBrush m_brush;

private: // data member
	// the part.
	// notes:
	//   1) this may be used outside this dialog.
	//   2) this variable only include basic info and the preview data.
	PartPtr m_pPart;
};

END_CUTLEADER_NAMESPACE()

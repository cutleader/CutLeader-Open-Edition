#pragma once

#include "clDataManagerCommon.h"
#include "taoresource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// 零件修复对话框。
// 注：
//  1) 如果某些“工艺数据”丢失了对应的“参数配置”信息，就需要修复。
class ClDataManager_Export PartRepairDlg : public CLDialog
{
	DECLARE_DYNCREATE(PartRepairDlg)

public:
	PartRepairDlg();
	~PartRepairDlg(void);

	enum { IDD = IDD_clDataManagerUI_PartRepair };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg LRESULT GetCellEditorData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBnClickedOk();

	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()

public:
	void SetPart(LONGLONG iPartID) { m_iPartID = iPartID; }

private:
	// init the column header of the list ctrl.
	void InitGridStyle();

	// init the content of the list ctrl.
	void InitCAMGridData();

	// release the data which stored in grid.
	void ReleaseGridItemData();

private: // controls
	CString m_strMsg;

	SkinInvCellGrid m_partCamDataGrid;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private: // data member.
	LONGLONG m_iPartID;
};

END_CUTLEADER_NAMESPACE()

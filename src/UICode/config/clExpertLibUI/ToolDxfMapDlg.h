#pragma once

#include "clExpertLibUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(DxfLayerToolMap)

BEGIN_CUTLEADER_NAMESPACE()

class ToolDxfMapDlg : public CLDialog
{
	DECLARE_DYNCREATE(ToolDxfMapDlg)

public:
	ToolDxfMapDlg();
	~ToolDxfMapDlg(void);

	enum { IDD = IDD_clExpertLibUI_ToolDxfMap };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnDestroy();

	afx_msg void OnLayerGrid_endLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColorGrid_endLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT GetCellEditorData(WPARAM wParam, LPARAM lParam);

	afx_msg void OnMapByLayerClicked();
	afx_msg void OnMapByColorClicked();
	afx_msg void OnMapNoneClicked();

	afx_msg void OnAddRule();
	afx_msg void OnDeleteRule();
	afx_msg void OnOkClicked();
	afx_msg void OnCancelClicked();

	DECLARE_MESSAGE_MAP()

public:
	void InitDlg(ToolAssignConfigItemPtr pToolAssignConfigItem) { m_pToolAssignConfigItem = pToolAssignConfigItem; }

private:
	// 初始化两个grid的样式。
	void InitGridStyle();

	// 更新两个列表的可见状态
	void UpdateStatus_of_twoGrids();

	// 检查图层名称重复
	bool LayerName_exist(CString strName);
	CString CheckLayerName(CString strName); // 检查要是重名，就加后缀1,2,3...

	// 检查颜色索引重复
	bool LayerColor_exist(int iColorIndex);
	int CheckLayerColor(int iColorIndex); // 检查要是重名，就累加1

private: // controls
	int m_nDxfMapType;
	SkinGridListCtrlGroups m_gridList_layerMap;
	SkinGridListCtrlGroups m_gridList_colorMap;
	SkinGridListCtrlGroups m_gridListNoneMap;

	SkinMFCButton m_btnAddRule;
	SkinMFCButton m_btnDeleteRule;
	SkinMFCButton m_btnDXFMapOk;
	SkinMFCButton m_btnDXFMapCancel;

private: // data
	ToolAssignConfigItemPtr m_pToolAssignConfigItem;
};

END_CUTLEADER_NAMESPACE()

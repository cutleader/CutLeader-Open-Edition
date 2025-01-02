#pragma once

#include "ClTaskUICommon.h"
#include "taoresource.h"
#include "CGridListCtrlGroups.h"
#include "DataProperty.h"
#include <vector>


BEGIN_CUTLEADER_NAMESPACE()


// 当保存任务时，用这个对话框显示库存不足的材料。
class ClTaskUI_Export MaterialStockWarningDlg : public CLDialog
{
	DECLARE_DYNCREATE(MaterialStockWarningDlg)

public:
	MaterialStockWarningDlg();
	~MaterialStockWarningDlg(void);

	enum { IDD = IDD_clTaskUI_MaterialStockWarning };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    afx_msg void OnIncreaseStockAndSave();
    afx_msg void OnCancelSave();

	DECLARE_MESSAGE_MAP()

public:
    void Set_MaterialData_NoEnoughStock(const std::vector<std::pair<DataProperty, int>>& materialData_noEnoughStock) { m_materialData_noEnoughStock = materialData_noEnoughStock; }

private: // controls
	CString m_strMsg;

	CGridListCtrlGroups m_grid_MaterialNoEnoughStock;

	// use this brush to paint the background.
	CBrush m_brush;

private: // data member.
    // 没有足够库存的材料，<材料信息，期望的张数>
	std::vector<std::pair<DataProperty, int>> m_materialData_noEnoughStock;
};

END_CUTLEADER_NAMESPACE()

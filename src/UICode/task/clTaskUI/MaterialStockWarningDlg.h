#pragma once

#include "ClTaskUICommon.h"
#include "taoresource.h"
#include "CGridListCtrlGroups.h"
#include "DataProperty.h"
#include <vector>


BEGIN_CUTLEADER_NAMESPACE()


// ����������ʱ��������Ի�����ʾ��治��Ĳ��ϡ�
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
    // û���㹻���Ĳ��ϣ�<������Ϣ������������>
	std::vector<std::pair<DataProperty, int>> m_materialData_noEnoughStock;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "ClTaskUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "ClNestJobCommon.h"
#include "SkinManager.h"


BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框用于批量修改任务零件信息。
class BatchEditTaskPartDlg : public CLDialog
{
	DECLARE_DYNAMIC(BatchEditTaskPartDlg)

public:
	BatchEditTaskPartDlg(CWnd* pParent = NULL);
	virtual ~BatchEditTaskPartDlg(void);

	enum { IDD = IDD_clTaskUI_BatchEditTaskPart };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnCheckPlanningCount();
	afx_msg void OnNestingRotateAngle();
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	BOOL IsEnablePlanningCount() const { return m_bEnablePlanningCount; }
	int GetPlanningCount() const { return m_iPlanningCount; }

	BOOL IsEnableNestingRotateAngle() const { return m_bEnableNestingRotateAngle; }
	PartRotateType GetNestingRotateAngle() const { return m_emNestingRotateAngle; }

private: // controls.
	BOOL m_bEnablePlanningCount;
	CNumEdit m_editBoxPlanningCount;
	BOOL m_bEnableNestingRotateAngle;
	CComboBox m_comboBoxNestingRotateAngle;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

	CBrush m_brush;

private: // data
	int m_iPlanningCount;
	PartRotateType m_emNestingRotateAngle;
};

END_CUTLEADER_NAMESPACE()

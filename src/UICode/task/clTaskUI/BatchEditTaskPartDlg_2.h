#pragma once

#include "ClTaskUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "ClNestJobCommon.h"
#include "SkinManager.h"


BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框用于批量修改任务零件信息。
class BatchEditTaskPartDlg_2 : public CLDialog
{
	DECLARE_DYNAMIC(BatchEditTaskPartDlg_2)

public:
	BatchEditTaskPartDlg_2(CWnd* pParent = NULL);
	virtual ~BatchEditTaskPartDlg_2(void);

	enum { IDD = IDD_clTaskUI_BatchEditTaskPart_2 };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnNestingRotateAngle();
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	BOOL IsEnableNestingRotateAngle() const { return m_bEnableNestingRotateAngle; }
	PartRotateType GetNestingRotateAngle() const { return m_emNestingRotateAngle; }

private: // controls.
	BOOL m_bEnableNestingRotateAngle;
	CComboBox m_comboBoxNestingRotateAngle;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

	CBrush m_brush;

private: // data
	PartRotateType m_emNestingRotateAngle;
};

END_CUTLEADER_NAMESPACE()

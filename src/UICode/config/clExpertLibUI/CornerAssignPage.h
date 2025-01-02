#pragma once

#include "clExpertLibUICommon.h"
#include "taoresource.h"
#include "NumEdit.h"
#include "CornerDrawStatic.h"
#include "CLResizableDialog.h"

DECLARE_CUTLEADER_CLASS(CornerConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLibUI_Export CornerAssignPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(CornerAssignPage)

public:
	CornerAssignPage(void);
	~CornerAssignPage(void);

	enum { IDD = IDD_clExpertLibUI_Corner };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnSelchangeRangeType();
    afx_msg void OnCheckCornerLineLength();
    afx_msg void OnKillCornerLineLength();

	afx_msg void OnSelchangeCornerType();
	afx_msg void OnKillInCornerVal();

	afx_msg void OnSelchangeMCornerType();
	afx_msg void OnKillInMCornerVal();

	DECLARE_MESSAGE_MAP()

public:
	void DisplayCornerConfigItem(CornerConfigItemPtr pCornerConfigItem);
	CornerConfigItemPtr GetCornerConfigItem() { return m_pCornerConfigItem; }

private:
	void LayoutCtrl();

	void InitRangeList();

	// init others according to the corner range.
	void InitByRange( CORNER_RANGE_TYPE rangeType );

	// get the range corner info which is being edited currently.
	CornerRangeInfoPtr GetCurRangeCorner();

	// 初始化手工分配分组。
	void InitManualGroup();

private: // controls
	// “自动分配”分组。
    CString m_strMsg;
	CListBox m_rangeType;
	CComboBox m_iCornerType;
	CNumEdit m_cornerVal;
	BOOL m_bEnableCornerLineLengthControl;
    CNumEdit m_dCornerLineLength;
	CornerDrawStatic m_cornerPreview;

	// “手工分配”分组。
	CComboBox m_manualCornerType;
	CNumEdit m_manualCornerVal;

private:
	CornerConfigItemPtr m_pCornerConfigItem;
};

END_CUTLEADER_NAMESPACE()

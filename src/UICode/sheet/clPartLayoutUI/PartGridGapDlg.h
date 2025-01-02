#pragma once

#include "clPartLayoutUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// 设置阵列中零件间距。
class ClPartLayoutUI_Export PartGridGapDlg : public CLDialog
{
	DECLARE_DYNCREATE(PartGridGapDlg)

public:
	PartGridGapDlg(CWnd* pParent = NULL);
	~PartGridGapDlg(void);

	enum { IDD = IDD_clPartLayoutUI_GridGap };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnCheckComcut();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	double GetPartDis(BOOL& bEnableComcut);

private: // controls

	// 零件间距。
	CNumEdit m_ctrlPartDis;
	BOOL m_bComCut;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private:
	double m_dPartDis;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clCutSequenceUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequenceUI_Export SequenceSortDlg : public CLDialog
{
	DECLARE_DYNCREATE(SequenceSortDlg)

public:
	SequenceSortDlg();
	~SequenceSortDlg(void);

	enum { IDD = IDD_clCutSequenceUI_SortSequence };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	BOOL IsSortFromStart() { return m_bSortFromStart; }

private: // controls
	BOOL m_bSortFromStart;
	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;
};

END_CUTLEADER_NAMESPACE()

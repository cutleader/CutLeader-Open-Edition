#pragma once

#include "clGeometryFeatureUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// 回路偏移对话框。
class PatternLoopOffsetDlg : public CLDialog
{
	DECLARE_DYNCREATE(PatternLoopOffsetDlg)

public:
	PatternLoopOffsetDlg();
	~PatternLoopOffsetDlg(void);

	enum { IDD = IDD_clGeometryFeatureUI_OffsetLoop };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnBnClickedOk();
	afx_msg void OnInside();
	afx_msg void OnOutside();
	afx_msg void OnKillOffset();

	DECLARE_MESSAGE_MAP()

public:
	double GetOffsetVal() { return m_dOffsetVal; }
	BOOL IsInside() { return m_nSide == 0; }

private: // controls
	int m_nSide;
	CNumEdit m_ctrlOffset;
	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	double m_dOffsetVal;
};

END_CUTLEADER_NAMESPACE()

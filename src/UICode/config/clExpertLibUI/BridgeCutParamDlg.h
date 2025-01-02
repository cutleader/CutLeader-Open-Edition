#pragma once

#include "clExpertLibUICommon.h"
#include "NumEdit.h"
#include "Rect2D.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(BridgeCutParam)

BEGIN_CUTLEADER_NAMESPACE()

class BridgeCutParamDlg : public CLDialog
{
	DECLARE_DYNCREATE(BridgeCutParamDlg)

public:
	BridgeCutParamDlg();
	BridgeCutParamDlg(BridgeCutParamPtr pBdgeCutParam);
	~BridgeCutParamDlg(void);

	enum { IDD = IDD_clExpertLibUI_BridgeCut_Config };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

private:
	void DisplayComCutParam();

private: // controls
	CNumEdit m_dBdgeWidth;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	BridgeCutParamPtr m_pBdgeCutParam;
};

END_CUTLEADER_NAMESPACE()

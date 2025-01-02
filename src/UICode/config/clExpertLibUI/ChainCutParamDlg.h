#pragma once

#include "clExpertLibUICommon.h"
#include "NumEdit.h"
#include "Rect2D.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ChainCutParam)

BEGIN_CUTLEADER_NAMESPACE()

class ChainCutParamDlg : public CLDialog
{
	DECLARE_DYNCREATE(ChainCutParamDlg)

public:
	ChainCutParamDlg();
	ChainCutParamDlg(ChainCutParamPtr pChainCutParam);
	~ChainCutParamDlg(void);

	enum { IDD = IDD_clExpertLibUI_ChainCutConfig };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

private:
	void DisplayChainCutParam();

private: // controls
	BOOL m_bCutAlongContour;
	CNumEdit m_dAvoidDis;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	ChainCutParamPtr m_pChainCutParam;
};

END_CUTLEADER_NAMESPACE()

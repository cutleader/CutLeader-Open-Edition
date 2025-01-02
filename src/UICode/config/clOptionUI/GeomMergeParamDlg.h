#pragma once

#include "clOptionUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ImpExpConfig)

BEGIN_CUTLEADER_NAMESPACE()

class GeomMergeParamDlg : public CLDialog
{
	DECLARE_DYNCREATE(GeomMergeParamDlg)

public:
	GeomMergeParamDlg();
	GeomMergeParamDlg(ImpExpConfigPtr pImpExpConfig);

	~GeomMergeParamDlg(void);

	enum { IDD = IDD_clOptionUI_MergeConfig };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

private: // controls
	CNumEdit m_ctrlMergeCount;
	CNumEdit m_ctrlMergeDis;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	ImpExpConfigPtr m_pImpExpConfig;
};

END_CUTLEADER_NAMESPACE()

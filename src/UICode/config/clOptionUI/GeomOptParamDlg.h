#pragma once

#include "clOptionUICommon.h"
#include "NumEdit.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ImpExpConfig)

BEGIN_CUTLEADER_NAMESPACE()

class GeomOptParamDlg : public CLDialog
{
	DECLARE_DYNCREATE(GeomOptParamDlg)

public:
	GeomOptParamDlg();
	GeomOptParamDlg(ImpExpConfigPtr pImpExpConfig);

	~GeomOptParamDlg(void);

	enum { IDD = IDD_clOptionUI_OptimizeConfig };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

private: // controls
	CNumEdit m_ctrlAllowOffset;
	CNumEdit m_dPatFilterSize;
	int m_nMoreArc;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private:
	ImpExpConfigPtr m_pImpExpConfig;
};

END_CUTLEADER_NAMESPACE()

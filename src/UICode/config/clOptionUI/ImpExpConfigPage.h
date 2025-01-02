#pragma once

#include "clOptionUICommon.h"
#include "taoresource.h"
#include "NumEdit.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(ImpExpConfig)

BEGIN_CUTLEADER_NAMESPACE()

class ImpExpConfigPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(ImpExpConfigPage)

public:
	ImpExpConfigPage();
	~ImpExpConfigPage();

	enum { IDD = IDD_clOptionUI_ImportExport_Option };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnGeomOptAdvConfig();
	afx_msg void OnGeomMergeConfig();

	DECLARE_MESSAGE_MAP()

public:
	void DisplayImpExpConfig(ImpExpConfigPtr pImpExpConfig);

	// fill the ImpExpConfig using the user input
	ImpExpConfigPtr GetImpExpConfig();

private: // controls
	// 导入选项。
	CComboBox m_ctrlImpUnit;
	CNumEdit m_dImpTol;
	CNumEdit m_dLoopFilterSize;
	CNumEdit m_iOptimizeLimit;
	CNumEdit m_iIgnoreCount;
	SkinMFCButton m_btnOptConfig;

	// 导出选项。
	CComboBox m_ctrlExpUnit;
	CComboBox m_ctrlDxfVer;
	BOOL m_bExpShtBorder;
	SkinMFCButton m_btnMergeGeom;


private:
	ImpExpConfigPtr m_pImpExpConfig;
};

END_CUTLEADER_NAMESPACE()

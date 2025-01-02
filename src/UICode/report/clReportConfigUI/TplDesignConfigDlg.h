#pragma once

#include "taoResource.h"
#include "CLResizableDialog.h"
#include "TplParamPage.h"
#include "TplDisplayParamPage.h"
#include "DesignParamPage.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(TplDesignConfig)

BEGIN_CUTLEADER_NAMESPACE()

typedef enum tagConfigType
{
	CONFIG_NOT_INIT		= -1,

	CONFIG_TEMPLATE		= 1,
	CONFIG_DISPLAY		= 2,
	CONFIG_DESIGN		= 3,

} CONFIG_TYPE;

class ClReportConfigUI_Export TplDesignConfigDlg : public CLResizableDialog
{
public:
	TplDesignConfigDlg(void);

	~TplDesignConfigDlg(void);

	enum { IDD = IDD_clReportConfigUI_Config };

protected: // inherit the interface
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public: // message mapping.
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnTemplateConfig();
	afx_msg void OnDisplayConfig();
	afx_msg void OnDesignConfig();

	afx_msg void OnSave();
	afx_msg void OnQuit();
	DECLARE_MESSAGE_MAP()

private:
	/************************************************************************/
	// about sub-pages.

	//
	void InitSubDialog();

	// hide all sub-dialogs.
	void HideAllSubDialog();

	void RepositionSubDlg();
	/************************************************************************/

	void CheckModification();

private: // for UI.
	TplParamPage m_tplParamPage;
	TplDisplayParamPage m_TplDisplayParamPage;
	DesignParamPage m_designParamPage;

	SkinMFCButton m_btnTemplate;
	SkinMFCButton m_btnDisplay;
	SkinMFCButton m_btnDesign;
	SkinMFCButton m_btnSave;
	SkinMFCButton m_btnQuit;
	SkinMFCButton m_btnSeperator;

private: // for data
	TplDesignConfigPtr m_pTplDesignConfig;

	// the current page type.
	CONFIG_TYPE m_iConfigType;
};

END_CUTLEADER_NAMESPACE()

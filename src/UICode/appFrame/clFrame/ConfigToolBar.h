#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class ConfigToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(ConfigToolBar)

public:
	ConfigToolBar();
	~ConfigToolBar();

	enum { IDD = IDD_clFrame_ConfigToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnSystemConfig();
	afx_msg void OnMatLib();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnSystemConfig;
	SkinMFCButton m_btnMatLib;
};

END_CUTLEADER_NAMESPACE()

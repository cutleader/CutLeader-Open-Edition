﻿#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class StartCutPtToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(StartCutPtToolBar)

public:
	StartCutPtToolBar();
	~StartCutPtToolBar();

	enum { IDD = IDD_clFrame_StartCutPtToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnResetStartCutPt();

	afx_msg void OnSwitchCutDirection();
	afx_msg void OnCcwCutDirection();
	afx_msg void OnCwCutDirection();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnResetStartCutPt;

	SkinMFCButton m_btnSwitchCutDirection;
	SkinMFCButton m_btnCcwCutDirection;
	SkinMFCButton m_btnCwCutDirection;
};

END_CUTLEADER_NAMESPACE()
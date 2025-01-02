#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class LeadLineToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(LeadLineToolBar)

public:
	LeadLineToolBar();
	~LeadLineToolBar();

	enum { IDD = IDD_clFrame_LeadLineToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnResetLeadLine();
	afx_msg void OnLoopGap();

	afx_msg void OnCutOutSide();
	afx_msg void OnCutInSide();
	afx_msg void OnCutOn();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnResetLeadLine;
	SkinMFCButton m_btnLoopGap;

	SkinMFCButton m_btnCutOutSide;
	SkinMFCButton m_btnCutInSide;
	SkinMFCButton m_btnCutOn;
};

END_CUTLEADER_NAMESPACE()

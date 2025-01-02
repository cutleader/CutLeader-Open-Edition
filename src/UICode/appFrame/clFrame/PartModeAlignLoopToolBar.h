#pragma once

#include "clFrameCommon.h"
#include "SubToolBar.h"
#include "taoResource.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

class PartModeAlignLoopToolBar : public SubToolBar
{
	DECLARE_DYNCREATE(PartModeAlignLoopToolBar)

public:
	PartModeAlignLoopToolBar();
	~PartModeAlignLoopToolBar();

	enum { IDD = IDD_clFrame_PartModeAlignLoopToolBar };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnLeftAlignLoop();
	afx_msg void OnRightAlignLoop();
	afx_msg void OnHorizontalCenterAlignLoop();
	afx_msg void OnTopAlignLoop();
	afx_msg void OnBottomAlignLoop();
	afx_msg void OnVerticalCenterAlignLoop();

	DECLARE_MESSAGE_MAP()

private: // controls
	SkinMFCButton m_btnLeftAlignLoop;
	SkinMFCButton m_btnRightAlignLoop;
	SkinMFCButton m_btnHorizontalCenterAlignLoop;
	SkinMFCButton m_ctrlTopAlignLoop;
	SkinMFCButton m_ctrlBottomAlignLoop;
	SkinMFCButton m_ctrlVerticalCenterAlignLoop;
};

END_CUTLEADER_NAMESPACE()

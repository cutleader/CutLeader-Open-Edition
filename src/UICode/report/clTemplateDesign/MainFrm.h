#pragma once

#include "cutLeaderCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();

	afx_msg LRESULT OnShowRibbonItemMenu(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnUpdateStatusBar(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	void InitializeRibbon();

protected:  // control bar embedded members
	CMFCRibbonBar m_ribbonBar;
	CMFCRibbonApplicationButton m_mainButton;
	CMFCRibbonStatusBar m_statusBar;
};

END_CUTLEADER_NAMESPACE()

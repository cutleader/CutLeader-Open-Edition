#pragma once

#include "cutLeaderCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// this message will be sent to app to let app open a library.
// notes:
//   1) put the tab name in wParam.
#define WM_OPEN_LIB WM_USER + 0x4E06

// the main frame of the application.
class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();

	virtual ~CMainFrame();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

	afx_msg void OnWindowManager();

	DECLARE_MESSAGE_MAP()

private:
	void InitializeRibbon();

protected:
	CMFCRibbonBar m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
};

END_CUTLEADER_NAMESPACE()

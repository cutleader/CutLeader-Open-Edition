#pragma once

#include "clFrameCommon.h"
#include "clFrameResource.h"
#include "taoresource.h"

BEGIN_CUTLEADER_NAMESPACE()

class CLFRAME_Export CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

	afx_msg LRESULT OnChildFrmClose(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnShowRibbonItemMenu(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

public:
	// 设置tab标签的风格。
	void SetTabControlStyle();

	// 更新主窗口的关闭按钮。
	void UpdateMainWindowCloseButton();
};

END_CUTLEADER_NAMESPACE()

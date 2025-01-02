#pragma once

#include "clFrameCommon.h"
#include "XBarBase.h"
#include "PartFunctionToolBar.h"
#include "SheetFunctionToolBar.h"
#include "CanvasFunctionToolBar.h"

BEGIN_CUTLEADER_NAMESPACE()

// 任务模式下的常用功能面板。
class TaskCommonFunctionPanel : public XBarBase
{
public:
	TaskCommonFunctionPanel();
	virtual ~TaskCommonFunctionPanel();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	static TaskCommonFunctionPanel* GetInstance(CWnd* pParent = NULL);

public:
	void ShowCanvasFunctionToolBar();
	void ShowSheetFunctionToolBar();

private: // for controls
	static TaskCommonFunctionPanel* m_pTaskCommonFunctionPanel;

private: // data members.
	SheetFunctionToolBar m_sheetFunctionToolBar;
	CanvasFunctionToolBar m_canvasFunctionToolBar;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clFrameCommon.h"
#include "XBarBase.h"
#include "PartFunctionToolBar.h"
#include "SheetFunctionToolBar.h"


BEGIN_CUTLEADER_NAMESPACE()

// 常用功能面板。
class PartCommonFunctionPanel : public XBarBase
{
public:
	PartCommonFunctionPanel();
	virtual ~PartCommonFunctionPanel();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	static PartCommonFunctionPanel* GetInstance(CWnd* pParent = NULL);

public:
	PartFunctionToolBar* GetPartFunctionToolBar() { return &m_partFunctionToolBar; }

private: // for controls
	static PartCommonFunctionPanel* m_pPartCommonFunctionPanel;

private: // data members.
	PartFunctionToolBar m_partFunctionToolBar;
};

END_CUTLEADER_NAMESPACE()

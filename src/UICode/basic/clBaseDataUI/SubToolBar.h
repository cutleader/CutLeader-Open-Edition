#pragma once

#include "clBaseDataUICommon.h"
#include "CLDialog.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// 子工具条，当鼠标移到“顶级工具条”上面某些按钮上是，会显示“子工具条”。
class ClBaseDataUI_Export SubToolBar : public CLDialog
{
	DECLARE_DYNCREATE(SubToolBar)

public:
	SubToolBar();
	SubToolBar(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	~SubToolBar();

protected: // inherit from the super class.
	virtual void OnCancel() { return; }
	virtual void PostNcDestroy();

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE()

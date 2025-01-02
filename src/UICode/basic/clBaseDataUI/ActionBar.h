#pragma once

#include "clBaseDataUICommon.h"
#include "CLDialog.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

// action bar的基类。
// 注：
// 1) action bar一般显示在视图右上角。有的action bar不是总是显示在视图右上角，比如文字参数action bar，这种情况下就需要对应的action bar各自处理。
class ClBaseDataUI_Export ActionBar : public CLDialog
{
	DECLARE_DYNCREATE(ActionBar)

public:
	ActionBar();
	ActionBar(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	~ActionBar();

protected: // inherit from the super class.
	virtual void OnCancel() { return; }

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

public:
	void HideActionBar();

protected:
	// 把action bar显示在视图的右上角。
	// 注：
	// 1) 外面一般不需要直接调用这个函数，因为这个函数只显示位置，并没有设置action bar上面的数据。
	void PositionActionBar();
};

END_CUTLEADER_NAMESPACE()

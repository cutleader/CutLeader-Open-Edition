#pragma once

#include "clBaseDataUICommon.h"
#include "CLDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(SubToolBar)

BEGIN_CUTLEADER_NAMESPACE()

// 顶级工具条，这种工具条一般是对应零件视图、底图、板材的，就是那几个按钮竖着排列的工具条。
class ClBaseDataUI_Export TopToolBar : public CLDialog
{
	DECLARE_DYNCREATE(TopToolBar)

public:
	TopToolBar();
	TopToolBar(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	~TopToolBar();

protected: // inherit from the super class.
	virtual void OnCancel() { return; }

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

protected:
	// 检查是否显示该工具条。
	void CheckWhetherDisplaySubToolBar(const SkinMFCButton* pButton, SubToolBar* pSubToolBar);

protected:
	// about scroll bar.
	CRect m_rcOriginalRect; // dialog size as you see in the resource view (original size)
	int m_nScrollPos; // actual scroll position
	int m_nCurHeight; // actual dialog height

	// 定时器，用来显示“子工具条”。
	int m_nTimer;
};

END_CUTLEADER_NAMESPACE()

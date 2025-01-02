#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

extern ClUILib_Export UINT rwm_EXLISTBOX_DBLCLICK;
extern ClUILib_Export UINT rwm_EXLISTBOX_DELETE;
extern ClUILib_Export UINT rwm_EXLISTBOX_SELCHANGE;

// a new list box class.
// notes:
//   1) adds dragging to a listbox. It also sends registered messages to the owner
//		when DEL is pressed, when a line is double clicked and when the selection is changed 
//      (the last two as the dragging kills the normal notification mechanism).
//   2) sets the tab-position to half the horizontal width, and automatically sets the listbox scrollbar width when adding a string.
class ClUILib_Export ListBoxEx : public CListBox
{
public:
	ListBoxEx(void);
	~ListBoxEx(void);

protected:
	virtual void PreSubclassWindow();

protected:
	afx_msg void OnDblclk();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()

public:
	int AddString( LPCTSTR str );

private:
	int m_iDraggedLine;
};

END_CUTLEADER_NAMESPACE()

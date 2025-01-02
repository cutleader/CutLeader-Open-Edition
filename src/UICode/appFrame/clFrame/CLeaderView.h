#pragma once

#include "cLeaderDoc.h"
#include "taoresource.h"
#include "GlViewPort.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(IEditor)
DECLARE_CUTLEADER_CLASS(CommandList)
DECLARE_CUTLEADER_CLASS(ICommand)
DECLARE_CUTLEADER_CLASS(IAction)

BEGIN_CUTLEADER_NAMESPACE()

// this basic view class.
class CLFRAME_Export CLeaderView : public CView
{
	DECLARE_DYNCREATE(CLeaderView)

public:
	CLeaderView();
	virtual ~CLeaderView();

public: // inherit from the super class.
	virtual void OnDraw(CDC* pDC);

public: // interfaces defined.
	// 更新视图当前的undo/redo列表。
	virtual void UpdateUndoRedoDropdownList(CMFCRibbonBaseElement* pElem) {}

    // 子类可重载这个函数，在这个命令的Undo/redo后继续一些事情。
    virtual void ProcessAfterUndo(const ICommand* pCommand) {}
	virtual void ProcessAfterRedo(const ICommand* pCommand) {}

public:
	afx_msg void OnDestroy();

	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();

	afx_msg LRESULT OnDisplayPromptMessageOnStatusBar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayCursorPositionOnStatusBar(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public: // get/set functions.
	CLeaderDoc* GetDocument() const { return reinterpret_cast<CLeaderDoc*>(m_pDocument); }

	// get the drawer of the view.
	GlViewPortPtr GetDrawer() const { return m_pViewPort; }

protected:
	// 给视图发出一个异步消息，让它把这个action的提示消息显示在状态栏。
	void DisplayPromptMessageOfAction(const IAction* pAction);

protected:
	// current editor
	IEditorPtr m_pCurEditor;

	// current drawer.
	GlViewPortPtr m_pViewPort;

	// the undo/redo items of current view.
	// notes:
	//   1) in "UpdateUndoRedoDropdownList", we should update these variables.
	//   2) command items in these variables have the same order with those in "CommandManager".
	CommandListPtr m_pUndoCmdList;
	CommandListPtr m_pRedoCmdList;

	// indicate now the popup is displayed.
	BOOL m_bPopupShow;

	// for pan the view.
	CPoint m_OffSetPoint;

	// 这里记录状态栏上最后一次显示的提示消息，这样可以防止重复显示相同的提示信息，提高效率。
	int m_iStringID;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "CornerBoxWnd.h"
#include "HorzRulerWnd.h"
#include "VertRulerWnd.h"
#include "TplDesignWnd.h"

DECLARE_CUTLEADER_CLASS(RptCmdList)

BEGIN_CUTLEADER_NAMESPACE()

#define RULER_SIZE	24

class CtplDesignDoc;

class CtplDesignView : public CView
{
	DECLARE_DYNCREATE(CtplDesignView)

public:
	CtplDesignView();

	virtual ~CtplDesignView();

public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC*);

	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();

	// for draw entity.
	afx_msg void OnDrawLabel();
	afx_msg void OnDrawStatic();
	afx_msg void OnDrawField();
	afx_msg void OnDrawStaticImg();
	afx_msg void OnDrawDynamicImg();
	afx_msg void OnDrawGrid();
	afx_msg void OnDrawLine();
	afx_msg void OnDrawBox();
	afx_msg void OnDrawEllipse();
	afx_msg void OnDrawBarCode();

	// for edit entity.
	afx_msg void OnCopyEntity();
	afx_msg void OnCutEntity();
	afx_msg void OnPasteEntity();
	afx_msg void OnDeleteEntity();
	afx_msg void OnEditEntity();
	afx_msg void OnGroupEntity();
	afx_msg void OnUnGroupEntity();

	// for view page.
	afx_msg void OnNoZoomPage();
	afx_msg void OnZoomInPage();
	afx_msg void OnZoomOutPage();

	DECLARE_MESSAGE_MAP()

public:
	CtplDesignDoc* GetDocument() const { return reinterpret_cast<CtplDesignDoc*>(m_pDocument); }

	void UpdateUndoRedoDropdownList(CMFCRibbonBaseElement* pElem);

private: // for controls.
	CornerBoxWnd m_cornerBoxWnd;
	HorzRulerWnd m_horzRulerWnd;
	VertRulerWnd m_vertRulerWnd;
	TplDesignWnd m_tplDesignWnd;

private:
	/************************************************************************/
	// the undo/redo items of current view.
	// notes:
	//   1) in "UpdateUndoRedoDropdownList", we should update these variables.
	//   2) command items in these variables have the same order with those in "CommandManager".

	RptCmdListPtr m_undoCommandList;
	RptCmdListPtr m_redoCommandList;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()

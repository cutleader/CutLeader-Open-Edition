#pragma once

#include "cutLeaderCommon.h"
#include "ITplEntity.h"

DECLARE_CUTLEADER_CLASS(TplEditParam)
DECLARE_CUTLEADER_CLASS(TplEntityList)
DECLARE_CUTLEADER_CLASS(ReportTplPage)

BEGIN_CUTLEADER_NAMESPACE()

// the window where you can edit template entity.
// notes:
//   1) some rules:
//      a) we will draw an entity from left-top to bottom-right except for the Line entity.
//      b) user should draw entity within the paper margin, otherwise those entities will not be printed.

typedef enum tagActionType
{
	ACTION_NONE				= 0,
	ACTION_DRAWING			= 1,
	ACTION_RESIZING			= 2,
	ACTION_MOVING			= 3,
	ACTION_RUBBERBANDING	= 4,
} ACTION_TYPE;

#define WM_UPDATE_STATUS_BAR WM_USER + 0x4E96

class TplDesignWnd : public CWnd
{
public:
	TplDesignWnd(void);
	~TplDesignWnd(void);

protected:
	// figure out the cursor which will be displayed.
	virtual HCURSOR GetCursor();

	virtual void SetHScroll( int pos );
	virtual void SetVScroll( int pos );

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnCopyEntity();
	afx_msg void OnCutEntity();
	afx_msg void OnPasteEntity();
	afx_msg void OnDeleteEntity();
	afx_msg void OnEditEntity();
	afx_msg void OnNoZoom();
	afx_msg void OnGroupEntity();
	afx_msg void OnUnGroupEntity();

	DECLARE_MESSAGE_MAP()

public: // get/set functions.
	void SetReportTpl(ReportTplPagePtr pReportTplPage) { m_pReportTplPage = pReportTplPage; }

public:
	/************************************************************************/
	// about edit the entity.

	// copy the selected entities to clipboard.
	void CopySelectedEntity();

	// cut the selected entities to clipboard.
	void CutSelectedEntity();

	// paste the entities in clipboard to position.
	void PasteSelEntityToPt(CPoint pastePt);

	// delete the selected entities.
	void DelSelectedEntity();

	// edit the property of the entity.
	void EditEntityProp();

	// group the selected entities.
	void GroupEntity();

	// unGroup the selected group.
	void UnGroupEntity();
	/************************************************************************/

	/************************************************************************/
	// about view the page.

	void NoZoomPage();
	void ZoomInPage();
	void ZoomOutPage();
	/************************************************************************/

	// start draw action.
	void StartDrawAction(ENTITY_TYPE entityType);

	void RefreshForUndoRedo();

private:
	/************************************************************************/
	// about coordinate conversion.

	void ScreenToWorld(CPoint& point) const;
	void WorldToScreen(CPoint& point) const;
	void ScreenToWorld(CSize& size) const;
	void ScreenToWorld(CRect& rect) const;
	void WorldToScreen(CRect& rect) const;
	/************************************************************************/

	/************************************************************************/
	// about scroll bar.

	void SetupScrollbars();
	CPoint ScrollToPoint(CPoint point);
	int HScroll(int scroll);
	int VScroll(int scroll);
	/************************************************************************/

	int SnapX(int iCoord) const;
	int SnapY(int iCoord) const;

	// screen coordinate.
	ITplEntityPtr GetSelectedEntity(TplEntityListPtr pTplEntityList, CPoint pt);

private:
	/************************************************************************/
	//

	COLORREF m_paperBkColor;
	COLORREF m_noPaperBkColor;
	/************************************************************************/

	// Current zoom level
	double m_dZoomLevel;

	TplEditParamPtr m_pTplEditParam;

	// the current report template.
	ReportTplPagePtr m_pReportTplPage;

	// about selecting entity.
	TplEntityListPtr m_pSelEntityList;
	CRect m_selectionRect;

	ACTION_TYPE m_actionType;
	ENTITY_POSITION_PT m_entityPosPt;

	// about drawing entity.
	ITplEntityPtr m_pDrawingEntity;
	ENTITY_TYPE	m_drawingEntityType;

	// about moving entity.
	ITplEntityPtr m_pAnchorEntity;
	CPoint m_moveDeltaPt;

	// for modify the entities.
	// notes:
	//   1) "m_pNewEntityList" is the temp entities and have the same order with "m_pOldEntityList".
	TplEntityListPtr m_pOldEntityList;
	TplEntityListPtr m_pNewEntityList;

	// for copy/paste the entities.
	TplEntityListPtr m_pCopiedEntityList;
	CPoint m_pastePoint;
};

END_CUTLEADER_NAMESPACE()

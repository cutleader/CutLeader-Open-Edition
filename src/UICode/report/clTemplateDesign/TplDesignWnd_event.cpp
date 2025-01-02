#include "StdAfx.h"
#include "TplDesignWnd.h"

#include "XMenu.h"
#include "baseConst.h"
#include "UnitHelper.h"
#include "clBaseDataResource.h"
#include "clFrameResource.h"

#include "clReportResource.h"
#include "reportConst.h"
#include "TplDisplayParam.h"
#include "TplDisplayParamLoader.h"
#include "TplEditParam.h"
#include "TplDesignConfig.h"

#include "EntityGroup.h"
#include "EntityGroupList.h"
#include "TplEntityList.h"
#include "ReportTplPage.h"
#include "ReportTplDrawer.h"
#include "TplEntityDrawer.h"
#include "TplEntityMgr.h"

#include "AddEntityCommand.h"
#include "MoveEntityCommand.h"
#include "RszEntityCommand.h"
#include "PasteEntityCommand.h"
#include "DelEntityCommand.h"
#include "CutEntityCommand.h"
#include "RptCmdManager.h"

BEGIN_CUTLEADER_NAMESPACE()

BEGIN_MESSAGE_MAP(TplDesignWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()

	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()

	ON_WM_HSCROLL()
	ON_WM_VSCROLL()

	ON_COMMAND(IDC_ENTITY_COPY, OnCopyEntity)
	ON_COMMAND(IDC_ENTITY_CUT, OnCutEntity)
	ON_COMMAND(IDC_ENTITY_PASTE, OnPasteEntity)
	ON_COMMAND(IDC_ENTITY_DELETE, OnDeleteEntity)
	ON_COMMAND(IDC_ENTITY_PROP, OnEditEntity)
	ON_COMMAND(IDC_ENTITY_GROUP, OnGroupEntity)
	ON_COMMAND(IDC_ENTITY_UNGROUP, OnUnGroupEntity)

	ON_COMMAND(IDC_ENTITY_NO_ZOOM, OnNoZoom)

END_MESSAGE_MAP()

BOOL TplDesignWnd::OnEraseBkgnd( CDC* )
{
	return TRUE;
}

void TplDesignWnd::OnPaint()
{
	CPaintDC outputdc( this );

	// Getting coordinate data
	CRect rect;
	GetClientRect( &rect );

	SCROLLINFO sih;
	sih.cbSize = sizeof( SCROLLINFO );
	sih.fMask = SIF_POS;
	SCROLLINFO siv;
	siv.cbSize = sizeof( SCROLLINFO );
	siv.fMask = SIF_POS;
	if( !GetScrollInfo( SB_HORZ, &sih ) )
		sih.nPos = 0;
	if( !GetScrollInfo( SB_VERT, &siv ) )
		siv.nPos = 0;

	CRect totalRect;
	int iPaperWidth = m_pReportTplPage->GetPaperWidth();
	int iPaperHeight = m_pReportTplPage->GetPaperHeight();
	int virtwidth = (int)(iPaperWidth * m_dZoomLevel) + 1;
	int virtheight = (int)(iPaperHeight * m_dZoomLevel) + 1;
	totalRect.SetRect(0, 0, virtwidth, virtheight);

	// Creating memory CDC
	CDC dc;
	dc.CreateCompatibleDC( &outputdc );
	CBitmap bmp;
	bmp.CreateCompatibleBitmap( &outputdc, rect.right, rect.bottom );
	CBitmap* oldbmp = dc.SelectObject( &bmp );

	// Draws the paper and non-paper background
	dc.FillSolidRect( rect, m_noPaperBkColor );
	dc.FillSolidRect( totalRect, m_paperBkColor );

	dc.SetWindowOrg( sih.nPos, siv.nPos );

	/************************************************************************/
	// draw

	if (m_actionType == ACTION_NONE)
	{
		ReportTplDrawer::DrawReportTpl(m_pReportTplPage, m_pSelEntityList, &dc, totalRect, m_dZoomLevel);
		TplEntityDrawer::DrawEntity(m_pSelEntityList, &dc, m_dZoomLevel, TRUE);
	}
	else if (m_actionType == ACTION_DRAWING)
	{
		ReportTplDrawer::DrawReportTpl(m_pReportTplPage, &dc, totalRect, m_dZoomLevel);
		TplEntityDrawer::DrawEntity(m_pDrawingEntity, &dc, m_dZoomLevel, TRUE);
	}
	else if (m_actionType == ACTION_MOVING)
	{
		ReportTplDrawer::DrawReportTpl(m_pReportTplPage, m_pOldEntityList, &dc, totalRect, m_dZoomLevel);
		TplEntityDrawer::DrawEntity(m_pNewEntityList, &dc, m_dZoomLevel, TRUE);
	}
	else if (m_actionType == ACTION_RESIZING)
	{
		ReportTplDrawer::DrawReportTpl(m_pReportTplPage, m_pOldEntityList, &dc, totalRect, m_dZoomLevel);
		TplEntityDrawer::DrawEntity(m_pNewEntityList, &dc, m_dZoomLevel, TRUE);
	}
	else if (m_actionType == ACTION_RUBBERBANDING)
	{
		ReportTplDrawer::DrawReportTpl(m_pReportTplPage, m_pSelEntityList, &dc, totalRect, m_dZoomLevel);
		TplEntityDrawer::DrawEntity(m_pSelEntityList, &dc, m_dZoomLevel, TRUE);
	}
	/************************************************************************/

	// Blit the memory CDC to screen
	outputdc.BitBlt( 0, 0, rect.right, rect.bottom, &dc, sih.nPos, siv.nPos, SRCCOPY );
	dc.SelectObject( oldbmp );
}

void TplDesignWnd::OnSize( UINT nType, int cx, int cy )
{
	CWnd::OnSize( nType, cx, cy );

	SetupScrollbars();
}

void TplDesignWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// set focus.
	SetFocus();
	SetCapture();

	// get the world coordinate.
	CPoint worldPt(point);
	ScreenToWorld(worldPt);
	if (worldPt.x > m_pReportTplPage->GetPaperWidth() || worldPt.y > m_pReportTplPage->GetPaperHeight())
	{
		m_actionType = ACTION_NONE;
		return;
	}

	if (m_actionType == ACTION_NONE)
	{
		if (m_pSelEntityList->size() == 0)
		{
			// check whether this hit selected an entity.
			ITplEntityPtr pSelTplEntity = GetSelectedEntity(m_pReportTplPage->GetTplEntityList(), worldPt);
			if (pSelTplEntity)
			{
				// get the associated group and select the entities in it.
				EntityGroupListPtr pEntityGroupList = m_pReportTplPage->GetEntityGroupList();
				EntityGroupPtr pSelEntityGroup = pEntityGroupList->GetGroupByEntity(pSelTplEntity);
				if (pSelEntityGroup)
					m_pSelEntityList->AddEntity(pSelEntityGroup->GetTplEntityList());
				else
					m_pSelEntityList->push_back(pSelTplEntity);
			}
			else
			{
				// we will enter rubber banding action.
				m_actionType = ACTION_RUBBERBANDING;

				m_selectionRect.left = m_selectionRect.right = point.x;
				m_selectionRect.top = m_selectionRect.bottom = point.y;
			}
		}
		else
		{
			// some entities were selected currently.

			// whether the mouse click is outside all selected entities.
			ITplEntityPtr pSelTplEntity = GetSelectedEntity(m_pReportTplPage->GetTplEntityList(), worldPt);
			if (pSelTplEntity && m_pSelEntityList->GetEntityIndex(pSelTplEntity) == INVALID_IDX)
				pSelTplEntity.reset();
			if (!pSelTplEntity)
			{
				// if ctrl button is not pressed, de-select all selected entities.
				if (!(nFlags & MK_CONTROL))
					m_pSelEntityList.reset(new TplEntityList);

				// check whether the mouse click is inside any other entities.
				pSelTplEntity = GetSelectedEntity(m_pReportTplPage->GetTplEntityList(), worldPt);
				if (pSelTplEntity)
				{
					// get the associated group and select the entities in it.
					EntityGroupListPtr pEntityGroupList = m_pReportTplPage->GetEntityGroupList();
					EntityGroupPtr pSelEntityGroup = pEntityGroupList->GetGroupByEntity(pSelTplEntity);
					if (pSelEntityGroup)
						m_pSelEntityList->AddEntity(pSelEntityGroup->GetTplEntityList());
					else
						m_pSelEntityList->push_back(pSelTplEntity);
				}
			}
			else
			{
				// if ctrl button is pressed, de-select the selected entity.
				if (nFlags & MK_CONTROL)
				{
					// get the associated group and de-select the entities in it.
					EntityGroupListPtr pEntityGroupList = m_pReportTplPage->GetEntityGroupList();
					EntityGroupPtr pSelEntityGroup = pEntityGroupList->GetGroupByEntity(pSelTplEntity);
					if (pSelEntityGroup)
						m_pSelEntityList->DeleteEntity(pSelEntityGroup->GetTplEntityList());
					else
						m_pSelEntityList->DeleteEntity(pSelTplEntity);
				}
				else
				{
					// which position point was touched.
					ENTITY_POSITION_PT entityPosPt = pSelTplEntity->GetPtPosition(worldPt);
					if (entityPosPt == ENTITY_POSITION_BODY)
					{
						// prepare data for moving.
						m_pOldEntityList = m_pSelEntityList;
						m_pNewEntityList = boost::dynamic_pointer_cast<TplEntityList>(m_pSelEntityList->Clone());

						// set the anchor entity.
						int iIndex = m_pOldEntityList->GetEntityIndex(pSelTplEntity);
						m_pAnchorEntity = m_pNewEntityList->at(iIndex);

						// calc the distance to the left top corner of anchor entity.
						CPoint currentPt = point;
						ScreenToWorld(currentPt);
						CRect tmpRect = m_pAnchorEntity->GetBorderRect();
						m_moveDeltaPt.x = currentPt.x - tmpRect.left;
						m_moveDeltaPt.y = currentPt.y - tmpRect.top;

						// enter move action.
						::SetCursor(pSelTplEntity->GetCursorByPosPt(ENTITY_POSITION_BODY));
						m_actionType = ACTION_MOVING;
					}
					else
					{
						// if "pSelTplEntity" is within a group, cannot resize it.
						EntityGroupPtr pSelEntityGroup = m_pReportTplPage->GetEntityGroupList()->GetGroupByEntity(pSelTplEntity);
						if (!pSelEntityGroup)
						{
							// select only this entity.
							m_pSelEntityList->clear();
							m_pSelEntityList->push_back(pSelTplEntity);

							// prepare data for resizing.
							m_pOldEntityList = m_pSelEntityList;
							m_pNewEntityList = boost::dynamic_pointer_cast<TplEntityList>(m_pSelEntityList->Clone());

							// we will enter resize action.
							m_actionType = ACTION_RESIZING;
							m_entityPosPt = entityPosPt;
						}
					}
				}
			}
		}
	}
	else if (m_actionType == ACTION_DRAWING)
	{
		// check whether need to snap to the grid.
		if (m_pTplEditParam->IsSnapToGrid())
		{
			worldPt.x = SnapX(worldPt.x);
			worldPt.y = SnapY(worldPt.y);
		}

		m_pDrawingEntity = TplEntityMgr::BuildDflTplEntity(m_drawingEntityType);
		m_pDrawingEntity->SetBorderRect(CRect(worldPt.x, worldPt.y, worldPt.x, worldPt.y));
		m_pSelEntityList->clear();
	}

	RedrawWindow();
	CWnd::OnLButtonDown(nFlags, point);
}

void TplDesignWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Reset modes
	ReleaseCapture();

	if (m_actionType == ACTION_DRAWING)
	{
		// create command.
		TplEntityListPtr pTplEntityList = m_pReportTplPage->GetTplEntityList();
		IRptCmdPtr pCommand(new AddEntityCommand(GetParent(), pTplEntityList, m_pDrawingEntity));
		pCommand->Do();
		RptCmdManager::GetInstance()->AddCommand(pCommand);

		// reset the status.
		m_pSelEntityList->push_back(m_pDrawingEntity);
		m_pDrawingEntity.reset();
		m_actionType = ACTION_NONE;
	}
	else if (m_actionType == ACTION_MOVING)
	{
		// check whether we moved the selected entities.
		BOOL bMoved = FALSE;
		for (unsigned int i = 0; i < m_pNewEntityList->size(); i++)
		{
			ITplEntityPtr pNewEntity = m_pNewEntityList->at(i);
			ITplEntityPtr pOldEntity = m_pOldEntityList->at(i);
			if (pNewEntity->GetBorderRect() != pOldEntity->GetBorderRect())
			{
				bMoved = TRUE;
				break;
			}
		}

		if (bMoved)
		{
			// create command.
			TplEntityListPtr pTplEntityList = m_pReportTplPage->GetTplEntityList();
			IRptCmdPtr pCommand(new MoveEntityCommand(GetParent(), pTplEntityList, m_pOldEntityList, m_pNewEntityList));
			pCommand->Do();
			RptCmdManager::GetInstance()->AddCommand(pCommand);

			m_pSelEntityList = m_pOldEntityList;
		}

		// reset the status.
		m_pOldEntityList.reset();
		m_pNewEntityList.reset();
		m_actionType = ACTION_NONE;
	}
	else if (m_actionType == ACTION_RESIZING)
	{
		// create command.
		TplEntityListPtr pTplEntityList = m_pReportTplPage->GetTplEntityList();
		IRptCmdPtr pCommand(new RszEntityCommand(GetParent(), pTplEntityList, m_pOldEntityList->at(0), m_pNewEntityList->at(0)));
		pCommand->Do();
		RptCmdManager::GetInstance()->AddCommand(pCommand);

		// reset the status.
		m_pSelEntityList = m_pOldEntityList;
		m_pOldEntityList.reset();
		m_pNewEntityList.reset();
		m_actionType = ACTION_NONE;
	}
	else if (m_actionType == ACTION_RUBBERBANDING)
	{
		// reset the status.
		m_actionType = ACTION_NONE;

		// convert the selection rect to virtual coordinates, and make sure that the rect contains at least some 
		// width and height.
		CRect rect(m_selectionRect);
		ScreenToWorld(rect);
		if (rect.TopLeft() == rect.BottomRight())
			rect.InflateRect(1, 1);

		/************************************************************************/		
		// update the selected entities.

		TplEntityListPtr pTplEntityList = m_pReportTplPage->GetTplEntityList();
		EntityGroupListPtr pEntityGroupList = m_pReportTplPage->GetEntityGroupList();

		// get the entities which intersect with the selection rect.
		TplEntityListPtr pSelTplEntityList = pTplEntityList->GetEntityInRect(rect);

		// get all associated groups and select the entities in them.
		for (unsigned int i = 0; i < pSelTplEntityList->size(); i++)
		{
			EntityGroupPtr pSelEntityGroup = pEntityGroupList->GetGroupByEntity(pSelTplEntityList->at(i));
			if (pSelEntityGroup)
				pSelTplEntityList->AddEntity(pSelEntityGroup->GetTplEntityList());
		}

		m_pSelEntityList = pSelTplEntityList;
		/************************************************************************/
	}

	RedrawWindow();
	CWnd::OnLButtonUp( nFlags, point );
}

void TplDesignWnd::OnLButtonDblClk( UINT nFlags, CPoint point ) 
{
	CString strMsg, strTitle;
	strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_pSelEntityList->size() > 0)
	{
		if (m_pSelEntityList->size() > 1)
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_ONE_ENTITY_PROP);
			MessageBox(strMsg, strTitle, MB_OK | MB_ICONWARNING);
		}
		else
		{
			ITplEntityPtr pSelEntity = m_pSelEntityList->at(0);
			TplEntityMgr::EditEntityProp(m_pReportTplPage, pSelEntity);

			RedrawWindow();
		}
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void TplDesignWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_actionType == ACTION_NONE)
	{
		// get the world coordinate.
		CPoint worldPt(point);
		ScreenToWorld(worldPt);

		// keep the current cursor point.
		m_pastePoint = worldPt;

		// the click should be within the paper.
		if (worldPt.x <= m_pReportTplPage->GetPaperWidth() && worldPt.y <= m_pReportTplPage->GetPaperHeight())
		{
			if (m_pSelEntityList->size() == 0)
			{
				// check whether this hit selected an entity.
				ITplEntityPtr pSelTplEntity = GetSelectedEntity(m_pReportTplPage->GetTplEntityList(), worldPt);
				if (pSelTplEntity)
					m_pSelEntityList->push_back(pSelTplEntity);
			}
			else
			{
				// whether the mouse click is outside all selected entities.
				ITplEntityPtr pSelTplEntity = GetSelectedEntity(m_pSelEntityList, worldPt);
				if (!pSelTplEntity)
				{
					// de-select all selected entities.
					m_pSelEntityList.reset(new TplEntityList);

					// check whether the mouse click is inside any other entities.
					pSelTplEntity = GetSelectedEntity(m_pReportTplPage->GetTplEntityList(), worldPt);
					if (pSelTplEntity)
						m_pSelEntityList->push_back(pSelTplEntity);
				}
			}
		}
	}

	RedrawWindow();
	CWnd::OnRButtonDown( nFlags, point );
}

void TplDesignWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	CString str;

	CPoint pos( point );
	ClientToScreen( &pos );

	if (m_pSelEntityList->size() == 0)
	{
		XMenu xMenu;
		xMenu.CreatePopupMenu();

		if (m_pCopiedEntityList->size() > 0)
		{
			// "paste" menu item.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_PASTE);
			xMenu.AppendMenu(MF_STRING, IDC_ENTITY_PASTE, str.AllocSysString());

			xMenu.AppendMenu(MF_SEPARATOR);
		}

		// "no zoom" menu item.
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_NO_ZOOM);
		xMenu.AppendMenu(MF_STRING, IDC_ENTITY_NO_ZOOM, str.AllocSysString());

		xMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, pos.x, pos.y, this);
	}
	else
	{
		XMenu xMenu;
		xMenu.CreatePopupMenu();

		// "cut" menu item.
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_CUT);
		xMenu.AppendMenu(MF_STRING, IDC_ENTITY_CUT, str.AllocSysString());

		// "copy" menu item.
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_COPY);
		xMenu.AppendMenu(MF_STRING, IDC_ENTITY_COPY, str.AllocSysString());

		// "delete" menu item.
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_DELETE);
		xMenu.AppendMenu(MF_STRING, IDC_ENTITY_DELETE, str.AllocSysString());

		/************************************************************************/
		// for "group/ungroup" menu.

		EntityGroupListPtr pEntityGroupList = m_pReportTplPage->GetEntityGroupList();

		// check whether some selected entity have been grouped.
		BOOL bGroupExist = FALSE;
		EntityGroupListPtr pSelEntityGroupList = pEntityGroupList->GetGroupByEntity(m_pSelEntityList);
		if (pSelEntityGroupList->size() > 0)
			bGroupExist = TRUE;

		// whether can display "group" menu.
		BOOL bShowGroupMenu = TRUE;
		if (bGroupExist || m_pSelEntityList->size() == 1)
			bShowGroupMenu = FALSE;

		// whether can display "ungroup" menu.
		BOOL bShowUnGroupMenu = TRUE;
		if (!bGroupExist)
			bShowUnGroupMenu = FALSE;

		// separator menu.
		if (bShowGroupMenu || bShowUnGroupMenu)
			xMenu.AppendMenu(MF_SEPARATOR);

		// "group" menu.
		if (bShowGroupMenu)
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_GROUP);
			xMenu.AppendMenu(MF_STRING, IDC_ENTITY_GROUP, str.AllocSysString());
		}

		// "ungroup" menu.
		if (bShowUnGroupMenu)
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_UNGROUP);
			xMenu.AppendMenu(MF_STRING, IDC_ENTITY_UNGROUP, str.AllocSysString());
		}
		/************************************************************************/

		if (m_pSelEntityList->size() == 1)
		{
			xMenu.AppendMenu(MF_SEPARATOR);

			// "property" menu item.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ENTITY_PROP);
			xMenu.AppendMenu(MF_STRING, IDC_ENTITY_PROP, str.AllocSysString());
		}

		xMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, pos.x, pos.y, this);
	}

	CWnd::OnRButtonUp(nFlags, point);
}

void TplDesignWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);

	// when move cursor upon the view, set the focus.
	SetFocus();

	// get the world coordinate.
	CPoint worldPt(point);
	ScreenToWorld(worldPt);

	/************************************************************************/
	// update the status bar.

	CString strMsg;
	strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CURSOR_POSITION);

	// the display unit.
	TplDisplayParamPtr pDisplayParam = TplDesignConfig::GetInstance()->GetTplDisplayParam();
	TPL_UNIT_TYPE tplUnit = pDisplayParam->GetTplUnit();

	// init the string on the status bar.
	CString strCoordinate, strUnit;
	if (tplUnit == TPL_UNIT_PIXEL)
	{
		strCoordinate.Format(_T("%d , %d"), worldPt.x, worldPt.y);
		strUnit = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_UNIT_PIXEL);
	}
	else if (tplUnit == TPL_UNIT_INCH)
	{
		strCoordinate.Format(_T("%f , %f"), UnitHelper::PixelsToInches(worldPt.x), UnitHelper::PixelsToInches(worldPt.y));
		strUnit = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_UNIT_INCH);
	}
	else if (tplUnit == TPL_UNIT_CM)
	{
		strCoordinate.Format(_T("%f , %f"), UnitHelper::PixelsToCentimeters(worldPt.x), UnitHelper::PixelsToCentimeters(worldPt.y));
		strUnit = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_UNIT_CM);
	}
	strMsg.Replace(_T("WILLBEREPLACED1"), strCoordinate);
	strMsg.Replace(_T("WILLBEREPLACED2"), strUnit);

	// update status bar.
	AfxGetMainWnd()->SendMessage(WM_UPDATE_STATUS_BAR, (WPARAM)(LPCTSTR)strMsg, NULL);
	/************************************************************************/

	if (m_actionType == ACTION_DRAWING && m_pDrawingEntity)
	{
		// check whether need to snap to the grid.
		if (m_pTplEditParam->IsSnapToGrid())
		{
			worldPt.x = SnapX(worldPt.x);
			worldPt.y = SnapY(worldPt.y);
		}

		int iPosX = worldPt.x;
		int iPosY = worldPt.y;

		// set the rect of the entity.
		CRect borderRect = m_pDrawingEntity->GetBorderRect();
		if (m_pDrawingEntity->GetType() == ENTITY_LINE)
		{
			borderRect.right = iPosX;
			borderRect.bottom = iPosY;
		}
		else
		{
			if (iPosX > borderRect.left)
				borderRect.right = iPosX;
			if (iPosY > borderRect.top)
				borderRect.bottom = iPosY;
		}
		m_pDrawingEntity->SetBorderRect(borderRect);

		ScrollToPoint(point);
		RedrawWindow();
	}
	else if (m_actionType == ACTION_MOVING)
	{
		// the current click point.
		CPoint movingPt = point;
		ScreenToWorld(movingPt);

		// calc the new left top corner of the anchor entity.
		CPoint leftTopPt;
		leftTopPt.x = movingPt.x - m_moveDeltaPt.x;
		leftTopPt.y = movingPt.y - m_moveDeltaPt.y;

		// check whether need to snap to the grid.
		if (m_pTplEditParam->IsSnapToGrid())
		{
			leftTopPt.x = SnapX(leftTopPt.x);
			leftTopPt.y = SnapY(leftTopPt.y);
		}

		// the offset from the old one.
		CRect tmpRect = m_pAnchorEntity->GetBorderRect();
		int iOffsetX = leftTopPt.x - tmpRect.left;
		int iOffsetY = leftTopPt.y - tmpRect.top;

		// move all selected entities.
		for (unsigned int i = 0; i < m_pNewEntityList->size(); i++)
		{
			ITplEntityPtr pTplEntity = m_pNewEntityList->at(i);
			pTplEntity->MoveRect(iOffsetX, iOffsetY);
		}

		RedrawWindow();
	}
	else if (m_actionType == ACTION_RESIZING)
	{
		// check whether need to snap to the grid.
		if (m_pTplEditParam->IsSnapToGrid())
		{
			worldPt.x = SnapX(worldPt.x);
			worldPt.y = SnapY(worldPt.y);
		}

		int iPosX = worldPt.x;
		int iPosY = worldPt.y;
		ITplEntityPtr pResizingEntity = m_pNewEntityList->at(0);

		// get the new rect.
		int iLeft = pResizingEntity->GetBorderVal(ENTITY_BORDER_LEFT);
		int iTop = pResizingEntity->GetBorderVal(ENTITY_BORDER_TOP);
		int iRight = pResizingEntity->GetBorderVal(ENTITY_BORDER_RIGHT);
		int iBottom = pResizingEntity->GetBorderVal(ENTITY_BORDER_BOTTOM);
		if (m_entityPosPt == ENTITY_POSITION_BOTTOMMIDDLE)
		{
			if (pResizingEntity->GetType() == ENTITY_LINE)
				iBottom = iPosY;
			else
				if (iPosY > iTop)
					iBottom = iPosY;
		}
		else if (m_entityPosPt == ENTITY_POSITION_TOPMIDDLE)
		{
			if (pResizingEntity->GetType() == ENTITY_LINE)
				iTop = iPosY;
			else
				if (iPosY < iBottom)
					iTop = iPosY;
		}
		else if (m_entityPosPt == ENTITY_POSITION_LEFTMIDDLE)
		{
			if (pResizingEntity->GetType() == ENTITY_LINE)
				iLeft = iPosX;
			else
				if (iPosX < iRight)
					iLeft = iPosX;
		}
		else if (m_entityPosPt == ENTITY_POSITION_RIGHTMIDDLE)
		{
			if (pResizingEntity->GetType() == ENTITY_LINE)
				iRight = iPosX;
			else
				if (iPosX > iLeft)
					iRight = iPosX;
		}
		else if (m_entityPosPt == ENTITY_POSITION_TOPLEFT)
		{
			if (pResizingEntity->GetType() == ENTITY_LINE)
			{
				iLeft = iPosX;
				iTop = iPosY;
			}
			else
			{
				if (iPosX < iRight)
					iLeft = iPosX;
				if (iPosY < iBottom)
					iTop = iPosY;
			}
		}
		else if (m_entityPosPt == ENTITY_POSITION_TOPRIGHT)
		{
			if (pResizingEntity->GetType() == ENTITY_LINE)
			{
				iTop = iPosY;
				iRight = iPosX;
			}
			else
			{
				if (iPosX > iLeft)
					iRight = iPosX;
				if (iPosY < iBottom)
					iTop = iPosY;
			}
		}
		else if (m_entityPosPt == ENTITY_POSITION_BOTTOMLEFT)
		{
			if (pResizingEntity->GetType() == ENTITY_LINE)
			{
				iLeft = iPosX;
				iBottom = iPosY;
			}
			else
			{
				if (iPosX < iRight)
					iLeft = iPosX;
				if (iPosY > iTop)
					iBottom = iPosY;
			}
		}
		else if (m_entityPosPt == ENTITY_POSITION_BOTTOMRIGHT)
		{
			if (pResizingEntity->GetType() == ENTITY_LINE)
			{
				iRight = iPosX;
				iBottom = iPosY;
			}
			else
			{
				if (iPosX > iLeft)
					iRight = iPosX;
				if (iPosY > iTop)
					iBottom = iPosY;
			}
		}

		//
		pResizingEntity->SetBorderRect(CRect(iLeft, iTop, iRight, iBottom));

		ScrollToPoint(point);
		RedrawWindow();
	}
	else if (m_actionType == ACTION_RUBBERBANDING)
	{
		// We are selecting objects
		CRect rect(m_selectionRect);

		// Erase old selection rect
		if (m_selectionRect.left != m_selectionRect.right || m_selectionRect.top != m_selectionRect.bottom)
		{
			rect.NormalizeRect();
			dc.DrawFocusRect( rect );
		}

		// Scroll if we are outside any edge
		CPoint outside = ScrollToPoint(point);
		m_selectionRect.OffsetRect(-outside.x, -outside.y);

		// Update and draw the selection rect
		m_selectionRect.right = point.x;
		m_selectionRect.bottom = point.y;
		rect = m_selectionRect;
		rect.NormalizeRect();

		dc.DrawFocusRect(rect);
	}

	CWnd::OnMouseMove(nFlags, point);
}

BOOL TplDesignWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (nFlags & MK_CONTROL)
	{
		if (zDelta < 0)
			m_dZoomLevel += ZOOM_STEP;
		else
			m_dZoomLevel -= ZOOM_STEP;
		RedrawWindow();
	}
	else
	{
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS | SIF_RANGE;
		static int mouseDelta;

		mouseDelta += zDelta;
		int iWheelDelta = 10;
		int increment = (mouseDelta / iWheelDelta);
		if (GetScrollInfo(SB_VERT, &si))
		{
			if (si.nMax)
			{
				si.nPos -= increment;
				mouseDelta -= increment * iWheelDelta;
				SetVScroll(si.nPos);
				RedrawWindow();
			}
		}
	}

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void TplDesignWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( nChar == VK_DELETE )
	{
		DelSelectedEntity();
	}
	else if( nChar == _TCHAR( 'C' ) && GetAsyncKeyState( VK_CONTROL ) )
	{
		m_pCopiedEntityList = m_pSelEntityList;
	}
	else if( nChar == _TCHAR( 'V' ) && GetAsyncKeyState( VK_CONTROL ) )
	{
		PasteSelEntityToPt(CPoint(0,0));
		RedrawWindow();
	}
	else if( nChar == _TCHAR( 'A' ) && GetAsyncKeyState( VK_CONTROL ) )
	{
		m_pSelEntityList = m_pReportTplPage->GetTplEntityList();
		RedrawWindow();
	}
	else if( nChar == _TCHAR( 'Z' ) && GetAsyncKeyState( VK_CONTROL ) )
	{
		GetParent()->SendMessage(WM_COMMAND, IDC_TPL_UNDO, NULL);
	}
	else if( nChar == _TCHAR( 'Y' ) && GetAsyncKeyState( VK_CONTROL ) )
	{
		GetParent()->SendMessage(WM_COMMAND, IDC_TPL_REDO, NULL);
	}
}

void TplDesignWnd::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	si.fMask = SIF_POS | SIF_RANGE;
	GetScrollInfo( SB_HORZ, &si );
	switch( nSBCode )
	{
		case SB_LEFT:			// Scroll to far left.
			si.nPos = si.nMin;
			break;
		case SB_LINELEFT:		// Scroll left.
			si.nPos = max( si.nPos - 1, si.nMin );
			break;
		case SB_LINERIGHT:		// Scroll right.
			si.nPos = min( si.nPos + 1, si.nMax );
			break;
		case SB_PAGELEFT:		// Scroll one page left.
			si.nPos = max( si.nPos - static_cast< int >( si.nPage ), si.nMin );
			break;
		case SB_PAGERIGHT:		// Scroll one page right.
			si.nPos = min( si.nPos + static_cast< int >( si.nPage ), si.nMax );
			break;
		case SB_RIGHT:			// Scroll to far right.
			si.nPos = si.nMax;
			break;
		case SB_THUMBPOSITION:	// Scroll to absolute position. The current position is specified by the nPos parameter.
			si.nPos = nPos;
			break;
		case SB_THUMBTRACK:		//
			si.nPos = nPos;
			break;
	}

	SetHScroll( si.nPos );
	RedrawWindow();
	CWnd::OnHScroll( nSBCode, nPos, pScrollBar );
}

void TplDesignWnd::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar ) 
{
	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	si.fMask = SIF_POS | SIF_RANGE;
	GetScrollInfo( SB_VERT, &si );
	switch( nSBCode )
	{
		case SB_TOP:			// Scroll to far left.
			si.nPos = si.nMin;
			break;
		case SB_LINEUP:			// Scroll left.
			si.nPos = max( si.nPos - 1, si.nMin );
			break;
		case SB_LINEDOWN:		// Scroll right.
			si.nPos = min( si.nPos + 1, si.nMax );
			break;
		case SB_PAGEUP:			// Scroll one page left.
			si.nPos = max( si.nPos - static_cast< int >( si.nPage ), si.nMin );
			break;
		case SB_PAGEDOWN:		// Scroll one page right.
			si.nPos = min( si.nPos + static_cast< int >( si.nPage ), si.nMax );
			break;
		case SB_BOTTOM:			// Scroll to far right.
			si.nPos = si.nMax;
			break;
		case SB_THUMBPOSITION:	// Scroll to absolute position. The current position is specified by the nPos parameter.
			si.nPos = nPos;
			break;
		case SB_THUMBTRACK:		//
			si.nPos = nPos;
			break;
	}

	SetVScroll( si.nPos );
	RedrawWindow();

	CWnd::OnVScroll( nSBCode, nPos, pScrollBar );
}

BOOL TplDesignWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	BOOL res = TRUE;

	HCURSOR cursor = GetCursor();
	if (cursor)
		::SetCursor(cursor);
	else
		res = CWnd::OnSetCursor(pWnd, nHitTest, message);

	return res;
}

void TplDesignWnd::OnCopyEntity()
{
	CopySelectedEntity();
}

void TplDesignWnd::OnCutEntity()
{
	CutSelectedEntity();
}

void TplDesignWnd::OnPasteEntity()
{
	PasteSelEntityToPt(m_pastePoint);
}

void TplDesignWnd::OnDeleteEntity()
{
	DelSelectedEntity();
}

void TplDesignWnd::OnEditEntity()
{
	EditEntityProp();
}

void TplDesignWnd::OnNoZoom()
{
	NoZoomPage();
}

void TplDesignWnd::OnGroupEntity()
{
	GroupEntity();
}

void TplDesignWnd::OnUnGroupEntity()
{
	UnGroupEntity();
}

END_CUTLEADER_NAMESPACE()

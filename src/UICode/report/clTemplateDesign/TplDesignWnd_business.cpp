#include "StdAfx.h"
#include "TplDesignWnd.h"

#include "XMenu.h"
#include "clBaseDataResource.h"

#include "clReportResource.h"
#include "EntityGroup.h"
#include "EntityGroupList.h"
#include "TplEntityList.h"
#include "reportConst.h"
#include "ReportTplPage.h"
#include "ReportTplDrawer.h"
#include "TplEntityDrawer.h"
#include "TplEntityMgr.h"
#include "DisplayParam.h"
#include "TplEditParam.h"
#include "TplDesignConfig.h"

#include "AddEntityCommand.h"
#include "MoveEntityCommand.h"
#include "RszEntityCommand.h"
#include "CutEntityCommand.h"
#include "PasteEntityCommand.h"
#include "DelEntityCommand.h"
#include "GroupEntityCommand.h"
#include "UnGroupEntityCommand.h"
#include "RptCmdManager.h"

BEGIN_CUTLEADER_NAMESPACE()

TplDesignWnd::TplDesignWnd(void)
{
	m_paperBkColor = ::GetSysColor( COLOR_WINDOW );
	m_noPaperBkColor = ::GetSysColor( COLOR_3DSHADOW );
	m_dZoomLevel = 1;

	m_actionType = ACTION_NONE;
	m_pDrawingEntity.reset();

	m_pSelEntityList.reset(new TplEntityList);
	m_selectionRect.SetRectEmpty();

	m_moveDeltaPt = CSize( 0, 0 );
	m_pAnchorEntity.reset();

	m_pOldEntityList.reset(new TplEntityList);
	m_pNewEntityList.reset(new TplEntityList);
	m_pCopiedEntityList.reset(new TplEntityList);

	m_pTplEditParam = TplDesignConfig::GetInstance()->GetTplEditParam();
}

TplDesignWnd::~TplDesignWnd(void)
{
}

void TplDesignWnd::CopySelectedEntity()
{
	m_pCopiedEntityList = m_pSelEntityList;
}

void TplDesignWnd::CutSelectedEntity()
{
	if (m_pSelEntityList->size() > 0)
	{
		TplEntityListPtr pTplEntityList = m_pReportTplPage->GetTplEntityList();
		EntityGroupListPtr pEntityGroupList = m_pReportTplPage->GetEntityGroupList();

		// create command.
		EntityGroupListPtr pSelEntityGroupList = pEntityGroupList->GetGroupByEntity(m_pSelEntityList);
		IRptCmdPtr pCommand(new CutEntityCommand(GetParent(), pTplEntityList, m_pSelEntityList,
												 pEntityGroupList, pSelEntityGroupList));
		pCommand->Do();
		RptCmdManager::GetInstance()->AddCommand(pCommand);

		m_pCopiedEntityList = m_pSelEntityList;
		m_pSelEntityList.reset(new TplEntityList);
		RedrawWindow();
	}
}

void TplDesignWnd::PasteSelEntityToPt(CPoint pastePt)
{
	// create the new entities.
	IDataPtr pData = m_pCopiedEntityList->Clone();
	TplEntityListPtr pNewEntityList = boost::dynamic_pointer_cast<TplEntityList>(pData);

	// set the proper position of the new entities.
	double dLeft, dTop;
	pNewEntityList->GetLeftTopPt(dLeft, dTop);
	double dOffsetX = (pastePt.x - dLeft) - 10;
	double dOffsetY = (pastePt.y - dTop) - 10;
	pNewEntityList->OffesetEntity(dOffsetX, dOffsetY);

	// create command.
	TplEntityListPtr pTplEntityList = m_pReportTplPage->GetTplEntityList();
	IRptCmdPtr pCommand(new PasteEntityCommand(GetParent(), pTplEntityList, pNewEntityList));
	pCommand->Do();
	RptCmdManager::GetInstance()->AddCommand(pCommand);

	m_pSelEntityList = pNewEntityList;

	RedrawWindow();
}

void TplDesignWnd::DelSelectedEntity()
{
	CString strMsg, strTitle;
	strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_pSelEntityList->size() > 0)
	{
		TplEntityListPtr pTplEntityList = m_pReportTplPage->GetTplEntityList();
		EntityGroupListPtr pEntityGroupList = m_pReportTplPage->GetEntityGroupList();

		// create command.
		EntityGroupListPtr pSelEntityGroupList = pEntityGroupList->GetGroupByEntity(m_pSelEntityList);
		IRptCmdPtr pCommand(new DelEntityCommand(GetParent(), pTplEntityList, m_pSelEntityList, 
												 pEntityGroupList, pSelEntityGroupList));
		pCommand->Do();
		RptCmdManager::GetInstance()->AddCommand(pCommand);

		m_pSelEntityList.reset(new TplEntityList);
		RedrawWindow();
	}
	else
	{
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NO_ENTITY_SELECT);
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONWARNING);
	}
}

void TplDesignWnd::EditEntityProp()
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
	else
	{
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NO_ENTITY_SELECT);
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONWARNING);
	}
}

void TplDesignWnd::GroupEntity()
{
	CString strMsg, strTitle;
	strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_pSelEntityList->size() > 0)
	{
		EntityGroupListPtr pEntityGroupList = m_pReportTplPage->GetEntityGroupList();

		// check whether some selected entity have been grouped.
		for (unsigned int i = 0; i < m_pSelEntityList->size(); i++)
		{
			EntityGroupPtr pEntityGroup = pEntityGroupList->GetGroupByEntity(m_pSelEntityList->at(i));
			if (pEntityGroup)
			{
				strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SOME_ENTITY_GROUPED);
				MessageBox(strMsg, strTitle, MB_OK | MB_ICONWARNING);
				return;
			}
		}

		// cannot group only one entity.
		if (m_pSelEntityList->size() == 1)
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_GROUP_ONE_ENTITY);
			MessageBox(strMsg, strTitle, MB_OK | MB_ICONWARNING);
			return;
		}

		// if reach here, we can group the selected entities.
		EntityGroupPtr pEntityGroup(new EntityGroup);
		pEntityGroup->SetTplEntityList(m_pSelEntityList);
		IRptCmdPtr pCommand(new GroupEntityCommand(GetParent(), pEntityGroupList, pEntityGroup));
		pCommand->Do();
		RptCmdManager::GetInstance()->AddCommand(pCommand);

		// prompt.
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_GROUP_SUCCESS);
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONWARNING);
	}
	else
	{
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NO_ENTITY_SELECT);
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONWARNING);
	}
}

void TplDesignWnd::UnGroupEntity()
{
	CString strMsg, strTitle;
	strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_pSelEntityList->size() > 0)
	{
		EntityGroupListPtr pEntityGroupList = m_pReportTplPage->GetEntityGroupList();

		// check whether some group were selected.
		EntityGroupListPtr pSelEntityGroupList = pEntityGroupList->GetGroupByEntity(m_pSelEntityList);
		if (pSelEntityGroupList->size() == 0)
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NO_GROUP_SELECT);
			MessageBox(strMsg, strTitle, MB_OK | MB_ICONWARNING);
			return;
		}

		// if reach here, we can ungroup the selected entities.
		IRptCmdPtr pCommand(new UnGroupEntityCommand(GetParent(), pEntityGroupList, pSelEntityGroupList));
		pCommand->Do();
		RptCmdManager::GetInstance()->AddCommand(pCommand);

		// prompt.
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_UNGROUP_SUCCESS);
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONWARNING);
	}
	else
	{
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NO_ENTITY_SELECT);
		MessageBox(strMsg, strTitle, MB_OK | MB_ICONWARNING);
	}
}

void TplDesignWnd::NoZoomPage()
{
	m_dZoomLevel = 1.0;
	RedrawWindow();
}

void TplDesignWnd::ZoomInPage()
{
	m_dZoomLevel += ZOOM_STEP;
	RedrawWindow();
}

void TplDesignWnd::ZoomOutPage()
{
	m_dZoomLevel -= ZOOM_STEP;
	RedrawWindow();
}

void TplDesignWnd::StartDrawAction(ENTITY_TYPE entityType)
{
	m_actionType = ACTION_DRAWING;
	m_drawingEntityType = entityType;

	::SetCursor(LoadCursor(NULL, IDC_CROSS));
}

void TplDesignWnd::RefreshForUndoRedo()
{
	m_pSelEntityList.reset(new TplEntityList);
	RedrawWindow();
}

HCURSOR TplDesignWnd::GetCursor()
{
	HCURSOR hRet = NULL;

	if (m_actionType == ACTION_DRAWING)
	{
		hRet = LoadCursor(NULL, IDC_CROSS);
	}
	else
	{
		// get the mouse position.
		const MSG* msg = GetCurrentMessage();
		CPoint point(msg->pt);
		ScreenToClient(&point);
		ScreenToWorld(point);

		// check whether we touched some snap point of some selected entity.
		for (unsigned int i = 0; i < m_pSelEntityList->size(); i++)
		{
			ITplEntityPtr pTplEntity = m_pSelEntityList->at(i);

			// which position point was touched.
			ENTITY_POSITION_PT entityPosPt = pTplEntity->GetPtPosition(point);
			if (entityPosPt != ENTITY_POSITION_NONE && entityPosPt != ENTITY_POSITION_BODY)
			{
				// whether snap to an entity in the group.
				if (!m_pReportTplPage->GetEntityGroupList()->GetGroupByEntity(pTplEntity))
					hRet = pTplEntity->GetCursorByPosPt(entityPosPt);
				break;
			}
		}
	}

	return hRet;
}

void TplDesignWnd::SetHScroll( int pos )
{
	pos = max( 0, pos );

	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	si.fMask = SIF_POS;
	si.nPos = pos;

	SetScrollInfo( SB_HORZ, &si );
}

void TplDesignWnd::SetVScroll( int pos )
{
	pos = max( 0, pos );

	SCROLLINFO si;
	si.cbSize = sizeof( SCROLLINFO );
	si.fMask = SIF_POS;
	si.nPos = pos;

	SetScrollInfo( SB_VERT, &si );
}

void TplDesignWnd::SetupScrollbars()
{
	//
	int iPaperWidth = m_pReportTplPage->GetPaperWidth();
	int iPaperHeight = m_pReportTplPage->GetPaperHeight();

	if( m_hWnd )
	{
		CRect rect;
		GetClientRect( rect );

		DWORD add = 0;
		DWORD remove = 0;

		if ((int)(iPaperWidth * m_dZoomLevel ) <= rect.right)
		{
			if( GetStyle() & WS_HSCROLL )
				remove |= WS_HSCROLL;
		}
		else
		{
			if( !( GetStyle() & WS_HSCROLL ) )
				add |= WS_HSCROLL;
		}

		if ((int)(iPaperHeight * m_dZoomLevel) <= rect.bottom )
		{
			if( GetStyle() & WS_VSCROLL )
				remove |= WS_VSCROLL;
		}
		else
		{
			if( !( GetStyle() & WS_VSCROLL ) )
				add |= WS_VSCROLL;
		}

		SCROLLINFO si;
		si.cbSize = sizeof( SCROLLINFO );
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		int width = (int)(iPaperWidth * m_dZoomLevel);
		if( width > rect.right )
		{
			si.nMax = width;
			si.nPage = rect.right;
		}
		else
		{
			si.nMax = 0;
			si.nPage = 0;
		}

		SetScrollInfo( SB_HORZ, &si );
		
		int height = (int)(iPaperHeight * m_dZoomLevel);
		if( height > rect.bottom )
		{
			si.nMax = height;
			si.nPage = rect.bottom;
		}
		else
		{
			si.nMax = 0;
			si.nPage = 0;
		}

		SetScrollInfo( SB_VERT, &si );

		if( add || remove )
			ModifyStyle( remove, add, SWP_FRAMECHANGED );
	}
}

void TplDesignWnd::ScreenToWorld(CPoint& point) const
{
	// Convert point to the virtual coordinate system. We have to normalize, 
	// add scroll bar positions and apply current scale.

	SCROLLINFO sih;
	SCROLLINFO siv;

	sih.cbSize = sizeof(SCROLLINFO);
	sih.fMask = SIF_POS;
	siv.cbSize = sizeof(SCROLLINFO);
	siv.fMask = SIF_POS;

	if (!::GetScrollInfo(m_hWnd, SB_HORZ, &sih))
		sih.nPos = 0;
	if (!::GetScrollInfo(m_hWnd, SB_VERT, &siv))
		siv.nPos = 0;

	point.x = (int)((point.x + sih.nPos) / m_dZoomLevel);
	point.y = (int)((point.y + siv.nPos) / m_dZoomLevel);
}

void TplDesignWnd::WorldToScreen(CPoint& point) const
{
	point.x = (int)(point.x * m_dZoomLevel);
	point.y = (int)(point.y * m_dZoomLevel);
}

void TplDesignWnd::ScreenToWorld( CSize& size ) const
{
	// We have to deduct scroll bar positions and apply current zoom.

	SCROLLINFO sih;
	SCROLLINFO siv;

	sih.cbSize = sizeof( SCROLLINFO );
	sih.fMask = SIF_POS;
	siv.cbSize = sizeof( SCROLLINFO );
	siv.fMask = SIF_POS;

	if( !::GetScrollInfo( m_hWnd, SB_HORZ, &sih ) )
		sih.nPos = 0;
	if( !::GetScrollInfo( m_hWnd, SB_VERT, &siv ) )
		siv.nPos = 0;

	size.cx = (int)((size.cx + sih.nPos) / m_dZoomLevel);
	size.cy = (int)((size.cy + siv.nPos) / m_dZoomLevel);
}

void TplDesignWnd::ScreenToWorld(CRect& rect) const
{
	// We have to normalize, add scroll bar positions and apply current zoom.

	rect.NormalizeRect();

	SCROLLINFO sih;
	SCROLLINFO siv;

	sih.cbSize = sizeof( SCROLLINFO );
	sih.fMask = SIF_POS;
	siv.cbSize = sizeof( SCROLLINFO );
	siv.fMask = SIF_POS;

	if( !::GetScrollInfo( m_hWnd, SB_HORZ, &sih ) )
		sih.nPos = 0;
	if( !::GetScrollInfo( m_hWnd, SB_VERT, &siv ) )
		siv.nPos = 0;

	rect.SetRect( (int)((rect.left + sih.nPos) / m_dZoomLevel), 
				  (int)((rect.top + siv.nPos) / m_dZoomLevel), 
				  (int)((rect.right	+ sih.nPos) / m_dZoomLevel), 
				  (int)((rect.bottom + siv.nPos) / m_dZoomLevel));
}

void TplDesignWnd::WorldToScreen(CRect& rect) const
{
	// We have to normalize, deduct scroll bar positions and apply current zoom.

	rect.NormalizeRect();

	SCROLLINFO sih;
	SCROLLINFO siv;

	sih.cbSize = sizeof(SCROLLINFO);
	sih.fMask = SIF_POS;
	siv.cbSize = sizeof(SCROLLINFO);
	siv.fMask = SIF_POS;

	if (!::GetScrollInfo(m_hWnd, SB_HORZ, &sih))
		sih.nPos = 0;
	if (!::GetScrollInfo(m_hWnd, SB_VERT, &siv))
		siv.nPos = 0;

	rect.SetRect((int)(rect.left * m_dZoomLevel - sih.nPos), 
				 (int)(rect.top * m_dZoomLevel - siv.nPos), 
				 (int)(rect.right * m_dZoomLevel - sih.nPos), 
				 (int)(rect.bottom * m_dZoomLevel - siv.nPos));
}

CPoint TplDesignWnd::ScrollToPoint( CPoint point )
{
	CPoint out( 0, 0 );
	CRect clientRect;
	GetClientRect( &clientRect );

	if( point.x > clientRect.right )
		out.x = HScroll( 10 );

	if( point.x < 0 )
		out.x = HScroll( -10 );

	if( point.y > clientRect.bottom )
		out.y = VScroll( 10 );

	if( point.y < 0 )
		out.y = VScroll( -10 );

	return out;
}

int TplDesignWnd::HScroll( int scroll )
{
	int retval = 0;

	if( m_hWnd )
	{
		CRect clientRect;
		GetClientRect( &clientRect );
		if(clientRect.Size().cx < m_pReportTplPage->GetPaperWidth() )	
		{
			int desiredpos = 0;

			SCROLLINFO si;
			si.cbSize = sizeof( SCROLLINFO );
			if( GetScrollInfo( SB_HORZ, &si ) )
			{
				desiredpos = si.nPos + scroll;
				si.nPos = desiredpos;

				SetHScroll( si.nPos );
				GetScrollInfo( SB_HORZ, &si );
				retval = scroll - ( desiredpos - si.nPos );
				if( retval )
					RedrawWindow();
			}
		}
	}

	return retval;
}

int TplDesignWnd::VScroll( int scroll )
{
	int retval = 0;

	if( m_hWnd )
	{
		CRect clientRect;
		GetClientRect( &clientRect );
		if(clientRect.Size().cy < m_pReportTplPage->GetPaperHeight() )
		{
			int desiredpos = 0;

			SCROLLINFO si;
			si.cbSize = sizeof( SCROLLINFO );
			if( GetScrollInfo( SB_VERT, &si ) )
			{
				desiredpos = si.nPos + scroll;
				si.nPos = desiredpos;

				SetVScroll( si.nPos );
				GetScrollInfo( SB_VERT, &si );
				retval = scroll - ( desiredpos - si.nPos );
				if( retval )
					RedrawWindow();
			}
		}
	}

	return retval;
}

ITplEntityPtr TplDesignWnd::GetSelectedEntity(TplEntityListPtr pTplEntityList, CPoint pt)
{
	ITplEntityPtr pSelTplEntity;

	// 所有包含这个点的实体。
	TplEntityListPtr pTmpTplEntitys(new TplEntityList);
	for (unsigned int i = 0; i < pTplEntityList->size(); i++)
	{
		ITplEntityPtr pTplEntity = pTplEntityList->at(i);

		ENTITY_POSITION_PT entityPosPt = pTplEntity->GetPtPosition(pt);
		if (entityPosPt != ENTITY_POSITION_NONE)
		{
			pTmpTplEntitys->push_back(pTplEntity);
		}
	}

	// 找到最近的那个。
	if (pTmpTplEntitys->size() > 0)
	{
		pSelTplEntity = pTmpTplEntitys->at(0);
		int iDis = pSelTplEntity->GetDis2Border(pt);
		for (unsigned int i = 1; i < pTmpTplEntitys->size(); i++)
		{
			ITplEntityPtr pTplEntity = pTmpTplEntitys->at(i);
			int iTmpDis = pTplEntity->GetDis2Border(pt);
			if (iTmpDis < iDis)
			{
				pSelTplEntity = pTplEntity;
				iDis = iTmpDis;
			}
		}
	}

	return pSelTplEntity;
}

int TplDesignWnd::SnapX(int iCoord) const
{
	int dGridCellSizeX = m_pTplEditParam->GetGridCellSizeX();
	return (int)((iCoord / dGridCellSizeX) * dGridCellSizeX);
}

int TplDesignWnd::SnapY(int iCoord) const
{
	int dGridCellSizeY = m_pTplEditParam->GetGridCellSizeY();
	return (int)((iCoord / dGridCellSizeY) * dGridCellSizeY);
}

END_CUTLEADER_NAMESPACE()

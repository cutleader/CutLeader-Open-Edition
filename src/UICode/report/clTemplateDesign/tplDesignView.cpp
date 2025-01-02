#include "stdafx.h"
#include "clTemplateDesign.h"

#include "clReportResource.h"
#include "TplDesignConfig.h"
#include "TplDisplayParam.h"

#include "StaticTplEntity.h"
#include "FieldTplEntity.h"
#include "tplDesignDoc.h"
#include "tplDesignView.h"

#include "IRptCmd.h"
#include "RptCmdList.h"
#include "RptCmdManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CtplDesignView, CView)

BEGIN_MESSAGE_MAP(CtplDesignView, CView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()

	// undo/redo
	ON_COMMAND(IDC_TPL_UNDO, OnEditUndo)
	ON_COMMAND(IDC_TPL_REDO, OnEditRedo)

	// draw action.
	ON_COMMAND(IDC_DRAW_STATIC, OnDrawStatic)
	ON_COMMAND(IDC_DRAW_LABEL, OnDrawLabel)
	ON_COMMAND(IDC_DRAW_FIELD, OnDrawField)
	ON_COMMAND(IDC_DRAW_STATIC_IMG, OnDrawStaticImg)
	ON_COMMAND(IDC_DRAW_DYN_IMG, OnDrawDynamicImg)
	ON_COMMAND(IDC_DRAW_GRID, OnDrawGrid)
	ON_COMMAND(IDC_DRAW_LINE, OnDrawLine)
	ON_COMMAND(IDC_DRAW_BOX, OnDrawBox)
	ON_COMMAND(IDC_DRAW_ELLIPSE, OnDrawEllipse)
	ON_COMMAND(IDC_DRAW_BARCODE, OnDrawBarCode)

	// edit action.
	ON_COMMAND(IDC_ENTITY_COPY, OnCopyEntity)
	ON_COMMAND(IDC_ENTITY_CUT, OnCutEntity)
	ON_COMMAND(IDC_ENTITY_PASTE, OnPasteEntity)
	ON_COMMAND(IDC_ENTITY_DELETE, OnDeleteEntity)
	ON_COMMAND(IDC_ENTITY_GROUP, OnGroupEntity)
	ON_COMMAND(IDC_ENTITY_UNGROUP, OnUnGroupEntity)
	ON_COMMAND(IDC_ENTITY_PROP, OnEditEntity)

	// view action.
	ON_COMMAND(IDC_ENTITY_NO_ZOOM, OnNoZoomPage)
	ON_COMMAND(IDC_ENTITY_ZOOM_IN, OnZoomInPage)
	ON_COMMAND(IDC_ENTITY_ZOOM_OUT, OnZoomOutPage)
END_MESSAGE_MAP()

CtplDesignView::CtplDesignView()
{
}

CtplDesignView::~CtplDesignView()
{
}

void CtplDesignView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// get the current report template.
	ReportTplPagePtr pReportTplPage = GetDocument()->GetReportTplPage();
	m_tplDesignWnd.SetReportTpl(pReportTplPage);

	// the display param.
	TplDisplayParamPtr pDisplayParam = TplDesignConfig::GetInstance()->GetTplDisplayParam();
	TPL_UNIT_TYPE tplUnit = pDisplayParam->GetTplUnit();

	if (!m_tplDesignWnd.m_hWnd)
	{
		CRect rect;
		GetClientRect(rect);

		// init the design window.
		rect.left = RULER_SIZE;
		rect.top = RULER_SIZE;
		m_tplDesignWnd.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, 99);

		// init the horizontal rule window.
		rect.top = 0;
		rect.bottom = RULER_SIZE;
		m_horzRulerWnd.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, 100);
		m_horzRulerWnd.SetMeasurements(tplUnit);

		// init the vertical rule window.
		GetClientRect(rect);
		rect.right = RULER_SIZE;
		rect.top = RULER_SIZE;
		m_vertRulerWnd.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, 101);
		m_vertRulerWnd.SetMeasurements(tplUnit);

		// init the corner window.
		rect.left = 0;
		rect.top = 0;
		rect.right = RULER_SIZE;
		rect.bottom = RULER_SIZE;
		m_cornerBoxWnd.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, 102);
	}
}

void CtplDesignView::OnDraw(CDC* /*pDC*/)
{
	CtplDesignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

void CtplDesignView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if( m_tplDesignWnd.m_hWnd )
	{
		m_tplDesignWnd.MoveWindow(RULER_SIZE, RULER_SIZE, cx - RULER_SIZE, cy - RULER_SIZE);
		m_horzRulerWnd.MoveWindow(RULER_SIZE, 0, cx, RULER_SIZE);
		m_vertRulerWnd.MoveWindow(0, RULER_SIZE, RULER_SIZE, cy);
	}
}

BOOL CtplDesignView::OnEraseBkgnd( CDC* /*pDC*/ ) 
{
	return TRUE;
}

void CtplDesignView::OnEditUndo()
{
	CMFCRibbonBar* pRibbon = ((CMDIFrameWndEx*)GetTopLevelFrame())->GetRibbonBar();
	CMFCRibbonUndoButton* pUndoBtn = DYNAMIC_DOWNCAST(CMFCRibbonUndoButton, pRibbon->FindByID(IDC_TPL_UNDO));

	int iCount = pUndoBtn->GetActionNumber();
	if (iCount == -1)
	{
		UpdateUndoRedoDropdownList(pUndoBtn);

		if (m_undoCommandList->size() > 0)
		{
			IRptCmdPtr pCommand = m_undoCommandList->at(m_undoCommandList->size()-1);
			RptCmdManager::GetInstance()->Undo(pCommand);
		}
	}
	else
	{
		if (pUndoBtn != NULL && iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				IRptCmdPtr pCommand = m_undoCommandList->at(m_undoCommandList->size()-i-1);
				RptCmdManager::GetInstance()->Undo(pCommand);
			}
		}
	}

	m_tplDesignWnd.RefreshForUndoRedo();
}

void CtplDesignView::OnEditRedo()
{
	CMFCRibbonBar* pRibbon = ((CMDIFrameWndEx*)GetTopLevelFrame())->GetRibbonBar();
	CMFCRibbonUndoButton* pRedoBtn = DYNAMIC_DOWNCAST(CMFCRibbonUndoButton, pRibbon->FindByID(IDC_TPL_REDO));

	int iCount = pRedoBtn->GetActionNumber();
	if (iCount == -1)
	{
		UpdateUndoRedoDropdownList(pRedoBtn);

		if (m_redoCommandList->size() > 0)
		{
			IRptCmdPtr pCommand = m_redoCommandList->at(m_redoCommandList->size()-1);
			RptCmdManager::GetInstance()->Redo(pCommand);
		}
	}
	else
	{
		if (pRedoBtn != NULL && iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				IRptCmdPtr pCommand = m_redoCommandList->at(m_redoCommandList->size()-i-1);
				RptCmdManager::GetInstance()->Redo(pCommand);
			}
		}
	}

	m_tplDesignWnd.RefreshForUndoRedo();
}

void CtplDesignView::OnDrawStatic()
{
	m_tplDesignWnd.StartDrawAction(ENTITY_STATIC);
}

void CtplDesignView::OnDrawLabel()
{
	m_tplDesignWnd.StartDrawAction(ENTITY_LABEL);
}

void CtplDesignView::OnDrawField()
{
	m_tplDesignWnd.StartDrawAction(ENTITY_FIELD);
}

void CtplDesignView::OnDrawStaticImg()
{
	m_tplDesignWnd.StartDrawAction(ENTITY_STATIC_IMG);
}

void CtplDesignView::OnDrawDynamicImg()
{
	m_tplDesignWnd.StartDrawAction(ENTITY_DYN_IMG);
}

void CtplDesignView::OnDrawGrid()
{
	m_tplDesignWnd.StartDrawAction(ENTITY_GRID);
}

void CtplDesignView::OnDrawLine()
{
	m_tplDesignWnd.StartDrawAction(ENTITY_LINE);
}

void CtplDesignView::OnDrawBox()
{
	m_tplDesignWnd.StartDrawAction(ENTITY_BOX);
}

void CtplDesignView::OnDrawEllipse()
{
	m_tplDesignWnd.StartDrawAction(ENTITY_ELLIPSE);
}

void CtplDesignView::OnDrawBarCode()
{
	m_tplDesignWnd.StartDrawAction(ENTITY_BARCODE);
}

void CtplDesignView::OnCopyEntity()
{
	m_tplDesignWnd.CopySelectedEntity();
}

void CtplDesignView::OnCutEntity()
{
	m_tplDesignWnd.CutSelectedEntity();
}

void CtplDesignView::OnPasteEntity()
{
	m_tplDesignWnd.PasteSelEntityToPt(CPoint(0,0));
}

void CtplDesignView::OnDeleteEntity()
{
	m_tplDesignWnd.DelSelectedEntity();
}

void CtplDesignView::OnGroupEntity()
{
	m_tplDesignWnd.GroupEntity();
}

void CtplDesignView::OnUnGroupEntity()
{
	m_tplDesignWnd.UnGroupEntity();
}

void CtplDesignView::OnEditEntity()
{
	m_tplDesignWnd.EditEntityProp();
}

void CtplDesignView::OnNoZoomPage()
{
	m_tplDesignWnd.NoZoomPage();
}

void CtplDesignView::OnZoomInPage()
{
	m_tplDesignWnd.ZoomInPage();
}

void CtplDesignView::OnZoomOutPage()
{
	m_tplDesignWnd.ZoomOutPage();
}

void CtplDesignView::UpdateUndoRedoDropdownList(CMFCRibbonBaseElement* pElem)
{
	switch (pElem->GetID())
	{
	case IDC_TPL_UNDO:
		{
			CMFCRibbonUndoButton* pUndo = DYNAMIC_DOWNCAST(CMFCRibbonUndoButton, pElem);
			pUndo->CleanUpUndoList();

			// get all undo items for this view.
			RptCmdListPtr pUndoCommands = RptCmdManager::GetInstance()->GetUndoCommands();
			m_undoCommandList = pUndoCommands->GetCommandByView(this);

			// get command names from end to begin.
			int iCount = m_undoCommandList->size();
			for (unsigned int i = 0; i < m_undoCommandList->size(); i++)
			{
				IRptCmdPtr pCommand = m_undoCommandList->at(m_undoCommandList->size()-i-1);
				pUndo->AddUndoAction(pCommand->GetCommandName());
			}
		}
		break;
	case IDC_TPL_REDO:
		{
			CMFCRibbonUndoButton* pRedo = DYNAMIC_DOWNCAST(CMFCRibbonUndoButton, pElem);
			pRedo->CleanUpUndoList();

			// get all redo items for this view.
			RptCmdListPtr pRedoCommands = RptCmdManager::GetInstance()->GetRedoCommands();
			m_redoCommandList = pRedoCommands->GetCommandByView(this);

			// get command names from end to begin.
			for (unsigned int i = 0; i < m_redoCommandList->size(); i++)
			{
				IRptCmdPtr pCommand = m_redoCommandList->at(m_redoCommandList->size()-i-1);
				pRedo->AddUndoAction(pCommand->GetCommandName());
			}
		}
		break;
	}
}

END_CUTLEADER_NAMESPACE()

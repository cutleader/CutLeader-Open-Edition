#include "stdafx.h"
#include "cLeaderView.h"

#include "XMenu.h"
#include "IEditor.h"
#include "CommandList.h"
#include "clUILibResource.h"
#include "CommandManager.h"
#include "clBaseDataResource.h"
#include "ClWindowFinder.h"
#include "ICommand.h"
#include "IAction.h"
#include "LogMgr.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "clFrameResource.h"
#include "cLeaderDoc.h"
#include "clFrameUIResource.h"
#include "UIManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CLeaderView, CView)

BEGIN_MESSAGE_MAP(CLeaderView, CView)
	ON_WM_DESTROY()

	ON_COMMAND(IDC_TAO_UNDO, OnEditUndo)
	ON_COMMAND(IDC_TAO_REDO, OnEditRedo)

	ON_MESSAGE(WM_displayPromptMessageOnStatusBar, OnDisplayPromptMessageOnStatusBar)
	ON_MESSAGE(WM_displayCursorPositionOnStatusBar, OnDisplayCursorPositionOnStatusBar)
END_MESSAGE_MAP()


CLeaderView::CLeaderView()
{
	m_pViewPort.reset(new GlViewPort);
	m_bPopupShow = FALSE;
    m_pUndoCmdList.reset(new CommandList);
    m_pRedoCmdList.reset(new CommandList);
	m_iStringID = INVALID_ID;
}

CLeaderView::~CLeaderView()
{
}

void CLeaderView::OnDraw(CDC*)
{
	CLeaderDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	// bind the OpenGL rendering context with device context.
	m_pViewPort->BindRendContext();

	// clear the current screen.
	m_pViewPort->ClearScreen();

	// set background color.
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();
	m_pViewPort->SetBackgroundColor(*pColorInfo->GetBackgroundColor());

	// 绘制坐标轴。
	m_pViewPort->DrawCoordinate(0, 0, 0, FALSE);

	if (!m_pCurEditor)
		return;
}

void CLeaderView::OnDestroy()
{
	// 从“命令管理器”清除掉这个视图的undo/redo项。
	CommandListPtr pUndoCommands = CommandManager::GetInstance()->GetUndoCommands();
	CommandListPtr pTmpUndoCommands = pUndoCommands->GetCommandByView(this);
	pUndoCommands->RemoveCommand(pTmpUndoCommands);
	CommandListPtr pRedoCommands = CommandManager::GetInstance()->GetRedoCommands();
	CommandListPtr pTmpRedoCommands = pRedoCommands->GetCommandByView(this);
	pRedoCommands->RemoveCommand(pTmpRedoCommands);

	if (m_pViewPort)
		m_pViewPort->ReleaseResource();

	CView::OnDestroy();
}

void CLeaderView::OnEditUndo()
{
	CMFCRibbonBar* pRibbon = ((CMDIFrameWndEx*)GetTopLevelFrame())->GetRibbonBar();
	CMFCRibbonUndoButton* pUndoBtn = DYNAMIC_DOWNCAST(CMFCRibbonUndoButton, pRibbon->FindByID(IDC_TAO_UNDO));
	if (!pUndoBtn)
	{
		CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DonotHideUndoRedoBtn);
		MessageBox(str, strProductName, MB_OK | MB_ICONINFORMATION);
		return;
	}

	int iCount = pUndoBtn->GetActionNumber();
	if (iCount == -1)
	{
		UpdateUndoRedoDropdownList(pUndoBtn);

		if (m_pUndoCmdList->size() > 0)
		{
			ICommandPtr pCommand = m_pUndoCmdList->at(m_pUndoCmdList->size()-1);

			// 看看要不要把这个命令送给任务视图去处理。
			if (ClWindowFinder::GetCurrentView_1()->GetSafeHwnd() != ClWindowFinder::GetCurrentView_2()->GetSafeHwnd()) { // 当前在板材视图下。
				if (pCommand->GetEditorData().GetView()->GetSafeHwnd() == ClWindowFinder::GetCurrentView_2()->GetSafeHwnd()) {
					LONGLONG* pCommandID = new LONGLONG(pCommand->GetID());
					ClWindowFinder::GetCurrentView_2()->PostMessage(WM_TaskViewExecuteUndoCommand, (WPARAM)pCommandID, 0);
					return;
				}
			}

			CommandManager::GetInstance()->Undo(pCommand);
            ProcessAfterUndo(pCommand.get());
			m_pCurEditor->UpdateActionForUndoRedo();
		}
	}
	else
	{
		if (pUndoBtn != NULL && iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				ICommandPtr pCommand = m_pUndoCmdList->at(m_pUndoCmdList->size()-i-1);

				// 看看要不要把这个命令送给任务视图去处理。
				if (ClWindowFinder::GetCurrentView_1()->GetSafeHwnd() != ClWindowFinder::GetCurrentView_2()->GetSafeHwnd()) { // 当前在板材视图下。
					if (pCommand->GetEditorData().GetView()->GetSafeHwnd() == ClWindowFinder::GetCurrentView_2()->GetSafeHwnd()) {
						LONGLONG* pCommandID = new LONGLONG(pCommand->GetID());
						ClWindowFinder::GetCurrentView_2()->PostMessage(WM_TaskViewExecuteUndoCommand, (WPARAM)pCommandID, 0);
						return;
					}
				}

				CommandManager::GetInstance()->Undo(pCommand);
                ProcessAfterUndo(pCommand.get());
			}

			m_pCurEditor->UpdateActionForUndoRedo();
		}
	}

	// 刷新视图。
	Invalidate();
}

void CLeaderView::OnEditRedo()
{
	CMFCRibbonBar* pRibbon = ((CMDIFrameWndEx*)GetTopLevelFrame())->GetRibbonBar();
	CMFCRibbonUndoButton* pRedoBtn = DYNAMIC_DOWNCAST(CMFCRibbonUndoButton, pRibbon->FindByID(IDC_TAO_REDO));
	if (!pRedoBtn)
	{
		CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DonotHideUndoRedoBtn);
		MessageBox(str, strProductName, MB_OK | MB_ICONINFORMATION);
		return;
	}

	int iCount = pRedoBtn->GetActionNumber();
	if (iCount == -1)
	{
		UpdateUndoRedoDropdownList(pRedoBtn);

		if (m_pRedoCmdList->size() > 0)
		{
			ICommandPtr pCommand = m_pRedoCmdList->at(m_pRedoCmdList->size()-1);

			// 看看要不要把这个命令送给任务视图去处理。
			if (ClWindowFinder::GetCurrentView_1()->GetSafeHwnd() != ClWindowFinder::GetCurrentView_2()->GetSafeHwnd()) { // 当前在板材视图下。
				if (pCommand->GetEditorData().GetView()->GetSafeHwnd() == ClWindowFinder::GetCurrentView_2()->GetSafeHwnd()) {
					LONGLONG* pCommandID = new LONGLONG(pCommand->GetID());
					ClWindowFinder::GetCurrentView_2()->PostMessage(WM_TaskViewExecuteRedoCommand, (WPARAM)pCommandID, 0);
					return;
				}
			}

			CommandManager::GetInstance()->Redo(pCommand);
			ProcessAfterRedo(pCommand.get());
			m_pCurEditor->UpdateActionForUndoRedo();
		}
	}
	else
	{
		if (pRedoBtn != NULL && iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				ICommandPtr pCommand = m_pRedoCmdList->at(m_pRedoCmdList->size()-i-1);

				// 看看要不要把这个命令送给任务视图去处理。
				if (ClWindowFinder::GetCurrentView_1()->GetSafeHwnd() != ClWindowFinder::GetCurrentView_2()->GetSafeHwnd()) { // 当前在板材视图下。
					if (pCommand->GetEditorData().GetView()->GetSafeHwnd() == ClWindowFinder::GetCurrentView_2()->GetSafeHwnd()) {
						LONGLONG* pCommandID = new LONGLONG(pCommand->GetID());
						ClWindowFinder::GetCurrentView_2()->PostMessage(WM_TaskViewExecuteRedoCommand, (WPARAM)pCommandID, 0);
						return;
					}
				}

				CommandManager::GetInstance()->Redo(pCommand);
				ProcessAfterRedo(pCommand.get());
			}

			// update the action after the undo/redo operation.
			m_pCurEditor->UpdateActionForUndoRedo();
		}
	}

	// 刷新视图。
	Invalidate();
}

void CLeaderView::DisplayPromptMessageOfAction(const IAction* pAction)
{
	int iMsgID = INVALID_ID;
	pAction->GetPromptMessage(iMsgID);
	if (iMsgID != m_iStringID) {
		m_iStringID = iMsgID;
		this->PostMessage(WM_displayPromptMessageOnStatusBar, m_iStringID, 0);
	}
}

LRESULT CLeaderView::OnDisplayPromptMessageOnStatusBar(WPARAM wParam, LPARAM lParam)
{
	if (wParam == INVALID_ID) {
		LogMgr::DebugWarn(_T("388909"));
		return 0;
	}

	CString str = MultiLanguageMgr::GetInstance()->TranslateString(wParam);
	UIManager::GetInstance()->GetStatusBar()->UpdateStatusBar(str, ID_PromptMessage_Pane);
	return 0;
}

LRESULT CLeaderView::OnDisplayCursorPositionOnStatusBar(WPARAM wParam, LPARAM lParam)
{
	// 当前光标位置。
	double dCursorPtX, dCursorPtY;
	m_pViewPort->GetCursorPos(dCursorPtX, dCursorPtY);
	CString strCoordinate;
	strCoordinate.Format(_T("%.4f , %.4f"), dCursorPtX, dCursorPtY);

	// 在状态栏显示。
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CURSOR_POSITION);
	str.Replace(_T("WILLBEREPLACED1"), strCoordinate);
	UIManager::GetInstance()->GetStatusBar()->UpdateStatusBar(str, ID_CursorPosition_Pane);

	return 0;
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "StatusBar.h"

#include "LogMgr.h"

BEGIN_CUTLEADER_NAMESPACE()

StatusBar::StatusBar(void)
{
}

StatusBar::~StatusBar(void)
{
}

void StatusBar::Init(CWnd* pParentWnd)
{
	m_statusBar.Create(pParentWnd);

	CMFCRibbonStatusBarPane* pCursorPositionPanel = new CMFCRibbonStatusBarPane(ID_CursorPosition_Pane, _T(""), TRUE);
	pCursorPositionPanel->SetAlmostLargeText(_T("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
	m_statusBar.AddElement(pCursorPositionPanel, _T(""));;

	m_statusBar.AddSeparator();
	m_statusBar.AddSeparator();

	CMFCRibbonStatusBarPane* pWorkModePanel = new CMFCRibbonStatusBarPane(ID_WorkMode_Pane, _T(""), TRUE);
	pWorkModePanel->SetAlmostLargeText(_T("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
	m_statusBar.AddElement(pWorkModePanel, _T(""));

	m_statusBar.AddSeparator();
	m_statusBar.AddSeparator();

	CMFCRibbonStatusBarPane* pPromptMessagePanel = new CMFCRibbonStatusBarPane(ID_PromptMessage_Pane, _T(""), TRUE);
	pPromptMessagePanel->SetAlmostLargeText(_T("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
	m_statusBar.AddElement(pPromptMessagePanel, _T(""));
}

void StatusBar::UpdateStatusBar(LPCTSTR str, int iID)
{
	CMFCRibbonBaseElement* pPane = m_statusBar.FindByID(iID);
	if (pPane) {
		pPane->SetText(str);
		pPane->Redraw();
	}
	else {
		LogMgr::DebugWarn(_T("653782"));
	}
}

CString StatusBar::GetPanelText(int iPanelID)
{
	CString strPanelText;

	if (iPanelID != ID_WorkMode_Pane && iPanelID != ID_CursorPosition_Pane && iPanelID != ID_PromptMessage_Pane)
	{
		LogMgr::DebugWarn(_T("632897"));
		return strPanelText;
	}

	CMFCRibbonBaseElement* pPane = m_statusBar.FindByID(iPanelID);
	if (pPane)
		strPanelText = pPane->GetText();
	else
		LogMgr::DebugWarn(_T("932419"));

	return strPanelText;
}

void StatusBar::Show()
{
	if (!m_statusBar.IsVisible())
		m_statusBar.ShowPane(TRUE, FALSE, TRUE);
}

void StatusBar::Hide()
{
	if (m_statusBar.IsVisible())
		m_statusBar.ShowPane(FALSE, FALSE, FALSE);
}

END_CUTLEADER_NAMESPACE()

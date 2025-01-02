#include "StdAfx.h"
#include "UIManager.h"
#include "PatternPropertyBar.h"

BEGIN_CUTLEADER_NAMESPACE()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

UIManager* UIManager::pUIManager = NULL;

UIManager::UIManager(void)
{
}

void UIManager::Init(CDockingManager* pDockingMgr, CWnd* pParent)
{
	m_pDockingMgr = pDockingMgr;
	m_pParent = pParent;

	m_ribbonBar.Init(m_pParent);
	m_statusBar.Init(m_pParent);
	m_barManager.Init(pDockingMgr, pParent);
}

UIManager* UIManager::GetInstance()
{
	if (pUIManager == NULL)
		pUIManager = new UIManager();
	return pUIManager;
}

UIManager::~UIManager(void)
{
}

void UIManager::UpdateRecentDataMenu()
{
	m_ribbonBar.UpdateRecentDataMenu();
}

END_CUTLEADER_NAMESPACE()

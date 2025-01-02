#pragma once

#include "clFrameCommon.h"
#include "StatusBar.h"
#include "BarManager.h"
#include "RibbonBar.h"

BEGIN_CUTLEADER_NAMESPACE()

// 界面管理器。
class CLFRAME_Export UIManager
{
private:
	UIManager(void);

public:
	~UIManager(void);

public: // set/get functions.
	BarManager* GetBarManager() { return &m_barManager; }
	RibbonBar* GetRibbonBar() { return &m_ribbonBar; }
	StatusBar* GetStatusBar() { return &m_statusBar; }

public: // other functions.
	static UIManager* GetInstance();

	void Init(CDockingManager* pDockingMgr, CWnd* pParent);

	// update "recent data" menu.
	void UpdateRecentDataMenu();

private:
	// the singleton object
	static UIManager *pUIManager;

	CWnd* m_pParent;
	CDockingManager* m_pDockingMgr;

	// the ribbon bar.
	RibbonBar m_ribbonBar;

	// the status bar.
	StatusBar m_statusBar;

	// manager of docking bars.
	BarManager m_barManager;
};

END_CUTLEADER_NAMESPACE()

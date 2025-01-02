#include "stdafx.h"
#include "MainFrame.h"

#include "CommandManager.h"
#include "ICommand.h"
#include "CommandList.h"
#include "ProcessMgr.h"
#include "LogMgr.h"
#include "ClWindowFinder.h"
#include "TaskEx.h"
#include "ProgressDlg.h"
#include "SkinManager.h"

#include "CLOptionInfo.h"
#include "ProductParam.h"

#include "PartDocument.h"
#include "BarManager.h"
#include "DocManager.h"
#include "UIManager.h"
#include "cLeaderView.h"
#include "PartInplaceEditData.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int iMaxUserToolbars = 10;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_COPYDATA()

	ON_MESSAGE(WM_CHILDFRM_CLOSE, OnChildFrmClose)
	ON_REGISTERED_MESSAGE(AFX_WM_ON_BEFORE_SHOW_RIBBON_ITEM_MENU, OnShowRibbonItemMenu)
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	UINT nKeyCode = pMsg->wParam;

	// 复制和撤销。
	if ((nKeyCode == _T('C') || nKeyCode == _T('V') || nKeyCode == _T('Z') || nKeyCode == _T('S')) && 
		(::GetKeyState(VK_CONTROL) & 0x8000))
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp()) // 当以后台cam方式运行时，禁用主窗口的最小化按钮。
	{
		cs.style &= ~(WS_MINIMIZEBOX);
	}

	return TRUE;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
		return FALSE;

	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( SkinManager ) );

	// set the Tab style.
	SetTabControlStyle();

	// enable docking for global.
	EnableDocking(CBRS_ALIGN_ANY);

	// init the UI manager
	UIManager::GetInstance()->Init(GetDockingManager(), this);

	CDockingManager::SetDockingMode(DT_SMART);
	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

	// 更新主窗口的关闭按钮。
	UpdateMainWindowCloseButton();

	return 0;
}

void CMainFrame::OnClose()
{
	// 分别给每个文档（其实是其对应的ChildFrame）发关闭消息。
	vector<CMDIChildWndEx*> allChildFrames = ClWindowFinder::FindAllChildFrames();
	for (unsigned int i = 0; i < allChildFrames.size(); i++)
	{
		CMDIChildWndEx* pChildFrame = allChildFrames[i];
		if (pChildFrame == nullptr)
		{
			LogMgr::DebugWarn(_T("536214"));
			return;
		}
		::SendMessage(pChildFrame->m_hWnd, WM_CLOSE, 0, 0);
	}

	CMDIFrameWndEx::OnClose();
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	WndMsg* pWndMsg = (WndMsg*)(pCopyDataStruct->lpData);
	if (pWndMsg->iWndMsgType == WNDMSG_ACTIVE_MAIN)
	{
		ShowWindow(SW_SHOWMAXIMIZED);
		::SetForegroundWindow(m_hWnd);
	}

	return CMDIFrameWndEx::OnCopyData(pWnd, pCopyDataStruct);
}

LRESULT CMainFrame::OnChildFrmClose(WPARAM wp, LPARAM lp)
{
	if (!MDIGetActive())
	{
		UIManager::GetInstance()->GetBarManager()->HideAllBar();
		UIManager::GetInstance()->GetStatusBar()->Hide();
	}

	return 0;
}

LRESULT CMainFrame::OnShowRibbonItemMenu(WPARAM, LPARAM lp)
{
	CMDIChildWnd* pChild = (CMDIChildWnd*)GetActiveFrame();
	if (pChild)
	{
		CView* pView = pChild->GetActiveView();
		CLeaderView* pTmpView = dynamic_cast<CLeaderView*>(pView);
		if (pTmpView)
			pTmpView->UpdateUndoRedoDropdownList((CMFCRibbonBaseElement*)lp);
	}

	return 0;
}

void CMainFrame::SetTabControlStyle()
{
	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE;
	mdiTabParams.m_bTabIcons = TRUE;
	mdiTabParams.m_bAutoColor = FALSE;
	mdiTabParams.m_bDocumentMenu = TRUE;
	if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp() || PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode())
	{
		mdiTabParams.m_bActiveTabCloseButton = FALSE;
		mdiTabParams.m_bTabCloseButton = FALSE;
	}
	else {
		mdiTabParams.m_bActiveTabCloseButton = TRUE;
		mdiTabParams.m_bTabCloseButton = TRUE;
	}
	EnableMDITabbedGroups(TRUE, mdiTabParams);
}

void CMainFrame::UpdateMainWindowCloseButton()
{
	if (CLOptionInfo::GetInstance()->GetProductParam()->GetIsRunAsBackgroundApp() ||
		PartInplaceEditData::GetInstance()->GetIsUnderInPlaceEditMode()) // 禁用主窗口的关闭按钮。
	{
		HMENU hSysMenu = ::GetSystemMenu(m_hWnd, FALSE);
		if (hSysMenu) {
			::EnableMenuItem(hSysMenu, SC_CLOSE, (MF_DISABLED | MF_GRAYED | MF_BYCOMMAND));
		}
		//ModifyStyle(WS_SYSMENU, 0, 0); // 这个函数会删掉3个按钮。
	} else {
		HMENU hSysMenu = ::GetSystemMenu(m_hWnd, FALSE);
		if (hSysMenu)
			::EnableMenuItem(hSysMenu, SC_CLOSE, MF_ENABLED);
	}
}

END_CUTLEADER_NAMESPACE()

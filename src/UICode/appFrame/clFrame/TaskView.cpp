#include "stdafx.h"
#include "TaskView.h"

#include "UnitHelper.h"
#include "ClBaseDataResource.h"
#include "LogMgr.h"
#include "EditorData.h"
#include "CommandManager.h"
#include "CommandList.h"
#include "ClWindowFinder.h"
#include "IntegerList.h"

#include "ReportPage.h"
#include "ReportPageList.h"
#include "ReportData.h"
#include "RptPageDrawer.h"
#include "ReportTplLoader.h"
#include "ClReportResource.h"
#include "ImpExpConfig.h"
#include "CLOptionInfo.h"

#include "TaskDocument.h"
#include "SheetList.h"
#include "Sheet.h"
#include "TaskItem.h"
#include "SheetSubView.h"
#include "ClTaskResource.h"
#include "UIManager.h"
#include "ClFrameResource.h"
#include "TaskItemManager.h"
#include "NestTaskPartsDlg.h"
#include "TaskPartManagementBar.h"
#include "TaskSheetManagementBar.h"
#include "NestTaskPartCommand.h"
#include "NestJob.h"
#include "ClPartLayoutResource.h"
#include "ChangeLoopToolPane.h"
#include "clCanvasGeometryUIResource.h"
#include "PatternList.h"
#include "TaskHelper.h"
#include "TextStructList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace std;


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TaskView, CTabView)

BEGIN_MESSAGE_MAP(TaskView, CTabView)
    ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()

    ON_MESSAGE(WM_openLastSheetInTask, OnOpenLastSheetInTask)
    ON_MESSAGE(WM_deleteSheetSubView, OnDeleteSheetSubView)
    ON_MESSAGE(WM_activeSheetSubView, OnActiveSheetSubView)
    ON_MESSAGE(WM_placePartOnCurrentSheetSubView, OnPlacePartOnCurrentSheetSubView)

    ON_MESSAGE(WM_PreviewTaskReport, OnPreviewTaskReport)
    ON_MESSAGE(WM_PrintTaskReport, OnPrintTaskReport)

    ON_MESSAGE(WM_TaskViewExecuteUndoCommand, OnExecuteUndoCommand)
    ON_MESSAGE(WM_TaskViewExecuteRedoCommand, OnExecuteRedoCommand)

    ON_MESSAGE(WM_NestTaskParts, OnNestTaskParts)

	ON_MESSAGE(WM_ReDisplayNestedCountOfEachPart, OnReDisplayNestedCountOfEachPart)
	ON_MESSAGE(WM_redrawSheetThumbnailOnSheetManagementPanel, OnRedrawSheetThumbnailOnSheetManagementPanel)
	ON_MESSAGE(WM_reDisplayTaskSheets, OnReDisplayTaskSheets)
	ON_MESSAGE(WM_PartAddedOnSheet, OnPartAddedOnSheet)
	ON_MESSAGE(WM_AddPattern2CurrentTask, OnAddPattern2CurrentTask)
    ON_MESSAGE(WM_UpdateLoopToolPanel, OnUpdateLoopToolPanel)
END_MESSAGE_MAP()


TaskView::TaskView()
{
}

TaskView::~TaskView()
{
}

BOOL TaskView::PreCreateWindow(CREATESTRUCT& cs)
{
	// You can modify the Window class or styles here by modifying the CREATESTRUCT cs.
	return CTabView::PreCreateWindow(cs);
}

void TaskView::OnActivateView(CView* pView)
{
	if (m_bDisableTabActiveEvent)
	{
		return;
	}

	if (pView)
	{
		if (SheetSubView* pSheetSubView = dynamic_cast<SheetSubView*>(pView))
		{
			// 根据激活的是第几个标签，得到对应的第几张板材，用这个板材初始化板材视图。
			{
				int iActiveTabIndex = this->GetTabControl().GetActiveTab(); // 得到当前tab的索引。
				const TaskItem* pTaskItem = this->GetTaskItem().get();
				SheetPtr pSheet = pTaskItem->GetSheetList()->operator [](iActiveTabIndex);
				pSheetSubView->InitSheetView(pSheet);
			}

			UIManager::GetInstance()->GetBarManager()->HideAllRightBar(); // 隐藏所有右侧的面板。
			ChangeLoopToolPanel::GetInstance()->GetDlg()->Init(pSheetSubView->GetSheet()->GetParamConfigID());
		}
		else
		{
			LogMgr::DebugWarn(_T("724198"));
		}
	}
	else // 在任务打开的早期，pView可能为空指针，这时不要处理。
	{
		;
	}
}

void TaskView::Print(CDC* pDC, CPrintInfo* pInfo) 
{
    // the report page which will display.
    UINT iCurPageIndex = pInfo->m_nCurPage;
    ReportPageListPtr pReportPages = m_pReportData->GetReportPageList();
    ReportPagePtr pReportPage = pReportPages->at(iCurPageIndex-1);

    // draw the current report page.
    RptPageDrawer::DrawRptPage(pReportPage, pDC);
}

void TaskView::PreparePrinting(CPrintInfo* pInfo)
{
    // set the report page count.
    ReportPageListPtr pReportPages = m_pReportData->GetReportPageList();
    pInfo->SetMaxPage(pReportPages->size());
}

CString TaskView::GetWndText()
{
    CString str;
    GetWindowText(str);
    return str;
}

void TaskView::SetWndFocus()
{
    SetFocus();
}

void TaskView::Update(int iEventID, const CComVariant& varEventContent)
{
	TaskItemPtr pTaskItem = this->GetTaskItem();
	if (iEventID == ID_EVENT_SheetChanged_byNesting)
	{
		// 这一段代码中会忽略标签页激活的事件，在这段代码中自己初始化板材视图。
		m_bDisableTabActiveEvent = true;

		// 得到当前的板材视图。
		SheetSubView* pSheetSubView = dynamic_cast<SheetSubView*>(ClWindowFinder::GetCurrentView_1());
		if (!pSheetSubView)
		{
			LogMgr::DebugWarn(_T("874567"));
			return;
		}

		int iExistingTabCount = this->GetTabControl().GetTabsNum(); // 当前板材的数量
		unsigned int iNewSheetCount = pTaskItem->GetSheetList()->size(); // 期望更新的任务中的板材数量 
		if (iExistingTabCount == iNewSheetCount) // 板材张数没有变化
		{
			;
		}
		else if (iExistingTabCount > iNewSheetCount) // 板材数量少了
		{
			// 删除标签页。
			while (this->GetTabControl().GetTabsNum() > iNewSheetCount)
			{
				int iTabsNum = this->GetTabControl().GetTabsNum();
				if (iTabsNum == 1) // 只有一个tab了，出问题了。
				{
					LogMgr::DebugWarn(_T("532199"));
					return;
				}
				RemoveView(iTabsNum - 1);
			}
		}
		else if (iExistingTabCount < iNewSheetCount) // 板材数量多了
		{
			// 添加新的标签页。
			unsigned int iNewTabCount = pTaskItem->GetSheetList()->size();
			for (int i = iExistingTabCount; i < iNewTabCount; i++)
			{
				const Sheet* pSheet = pTaskItem->GetSheetList()->operator [](i).get();
				m_wndTabs.InsertTab(pSheetSubView, pSheet->GetName(), i);
			}

			// InsertTab不会激活标签页，所以这里要激活最后一个标签页。
			SetActiveView(pTaskItem->GetSheetList()->size() - 1);
		}

		m_bDisableTabActiveEvent = false;

		// 初始化板材视图。
		SheetPtr pSheet = this->GetTaskItem()->GetSheetList()->operator [](this->GetTabControl().GetActiveTab());
		pSheetSubView->InitSheetView(pSheet);

		// 刷新任务管理器。
		TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pTaskItem);
		TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->Redisplay_TaskSheetManagementDlg(pTaskItem);

		UIManager::GetInstance()->GetBarManager()->HideAllRightBar(); // 隐藏所有右侧的面板。
		ChangeLoopToolPanel::GetInstance()->GetDlg()->Init(pSheetSubView->GetSheet()->GetParamConfigID());
	}
}

void TaskView::OnDraw(CDC* /*pDC*/)
{
}

int TaskView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
    {
		return -1;
    }

    // 修改样式
    GetTabControl().ModifyTabStyle(CMFCTabCtrl::STYLE_3D_VS2005);

	return 0;
}

void TaskView::OnDestroy()
{
    // 从“命令管理器”清除掉这个视图的undo/redo项。
    CommandListPtr pUndoCommands = CommandManager::GetInstance()->GetUndoCommands();
    CommandListPtr pTmpUndoCommands = pUndoCommands->GetCommandByView(this);
    pUndoCommands->RemoveCommand(pTmpUndoCommands);
    CommandListPtr pRedoCommands = CommandManager::GetInstance()->GetRedoCommands();
    CommandListPtr pTmpRedoCommands = pRedoCommands->GetCommandByView(this);
    pRedoCommands->RemoveCommand(pTmpRedoCommands);

    CTabView::OnDestroy();
}

BOOL TaskView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

CView* TaskView::GetSheetSubViewByTabIndex(int iTabIndex) const
{
    CView* pSheetSubView = NULL;

    int iViewIndex = -1;
    CDocument* pDoc = this->GetDocument();
    POSITION pos = pDoc->GetFirstViewPosition();
    while (pos) 
    {
        CView* pView = pDoc->GetNextView(pos); 
        if (iViewIndex == iTabIndex)
        {
            pSheetSubView = pView;
            break;
        }
        iViewIndex++;
    }
   
    return pSheetSubView;
}

void TaskView::InitTaskView()
{
	TaskItemPtr pTaskItem = this->GetTaskItem();
	const SheetList* pSheetList = pTaskItem->GetSheetList().get();
	if (pSheetList->size() == 0)
	{
		LogMgr::DebugWarn(_T("629842"));
		return;
	}

	// 这段代码会创建每个标签页以及那个唯一的板材视图，并用第一个板材来初始化板材视图，第一个标签页是当前的标签页。
	// 注：
	// 1) 这里忽略掉标签页的切换事件，相应的工作在这个函数完成。
	// 2) 从第二个标签页开始创建，第一个标签页最后创建，要是不按照这个顺序，会导致板材视图接受不到鼠标消息。
	// 3) 调用InsertTab添加第一个标签页时会触发标签页切换事件，再调用就不会了。
	m_bDisableTabActiveEvent = true;
	SheetSubView* pSheetSubView;
	{
		// 创建一个板材视图对象并加入文档。
		pSheetSubView = DYNAMIC_DOWNCAST(SheetSubView, RUNTIME_CLASS(SheetSubView)->CreateObject());
		pSheetSubView->Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), &m_wndTabs, (UINT)-1, NULL);
		GetDocument()->AddView(pSheetSubView);

		// 从第二个标签页开始添加。
		for (unsigned int i = 1; i < pSheetList->size(); i++)
		{
			SheetPtr pSheet = pSheetList->operator [](i);
			m_wndTabs.InsertTab(pSheetSubView, pSheet->GetName(), i - 1);
		}

		// 添加第一个标签。
		m_wndTabs.InsertTab(pSheetSubView, pSheetList->front()->GetName(), 0);

		// 用第一张板材来初始化板材视图。
		pSheetSubView->InitSheetView(pSheetList->front());
	}
	m_bDisableTabActiveEvent = false;

	// 禁止拖动标签。
	m_wndTabs.EnableTabSwap(FALSE);

	UIManager::GetInstance()->GetBarManager()->HideAllRightBar(); // 隐藏所有右侧的面板。

	pTaskItem->AddObserver(this);
}

void TaskView::InitTaskView_4SaveAsTask()
{
	TaskItemPtr pTaskItem = this->GetTaskItem();

	// 根据当前激活的是第几个标签，得到对应的第几张板材，用这个板材初始化板材视图。
	{
		// 初始化板材视图。
		SheetSubView* pSheetSubView = dynamic_cast<SheetSubView*>(ClWindowFinder::GetCurrentView_1());
		if (!pSheetSubView)
		{
			LogMgr::DebugWarn(_T("762197"));
			return;
		}
		SheetPtr pSheet = pTaskItem->GetSheetList()->operator [](this->GetTabControl().GetActiveTab());
		pSheetSubView->InitSheetView(pSheet);

		UIManager::GetInstance()->GetBarManager()->HideAllRightBar(); // 隐藏所有右侧的面板。
	}

	pTaskItem->AddObserver(this);
}

LRESULT TaskView::OnOpenLastSheetInTask(WPARAM wParam, LPARAM lParam)
{
	// 得到板材视图。
	SheetSubView* pSheetSubView = dynamic_cast<SheetSubView*>(ClWindowFinder::GetCurrentView_1());
	if (!pSheetSubView)
	{
		LogMgr::DebugWarn(_T("645371"));
		return 0;
	}

	// 给tab view添加第一个标签。
	const TaskItem* pTaskItem = GetTaskItem().get();
	SheetPtr pSheet = pTaskItem->GetSheetList()->back();
	m_wndTabs.InsertTab(pSheetSubView, pSheet->GetName(), pTaskItem->GetSheetList()->size() - 1);

	// 这次InsertTab调用不会触发标签页激活事件，所以这里要激活这个标签页。
	SetActiveView(pTaskItem->GetSheetList()->size() - 1);

	// 隐藏所有右侧的面板。
	UIManager::GetInstance()->GetBarManager()->HideAllRightBar();

	return 0;
}

LRESULT TaskView::OnDeleteSheetSubView(WPARAM wParam, LPARAM lParam)
{
	int iTabIndex = wParam;

	if (this->GetTabControl().GetTabsNum() == 1) // 只有一个tab时不可能跑到这里。 
	{
		LogMgr::DebugWarn(_T("213876"));
	}

	// 删掉一个tab，mfc框架会自然切换到被删除标签的下一个标签，这也正是我们期望的。这里会忽略标签页激活的事件，因为会促发两次，没必要，下面自己初始化板材视图。
	m_bDisableTabActiveEvent = true;
	RemoveView(iTabIndex);
	m_bDisableTabActiveEvent = false;

	// 根据激活的是第几个标签，得到对应的第几张板材，用这个板材初始化板材视图。
	{
		// 初始化板材视图。
		SheetSubView* pSheetSubView = dynamic_cast<SheetSubView*>(ClWindowFinder::GetCurrentView_1());
		if (!pSheetSubView)
		{
			LogMgr::DebugWarn(_T("840966"));
			return 0;
		}
		SheetPtr pSheet = this->GetTaskItem()->GetSheetList()->operator [](this->GetTabControl().GetActiveTab());
		pSheetSubView->InitSheetView(pSheet);

		UIManager::GetInstance()->GetBarManager()->HideAllRightBar(); // 隐藏所有右侧的面板。
		ChangeLoopToolPanel::GetInstance()->GetDlg()->Init(pSheetSubView->GetSheet()->GetParamConfigID());
	}

    return 0;
}

LRESULT TaskView::OnActiveSheetSubView(WPARAM wParam, LPARAM lParam)
{
    LONGLONG* pSheetID = (LONGLONG*)wParam;

	// 得到要激活第几个板材，也对应第几个标签。
	const TaskItem* pTaskItem = this->GetTaskItem().get();
	int iSheetIndex = INVALID_IDX;
	for (unsigned int i = 0; i < pTaskItem->GetSheetList()->size(); i++)
	{
		SheetPtr pTmpSheet = pTaskItem->GetSheetList()->operator[](i);
		if (pTmpSheet->GetID() == *pSheetID)
		{
			iSheetIndex = i;
			break;
		}
	}
	if (iSheetIndex == INVALID_IDX)
	{
		LogMgr::DebugWarn(_T("298549"));
		return 0;
	}

	// 激活视图，这会触发标签页激活事件，那里会初始化板材视图。
	SetActiveView(iSheetIndex);

    return 0;
}

LRESULT TaskView::OnPlacePartOnCurrentSheetSubView(WPARAM wParam, LPARAM lParam)
{
    LONGLONG* pPartID = (LONGLONG*)wParam;
    SheetSubView* pSheetView = (SheetSubView*)GetActiveView();
    pSheetView->PlacePartOnCurrentSheetSubView(*pPartID);

    return 0;
}

LRESULT TaskView::OnPreviewTaskReport(WPARAM wParam, LPARAM lParam)
{
    if (GenerateTaskRpt())
    {
        PreviewData();
    }

    return 0;
}

LRESULT TaskView::OnPrintTaskReport(WPARAM wParam, LPARAM lParam)
{
    if (GenerateTaskRpt())
    {
        PrintData();
    }

    return 0;
}

LRESULT TaskView::OnExecuteUndoCommand(WPARAM wParam, LPARAM lParam)
{
    LONGLONG* pCommandID = (LONGLONG*)wParam;
    CommandListPtr pUndoCommands = CommandManager::GetInstance()->GetUndoCommands();
    ICommandPtr pCommand = pUndoCommands->GetCommandByID(*pCommandID);
    CommandManager::GetInstance()->Undo(pCommand);
    delete pCommandID;

    return 0;
}

LRESULT TaskView::OnExecuteRedoCommand(WPARAM wParam, LPARAM lParam)
{
    LONGLONG* pCommandID = (LONGLONG*)wParam;
    CommandListPtr pRedoCommands = CommandManager::GetInstance()->GetRedoCommands();
    ICommandPtr pCommand = pRedoCommands->GetCommandByID(*pCommandID);
    CommandManager::GetInstance()->Redo(pCommand);
    delete pCommandID;

    return 0;
}

LRESULT TaskView::OnNestTaskParts(WPARAM wParam, LPARAM lParam)
{
	// 得到当前板材
	SheetPtr pActiveSheet;
	{
		CMDIFrameWnd* pMainFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
		CMDIChildWnd* pChildFrame = (CMDIChildWnd*)pMainFrame->GetActiveFrame();
		SheetSubView* pSheetSubView = (SheetSubView*)pChildFrame->GetActiveView();
		if (!pSheetSubView)
		{
			LogMgr::DebugWarn(_T("21032"));
			return 0;
		}
		pActiveSheet = pSheetSubView->GetSheet();
	}

    // 任务中必须要有未排完的零件
    TaskItemPtr pTaskItem = GetTaskItem();
    if (!TaskItemManager::WhetherHaveUnfinishedParts(pTaskItem.get()))
    {
        CString strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
        CString strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AllPartsfinished);
        MessageBox(strMsg, strTitle, MB_ICONWARNING);
        return 0;
    }

    NestTaskPartsDlg dlg;
	dlg.Init(pTaskItem, TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->GetSelectedTaskParts(), pActiveSheet);
    if (dlg.DoModal() == IDC_BTN_StartNesting)
    {
        TaskPartsNestResultPtr pTaskPartsNestResult = dlg.GetTaskPartsNestResult();

        // 执行命令以应用排版结果。
        EditorData editorData(ClWindowFinder::GetCurrentView_2(), _T(""));
        ICommandPtr pCommand(new NestTaskPartCommand(editorData, pTaskItem, pTaskPartsNestResult));
        pCommand->Do();
        CommandManager::GetInstance()->AddCommand(pCommand);
    }

    return 0;
}

LRESULT TaskView::OnReDisplayNestedCountOfEachPart(WPARAM wParam, LPARAM lParam)
{
	TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->ReDisplayNestedCount_ofEachPart();
	return 0;
}

LRESULT TaskView::OnReDisplayTaskSheets(WPARAM wParam, LPARAM lParam)
{
	TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->Redisplay_TaskSheetManagementDlg(GetTaskItem());
	return 0;
}

LRESULT TaskView::OnAddPattern2CurrentTask(WPARAM wParam, LPARAM lParam)
{
	PatternList* pPatterns = (PatternList*)wParam;
	if (!pPatterns || pPatterns->size() == 0)
	{
		LogMgr::DebugWarn(_T("243085"));
		return 0;
	}
	TextStructList* pTextStructs = (TextStructList*)lParam;
	if (!pTextStructs)
	{
		LogMgr::DebugWarn(_T("998435"));
		return 0;
	}
	TaskHelper::CreatePartAndAdd2Task(pPatterns, pTextStructs, GetTaskItem());

	return 0;
}

LRESULT TaskView::OnUpdateLoopToolPanel(WPARAM wParam, LPARAM lParam)
{
    SheetSubView* pSheetSubView = dynamic_cast<SheetSubView*>(ClWindowFinder::GetCurrentView_1());
    if (!pSheetSubView)
    {
        LogMgr::DebugWarn(_T("653899"));
        return 0;
    }

	ChangeLoopToolPanel::GetInstance()->GetDlg()->Init(pSheetSubView->GetSheet()->GetParamConfigID());

	return 0;
}

LRESULT TaskView::OnPartAddedOnSheet(WPARAM wParam, LPARAM lParam)
{
	TaskItemPtr pTaskItem = GetTaskItem();
	if (TaskItemManager::CheckWhetherNewPartsInSheets(pTaskItem)) // 任务中增加了新零件，所以需要重新显示零件管理器面板。
	{
		TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pTaskItem);
	}
	else // 任务中没有增加新零件，所以只需更新零件管理器面板中的零件已排数量。
	{
		TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->ReDisplayNestedCount_ofEachPart();
	}
	
	return 0;
}

TaskItemPtr TaskView::GetTaskItem() const
{
    TaskDocument* pTaskDoc = dynamic_cast<TaskDocument*>(GetDocument());
    return pTaskDoc->GetTaskItem();
}

BOOL TaskView::GenerateTaskRpt()
{
    // 加载报表模板。
    ImpExpConfigPtr pImpExpConfig = CLOptionInfo::GetInstance()->GetImpExpConfig();
    LONGLONG iNestRptTplID = pImpExpConfig->GetNestRptTplID();
    ReportTplPtr pReportTpl = ReportTplLoader::LoadReportTpl(iNestRptTplID);
    if (pReportTpl)
    {
        CClientDC dc(this);
        UnitHelper::Init(dc.GetDeviceCaps(LOGPIXELSX));
        TaskItemPtr pTaskItem = GetTaskItem();
        m_pReportData = TaskItemManager::GenerateTaskRpt(pTaskItem, pReportTpl.get());
    }
    else
    {
        CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
        CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_WRONG_DFL_TPL);
        MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;
}

LRESULT TaskView::OnRedrawSheetThumbnailOnSheetManagementPanel(WPARAM wParam, LPARAM lParam)
{
	SheetSubView* pSheetSubView = dynamic_cast<SheetSubView*>(ClWindowFinder::GetCurrentView_1());
	TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->RedrawSheetThumbnail(pSheetSubView->GetSheet()->GetID());
	return 0;
}

END_CUTLEADER_NAMESPACE();
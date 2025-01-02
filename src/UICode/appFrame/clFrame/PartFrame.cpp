#include "StdAfx.h"
#include "PartFrame.h"

#include "baseConst.h"

#include "PartCadData.h"
#include "PartItem.h"
#include "Part.h"

#include "DataItemTracker.h"
#include "UIManager.h"
#include "PartView.h"
#include "PartDocument.h"
#include "PartCommonFunctionPanel.h"
#include "TaskCommonFunctionPanel.h"
#include "TaskPartManagementBar.h"
#include "TaskSheetManagementBar.h"
#include "ChangeLoopToolPane.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(PartFrame, CChildFrame)

BEGIN_MESSAGE_MAP(PartFrame, CChildFrame)
	ON_WM_MDIACTIVATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()

END_MESSAGE_MAP()

PartFrame::PartFrame(void)
{
}

PartFrame::~PartFrame(void)
{
}

void PartFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CChildFrame::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	if (bActivate)
	{
		// 隐藏面板。
		ChangeLoopToolPanel::GetInstance()->ShowBar(FALSE, TRUE);
		TaskPartManagementBar::GetInstance()->ShowBar(FALSE, TRUE);
		TaskSheetManagementBar::GetInstance()->ShowBar(FALSE, TRUE);

		// 显示常用功能面板。
		TaskCommonFunctionPanel::GetInstance()->ShowBar(FALSE, TRUE);
		PartCommonFunctionPanel::GetInstance()->ShowBar(TRUE, TRUE);

		// 隐藏所有右侧的面板。
		UIManager::GetInstance()->GetBarManager()->HideAllRightBar();

		// 显示状态栏。
		UIManager::GetInstance()->GetStatusBar()->Show();

		// 让主框架重新布局。
		((CMDIFrameWnd*)AfxGetApp()->GetMainWnd())->RecalcLayout();
	}
}

void PartFrame::OnClose()
{
	// check whether the current document has been modified.
	CDocument* pTmpDoc = GetActiveDocument();
	PartDocument* pPartDoc = dynamic_cast<PartDocument*>(pTmpDoc);
	pPartDoc->SetModifyStatus();

	CChildFrame::OnClose();
}

void PartFrame::OnDestroy()
{
	/************************************************************************/
	// remove the part from DataItemTracker. 

	CLeaderView* pView = (CLeaderView*)GetActiveView();
	PartDocument* pPartDocument = (PartDocument*)pView->GetDocument();
	PartItemPtr pPartItem = pPartDocument->GetPartItem();
	LONGLONG iPartID = pPartItem->GetPart()->GetID();
	DataItemTracker::GetInstance()->RemoveDataItem(iPartID);
	/************************************************************************/

	// tell CMainFrame that i has been destroyed.
	HWND hwnd = GetParent()->GetParent()->GetSafeHwnd();
	::SendMessage(hwnd, WM_CHILDFRM_CLOSE, 0, 0);

	CChildFrame::OnDestroy();
}

END_CUTLEADER_NAMESPACE()

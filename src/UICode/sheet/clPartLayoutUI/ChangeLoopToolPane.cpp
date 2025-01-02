#include "stdafx.h"
#include "ChangeLoopToolPane.h"

#include "clFrameResource.h"
#include "clPartLayoutUIResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

ChangeLoopToolPanel* ChangeLoopToolPanel::m_pChangeLoopToolPanel = NULL;

BEGIN_MESSAGE_MAP(ChangeLoopToolPanel, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

ChangeLoopToolPanel::ChangeLoopToolPanel()
{
}

ChangeLoopToolPanel::~ChangeLoopToolPanel()
{
}

ChangeLoopToolPanel* ChangeLoopToolPanel::GetInstance(CWnd* pParent)
{
	if (m_pChangeLoopToolPanel == NULL)
	{
		m_pChangeLoopToolPanel = new ChangeLoopToolPanel();
		m_pChangeLoopToolPanel->m_pMainWnd = pParent;
		m_pChangeLoopToolPanel->Create(_T(""), (CWnd*)pParent, CRect(0, 0, 40, 500), TRUE, IDC_ChangeLoopTool_Bar,
			WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_RIGHT|CBRS_FLOAT_MULTI);
	}

	return m_pChangeLoopToolPanel;
}

int ChangeLoopToolPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create the sub dialog.
	m_changeLoopToolDlg.Create(IDD_clPartLayoutUI_ChangeLoopTool, this);

	return 0;
}

void ChangeLoopToolPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// position the sub dialog
	CRect rectClient;
	GetClientRect(rectClient);

	m_changeLoopToolDlg.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	m_changeLoopToolDlg.ShowWindow(SW_SHOW);
}

END_CUTLEADER_NAMESPACE()

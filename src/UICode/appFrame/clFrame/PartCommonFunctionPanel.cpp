#include "stdafx.h"
#include "PartCommonFunctionPanel.h"

#include "clUtilityResource.h"

#include "ClFrameResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

PartCommonFunctionPanel* PartCommonFunctionPanel::m_pPartCommonFunctionPanel = NULL;

BEGIN_MESSAGE_MAP(PartCommonFunctionPanel, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

PartCommonFunctionPanel::PartCommonFunctionPanel()
{
}

PartCommonFunctionPanel::~PartCommonFunctionPanel()
{
}

PartCommonFunctionPanel* PartCommonFunctionPanel::GetInstance(CWnd* pParent)
{
	if (m_pPartCommonFunctionPanel == NULL)
	{
		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ToolBar);

		m_pPartCommonFunctionPanel = new PartCommonFunctionPanel();
		m_pPartCommonFunctionPanel->m_pMainWnd = pParent;
		m_pPartCommonFunctionPanel->Create(str, (CWnd*)pParent, CRect(0, 0, 50, 200), TRUE, IDC_PartCommonFunctionBar, 
			WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	}

	return m_pPartCommonFunctionPanel;
}

int PartCommonFunctionPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create the sub dialog.
	m_partFunctionToolBar.Create(IDD_clFrame_PartFunctionToolBar, this);

	return 0;
}

void PartCommonFunctionPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// position the sub dialog
	CRect rectClient;
	GetClientRect(rectClient);
	m_partFunctionToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	m_partFunctionToolBar.ShowWindow(SW_SHOW);
}

END_CUTLEADER_NAMESPACE()

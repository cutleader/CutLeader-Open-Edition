#include "stdafx.h"
#include "LoopToolEditPanel.h"

#include "baseConst.h"
#include "clCutFeatureResource.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

LoopToolEditPanel* LoopToolEditPanel::m_pLoopToolEditPane = NULL;

BEGIN_MESSAGE_MAP(LoopToolEditPanel, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()

END_MESSAGE_MAP()

LoopToolEditPanel::LoopToolEditPanel()
{
}

LoopToolEditPanel::~LoopToolEditPanel()
{
}

LoopToolEditPanel* LoopToolEditPanel::GetInstance(CWnd* pParent)
{
	if (m_pLoopToolEditPane == NULL)
	{
		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROPBAR_LOOPTOOL);

		m_pLoopToolEditPane = new LoopToolEditPanel();
		m_pLoopToolEditPane->m_pMainWnd = pParent;
		if (!m_pLoopToolEditPane->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200), TRUE, IDC_PROPBAR_LOOPTOOL, 
									  WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
		{
			return NULL;
		}
	}

	return m_pLoopToolEditPane;
}

int LoopToolEditPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create the sub dialog.
	m_loopToolEditDlg.Create(IDD_clCutFeatureUI_LoopToolInfo, this);

	return 0;
}

void LoopToolEditPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// position the sub dialog
	CRect rectClient;
	GetClientRect(rectClient);
	m_loopToolEditDlg.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	m_loopToolEditDlg.ShowWindow(SW_SHOW);
}

void LoopToolEditPanel::DisplayLoopToolInfo(const EditorData& editorData, const vector<LoopTopologyItemListPtr>& loopTopologyItemData, LoopToolDataListPtr pLoopToolList,
	LoopStartCutDataListPtr pLoopStaEndCutList, LeadConfigItemPtr pLeadConfigItem, ToolAssignConfigItemPtr pToolAssignConfigItem, ConditionTypeListPtr pCondTypeList)
{
	m_editorData = editorData;
	m_loopToolEditDlg.DisplayLoopToolInfo(loopTopologyItemData, pLoopToolList, pLoopStaEndCutList, pLeadConfigItem, pToolAssignConfigItem, pCondTypeList, GetEditorData());

    if (!IsVisible())
        ShowPane(TRUE, FALSE, TRUE);
}

END_CUTLEADER_NAMESPACE()

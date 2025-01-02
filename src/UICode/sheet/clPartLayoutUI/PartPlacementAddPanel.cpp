#include "stdafx.h"
#include "PartPlacementAddPanel.h"

#include "Rect2D.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"
#include "DataCenterItemLoader.h"

#include "PatternList.h"
#include "PartCadData.h"
#include "PartItem.h"
#include "PartLoader.h"
#include "PartManager.h"
#include "clPartLayoutResource.h"
#include "PartLayoutEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementAddPanel* PartPlacementAddPanel::m_pAddPrtPmtPane = NULL;

BEGIN_MESSAGE_MAP(PartPlacementAddPanel, XBarBase)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

PartPlacementAddPanel::PartPlacementAddPanel()
{
}

PartPlacementAddPanel::~PartPlacementAddPanel()
{
}

PartPlacementAddPanel* PartPlacementAddPanel::GetInstance(CWnd* pParent)
{
	if (m_pAddPrtPmtPane == NULL)
	{
		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROPBAR_AddPRTPMT);

		m_pAddPrtPmtPane = new PartPlacementAddPanel();
		m_pAddPrtPmtPane->m_pMainWnd = pParent;
		if (!m_pAddPrtPmtPane->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200), TRUE, IDC_PROPBAR_AddPRTPMT, 
			WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
		{
			return NULL;
		}
	}

	return m_pAddPrtPmtPane;
}

int PartPlacementAddPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create the sub dialog.
	m_partAddDlg.Create(IDD_clPartLayoutUI_PartAdd, this);

	return 0;
}

void PartPlacementAddPanel::OnDestroy()
{
	__super::OnDestroy();
}

void PartPlacementAddPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// position the sub dialog
	CRect rectClient;
	GetClientRect(rectClient);
	m_partAddDlg.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	m_partAddDlg.ShowWindow(SW_SHOW);
}

void PartPlacementAddPanel::DisplayPartList()
{
	FolderNodePtr pWorkspace = DataCenterItemLoader::LoadProductTree(DataBaseManager::GetInstance()->GetMainDBConnect());
	m_partAddDlg.DisplayPartList(pWorkspace);

	if (!this->IsVisible()) {
		this->ShowPane(TRUE, FALSE, TRUE);
	}
}

END_CUTLEADER_NAMESPACE()

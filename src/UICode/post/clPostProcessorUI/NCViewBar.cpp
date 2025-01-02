#include "stdafx.h"
#include "NCViewBar.h"

#include "baseConst.h"
#include "clUtilityResource.h"
#include "GlViewPort.h"
#include "clUILibResource.h"
#include "clBaseDataResource.h"
#include "versionInfo.h"
#include "MultiLanguageMgr.h"
#include "CGridColumnTraitText.h"
#include "CGridColumnTraitEdit.h"

#include "CutNode.h"
#include "CutNodeList.h"
#include "ISequenceSimulateShape.h"
#include "SequenceSimulateShapeList.h"
#include "CutSequenceDrawer.h"

#include "NCCodeData.h"
#include "NCContainer.h"
#include "NCSimulateItem.h"
#include "NCSimulateItemList.h"
#include "NCRow.h"
#include "NCRowList.h"
#include "clPostProcessorResource.h"
#include "NCSimulateAction.h"
#include "MacRouterEditor.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

NCViewBar* NCViewBar::m_pNCViewBar = NULL;

BEGIN_MESSAGE_MAP(NCViewBar, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()

	ON_NOTIFY(LVN_GETDISPINFO, IDC_LISTCTRL_NC, OnGetdispinfo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL_NC, OnItemchangedList)
    ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LISTCTRL_NC, OnGrid_beginLabelEdit)
    ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LISTCTRL_NC, OnGrid_endLabelEdit)

	ON_BN_CLICKED(IDC_MACROUTER_SAVE_NC_CODE, OnSaveNCCode)
	ON_UPDATE_COMMAND_UI(IDC_MACROUTER_SAVE_NC_CODE, OnUpdateClickNCBtn)

END_MESSAGE_MAP()

NCViewBar::NCViewBar()
{
	m_bFillingGrid = FALSE;
}

NCViewBar::~NCViewBar()
{
}

// get the singleton object
NCViewBar* NCViewBar::GetInstance(CWnd* pParent)
{
	if (m_pNCViewBar == NULL)
	{
		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MACROUTER_NC_CODE);

		m_pNCViewBar = new NCViewBar();
		m_pNCViewBar->m_pMainWnd = pParent;
		if (!m_pNCViewBar->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200), TRUE, IDC_MACROUTER_VIEW_NC_BAR,
			WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
			return NULL;
	}

	return m_pNCViewBar;
}

/////////////////////////////////////////////////////////////////////////////

int NCViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create controls.
	CreateNCGrid();
	CreateNCBtn();

	// layout the windows in this pane.
	AdjustLayout();

	return 0;
}

void NCViewBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// layout the windows in this pane.
	AdjustLayout();
}

void NCViewBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_saveNCBtn.EnableWindow(TRUE);
}

void NCViewBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	// the height of the two windows.
	int iHeight1 = rectClient.Height() * 19 / 20;
	int iHeight2 = rectClient.Height() * 1 / 20;
	int iDeduction = 0; // the space between two windows.

	m_ncListGrid.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), iHeight1-iDeduction, SWP_NOACTIVATE | SWP_NOZORDER);
	m_saveNCBtn.SetWindowPos(NULL, rectClient.left, iHeight1, rectClient.Width(), iHeight2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void NCViewBar::InitBar(CString strSheetName, SequenceSimulateShapeListPtr pSeqSimShapeList, NCCodeDataPtr pNCCodeData, MacRouterEditorPtr pPostEditor, CString strNCPostfix,
    LoopInstanceListPtr pAllLoopInstances, const std::set<LONGLONG>& patternLoops_failedToOffset)
{
	m_strSheetName = strSheetName;
	m_pSequenceSimulateShapeList = pSeqSimShapeList;
	m_pNCCodeData = pNCCodeData;
	m_pMacRouterEditor = pPostEditor;
	m_strNCPostfix = strNCPostfix;
    m_pAllLoopInstances = pAllLoopInstances;
    m_patternLoops_failedToOffset = patternLoops_failedToOffset;

	// init the grid.
	m_bFillingGrid = TRUE;
	InitNCGrid();
	m_bFillingGrid = FALSE;

	// select the first row.
	m_ncListGrid.SelectRow(0, TRUE);
}

void NCViewBar::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;
	int iItemIndex = pItem->iItem;

	// 这一行NC代码
	const NCRow* pNCRow = m_pNCCodeData->GetNCRowList()->operator [](iItemIndex).get();

	if (pItem->mask & LVIF_TEXT)
	{
		if (pItem->iSubItem == 1)
		{
			CString strNum;
			strNum.Format(INT_FORMAT, iItemIndex + 1);
			lstrcpyn(pItem->pszText, strNum, pItem->cchTextMax);
		}
		else if (pItem->iSubItem == 2)
		{
			lstrcpyn(pItem->pszText, pNCRow->GetNCRow(), pItem->cchTextMax);
		}
	}

	*pResult = 0;
}

void NCViewBar::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_bFillingGrid)
		return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// get the selected NC row, and simulate it.
	if (pNMListView->iItem >= 0 && pNMListView->uNewState & LVIS_SELECTED)
	{
		NCSimulateItemListPtr pNCSimItemList = m_pNCCodeData->GetNCSimItemList();

		// 选中的NC代码行。
		IntListPtr pNCRowList = GetSelectedNCRow();

		// 得到选中的NC代码行对应的切割节点。
		CutNodeListPtr pCutNodeList(new CutNodeList);
		for (unsigned int i = 0; i < pNCRowList->size(); i++)
		{
			int iNCRowIndex = pNCRowList->operator [](i);
			const NCRow* pNCRow = m_pNCCodeData->GetNCRowList()->operator [](iNCRowIndex).get();

			// get the simulate shapes of the NC row.
			const NCSimulateItem* pNCSimItem = pNCSimItemList->GetNCSimItem(pNCRow->GetNCRowID());
			if (pNCSimItem)
			{
				// because different NC row can use same cut node, we should filter.
				ICutNodePtr pCutNode = pNCSimItem->GetCutNode();
				if (!pCutNodeList->GetCutNodeByID(pCutNode->GetID()))
				{
					pCutNodeList->push_back(pCutNode);
				}
			}
		}

		// 创建action显示选中的NC代码行。
		{
			m_pMacRouterEditor->SetAction(MACROUTER_NC_SIMULATE);
			NCSimulateActionPtr pNCSimulateAction = boost::dynamic_pointer_cast<NCSimulateAction>(m_pMacRouterEditor->GetActionObject());
			pNCSimulateAction->ViewCutNode(m_pSequenceSimulateShapeList, pCutNodeList);

			// redraw the view.
			GetRelativeView()->Invalidate();
		}
	}

	*pResult = 0;
}

void NCViewBar::OnGrid_beginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);
    int iRowIndex = pDispInfo->item.iItem;
    CString str = pDispInfo->item.pszText;

    int iColumnIndex = pDispInfo->item.iSubItem;
    if (iColumnIndex == 2)
    {
        m_strCellText_beforeEdit = m_ncListGrid.GetItemText(iRowIndex, 2);
    }
}

void NCViewBar::OnGrid_endLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);
    int iRowIndex = pDispInfo->item.iItem;
    CString str = pDispInfo->item.pszText;

    int iColumnIndex = pDispInfo->item.iSubItem;
    if (iColumnIndex == 2)
    {
        if (str != m_strCellText_beforeEdit)
        {
			NCRowPtr pNCRow = m_pNCCodeData->GetNCRowList()->operator [](iRowIndex);
            pNCRow->SetNCRow(str);
        }
    }
}

void NCViewBar::OnSaveNCCodeTo( CString strFilePath, CString strNCExt, BOOL bMustOverride)
{
	// get the full path name.
	if (strFilePath.Find(strNCExt) == -1)
		strFilePath += strNCExt;

	// check whether this file exists.
	CFileFind finder;  
	if ( ( bMustOverride == FALSE ) && finder.FindFile(strFilePath))  
	{
		CString tmpStr;
		tmpStr = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NC_FILE_EXIST);
		if (MessageBox(tmpStr, NULL, MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;
	}
	finder.Close();

	// save the NC code
	CStdioFile file(strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	for (int i = 0; i < m_ncListGrid.GetItemCount(); i++)
	{
		CString str = m_ncListGrid.GetItemText(i, 2);
		file.WriteString(str);
		file.WriteString(_T("\n"));
	}
}

void NCViewBar::OnSaveNCCode()
{
	// prepare some data.
	CString strFilter, strNCExt;
	strFilter = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NC_FILEFILTER);
	strFilter.Replace(_T("WILLBEREPLACED1"), m_strNCPostfix);
	strNCExt = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NC_FILEEXT);
	strNCExt.Replace(_T("WILLBEREPLACED1"), m_strNCPostfix);

	CFileDialog saveDlg(FALSE, NULL, m_strSheetName, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter, NULL);
	if (saveDlg.DoModal() == IDOK) {
		CString strFilePath = saveDlg.GetPathName();
		OnSaveNCCodeTo( strFilePath, strNCExt, FALSE );
	}
}

void NCViewBar::OnUpdateClickNCBtn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

BOOL NCViewBar::CreateNCGrid()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// create the listctrl.
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_REPORT | LVS_OWNERDATA | LVS_NOSORTHEADER;
	if (!m_ncListGrid.Create(dwStyle, rectDummy, this, IDC_LISTCTRL_NC))
		return FALSE;

	CString str;

    // Requires one never uses column 0
    m_ncListGrid.InsertHiddenLabelColumn();

    // 序号列
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
    CGridColumnTrait* pNumCol = new CGridColumnTraitText;
    m_ncListGrid.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNumCol);

    // 内容列
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MACROUTER_NC_CODE);
    CGridColumnTrait* pContentCol = new CGridColumnTraitEdit;
    m_ncListGrid.InsertColumnTrait(2, str, LVCFMT_LEFT, 180, 0, pContentCol);

    m_ncListGrid.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;
}

BOOL NCViewBar::CreateNCBtn()
{
	CString str;

	CRect rectDummy (0, 0, 0, 0);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MACROUTER_SAVE_NC_CODE);
	m_saveNCBtn.Create(str, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rectDummy, this, IDC_MACROUTER_SAVE_NC_CODE);

	return TRUE;
}

void NCViewBar::InitNCGrid()
{
	m_ncListGrid.DeleteAllItems();

	NCRowListPtr pNCRowList = m_pNCCodeData->GetNCRowList();
	m_ncListGrid.SetItemCount(pNCRowList->size());
}

IntListPtr NCViewBar::GetSelectedNCRow()
{
	IntListPtr pIntList(new vector<int>);

	POSITION pos = m_ncListGrid.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		int iRow = m_ncListGrid.GetNextSelectedItem(pos);
		pIntList->push_back(iRow);
	}

	return pIntList;
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "PartAddDlg.h"

#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"
#include "DateHelper.h"
#include "clUtilityResource.h"
#include "baseConst.h"
#include "clBaseDataResource.h"
#include "clUILibResource.h"
#include "Rect2D.h"
#include "LineArc2DList.h"
#include "ClFolderHelper.h"
#include "DataBaseManager.h"
#include "ImgData.h"
#include "SelectDataCenterFolderDlg.h"
#include "FolderNode.h"
#include "DataCenterManager.h"
#include "DataCenterItemList.h"
#include "DataCenterItemLoader.h"
#include "SearchPartDlg.h"
#include "ClWindowFinder.h"

#include "clPartResource.h"
#include "PartManager.h"
#include "PartLoader.h"
#include "clPartLayoutResource.h"
#include "clPartLayoutUIResource.h"
#include "PartLayoutEditor.h"
#include "ClTaskResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(PartAddDlg, CLResizableDialog)

PartAddDlg::PartAddDlg() : CLResizableDialog(PartAddDlg::IDD)
{
	m_strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PART_DB_CLICK_ADD);
	m_bFinishAddPart = TRUE;
}

PartAddDlg::~PartAddDlg()
{
}

void PartAddDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MSG, m_strMsg);
	DDX_Control(pDX, IDC_PART_LIST, m_partList);
	DDX_Control(pDX, IDC_STATIC_PART_PREVIEW, m_previewWnd);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSeach);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAdd);
}

BEGIN_MESSAGE_MAP(PartAddDlg, CLResizableDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_SIZE()

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PART_LIST, OnPartListchanged)
	ON_NOTIFY(NM_DBLCLK, IDC_PART_LIST, OnDblclkList)

	ON_BN_CLICKED(IDC_BTN_SEARCH, OnSearchPart)
	ON_BN_CLICKED(IDC_BTN_ADD, OnAddToSheet)
END_MESSAGE_MAP()

BOOL PartAddDlg::OnInitDialog()
{
	__super::OnInitDialog();

	/************************************************************************/
	// about scroll bar.

	// get the original size
	GetWindowRect(m_rcOriginalRect);

	// initial scroll position
	m_nScrollPos = 0; 
	/************************************************************************/

	// init the style of the list ctrl
	InitListStyle();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	SetWindowTheme( GetDlgItem( IDC_GROUP_AVAIL_PART )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_PART_PREVIEW )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void PartAddDlg::OnDestroy()
{
	// release the item data.
	ReleaseGridData();

	__super::OnDestroy();
}

HBRUSH PartAddDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void PartAddDlg::OnPaint()
{
	__super::OnPaint();

	PreviewPart();
}

void PartAddDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = m_rcOriginalRect.Height() - m_nCurHeight;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;

		nDelta = min(max(nMaxPos/20,5), nMaxPos-m_nScrollPos);
		break;

	case SB_LINEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos/20,5), m_nScrollPos);
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos/10,5), nMaxPos-m_nScrollPos);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPos;
		break;

	case SB_PAGEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(max(nMaxPos/10,5), m_nScrollPos);
		break;

	default:
		return;
	}
	m_nScrollPos += nDelta;
	SetScrollPos(SB_VERT, m_nScrollPos, TRUE);
	ScrollWindow(0, -nDelta);

	PreviewPart();

	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}

void PartAddDlg::OnSize(UINT nType, int cx, int cy) 
{
	m_nCurHeight = cy;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL; 
	si.nMin = 0;
	si.nMax = m_rcOriginalRect.Height();
	si.nPage = cy;
	si.nPos = m_nScrollPos;
	SetScrollInfo(SB_VERT, &si, TRUE);

	__super::OnSize(nType, cx, cy);
}

void PartAddDlg::OnSearchPart()
{
	SearchPartDlg dlg;
	dlg.Init(m_pWorkspace);
	if (dlg.DoModal() == IDOK)
	{
		DataCenterItemListPtr pProductItemList = dlg.GetResult();
		UpdatePartGrid(pProductItemList);
	}
}

void PartAddDlg::OnAddToSheet()
{
	// 得到选中项。
	int iIndex = INVALID_IDX;
	for (int i = 0; i < m_partList.GetItemCount(); i++)
	{
		if (m_partList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
			iIndex = i;
	}
	if (iIndex == INVALID_IDX)
	{
		CString strProductName, str;
		strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SELECTPRTPMT);
		MessageBox(str, strProductName, MB_ICONWARNING);
		return;
	}

	// 添加零件到板材。
	LONGLONG* pPartID = (LONGLONG*)m_partList.GetItemData(iIndex);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_placePartOnCurrentSheetSubView, (WPARAM)pPartID, 0);
}

void PartAddDlg::OnPartListchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// preview
	PreviewPart();

	*pResult = 0;
}

void PartAddDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem == -1)
		return;

	// 添加零件到板材。
	LONGLONG* pPartID = (LONGLONG*)m_partList.GetItemData(pNMListView->iItem);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_placePartOnCurrentSheetSubView, (WPARAM)pPartID, 0);
}

void PartAddDlg::DisplayPartList(FolderNodePtr pWorkspace)
{
	m_pWorkspace = pWorkspace;

	// 显示所有零件。
	DataCenterItemListPtr pProductItemList = DataCenterManager::SearchDataItem(m_pWorkspace, TRUE, DATANODE_PART, FALSE, _T(""));
	UpdatePartGrid(pProductItemList);
}

void PartAddDlg::InitListStyle()
{
	CString str;

	// Requires one never uses column 0
	m_partList.InsertHiddenLabelColumn();

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
	CGridColumnTrait* pNUMCol1 = new CGridColumnTraitText;
	m_partList.InsertColumnTrait(1, str, LVCFMT_LEFT, 35, 0, pNUMCol1);

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTNAME);
	CGridColumnTrait* pNameCo2 = new CGridColumnTraitText;
	m_partList.InsertColumnTrait(2, str, LVCFMT_LEFT, 130, 0, pNameCo2);

	m_partList.SetExtendedStyle(m_partList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
}

void PartAddDlg::UpdatePartGrid(DataCenterItemListPtr pProductItemList)
{
	// 禁止消息。
	m_bFinishAddPart = FALSE;

	// 清除表格。
	ReleaseGridData();

	// 初始化表格。
	int iItemCount = 0;
	for (unsigned int i = 0; i < pProductItemList->size(); i++)
	{
		IDataCenterItemPtr pProductItem = pProductItemList->at(i);
		DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pProductItem);

		if (pDataItem->GetName() == HiddenDataItemName) // 不显示隐藏的数据项。
			continue;

		// insert a new row.
		LVITEM lvItem;
		lvItem.mask = LVIF_TEXT | LVIF_PARAM;
		lvItem.iItem = iItemCount;
		lvItem.iSubItem = 0;
		lvItem.pszText = _T("");
		lvItem.lParam = (LPARAM)(new LONGLONG(pDataItem->GetDataID()));
		m_partList.InsertItem(&lvItem);

		// "num" column
		CString strNum;
		strNum.Format(INT_FORMAT, iItemCount+1);
		m_partList.SetItemText(iItemCount, 1, strNum);

		// the part name column
		m_partList.SetItemText(iItemCount, 2, pDataItem->GetName());

		iItemCount++;
	}

	// 打开消息。
	m_bFinishAddPart = TRUE;

	// 选择第一行。
	if (iItemCount > 0)
	{
		m_partList.EnsureVisible(0, FALSE);
		m_partList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
		m_partList.SetFocus();
	}
}

void PartAddDlg::ReleaseGridData()
{
	for (int i = 0; i < m_partList.GetItemCount(); i++)
	{
		DWORD_PTR pItemData = m_partList.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}

	m_partList.DeleteAllItems();
}

void PartAddDlg::PreviewPart()
{
	// 得到选中项。
	int iIndex = INVALID_IDX;
	for (int i = 0; i < m_partList.GetItemCount(); i++)
	{
		if (m_partList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
			iIndex = i;
	}

	// 得到预览数据。
	ImgDataPtr pImgData;
	pImgData.reset();
	if (iIndex != INVALID_IDX)
	{
		LONGLONG* pPartID = (LONGLONG*)m_partList.GetItemData(iIndex);
		DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(*pPartID);
		pImgData = PartLoader::LoadPreviewData(pPartDBCon);
	}

	// 预览。
	ImgData::DrawImgData(pImgData.get(), &m_previewWnd, SkinManager::GetBackgroundColor());
}

END_CUTLEADER_NAMESPACE()

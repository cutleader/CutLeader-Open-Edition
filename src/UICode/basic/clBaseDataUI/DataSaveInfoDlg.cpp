#include "stdafx.h"
#include "DataSaveInfoDlg.h"

#include "baseConst.h"
#include "FolderNode.h"
#include "DataCenterItemList.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(DataSaveInfoDlg, CDialog)

BEGIN_MESSAGE_MAP(DataSaveInfoDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)

END_MESSAGE_MAP()

DataSaveInfoDlg::DataSaveInfoDlg(CWnd* pParent /*=NULL*/)
	: CLDialog(DataSaveInfoDlg::IDD, pParent)
{
	m_pImageList = new CImageList();
}

DataSaveInfoDlg::~DataSaveInfoDlg()
{
	delete m_pImageList;
}

void DataSaveInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text( pDX, IDC_EDIT_NAME, m_strName );
	DDX_Control( pDX, IDC_TREE_PRODUCT, m_productTree );
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL DataSaveInfoDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// init the tree.
	InitTree();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_SELECT_FOLDER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_TREE_PRODUCT )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

HBRUSH DataSaveInfoDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void DataSaveInfoDlg::OnDestroy()
{
	// release the data which stored in the tree node.
	HTREEITEM hFirstRootItem = m_productTree.GetRootItem();
	if (hFirstRootItem)
		ReleaseNodeData(hFirstRootItem);

	CDialogEx::OnDestroy();
}

void DataSaveInfoDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	// get the selected tree node.
	HTREEITEM hItem = m_productTree.GetSelectedItem();
	LONGLONG* pData = (LONGLONG*)m_productTree.GetItemData(hItem);

	// search the select folder node.
	IDataCenterItemPtr pProductItem = m_pWorkspace->SearchDataCenterItem(*pData);
	m_pSelFolderNode = boost::dynamic_pointer_cast<FolderNode>(pProductItem);

	OnOK();
}

void DataSaveInfoDlg::Init(CString strTitle, FolderNodePtr pWorkspace, CString strName) 
{
	m_strTitle = strTitle;
	m_pWorkspace = pWorkspace;
	m_strName = strName;
}

void DataSaveInfoDlg::InitTree()
{
	/************************************************************************/
	// load the image for the tree.

	m_pImageList->Create(IDB_PRODUCT_EXPL_TREE, 16, 10, RGB( 192, 192, 192 ) );
	m_productTree.SetImageList(m_pImageList, TVSIL_NORMAL);
	/************************************************************************/

	/************************************************************************/
	// append the root node.

	// create tree node.
	TVINSERTSTRUCT workspaceItem;
	workspaceItem.hParent = NULL;
	workspaceItem.item.iImage = 0;
	workspaceItem.item.iSelectedImage = 0;
	workspaceItem.item.pszText = m_pWorkspace->GetName().AllocSysString();
	workspaceItem.item.cchTextMax = MAXLENGTH_STR;
	workspaceItem.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	workspaceItem.hInsertAfter = TVI_LAST;
	HTREEITEM hRootNode = m_productTree.InsertItem(&workspaceItem);

	// save folder ID in tree node.
	m_productTree.SetItemData(hRootNode, (DWORD_PTR)(new LONGLONG(m_pWorkspace->GetID())));
	/************************************************************************/

	// append all sub-nodes under "m_pWorkspace".
	AppendSubNode(m_pWorkspace->GetSubItemList(), hRootNode);

	// expand the root node.
	m_productTree.Expand(hRootNode, TVE_EXPAND);

	// select the root node.
	m_productTree.SelectItem(hRootNode);
}

void DataSaveInfoDlg::AppendSubNode(DataCenterItemListPtr pProductItemList, HTREEITEM hParentNode)
{
	for (unsigned int i = 0; i < pProductItemList->size(); i++)
	{
		IDataCenterItemPtr pProductItem = pProductItemList->at(i);

		if (pProductItem->IsFolder())
		{
			/************************************************************************/
			// append "pProductItem" to "hParentNode".

			// create tree node.
			TVINSERTSTRUCT hItem;
			hItem.hParent = hParentNode;
			hItem.item.pszText = pProductItem->GetName().AllocSysString();
			hItem.item.cchTextMax = MAXLENGTH_STR;
			hItem.hInsertAfter = TVI_LAST;
			//
			hItem.item.iImage = 1;
			hItem.item.iSelectedImage = 1;
			if (HasFolder(pProductItem))
				hItem.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_CHILDREN;
			else
				hItem.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;

			// save folder ID in tree node.
			HTREEITEM hSubNode = m_productTree.InsertItem(&hItem);
			m_productTree.SetItemData(hSubNode, (DWORD_PTR)(new LONGLONG(pProductItem->GetID())));
			/************************************************************************/

			// check whether we will init "hSubNode"
			FolderNodePtr pFolderNode = boost::dynamic_pointer_cast<FolderNode>(pProductItem);
			AppendSubNode(pFolderNode->GetSubItemList(), hSubNode);
		}
	}
}

BOOL DataSaveInfoDlg::HasFolder(IDataCenterItemPtr pFolderItem)
{
	FolderNodePtr pFolderNode = boost::dynamic_pointer_cast<FolderNode>(pFolderItem);
	DataCenterItemListPtr pProductItemList = pFolderNode->GetSubItemList();
	for (unsigned int i = 0; i < pProductItemList->size(); i++)
	{
		IDataCenterItemPtr pProductItem = pProductItemList->at(i);
		if (pProductItem->IsFolder())
			return TRUE;
	}

	return FALSE;
}

void DataSaveInfoDlg::ReleaseNodeData(HTREEITEM hParentItem)
{
	// release the data stored in this node.
	LONGLONG* pData = (LONGLONG*)m_productTree.GetItemData(hParentItem);
	delete pData;

	// go through all brother nodes of "hParentItem".
	HTREEITEM hBrotherNode = m_productTree.GetNextSiblingItem(hParentItem);
	if (hBrotherNode != NULL)
	{
		ReleaseNodeData(hBrotherNode);
	}

	// go through all child nodes of "hParentItem".
	HTREEITEM hChildNode = m_productTree.GetChildItem(hParentItem);
	if (hChildNode != NULL)
	{
		ReleaseNodeData(hChildNode);
	}
}

END_CUTLEADER_NAMESPACE()

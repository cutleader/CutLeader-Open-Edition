#include "stdafx.h"
#include "SelectDataCenterFolderDlg.h"

#include "baseConst.h"
#include "DateHelper.h"
#include "DataBaseManager.h"

#include "FolderNode.h"
#include "DataCenterItemList.h"
#include "DataCenterItemLoader.h"
#include "DataCenterItemWriter.h"
#include "FolderInfoDlg.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(SelectDataCenterFolderDlg, CDialog)

BEGIN_MESSAGE_MAP(SelectDataCenterFolderDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_NEW_FOLDER, OnCreateFolder)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

SelectDataCenterFolderDlg::SelectDataCenterFolderDlg(CWnd* pParent /*=NULL*/)
	: CLDialog(SelectDataCenterFolderDlg::IDD, pParent)
{
	m_pImageList = new CImageList();
	m_pImageList->Create(IDB_PRODUCT_EXPL_TREE, 16, 10, RGB( 192, 192, 192 ) );

	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	m_pWorkspace = DataCenterItemLoader::LoadProductTree(pMainDBCon);
}

SelectDataCenterFolderDlg::~SelectDataCenterFolderDlg()
{
	delete m_pImageList;
}

void SelectDataCenterFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_TREE_PRODUCT, m_dataCenterTree );
	DDX_Control( pDX, IDC_BTN_NEW_FOLDER, m_btnNewFolder );
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL SelectDataCenterFolderDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// init the tree.
	Init();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_TREE_PRODUCT )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

HBRUSH SelectDataCenterFolderDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void SelectDataCenterFolderDlg::OnDestroy()
{
	// release the data which stored in the tree node.
	HTREEITEM hFirstRootItem = m_dataCenterTree.GetRootItem();
	if (hFirstRootItem)
		ReleaseNodeData(hFirstRootItem);

	CDialogEx::OnDestroy();
}

void SelectDataCenterFolderDlg::OnCreateFolder()
{
	// 得到选中的目录。
	HTREEITEM hParentNode = m_dataCenterTree.GetSelectedItem();
	LONGLONG* pData = (LONGLONG*)m_dataCenterTree.GetItemData(hParentNode);
	FolderNodePtr pParentFolder = boost::dynamic_pointer_cast<FolderNode>(m_pWorkspace->SearchDataCenterItem(*pData));

	// create a folder item.
	FolderInfoDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strName, strComment;
		dlg.GetMatNameInfo(strName, strComment);


		/************************************************************************/
		// 创建目录对象并保存到数据库。

		// 创建目录对象。
		DataCenterItemListPtr pProductItemList(new DataCenterItemList);
		LONGLONG iTime = DateHelper::GetCurrentTime();
		FolderNodePtr pFolderNode(new FolderNode(pParentFolder->GetID(), strName, pProductItemList, iTime, iTime));
		pFolderNode->SetComment(strComment);
		pFolderNode->SetItemPath(pParentFolder->GetFullPath());
		pParentFolder->GetSubItemList()->push_back(pFolderNode);

		// 保存到数据库。
		DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
		DataCenterItemWriter::CreateDataCenterItem(pMainDBCon, pFolderNode);
		/************************************************************************/


		/************************************************************************/
		// 更新产品树。

		// 挂上树节点。
		TVINSERTSTRUCT hItem;
		hItem.hParent = hParentNode;
		hItem.item.pszText = pFolderNode->GetName().AllocSysString();
		hItem.item.cchTextMax = MAXLENGTH_STR;
		hItem.hInsertAfter = TVI_LAST;
		hItem.item.iImage = 0;
		hItem.item.iSelectedImage = 0;
		hItem.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
		HTREEITEM hSubNode = m_dataCenterTree.InsertItem(&hItem);
		m_dataCenterTree.SetItemData(hSubNode, (DWORD_PTR)(new LONGLONG(pFolderNode->GetID())));

		// 选中新的目录节点。
		m_dataCenterTree.Expand(hParentNode, TVE_EXPAND);
		m_dataCenterTree.SelectItem(hSubNode);
		m_pSelFolderNode = pFolderNode;
		/************************************************************************/
	}
}

void SelectDataCenterFolderDlg::OnBnClickedOk()
{
	// 得到选中的目录。
	HTREEITEM hItem = m_dataCenterTree.GetSelectedItem();
	LONGLONG* pData = (LONGLONG*)m_dataCenterTree.GetItemData(hItem);
	m_pSelFolderNode = boost::dynamic_pointer_cast<FolderNode>(m_pWorkspace->SearchDataCenterItem(*pData));

	OnOK();
}

void SelectDataCenterFolderDlg::Init()
{
	m_dataCenterTree.SetImageList(m_pImageList, TVSIL_NORMAL);

	// 初始化根节点。
	TVINSERTSTRUCT workspaceItem;
	workspaceItem.hParent = NULL;
	workspaceItem.item.iImage = 0;
	workspaceItem.item.iSelectedImage = 0;
	workspaceItem.item.pszText = m_pWorkspace->GetName().AllocSysString();
	workspaceItem.item.cchTextMax = MAXLENGTH_STR;
	workspaceItem.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	workspaceItem.hInsertAfter = TVI_LAST;
	HTREEITEM hRootNode = m_dataCenterTree.InsertItem(&workspaceItem);
	m_dataCenterTree.SetItemData(hRootNode, (DWORD_PTR)(new LONGLONG(m_pWorkspace->GetID())));

	m_dataCenterTree.Init();

	// append all sub-nodes under "m_pWorkspace".
	AppendSubNode(m_pWorkspace->GetSubItemList(), hRootNode);

	// expand the root node.
	m_dataCenterTree.Expand(hRootNode, TVE_EXPAND);

	// 选择节点
	if (m_iInitialFolderNodeID == INVALID_ID)
	{
		m_dataCenterTree.SelectItem(hRootNode);
	}
	else
	{
		LocateFolderNode(hRootNode, m_iInitialFolderNodeID);
	}
}

void SelectDataCenterFolderDlg::AppendSubNode(DataCenterItemListPtr pProductItemList, HTREEITEM hParentNode)
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
			HTREEITEM hSubNode = m_dataCenterTree.InsertItem(&hItem);
			m_dataCenterTree.SetItemData(hSubNode, (DWORD_PTR)(new LONGLONG(pProductItem->GetID())));
			/************************************************************************/

			// check whether we will init "hSubNode"
			FolderNodePtr pFolderNode = boost::dynamic_pointer_cast<FolderNode>(pProductItem);
			AppendSubNode(pFolderNode->GetSubItemList(), hSubNode);
		}
	}
}

BOOL SelectDataCenterFolderDlg::HasFolder(IDataCenterItemPtr pFolderItem)
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

void SelectDataCenterFolderDlg::ReleaseNodeData(HTREEITEM hParentItem)
{
	// release the data stored in this node.
	LONGLONG* pData = (LONGLONG*)m_dataCenterTree.GetItemData(hParentItem);
	delete pData;

	// go through all brother nodes of "hParentItem".
	HTREEITEM hBrotherNode = m_dataCenterTree.GetNextSiblingItem(hParentItem);
	if (hBrotherNode != NULL)
	{
		ReleaseNodeData(hBrotherNode);
	}

	// go through all child nodes of "hParentItem".
	HTREEITEM hChildNode = m_dataCenterTree.GetChildItem(hParentItem);
	if (hChildNode != NULL)
	{
		ReleaseNodeData(hChildNode);
	}
}

bool SelectDataCenterFolderDlg::LocateFolderNode(HTREEITEM hItem, LONGLONG iFolderNodeID)
{
	LONGLONG* pNodeID = (LONGLONG*)m_dataCenterTree.GetItemData(hItem);
	if (*pNodeID == iFolderNodeID)
	{
		m_dataCenterTree.SelectItem(hItem);
		return true;
	}

	if (m_dataCenterTree.ItemHasChildren(hItem))
	{
		m_dataCenterTree.Expand(hItem, TVE_EXPAND);

		bool bFound = false;
		HTREEITEM hChildItem = m_dataCenterTree.GetChildItem(hItem);
		while (hChildItem)
		{
			if (LocateFolderNode(hChildItem, iFolderNodeID))
			{
				bFound = true;
				return true;
			}
			else
			{
				hChildItem = m_dataCenterTree.GetNextSiblingItem(hChildItem);
			}
		}

		// IMPORTANT! if do not found it in this "folder" node, collapse it.
		if (!bFound)
		{
			m_dataCenterTree.Expand(hItem, TVE_COLLAPSE);
		}
	}

	return false;
}

END_CUTLEADER_NAMESPACE()

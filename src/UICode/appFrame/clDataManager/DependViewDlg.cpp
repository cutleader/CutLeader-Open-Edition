//
#include "stdafx.h"
#include "DependViewDlg.h"

#include "baseConst.h"
#include "baseConst.h"
#include "clUtilityResource.h"
#include "clUILibResource.h"

#include "DependNode.h"
#include "DependNodeList.h"
#include "DataItem.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(DependViewDlg, CDialog)

BEGIN_MESSAGE_MAP(DependViewDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_CLOSE, OnClose)

END_MESSAGE_MAP()

DependViewDlg::DependViewDlg(CWnd* pParent /*=NULL*/)
	: CLDialog(DependViewDlg::IDD, pParent)
{

}

DependViewDlg::~DependViewDlg()
{
}

void DependViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE_DEPEND, m_dependTree);
	DDX_Control (pDX, IDC_BTN_CLOSE, m_btnClose );
}

BOOL DependViewDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//
	InitDependTree();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_DEPEND_TREE )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH DependViewDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void DependViewDlg::InitDependTree()
{
	/************************************************************************/
	// load the images which will be used on the tree.

	CImageList* pImageList = new CImageList();
	pImageList->Create(IDB_DEPEND_TREE, 16, 10, RGB(192, 192, 192));

	m_dependTree.SetImageList(pImageList, TVSIL_NORMAL);
	/************************************************************************/

	for (unsigned int i = 0; i < m_pDependNodeList->size(); i++)
	{
		DependNodePtr pDependNode = m_pDependNodeList->at(i);
		DependNodeListPtr pDependNodeList = pDependNode->GetDependNodeList();

		// if some DependNode has an empty "DependNode::m_pDependNodeList",
		// do not display it.
		if (pDependNodeList->size() == 0)
			continue;

		// build the string which will be displayed on the tree node.
		CString strNodeName;
		DataItemPtr pDataItem = pDependNode->GetDataItem();
		strNodeName = pDataItem->GetName();
		strNodeName += _T("(");
		strNodeName += pDataItem->GetItemPath();
		strNodeName += _T(")");


		/************************************************************************/
		// create tree node.

		TVINSERTSTRUCT dependItem;
		dependItem.hParent = NULL;
		dependItem.item.pszText = strNodeName.AllocSysString();
		dependItem.item.cchTextMax = MAXLENGTH_STR;
		dependItem.hInsertAfter = TVI_LAST;

		// whether we should display "+" sign.
		if (pDependNodeList->size() > 0)
		{
			dependItem.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_CHILDREN;
		}
		else
		{
			dependItem.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
		}

		// select the image.
		if (pDataItem->GetDataType() == DATANODE_PART)
		{
			dependItem.item.iImage = 4;
			dependItem.item.iSelectedImage = 4;
		}
		else if (pDataItem->GetDataType() == DATANODE_Task)
		{
			dependItem.item.iImage = 6;
			dependItem.item.iSelectedImage = 6;
		}

		HTREEITEM hRootItem = m_dependTree.InsertItem(&dependItem);
		/************************************************************************/


		// init the sub-node under this node.
		AppendSubNode(pDependNodeList, hRootItem);

		// expand all the root nodes.
		m_dependTree.Expand(hRootItem, TVE_EXPAND);
	}
}

void DependViewDlg::AppendSubNode(DependNodeListPtr pDependNodeList, HTREEITEM hParentNode)
{
	for (unsigned int i = 0; i < pDependNodeList->size(); i++)
	{
		DependNodePtr pDependNode = pDependNodeList->at(i);
		DataItemPtr pDataItem = pDependNode->GetDataItem();
		DependNodeListPtr pTmpDependNodeList = pDependNode->GetDependNodeList();


		/************************************************************************/
		// append "pDependNode" to "hParentNode".

		// build the string which will be displayed on the tree node.
		CString strNodeName;
		strNodeName = pDataItem->GetName();
		strNodeName += _T("(");
		strNodeName += pDataItem->GetItemPath();
		strNodeName += _T(")");

		// create tree node.
		TVINSERTSTRUCT hItem;
		hItem.hParent = hParentNode;
		hItem.item.pszText = strNodeName.AllocSysString();
		hItem.item.cchTextMax = MAXLENGTH_STR;
		hItem.hInsertAfter = TVI_LAST;

		// whether show "+" sign.
		if (pTmpDependNodeList->size())
		{
			hItem.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_CHILDREN;
		}
		else
		{
			hItem.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
		}

		// decide the image.
		if (pDataItem->GetDataType() == DATANODE_PART)
		{
			hItem.item.iImage = 4;
			hItem.item.iSelectedImage = 4;
		}
		else if (pDataItem->GetDataType() == DATANODE_Task)
		{
			hItem.item.iImage = 6;
			hItem.item.iSelectedImage = 6;
		}

		//
		HTREEITEM hSubNode = m_dependTree.InsertItem(&hItem);
		/************************************************************************/


		// check whether we will init "hSubNode"
		if (pTmpDependNodeList->size())
		{
			AppendSubNode(pTmpDependNodeList, hSubNode);
		}
	}
}

void DependViewDlg::OnClose()
{
	OnOK();
}

END_CUTLEADER_NAMESPACE()

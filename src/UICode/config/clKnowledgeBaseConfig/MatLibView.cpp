#include "stdafx.h"
#include "clKnowledgeBaseConfig.h"
#include "MatLibView.h"

#include "baseConst.h"
#include "clUtilityResource.h"
#include "DateHelper.h"
#include "DBConnect.h"
#include "PathHelper.h"
#include "LogMessage.h"
#include "LogMgr.h"
#include "baseConst.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"
#include "LineArc2DLoop.h"
#include "LineArc2DLoopList.h"
#include "LineArc2DLoopManager.h"

#include "ImportMaterialDlg.h"
#include "MaterialLibNode.h"
#include "clMaterialLibResource.h"
#include "IMaterialSize.h"
#include "MaterialThickness.h"
#include "Material.h"
#include "DefaultMaterialSelectDlg.h"
#include "MaterialInfoDlg.h"
#include "MaterialThicknessInfoDlg.h"
#include "MaterialSizeInfoDlg.h"
#include "MaterialLibLoader.h"
#include "MaterialLibWriter.h"
#include "MaterialProcessor.h"
#include "ProductParam.h"
#include "CLOptionInfo.h"
#include "PartEditParam.h"
#include "ImpExpConfig.h"

#include "DxfData.h"
#include "DxfDwgConvertor.h"
#include "ImpExpHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(MatLibView, CResizableFormView)

BEGIN_MESSAGE_MAP(MatLibView, CResizableFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MATLIB, OnSelchangedTree)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_MATLIB, OnRClickTree)

	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()

	ON_COMMAND(IDC_BTN_CREATE_MATTYPE, OnCreateMatType)
	ON_COMMAND(IDC_DEL_MAT_TYPE, OnDelMatType)
	ON_COMMAND(IDC_EXP_MAT_TYPE, OnExportMat)

	ON_COMMAND(IDC_ADD_MAT_NAME, OnCreateMatName)
	ON_COMMAND(IDC_DEL_MAT_NAME, OnDelMatName)

	ON_COMMAND(IDC_BTN_DFL_MATSIZE, OnSetDflMatType)
	ON_COMMAND(IDC_ADD_MAT_SIZE, OnCreateMatSize)
	ON_COMMAND(IDC_IMPORT_MAT_SIZE, OnImportMatSize)
	ON_COMMAND(IDC_DEL_MAT_SIZE, OnDelMatSize)
END_MESSAGE_MAP()

MatLibView::MatLibView()
	: KBConfigView(MatLibView::IDD)
{
	m_matLibNodeType = MATLIB_NODE_NOT_INIT;
}

MatLibView::~MatLibView()
{
}

void MatLibView::DoDataExchange(CDataExchange* pDX)
{
	CResizableFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE_MATLIB, m_matLibTree);
	DDX_Control(pDX, IDC_BTN_CREATE_MATTYPE, m_createMatLibBtn);
	DDX_Control( pDX, IDC_BTN_DFL_MATSIZE, m_btnDflMatSize );
}

BOOL MatLibView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CResizableFormView::PreCreateWindow(cs);
}

void MatLibView::OnInitialUpdate()
{
	CResizableFormView::OnInitialUpdate();
	ResizeParentToFit();
}

BOOL MatLibView::OnInitDialog() 
{
	CResizableFormView::OnInitDialog();

	// 翻译
	MultiLanguageMgr::GetInstance()->TranslateDialog(this);

	// layout the controls.
	AddAnchor(IDC_GROUP_TREE, TOP_LEFT, ANCHOR(30,100));
	AddAnchor(IDC_TREE_MATLIB, TOP_LEFT, ANCHOR(30,100));
	AddAnchor(IDC_STATIC_CHILDAREA, ANCHOR(30,0), BOTTOM_RIGHT);
	AddAnchor(IDC_BTN_CREATE_MATTYPE, BOTTOM_LEFT);
	AddAnchor(IDC_BTN_DFL_MATSIZE, BOTTOM_LEFT);

	/************************************************************************/	
	// 初始化按钮菜单。

	m_btnMenu.CreatePopupMenu();

	// create sub items.
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEW);
	m_btnMenu.AppendMenu(MF_STRING, IDC_NEW_MAT_TYPE, str.AllocSysString());
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPORT);
	m_btnMenu.AppendMenu(MF_STRING, IDC_IMP_MAT_TYPE, str.AllocSysString());

	//
	m_createMatLibBtn.m_hMenu = m_btnMenu.GetSafeHmenu();
	m_createMatLibBtn.SizeToContent();
	m_createMatLibBtn.m_bStayPressed = TRUE;
	m_createMatLibBtn.m_bRightArrow = TRUE;
	/************************************************************************/

	// init sub dialogs.
	InitSubDialog();

	// init the expert library structure tree.
	InitMatLibTree();

	SetWindowTheme( GetDlgItem( IDC_GROUP_TREE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_TREE_MATLIB )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void MatLibView::InitMatLibTree()
{
	// 是否内部使用。
	ProductParamPtr pProductParam = CLOptionInfo::GetInstance()->GetProductParam();
	BOOL bInternal = pProductParam->IsUseInside();

	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
	IMaterialLibNodePtr pMatLibRootNode;
	if (bInternal)
		pMatLibRootNode = MaterialLibLoader::LoadMatLibTree(pMatLibDBCon, FALSE, FALSE);
	else
		pMatLibRootNode = MaterialLibLoader::LoadMatLibTree(pMatLibDBCon, TRUE, TRUE);
	m_matLibTree.Init(pMatLibRootNode);
}

void MatLibView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void MatLibView::OnSize(UINT nType, int cx, int cy)
{
	CResizableFormView::OnSize(nType, cx, cy);

	// re-position the sub dialog.
	RepositionSubDlg();
}

BOOL MatLibView::OnEraseBkgnd( CDC *pDC )
{
	CBrush* br = &SkinManager::GetDialogBkgndBrush(); // to be verified.
	CRect rcClip;
	pDC->GetClipBox( rcClip );
	pDC->FillSolidRect( rcClip, SkinManager::GetBackgroundColor() );
	return TRUE;
}

HBRUSH MatLibView::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void MatLibView::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item = pNMTreeView->itemNew;
	TVITEM oldItem = pNMTreeView->itemOld;

	/************************************************************************/
	// get the data that is stored in the new and old node.

	// the item data in new node.
	IMaterialLibNode* pNodeData = (IMaterialLibNode*)m_matLibTree.GetItemData(item.hItem);

	// whether the selected node type is changed.
	BOOL bNodeTypeChanged = FALSE;
	if (m_matLibNodeType != pNodeData->GetMatLibNodeType())
	{
		m_matLibNodeType = pNodeData->GetMatLibNodeType();
		bNodeTypeChanged = TRUE;
	}

	// if "oldItem.hItem" is NULL, we know this is the first time to select a node.
	if (oldItem.hItem)
	{
		// we should check whether we should save the data on the previous page.
		CheckModification(oldItem.hItem);
	}
	/************************************************************************/

	// re-position the sub dialog.
	if (bNodeTypeChanged)
		RepositionSubDlg();

	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
	if (m_matLibNodeType == MATLIB_NODE_MAT)
	{
		DataProperty matProp;
		MaterialLibLoader::GetMatProp(pMatLibDBCon, pNodeData->GetDataID(), matProp);
		vector<DataProperty> matThickProps = MaterialLibLoader::GetMatThickProp(pMatLibDBCon, pNodeData->GetDataID());
		m_matTypeInfoPage.DisplayMatType(matProp, matThickProps);
	}
	else if (m_matLibNodeType == MATLIB_NODE_MATTHICK)
	{
		DataProperty matThickProp;
		MaterialLibLoader::GetMatThickProp(pMatLibDBCon, pNodeData->GetDataID(), matThickProp);
		vector<DataProperty> matSizeProps;
		if (CLOptionInfo::GetInstance()->GetProductParam()->IsUseInside())
			matSizeProps = MaterialLibLoader::GetMatSizeProp(pMatLibDBCon, pNodeData->GetDataID(), FALSE);
		else
			matSizeProps = MaterialLibLoader::GetMatSizeProp(pMatLibDBCon, pNodeData->GetDataID(), TRUE);
		m_matNameInfoPage.DisplayMatThick(matThickProp, matSizeProps);
	}
	else if (m_matLibNodeType == MATLIB_NODE_MATSIZE)
	{
		IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(pMatLibDBCon, pNodeData->GetDataID());
		m_matSizeInfoPage.DisplayMatSize(pMaterialSize);
	}
}

void MatLibView::OnRClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	/************************************************************************/
	// select tree item which r-button clicked.

	POINT pos;
	if (!GetCursorPos(&pos))
		return;
	m_matLibTree.ScreenToClient(&pos);

	// get the item which was r-clicked, if click on the blank area, quit.
	UINT uFlags;
	HTREEITEM hItem = m_matLibTree.HitTest(pos, &uFlags);
	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
		m_matLibTree.SelectItem(hItem);
	else
		return;

	// the selected node type.
	UINT dataVal = m_matLibTree.GetItemData(hItem);
	IMaterialLibNode* pNodeData = (IMaterialLibNode*)dataVal;
	MATLIB_NODE_TYPE nodeType = pNodeData->GetMatLibNodeType();
	/************************************************************************/

	/************************************************************************/
	// display the popup menu.

	// figure out the position where the menu will pop up.
	CRect rect;
	this->GetClientRect(&rect);
	ClientToScreen(&rect);
	m_matLibTree.ClientToScreen(&pos);

	// display different popup.
	CString str;
	CMenu menu;
	menu.CreatePopupMenu();
	if (nodeType == MATLIB_NODE_MAT)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DEL_MAT);
		menu.AppendMenu(MF_STRING, IDC_DEL_MAT_TYPE, str);

		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXPORT);
		menu.AppendMenu(MF_STRING, IDC_EXP_MAT_TYPE, str);

		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADD_MAT_NAME);
		menu.AppendMenu(MF_STRING, IDC_ADD_MAT_NAME, str);
	}
	else if (nodeType == MATLIB_NODE_MATTHICK)
	{
		// 删除材料厚度。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DEL_MAT_NAME);
		menu.AppendMenu(MF_STRING, IDC_DEL_MAT_NAME, str);

		// 分隔符。
		menu.AppendMenu(MF_SEPARATOR, 0, _T(""));

		// 新建材料尺寸。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADD_MAT_SIZE);
		menu.AppendMenu(MF_STRING, IDC_ADD_MAT_SIZE, str);

		// 导入材料尺寸。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPORT_MAT_SIZE);
		menu.AppendMenu(MF_STRING, IDC_IMPORT_MAT_SIZE, str);
	}
	else if (nodeType == MATLIB_NODE_MATSIZE)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DEL_MAT_SIZE);
		menu.AppendMenu(MF_STRING, IDC_DEL_MAT_SIZE, str);
	}
	menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, pos.x, pos.y, this); 
	/************************************************************************/

	*pResult = 0;
}

void MatLibView::SaveCurrentPage()
{
	/************************************************************************/
	// prepare some data.

	CString strTaskName, strMsgContent;

	HTREEITEM hOldItem = m_matLibTree.GetSelectedItem();
	if (!hOldItem)
		return;

	UINT oldDataVal = m_matLibTree.GetItemData(hOldItem);
	IMaterialLibNode* pOldNodeData = (IMaterialLibNode*)oldDataVal;
	MATLIB_NODE_TYPE pOldNodeType = pOldNodeData->GetMatLibNodeType();
	/************************************************************************/

	if (SaveModification(hOldItem, FALSE))
	{
		// prepare the message.
		if (pOldNodeType == MATLIB_NODE_MAT)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAT_TYPE_SAVED);
		else if (pOldNodeType == MATLIB_NODE_MATTHICK)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAT_NAME_SAVED);
		else if (pOldNodeType == MATLIB_NODE_MATSIZE)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAT_SIZE_SAVED);
	}
	else
	{
		// prepare the message.
		if (pOldNodeType == MATLIB_NODE_MAT)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAT_TYPE_NOT_CHANGE);
		else if (pOldNodeType == MATLIB_NODE_MATTHICK)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAT_NAME_NOT_CHANGE);
		else if (pOldNodeType == MATLIB_NODE_MATSIZE)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAT_SIZE_NOT_CHANGE);
	}

	// write the message.
	LONGLONG iTimeStamp = DateHelper::GetCurrentTime();
	strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_MATLIB);
	LogMessagePtr pLogMsg(new LogMessage(iTimeStamp, LOG_MSG_INFO, strTaskName, strMsgContent));
	LogMgr::WriteLog(pLogMsg);

	// prompt user.
	CString strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	MessageBox(strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
}

BOOL MatLibView::DocModified()
{
	HTREEITEM hOldItem = m_matLibTree.GetSelectedItem();
	if (!hOldItem)
		return FALSE;

	IMaterialLibNode* pOldNodeData = (IMaterialLibNode*)m_matLibTree.GetItemData(hOldItem);
	MATLIB_NODE_TYPE pOldNodeType = pOldNodeData->GetMatLibNodeType();
	if (pOldNodeType == MATLIB_NODE_MAT)
	{
		m_matTypeInfoPage.SetFocus();
		if (m_matTypeInfoPage.IsModified())
		{
			return TRUE;
		}
	}
	else if (pOldNodeType == MATLIB_NODE_MATTHICK)
	{
		m_matNameInfoPage.SetFocus();
		if (m_matNameInfoPage.IsModified())
		{
			return TRUE;
		}
	}
	else if (pOldNodeType == MATLIB_NODE_MATSIZE)
	{
		m_matSizeInfoPage.SetFocus();

		IMaterialSizePtr pMaterialSize = m_matSizeInfoPage.GetMatSize();
		if (pMaterialSize->IsModified())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL MatLibView::SaveModification(HTREEITEM hOldItem, BOOL bNeedConfirm)
{
	BOOL bModified = FALSE;

	/************************************************************************/
	// prepare some data.

	CString strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	UINT oldDataVal = m_matLibTree.GetItemData(hOldItem);
	IMaterialLibNode* pOldNodeData = (IMaterialLibNode*)oldDataVal;
	MATLIB_NODE_TYPE pOldNodeType = pOldNodeData->GetMatLibNodeType();
	LONGLONG iOldNodeID = pOldNodeData->GetDataID();

	CString str;
	/************************************************************************/

	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();

	if (pOldNodeType == MATLIB_NODE_MAT)
	{
		m_matTypeInfoPage.SetFocus();
		if (m_matTypeInfoPage.IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_MAT_TYPE);
			if ((bNeedConfirm && MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				DataProperty dataProp = m_matTypeInfoPage.GetMatProp();
				LONGLONG iMatID = dataProp.GetItem(0).GetValue().llVal;
				CString strName = dataProp.GetItem(1).GetValue().bstrVal;
				double dGravity = dataProp.GetItem(2).GetValue().dblVal;
				CString strComment = dataProp.GetItem(3).GetValue().bstrVal;

				MaterialLibWriter::UpdateMat(pMatLibDBCon, iMatID, strName, strComment, dGravity, DateHelper::GetCurrentTime());
				m_matLibTree.SetItemText(hOldItem, strName);
			}
		}
	}
	else if (pOldNodeType == MATLIB_NODE_MATTHICK)
	{
		m_matNameInfoPage.SetFocus();
		if (m_matNameInfoPage.IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_MAT_NAME);
			if ((bNeedConfirm && MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				DataProperty dataProp = m_matNameInfoPage.GetMatThickProp();
				LONGLONG iMatThickID = dataProp.GetItem(0).GetValue().llVal;
				LONGLONG iMatID = dataProp.GetItem(1).GetValue().llVal;
				CString strName = dataProp.GetItem(2).GetValue().bstrVal;
				double dThickness = dataProp.GetItem(3).GetValue().dblVal;
				CString strComment = dataProp.GetItem(4).GetValue().bstrVal;

				pMatLibDBCon->BeginTransaction();
				MaterialLibWriter::UpdateMatThick(pMatLibDBCon, iMatThickID, strName, dThickness, strComment, FALSE);
				MaterialLibWriter::UpdateMatModData(pMatLibDBCon, iMatID, DateHelper::GetCurrentTime(), FALSE);
				pMatLibDBCon->CommitTransaction();

				// 更新“材料厚度”节点名称。
				m_matLibTree.SetItemText(hOldItem, strName);
			}
		}
	}
	else if (pOldNodeType == MATLIB_NODE_MATSIZE)
	{
		// force all edit boxes lose focus.
		m_matSizeInfoPage.SetFocus();

		IMaterialSizePtr pMaterialSize = m_matSizeInfoPage.GetMatSize();
		if (pMaterialSize->IsModified())
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_MAT_SIZE);
			if ((bNeedConfirm && MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				pMatLibDBCon->BeginTransaction();
				MaterialLibWriter::SaveMatSize(pMatLibDBCon, pMaterialSize, FALSE, FALSE);
				LONGLONG iMatID = MaterialLibLoader::GetMatID(pMatLibDBCon, pMaterialSize->GetParentID());
				MaterialLibWriter::UpdateMatModData(pMatLibDBCon, iMatID, DateHelper::GetCurrentTime(), FALSE);
				pMatLibDBCon->CommitTransaction();

				// update the tree node name.
				m_matLibTree.SetItemText(hOldItem, pMaterialSize->GetName());
			}
		}
	}

	return bModified;
}

void MatLibView::CheckModification(HTREEITEM hOldItem)
{
	UINT oldDataVal = m_matLibTree.GetItemData(hOldItem);
	IMaterialLibNode* pOldNodeData = (IMaterialLibNode*)oldDataVal;
	MATLIB_NODE_TYPE pOldNodeType = pOldNodeData->GetMatLibNodeType();

	if (SaveModification(hOldItem, TRUE))
	{
		// prepare the message.
		CString strTaskName, strMsgContent;
		if (pOldNodeType == MATLIB_NODE_MAT)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAT_TYPE_SAVED);
		else if (pOldNodeType == MATLIB_NODE_MATTHICK)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAT_NAME_SAVED);
		else if (pOldNodeType == MATLIB_NODE_MATSIZE)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAT_SIZE_SAVED);

		// display the message.
		LONGLONG iTimeStamp = DateHelper::GetCurrentTime();
		strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_MATLIB);
		LogMessagePtr pLogMsg(new LogMessage(iTimeStamp, LOG_MSG_INFO, strTaskName, strMsgContent));
		LogMgr::WriteLog(pLogMsg);
	}
}

void MatLibView::InitSubDialog()
{
	// create MaterialInfoPage.
	m_matTypeInfoPage.Create(IDD_clMaterialLibUI_MaterialPage, this);
	m_matTypeInfoPage.ShowWindow(SW_HIDE);

	// create MaterialThicknessInfoPage.
	m_matNameInfoPage.Create(IDD_clMaterialLibUI_MaterialThickness_Page, this);
	m_matNameInfoPage.ShowWindow(SW_HIDE);

	// create MaterialSizeInfoPage.
	m_matSizeInfoPage.Create(IDD_clMaterialLibUI_MaterialSize_Page, this);
	m_matSizeInfoPage.ShowWindow(SW_HIDE);
}

void MatLibView::HideAllSubDialog()
{
	m_matTypeInfoPage.ShowWindow(SW_HIDE);
	m_matNameInfoPage.ShowWindow(SW_HIDE);
	m_matSizeInfoPage.ShowWindow(SW_HIDE);
}

void MatLibView::RepositionSubDlg()
{
	// no node selected, of course no page to display.
	if (m_matLibNodeType == MATLIB_NODE_NOT_INIT)
		return;

	/************************************************************************/
	// calculate the position of the child page.

	CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILDAREA);
	CRect pagePos;
	if (pWndPos)
	{
		pWndPos->GetWindowRect(&pagePos);
		ScreenToClient(&pagePos);
	}
	/************************************************************************/

	/************************************************************************/
	// because the layout library will re-layout the controls, so we should
	// re-position the sub dialog.

	HideAllSubDialog();

	if (m_matLibNodeType == MATLIB_NODE_MAT)
	{
		m_matTypeInfoPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_matTypeInfoPage.ShowWindow(SW_SHOW);
	}
	else if (m_matLibNodeType == MATLIB_NODE_MATTHICK)
	{
		m_matNameInfoPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_matNameInfoPage.ShowWindow(SW_SHOW);
	}
	else if (m_matLibNodeType == MATLIB_NODE_MATSIZE)
	{
		m_matSizeInfoPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_matSizeInfoPage.ShowWindow(SW_SHOW);
	}
	/************************************************************************/
}

void MatLibView::OnCreateMatType()
{
	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();

	switch (m_createMatLibBtn.m_nMenuResult)
	{
	case IDC_NEW_MAT_TYPE:
		{
			MaterialInfoDlg dlg;
			if (dlg.DoModal() == IDOK)
			{
				// save the MatType.
				MaterialPtr pMat = dlg.GetMat();
				MaterialLibWriter::SaveMat(pMatLibDBCon, pMat, TRUE, TRUE);

				/************************************************************************/
				// append the node.

				// prepare the node.
				TVINSERTSTRUCT hItem;
				hItem.hParent = NULL;
				hItem.item.iImage = 0;
				hItem.item.iSelectedImage = 0;
				hItem.item.pszText = pMat->GetName().AllocSysString();
				hItem.item.cchTextMax = MAXLENGTH_STR;
				hItem.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_CHILDREN;
				hItem.item.cChildren = 1;
				hItem.hInsertAfter = TVI_LAST;

				// save data in tree node.
				MaterialLibNode* pMatLibNode = new MaterialLibNode(pMat->GetName(), MATLIB_NODE_MAT, pMat->GetID());
				hItem.item.lParam = (LPARAM)pMatLibNode;

				// insert the node.
				HTREEITEM hMatTypeNode = m_matLibTree.InsertItem(&hItem);
				m_matLibTree.SelectItem(hMatTypeNode);
				/************************************************************************/
			}
		}
		break;

	case IDC_IMP_MAT_TYPE:
		{
			ImpExpHelper::ImportMat(&m_matLibTree);
		}
		break;
	}
}

void MatLibView::OnSetDflMatType()
{
	// 默认的材料尺寸。
	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
	LONGLONG iDflMatSizeID = MaterialLibLoader::GetDflMatSize(pMatLibDBCon);

	DefaultMaterialSelectDlg dlg;
	dlg.SetDflMatSize(iDflMatSizeID);
	if (dlg.DoModal() == IDOK)
	{
		MaterialLibWriter::SaveDflMatSize(pMatLibDBCon, dlg.GetDflMatSize());
	}
}

void MatLibView::OnDelMatType()
{
	CString strProductName, str;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// get the selected MaterialLibNode.
	HTREEITEM hItem = m_matLibTree.GetSelectedItem();
	IMaterialLibNode* pNodeData = (IMaterialLibNode*)m_matLibTree.GetItemData(hItem);

	// 检查是否删除默认“材料”。
	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
	DataProperty dataProp;
	MaterialLibLoader::GetMatSizeProp(pMatLibDBCon, MaterialLibLoader::GetDflMatSize(pMatLibDBCon), dataProp);
	if (dataProp.GetItem(4).GetValue().llVal == pNodeData->GetDataID())
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_DFL_MAT);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// make sure to delete.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_MAT);
	if (MessageBox(str, strProductName, MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		MaterialLibWriter::DeleteMat(pMatLibDBCon, pNodeData->GetDataID());
		m_matLibTree.DeleteItem(hItem);
	}
}

void MatLibView::OnExportMat()
{
	ImpExpHelper::ExportMat(&m_matLibTree);
}

void MatLibView::OnCreateMatName()
{
	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();

	MaterialThicknessInfoDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MaterialThicknessPtr pMatThick = dlg.GetMatThick();

		/************************************************************************/
		// save MatName object.

		// get the selected MatType Node.
		HTREEITEM hMatTypeItem = m_matLibTree.GetSelectedItem();
		UINT dataVal = m_matLibTree.GetItemData(hMatTypeItem);
		IMaterialLibNode* pNodeData = (IMaterialLibNode*)dataVal;
		pMatThick->SetParentID(pNodeData->GetDataID());

		// save the MatName.
		MaterialLibWriter::SaveMatThick(pMatLibDBCon, pMatThick, TRUE, TRUE);
		/************************************************************************/

		/************************************************************************/
		// append the node.

		// prepare the node.
		TVINSERTSTRUCT hItem;
		hItem.hParent = hMatTypeItem;
		hItem.item.iImage = 1;
		hItem.item.iSelectedImage = 1;
		hItem.item.pszText = pMatThick->GetName().AllocSysString();
		hItem.item.cchTextMax = MAXLENGTH_STR;
		hItem.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_CHILDREN;
		hItem.item.cChildren = 1;
		hItem.hInsertAfter = TVI_LAST;

		// save data in tree node.
		MaterialLibNode* pMatLibNode = new MaterialLibNode(pMatThick->GetName(), MATLIB_NODE_MATTHICK, pMatThick->GetID());
		hItem.item.lParam = (LPARAM)pMatLibNode;

		// insert the node.
		HTREEITEM hMatNameNode = m_matLibTree.InsertItem(&hItem);
		m_matLibTree.SelectItem(hMatNameNode);
		/************************************************************************/
	}
}

void MatLibView::OnDelMatName()
{
	CString strProductName, str;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// get the selected MaterialLibNode.
	HTREEITEM hItem = m_matLibTree.GetSelectedItem();
	IMaterialLibNode* pNodeData = (IMaterialLibNode*)m_matLibTree.GetItemData(hItem);

	// 检查是否删除默认“材料厚度”。
	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
	DataProperty dataProp;
	MaterialLibLoader::GetMatSizeProp(pMatLibDBCon, MaterialLibLoader::GetDflMatSize(pMatLibDBCon), dataProp);
	if (dataProp.GetItem(5).GetValue().llVal == pNodeData->GetDataID())
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_DFL_MAT_THICK);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// make sure to delete.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_MAT_THICK);
	if (MessageBox(str, NULL, MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		MaterialLibWriter::DeleteMatName(pMatLibDBCon, pNodeData->GetDataID());
		m_matLibTree.DeleteItem(hItem);
	}
}

void MatLibView::OnCreateMatSize()
{
	MaterialSizeInfoDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		// get the material size object.
		IMaterialSizePtr pMaterialSize = dlg.GetMatSize();

		/************************************************************************/
		// save MatSize object.

		// get the selected MatName Node.
		HTREEITEM hMatNameItem = m_matLibTree.GetSelectedItem();
		IMaterialLibNode* pNodeData = (IMaterialLibNode*)m_matLibTree.GetItemData(hMatNameItem);
		pMaterialSize->SetParentID(pNodeData->GetDataID());

		// save the MatSize.
		DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
		MaterialLibWriter::SaveMatSize(pMatLibDBCon, pMaterialSize, TRUE);
		/************************************************************************/

		/************************************************************************/
		// append the node.

		// prepare the node.
		TVINSERTSTRUCT hItem;
		hItem.hParent = hMatNameItem;
		hItem.item.pszText = pMaterialSize->GetName().AllocSysString();
		hItem.item.cchTextMax = MAXLENGTH_STR;
		hItem.hInsertAfter = TVI_LAST;
		hItem.item.iImage = 2;
		hItem.item.iSelectedImage = 2;
		hItem.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;

		// save data in tree node.
		MaterialLibNode* pMatLibNode = new MaterialLibNode(pMaterialSize->GetName(), MATLIB_NODE_MATSIZE, pMaterialSize->GetID());
		hItem.item.lParam = (LPARAM)pMatLibNode;

		// insert the node.
		HTREEITEM hMatSizeNode = m_matLibTree.InsertItem(&hItem);
		m_matLibTree.SelectItem(hMatSizeNode);
		/************************************************************************/
	}
}

void MatLibView::OnImportMatSize()
{
	ImportMaterialDlg impFileDlg(OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT);
	if (impFileDlg.DoModal() == IDOK)
	{
		// 当前材料厚度节点。
		HTREEITEM hMatNameItem = m_matLibTree.GetSelectedItem();
		IMaterialLibNode* pNodeData = (IMaterialLibNode*)m_matLibTree.GetItemData(hMatNameItem);

		// 导入每个材料。
		POSITION pos = impFileDlg.GetStartPosition();
		while (pos != NULL)
		{
			CString strFilePath = impFileDlg.GetNextPathName(pos);

			// 从dxf/dwg文件得到基本图形集合。
			DxfDataPtr pDxfData;
			const CString& strDefaultFontFileName = CLOptionInfo::GetInstance()->GetPartEditParam()->GetDefaultFontFileName();
			if (PathHelper::IsDxfFile(strFilePath))
				pDxfData = DxfDwgConvertor::ExtractPatList(strFilePath, FALSE, strDefaultFontFileName);
			else if (PathHelper::IsDwgFile(strFilePath))
				pDxfData = DxfDwgConvertor::ExtractPatList(strFilePath, TRUE, strDefaultFontFileName);
			LineArc2DListPtr pGeom2DList = pDxfData->Calculate2DLineArcs();

			// 得到基本图形回路。
            double dImpTol = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetImpTol(); // 导入容差设置
			LineArc2DLoopListPtr pGeomLoopList = LineArc2DLoopManager::BuildGeomLoop(pGeom2DList, dImpTol);
			if (pGeomLoopList->size() == 0)
			{
				CString strProductName, str;
				strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
				str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_IMPORT_MATSIZE_ERROR1);
				MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
			}
			else
			{
				// 从“基本图形回路”集合生成“材料尺寸”对象。
				IMaterialSizePtr pMaterialSize = MaterialProcessor::BuildMatSize(PathHelper::GetFileName(strFilePath), pGeomLoopList.get());
				pMaterialSize->SetParentID(pNodeData->GetDataID());

				// save the MatSize.
				DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
				MaterialLibWriter::SaveMatSize(pMatLibDBCon, pMaterialSize, TRUE);

				/************************************************************************/
				// append the node.

				// prepare the node.
				TVINSERTSTRUCT hItem;
				hItem.hParent = hMatNameItem;
				hItem.item.pszText = pMaterialSize->GetName().AllocSysString();
				hItem.item.cchTextMax = MAXLENGTH_STR;
				hItem.hInsertAfter = TVI_LAST;
				hItem.item.iImage = 2;
				hItem.item.iSelectedImage = 2;
				hItem.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;

				// save data in tree node.
				MaterialLibNode* pMatLibNode = new MaterialLibNode(pMaterialSize->GetName(), MATLIB_NODE_MATSIZE, pMaterialSize->GetID());
				hItem.item.lParam = (LPARAM)pMatLibNode;

				// insert the node.
				HTREEITEM hMatSizeNode = m_matLibTree.InsertItem(&hItem);
				m_matLibTree.SelectItem(hMatSizeNode);
				/************************************************************************/
			}
		}
	}
}

void MatLibView::OnDelMatSize()
{
	CString strProductName, str;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// get the selected MaterialLibNode.
	HTREEITEM hItem = m_matLibTree.GetSelectedItem();
	IMaterialLibNode* pNodeData = (IMaterialLibNode*)m_matLibTree.GetItemData(hItem);

	// 检查是否删除默认“材料尺寸”。
	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
	LONGLONG iDflMatSizeID = MaterialLibLoader::GetDflMatSize(pMatLibDBCon);
	if (iDflMatSizeID == pNodeData->GetDataID())
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_DFL_MAT_SIZE);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// make sure to delete.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_MAT_SIZE);
	if (MessageBox(str, NULL, MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		MaterialLibWriter::DeleteMatSize(pMatLibDBCon, pNodeData->GetDataID());
		m_matLibTree.DeleteItem(hItem);
	}
}

END_CUTLEADER_NAMESPACE()

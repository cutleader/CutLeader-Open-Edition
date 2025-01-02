#include "stdafx.h"
#include "ExpertLibView.h"

#include "DateHelper.h"
#include "clUtilityResource.h"

#include "LogMessage.h"
#include "LogMgr.h"
#include "baseConst.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"

#include "MachineItemLoader.h"

#include "ExpertLibNode.h"
#include "ExpertLibNodeList.h"
#include "MachineSelectDlg.h"
#include "clExpertLibResource.h"
#include "ExpertLibItem.h"
#include "ExpertLibItemWriter.h"
#include "ExpertLibItemLoader.h"
#include "ParamConfigNameDlg.h"
#include "ParamConfig.h"
#include "ParamConfigList.h"
#include "ParamConfigLoader.h"
#include "ParamConfigWriter.h"
#include "DefaultParamConfig.h"
#include "DefaultParamConfigSelectDlg.h"

#include "CLOptionInfo.h"
#include "ProductParam.h"

#include "ImpExpHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ExpertLibView, CResizableFormView)

BEGIN_MESSAGE_MAP(ExpertLibView, CResizableFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_EXPLIB, OnSelchangedTree)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_EXPLIB, OnRClickTree)

	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()

	ON_COMMAND(IDC_CREATE_PARAM_CONFIG, OnCreateParamConfig)
	ON_COMMAND(IDC_RENAME_PARAM_CONFIG, OnRenameParamConfig)
	ON_COMMAND(IDC_DELETE_PARAM_CONFIG, OnDeleteParamConfig)
	ON_COMMAND(IDC_COPY_PARAM_CONFIG, OnCopyParamConfig)
	ON_COMMAND(IDC_BTN_DFL_PARAM_CONFIG, OnDflParamConfig)

	ON_COMMAND(IDC_BTN_CREATE_EXPITEM, OnCreateExpLibItem)
	ON_COMMAND(IDC_DEL_EXPLIB_ITEM, OnDelExpLibItem)
	ON_COMMAND(IDC_EXPORT_EXPLIBITEM, OnExportExpLibItem)
END_MESSAGE_MAP()

ExpertLibView::ExpertLibView()
	: KBConfigView(ExpertLibView::IDD)
{
	m_expLibNodeType = EXPLIB_NODE_NOT_INIT;
}

ExpertLibView::~ExpertLibView()
{
}

void ExpertLibView::DoDataExchange(CDataExchange* pDX)
{
	CResizableFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE_EXPLIB, m_expLibTree);
	DDX_Control(pDX, IDC_BTN_CREATE_EXPITEM, m_createExpLibItemBtn);
	DDX_Control( pDX, IDC_BTN_DFL_PARAM_CONFIG, m_btnDflParamConfig );
}

BOOL ExpertLibView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CResizableFormView::PreCreateWindow(cs);
}

void ExpertLibView::OnInitialUpdate()
{
	CResizableFormView::OnInitialUpdate();
	ResizeParentToFit();
}

void ExpertLibView::InitSubDialog()
{
	// create ExpertLibItemInfoPage.
	m_expLibItemInfoPage.Create(IDD_clExpertLibUI_ExpertLibItem, this);
	m_expLibItemInfoPage.ShowWindow(SW_HIDE);

	// create ParamConfigPage.
	m_schemePage.Create(IDD_clExpertLibUI_ParamConfigPage, this);
	m_schemePage.ShowWindow(SW_HIDE);
}

BOOL ExpertLibView::OnInitDialog()
{
	CResizableFormView::OnInitDialog();

	// 翻译
	MultiLanguageMgr::GetInstance()->TranslateDialog(this);

	// layout the controls.
	AddAnchor(IDC_GROUP_TREE, TOP_LEFT, ANCHOR(30,100));
	AddAnchor(IDC_TREE_EXPLIB, TOP_LEFT, ANCHOR(30,100));
	AddAnchor(IDC_STATIC_CHILDAREA, ANCHOR(30,0), BOTTOM_RIGHT);
	AddAnchor(IDC_BTN_CREATE_EXPITEM, BOTTOM_LEFT);
	AddAnchor(IDC_BTN_DFL_SCHEME, BOTTOM_LEFT);

	/************************************************************************/	
	// 初始化按钮菜单。

	m_btnMenu.CreatePopupMenu();

	// create sub items.
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEW);
	m_btnMenu.AppendMenu(MF_STRING, IDC_NEW_EXPLIBITEM, str.AllocSysString());
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPORT);
	m_btnMenu.AppendMenu(MF_STRING, IDC_IMPORT_EXPLIBITEM, str.AllocSysString());

	//
	m_createExpLibItemBtn.m_hMenu = m_btnMenu.GetSafeHmenu();
	m_createExpLibItemBtn.SizeToContent();
	m_createExpLibItemBtn.m_bStayPressed = TRUE;
	m_createExpLibItemBtn.m_bRightArrow = TRUE;
	/************************************************************************/

	// init sub dialogs.
	InitSubDialog();

	// init the expert library structure tree.
	InitExpLibTree();

	SetWindowTheme( GetDlgItem( IDC_GROUP_TREE )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void ExpertLibView::InitExpLibTree()
{
	// 是否内部使用。
	ProductParamPtr pProductParam = CLOptionInfo::GetInstance()->GetProductParam();
	BOOL bInternal = pProductParam->IsUseInside();

	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	ExpLibNodePtr pExpLibRootNode;
	if (bInternal)
		pExpLibRootNode = ExpertLibItemLoader::LoadExpLibTree(pExpLibDBCon, FALSE, FALSE);
	else
		pExpLibRootNode = ExpertLibItemLoader::LoadExpLibTree(pExpLibDBCon);
	m_expLibTree.Init(pExpLibRootNode, INVALID_ID);
}

void ExpertLibView::HideAllSubDialog()
{
	m_expLibItemInfoPage.ShowWindow(SW_HIDE);
	m_schemePage.ShowWindow(SW_HIDE);
}

void ExpertLibView::OnSize(UINT nType, int cx, int cy)
{
	CResizableFormView::OnSize(nType, cx, cy);

	// re-position the sub dialog.
	RepositionSubDlg();
}

BOOL ExpertLibView::OnEraseBkgnd( CDC *pDC )
{
	CBrush* br = &SkinManager::GetDialogBkgndBrush(); // to be verified.
	CRect r;
	GetClientRect(&r);
	pDC->FillRect(&r, br);

	return TRUE;
}

HBRUSH ExpertLibView::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ExpertLibView::RepositionSubDlg()
{
	// no node selected, of course no page to display.
	if (m_expLibNodeType == EXPLIB_NODE_NOT_INIT)
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
	// because the layout library will re-layout the controls, so we should re-position the sub dialog.

	HideAllSubDialog();

	if (m_expLibNodeType == EXPLIB_NODE_EXPLIBITEM)
	{
		m_expLibItemInfoPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_expLibItemInfoPage.ShowWindow(SW_SHOW);
	}
	else if (m_expLibNodeType == EXPLIB_NODE_CONFIGURATION)
	{
		m_schemePage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_schemePage.ShowWindow(SW_SHOW);
	}
	/************************************************************************/
}

void ExpertLibView::SaveCurrentPage()
{
	CString strTaskName, strMsgContent;

	/************************************************************************/
	// get the current node.

	HTREEITEM hOldItem = m_expLibTree.GetSelectedItem();
	if (!hOldItem)
		return;

	UINT oldDataVal = m_expLibTree.GetItemData(hOldItem);
	IExpertLibNode* pOldNodeData = (IExpertLibNode*)oldDataVal;
	EXPLIB_NODE_TYPE pOldNodeType = pOldNodeData->GetNodeType();
	/************************************************************************/

	if (SaveModification(hOldItem, FALSE))
	{
		// prepare the message.
		if (pOldNodeType == EXPLIB_NODE_EXPLIBITEM)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_EXPLIB_ITEM_SAVED);
		else if (pOldNodeType == EXPLIB_NODE_CONFIGURATION)
		{
			EXPLIB_NODE_TYPE nodeType = m_schemePage.GetCurNodeType();
            if (nodeType == EXPLIB_NODE_TOOL_ASSIGN)
                strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_TOOL_CONFIG_SAVED);
			else if (nodeType == EXPLIB_NODE_LEAD_ASSIGN)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_LEAD_SAVED);
			else if (nodeType == EXPLIB_NODE_PIERCE_ASSIGN)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PIERCE_SAVED);
			else if (nodeType == EXPLIB_NODE_CUT_MJ_ASSIGN)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MJ_ASSIGN_SAVED);
			else if (nodeType == EXPLIB_NODE_CORNER_ASSIGN)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CORNER_SAVED);
			else if (nodeType == EXPLIB_NODE_GENERATE_SEQ)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SEQ_CONFIG_SAVED);
			else if (nodeType == EXPLIB_NODE_GENERATE_NC)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NC_CONFIG_SAVED);
			else if (nodeType == EXPLIB_NODE_NEST_PARAM)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NESTPARAM_SAVED);
		}
	}
	else
	{
		// prepare the message.
		if (pOldNodeType == EXPLIB_NODE_EXPLIBITEM)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_EXPLIB_ITEM_NOT_CHANGE);
		else if (pOldNodeType == EXPLIB_NODE_CONFIGURATION)
		{
			EXPLIB_NODE_TYPE nodeType = m_schemePage.GetCurNodeType();
            if (nodeType == EXPLIB_NODE_TOOL_ASSIGN)
                strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_TOOL_CONFIG_NOT_CHANGE);
			else if (nodeType == EXPLIB_NODE_LEAD_ASSIGN)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_LEAD_NOT_CHANGE);
			else if (nodeType == EXPLIB_NODE_PIERCE_ASSIGN)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PIERCE_NOT_CHANGE);
			else if (nodeType == EXPLIB_NODE_CUT_MJ_ASSIGN)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MJ_ASSIGN_NOT_CHANGE);
			else if (nodeType == EXPLIB_NODE_CORNER_ASSIGN)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CORNER_NOT_CHANGE);
			else if (nodeType == EXPLIB_NODE_GENERATE_SEQ)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SEQ_CONFIG_NOT_CHANGE);
			else if (nodeType == EXPLIB_NODE_GENERATE_NC)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NC_CONFIG_NOT_CHANGE);
			else if (nodeType == EXPLIB_NODE_NEST_PARAM)
				strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NESTPARAM_NOT_CHANGE);
		}
	}

	// write the log.
	strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_EXPLIB);
	LogMessagePtr pLogMsg(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_INFO, strTaskName, strMsgContent));
	LogMgr::WriteLog(pLogMsg);

	// prompt user.
	CString strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	MessageBox(strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
}

BOOL ExpertLibView::DocModified()
{
	/************************************************************************/
	// get the current node.

	HTREEITEM hOldItem = m_expLibTree.GetSelectedItem();
	if (!hOldItem)
		return FALSE;

	UINT oldDataVal = m_expLibTree.GetItemData(hOldItem);
	IExpertLibNode* pOldNodeData = (IExpertLibNode*)oldDataVal;
	EXPLIB_NODE_TYPE pOldNodeType = pOldNodeData->GetNodeType();
	LONGLONG iOldNodeID = pOldNodeData->GetDataID();
	/************************************************************************/

	if (pOldNodeType == EXPLIB_NODE_EXPLIBITEM)
	{
		// force all edit boxes lose focus.
		m_expLibItemInfoPage.SetFocus();

		ExpertLibItemPtr pExpLibItem = m_expLibItemInfoPage.GetExpLibItem();
		if (pExpLibItem->IsModified())
			return TRUE;
	}
	else if (pOldNodeType == EXPLIB_NODE_CONFIGURATION)
	{
		return m_schemePage.DocModified();
	}

	return FALSE;
}

BOOL ExpertLibView::SaveModification(HTREEITEM hOldItem, BOOL bNeedConfirm)
{
	BOOL bModified = FALSE;

	CString strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	UINT oldDataVal = m_expLibTree.GetItemData(hOldItem);
	IExpertLibNode* pOldNodeData = (IExpertLibNode*)oldDataVal;
	EXPLIB_NODE_TYPE pOldNodeType = pOldNodeData->GetNodeType();
	LONGLONG iOldNodeID = pOldNodeData->GetDataID();

	if (pOldNodeType == EXPLIB_NODE_EXPLIBITEM)
	{
		// force all edit boxes lose focus.
		m_expLibItemInfoPage.SetFocus();

		ExpertLibItemPtr pExpLibItem = m_expLibItemInfoPage.GetExpLibItem();

		// check whether we should save the ExpertLibItem.
		if (pExpLibItem->IsModified())
		{
			CString str;
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_EXPLIB_ITEM);
			if ((bNeedConfirm && MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				//
				pExpLibItem->SetModifyTime(DateHelper::GetCurrentTime());

				// update db.
				DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
				ExpertLibItemWriter::UpdateBasicData(pExpLibDBCon, pExpLibItem);
				ExpertLibItemWriter::UpdateModifyDate(pExpLibDBCon, pExpLibItem->GetID(), pExpLibItem->GetModifyTime());

				// update UI.
				m_expLibTree.SetItemText(hOldItem, pExpLibItem->GetName());
				m_expLibItemInfoPage.UpdateModifyDate();
			}
		}
	}
	else if (pOldNodeType == EXPLIB_NODE_CONFIGURATION)
	{
		bModified = m_schemePage.SaveModification(bNeedConfirm);
	}

	return bModified;
}

void ExpertLibView::CheckModification(HTREEITEM hOldItem)
{
	UINT oldDataVal = m_expLibTree.GetItemData(hOldItem);
	IExpertLibNode* pOldNodeData = (IExpertLibNode*)oldDataVal;
	EXPLIB_NODE_TYPE pOldNodeType = pOldNodeData->GetNodeType();

	if (SaveModification(hOldItem, TRUE))
	{
		// prepare the message.
		CString strTaskName, strMsgContent;
		if (pOldNodeType == EXPLIB_NODE_EXPLIBITEM)
		{
			strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_EXPLIB);
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_EXPLIB_ITEM_SAVED);
		}
		else if (pOldNodeType == EXPLIB_NODE_CONFIGURATION)
		{
			// ParamConfigPage will care this.
			return;
		}

		// write the log.
		LogMessagePtr pLogMsg(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_INFO, strTaskName, strMsgContent));
		LogMgr::WriteLog(pLogMsg);
	}
}

void ExpertLibView::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item = pNMTreeView->itemNew;
	TVITEM oldItem = pNMTreeView->itemOld;

	/************************************************************************/
	// get the data that is stored in the new and old node.

	// the item data in new node.
	UINT dataVal = m_expLibTree.GetItemData(item.hItem);
	IExpertLibNode* pNodeData = (IExpertLibNode*)dataVal;
	m_expLibNodeType = pNodeData->GetNodeType();

	// get the item data in old node. if "oldItem.hItem" is NULL, we know
	// this is the first time to select a node.
	EXPLIB_NODE_TYPE oldNodeType = EXPLIB_NODE_NOT_INIT;
	IExpertLibNode* pOldNodeData = NULL;
	if (oldItem.hItem)
	{
		UINT oldDataVal = m_expLibTree.GetItemData(oldItem.hItem);
		pOldNodeData = (IExpertLibNode*)oldDataVal;
		oldNodeType = pOldNodeData->GetNodeType();

		// we should check whether we should save the data on the previous page.
		CheckModification(oldItem.hItem);
	}
	/************************************************************************/

	// re-position the sub dialog.
	RepositionSubDlg();

	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	if (m_expLibNodeType == EXPLIB_NODE_EXPLIBITEM)
	{
		ExpertLibItemPtr pExpLibItem = ExpertLibItemLoader::LoadExpLibItem(pExpLibDBCon, pNodeData->GetDataID(), EXPLIBITEM_LOAD_BASIC);
		m_expLibItemInfoPage.DisplayExpLibItem(pExpLibItem);
	}
	else if (m_expLibNodeType == EXPLIB_NODE_CONFIGURATION)
	{
		m_schemePage.DisplayPage(pNodeData->GetDataID());
	}
}

void ExpertLibView::OnRClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	/************************************************************************/
	// select tree item which r-button clicked.

	POINT pos;
	if (!GetCursorPos(&pos))
		return;
	m_expLibTree.ScreenToClient(&pos);

	// get the item which was r-clicked, if click on the blank area, quit.
	UINT uFlags;
	HTREEITEM hItem = m_expLibTree.HitTest(pos, &uFlags);
	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
		m_expLibTree.SelectItem(hItem);
	else
		return;

	// the selected node type.
	UINT dataVal = m_expLibTree.GetItemData(hItem);
	IExpertLibNode* pNodeData = (IExpertLibNode*)dataVal;
	EXPLIB_NODE_TYPE nodeType = pNodeData->GetNodeType();
	/************************************************************************/

	/************************************************************************/
	// display the popup menu.

	// figure out the position where the menu will pop up.
	CRect rect;
	this->GetClientRect(&rect);
	ClientToScreen(&rect);
	m_expLibTree.ClientToScreen(&pos);

	// display different popup.
	CString str;
	CMenu menu;
	menu.CreatePopupMenu();
	if (nodeType == EXPLIB_NODE_EXPLIBITEM)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE);
		menu.AppendMenu(MF_STRING, IDC_DEL_EXPLIB_ITEM, str);

		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_EXPORT);
		menu.AppendMenu(MF_STRING, IDC_EXPORT_EXPLIBITEM, str);

		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEW_PARAM_CONFIG);
		menu.AppendMenu(MF_STRING, IDC_CREATE_PARAM_CONFIG, str);
	}
	else if (nodeType == EXPLIB_NODE_CONFIGURATION)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COPY);
		menu.AppendMenu(MF_STRING, IDC_COPY_PARAM_CONFIG, str);

		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DELETE);
		menu.AppendMenu(MF_STRING, IDC_DELETE_PARAM_CONFIG, str);

		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RENAME);
		menu.AppendMenu(MF_STRING, IDC_RENAME_PARAM_CONFIG, str);
	}
	menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, pos.x, pos.y, this); 
	/************************************************************************/

	*pResult = 0;
}

void ExpertLibView::OnCreateParamConfig()
{
	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();

	ParamConfigNameDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strConfigName = dlg.GetName();

		// get the selected IExpertLibNode.
		HTREEITEM hSelItem = m_expLibTree.GetSelectedItem();
		UINT dataVal = m_expLibTree.GetItemData(hSelItem);
		IExpertLibNode* pPartSchemesNode = (IExpertLibNode*)dataVal;
		LONGLONG iExpLibItemID = pPartSchemesNode->GetDataID();

		// 检查是否有同名的“参数配置”。
		if (ParamConfigLoader::GetConfigIDByName(pExpLibDBCon, iExpLibItemID, strConfigName) != INVALID_ID)
		{
			CString str, strProductName;
			strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PARAM_CONFIG_EXIST);
			MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
			return;
		}

		// 加载作为模板的“专家库项”。
		LONGLONG iTplMacItemID = MachineItemLoader::GetReserverdMacItem(pMacLibDBConnect);
		LONGLONG iTplExpLibItemID = ExpertLibItemLoader::GetExpLibItemID(pExpLibDBCon, iTplMacItemID);
		ExpertLibItemPtr pExpLibItem = ExpertLibItemLoader::LoadExpLibItem(pExpLibDBCon, iTplExpLibItemID, EXPLIBITEM_LOAD_ALL);

		// 克隆一份新的“参数配置”。
		ParamConfigPtr pTplParamConfig = pExpLibItem->GetParamConfigList()->at(0);
		ParamConfigPtr pNewParamConfig = boost::dynamic_pointer_cast<ParamConfig>(pTplParamConfig->Clone());
		pNewParamConfig->SetParentID(iExpLibItemID);
		pNewParamConfig->SetName(strConfigName);

		// 保存新的“参数配置”。
		ParamConfigWriter::SaveParamConfig(pExpLibDBCon, pNewParamConfig);

		/************************************************************************/		
		// append the new part scheme node in the tree.

		TVINSERTSTRUCT schemeNode;
		schemeNode.hParent = hSelItem;
		schemeNode.item.pszText = strConfigName.AllocSysString();
		schemeNode.item.cchTextMax = MAXLENGTH_STR;
		schemeNode.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
		schemeNode.hInsertAfter = TVI_LAST;

		// set the node data.
		ExpertLibNode* pExpLibNode = new ExpertLibNode(strConfigName, EXPLIB_NODE_CONFIGURATION, pNewParamConfig->GetID());
		schemeNode.item.lParam = (LPARAM)pExpLibNode;
		schemeNode.item.iImage = 1;
		schemeNode.item.iSelectedImage = 1;

		// append this node.
		m_expLibTree.InsertItem(&schemeNode);
		/************************************************************************/
	}
}

void ExpertLibView::OnRenameParamConfig()
{
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();

	// 选中的“参数配置”节点。
	HTREEITEM hSelItem = m_expLibTree.GetSelectedItem();
	IExpertLibNode* pSchemeNode = (IExpertLibNode*)m_expLibTree.GetItemData(hSelItem);
	CString strCurName = m_expLibTree.GetItemText(hSelItem);

	ParamConfigNameDlg dlg;
	dlg.InitName(strCurName);
	if (dlg.DoModal() == IDOK)
	{
		CString strConfigName = dlg.GetName();

		// get the relative ExpertLibItem.
		LONGLONG iParamConfigID = pSchemeNode->GetDataID();
		LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pExpLibDBCon, iParamConfigID);

		// 检查是否有同名的“参数配置”。
		if (ParamConfigLoader::GetConfigIDByName(pExpLibDBCon, iExpLibItemID, strConfigName) != INVALID_ID)
		{
			CString str, strProductName;
			strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PARAM_CONFIG_EXIST);
			MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		}
		else
		{
			ParamConfigWriter::UpdateConfigName(pExpLibDBCon, iParamConfigID, strConfigName);
			m_expLibTree.SetItemText(hSelItem, strConfigName);
		}
	}
}

void ExpertLibView::OnDeleteParamConfig()
{
	CString str, strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// get the selected IExpertLibNode.
	HTREEITEM hSelItem = m_expLibTree.GetSelectedItem();
	UINT dataVal = m_expLibTree.GetItemData(hSelItem);
	IExpertLibNode* pNodeData = (IExpertLibNode*)dataVal;

	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();

	// 不能删除默认的“参数配置”。
	DefaultParamConfigPtr pDflParamConfig = ParamConfigLoader::LoadDflParamConfig(pExpLibDBCon);
	if (pDflParamConfig->GetParamConfigID() == pNodeData->GetDataID())
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_DFL_SCHEME);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// 删除。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_PARAM_CONFIG);
	if (MessageBox(str, strProductName, MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		ParamConfigWriter::DeleteConfig(pExpLibDBCon, pNodeData->GetDataID());
		m_expLibTree.DeleteItem(hSelItem);
	}
}

void ExpertLibView::OnCopyParamConfig()
{
	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();

	ParamConfigNameDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strConfigName = dlg.GetName();

		// 选中的“参数配置”。
		HTREEITEM hSelItem = m_expLibTree.GetSelectedItem();
		IExpertLibNode* pParamConfigNode = (IExpertLibNode*)m_expLibTree.GetItemData(hSelItem);
		ParamConfigPtr pSelectedParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pParamConfigNode->GetDataID());

		// 检查是否有同名的“参数配置”。
		LONGLONG iExpLibItemID = pSelectedParamConfig->GetParentID();
		if (ParamConfigLoader::GetConfigIDByName(pExpLibDBCon, iExpLibItemID, strConfigName) != INVALID_ID)
		{
			CString str, strProductName;
			strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PARAM_CONFIG_EXIST);
			MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
			return;
		}

		// 克隆一份新的“参数配置”。
		ParamConfigPtr pNewParamConfig = boost::dynamic_pointer_cast<ParamConfig>(pSelectedParamConfig->Clone());
		pNewParamConfig->SetParentID(iExpLibItemID);
		pNewParamConfig->SetName(strConfigName);

		// 保存新的“参数配置”。
		ParamConfigWriter::SaveParamConfig(pExpLibDBCon, pNewParamConfig);

		/************************************************************************/		
		// append the new part scheme node in the tree.

		TVINSERTSTRUCT schemeNode;
		schemeNode.hParent = m_expLibTree.GetParentItem(hSelItem);
		schemeNode.item.pszText = strConfigName.AllocSysString();
		schemeNode.item.cchTextMax = MAXLENGTH_STR;
		schemeNode.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
		schemeNode.hInsertAfter = TVI_LAST;

		// set the node data.
		ExpertLibNode* pExpLibNode = new ExpertLibNode(strConfigName, EXPLIB_NODE_CONFIGURATION, pNewParamConfig->GetID());
		schemeNode.item.lParam = (LPARAM)pExpLibNode;
		schemeNode.item.iImage = 1;
		schemeNode.item.iSelectedImage = 1;

		// append this node.
		m_expLibTree.InsertItem(&schemeNode);
		/************************************************************************/
	}
}

void ExpertLibView::OnDflParamConfig()
{
	// 加载默认的“参数配置”。
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	DefaultParamConfigPtr pDflParamConfig = ParamConfigLoader::LoadDflParamConfig(pExpLibDBCon);

	DefaultParamConfigSelectDlg dlg;
	dlg.SetDflParamConfig(pDflParamConfig);
	if (dlg.DoModal() == IDOK)
	{
		ParamConfigWriter::SaveDflParamConfig(pExpLibDBCon, pDflParamConfig);
	}
}

void ExpertLibView::OnCreateExpLibItem()
{
	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();

	switch (m_createExpLibItemBtn.m_nMenuResult)
	{
	case IDC_NEW_EXPLIBITEM:
		{
			MachineSelectDlg dlg;
			if (dlg.DoModal() == IDOK)
			{
				// the selected mac item.
				LONGLONG iMacItemID = dlg.GetSelectedMac();
				MachineItemPtr pMacItem = MachineItemLoader::LoadMacItem(pMacLibDBConnect, iMacItemID);

				// load the template expLib item.
				LONGLONG iRsvMacItemID = MachineItemLoader::GetReserverdMacItem(pMacLibDBConnect);
				LONGLONG iRsvExpLibItemID = ExpertLibItemLoader::GetExpLibItemID(pExpLibDBCon, iRsvMacItemID);
				ExpertLibItemPtr pExpLibItem = ExpertLibItemLoader::LoadExpLibItem(pExpLibDBCon, iRsvExpLibItemID, EXPLIBITEM_LOAD_ALL);

				// clone a new expLib item.
				IDataPtr pData = pExpLibItem->Clone();
				ExpertLibItemPtr pNewExpLibItem = boost::dynamic_pointer_cast<ExpertLibItem>(pData);

				// update the name of new expLib item to the mac name.
				DataProperty dataProp;
				MachineItemLoader::GetMacItemProp(pMacLibDBConnect, iMacItemID, dataProp);
				pNewExpLibItem->SetName(dataProp.GetItem(1).GetValue().bstrVal);

				// update some data.
				pNewExpLibItem->SetMacItem(pMacItem);
				pNewExpLibItem->SetCreateTime((DateHelper::GetCurrentTime()));
				pNewExpLibItem->SetModifyTime((DateHelper::GetCurrentTime()));

				// save it to db.
				ExpertLibItemWriter::SaveExpLibItem(pExpLibDBCon, pNewExpLibItem, TRUE);

				/************************************************************************/
				// append the expert library item node to tree.

				// init the expert library item node.
				ExpLibNodePtr pExpLibItemNode(new ExpertLibNode(EXPLIB_NODE_EXPLIBITEM));
				pExpLibItemNode->SetDataID(pNewExpLibItem->GetID());
				pExpLibItemNode->SetNodeName(pNewExpLibItem->GetName());
				DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
				ExpertLibItemLoader::InitExpLibItem(pExpLibDBCon, pExpLibItemNode);

				// create tree node.
				HTREEITEM hExpItemNode = m_expLibTree.AppendExpLibItemNode(pExpLibItemNode, INVALID_ID);
				m_expLibTree.SelectItem(hExpItemNode);
				/************************************************************************/
			}
		}
		break;

	case IDC_IMPORT_EXPLIBITEM:
		{
			ImpExpHelper::ImportExpLibItem(&m_expLibTree);
		}
		break;
	}
}

void ExpertLibView::OnDelExpLibItem()
{
	CString str, strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();

	// get the selected IExpertLibNode.
	HTREEITEM hSelItem = m_expLibTree.GetSelectedItem();
	UINT dataVal = m_expLibTree.GetItemData(hSelItem);
	IExpertLibNode* pNodeData = (IExpertLibNode*)dataVal;

	// 不能删除保留的“专家库项”。
	LONGLONG iMacID = ExpertLibItemLoader::GetMacIDByExpLibItem(pExpLibDBCon, pNodeData->GetDataID());
	if (MachineItemLoader::GetReserverdMacItem(pMacLibDBConnect) == iMacID)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_RESERVED_EXPLIBITEM);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// 不能删除默认的“专家库项”。
	DefaultParamConfigPtr pDflParamConfig = ParamConfigLoader::LoadDflParamConfig(pExpLibDBCon);
	if (pDflParamConfig->GetExpLibItemID() == pNodeData->GetDataID())
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_DFL_EXPLIBITEM);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// 删除专家库项。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DEL_EXPLIB_ITEM);
	if (MessageBox(str, strProductName, MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		ExpertLibItemWriter::DeleteExpLibItem(pExpLibDBCon, pNodeData->GetDataID());
		m_expLibTree.DeleteItem(hSelItem);
	}
}

void ExpertLibView::OnExportExpLibItem()
{
	ImpExpHelper::ExportExpLibItem(&m_expLibTree);
}

END_CUTLEADER_NAMESPACE()

#include "stdafx.h"
#include "clKnowledgeBaseConfig.h"
#include "MacMgrView.h"

#include "baseConst.h"
#include "DateHelper.h"
#include "LogMessage.h"
#include "LogMgr.h"
#include "baseConst.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"

#include "MachineLibNode.h"
#include "MachineLibNodeList.h"
#include "clMachineLibResource.h"
#include "MachineInfoDlg.h"
#include "MachineItem.h"
#include "MachineParam.h"
#include "ConditionTypeList.h"
#include "NCConfigTypeList.h"
#include "MachineItemLoader.h"
#include "MachineItemWriter.h"
#include "ExpertLibItemLoader.h"
#include "ProductParam.h"
#include "CLOptionInfo.h"
#include "clKnowledgeBaseConfigResource.h"
#include "SkinManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(MacMgrView, CResizableFormView)

BEGIN_MESSAGE_MAP(MacMgrView, CResizableFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MAC, OnSelchangedTree)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_MAC, OnRClickTree)

	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()

	// for popup menu.
	ON_COMMAND(IDC_DEL_MAC, OnDelMac)
	ON_COMMAND(IDC_ADD_MAC, OnCreateMac)
END_MESSAGE_MAP()

MacMgrView::MacMgrView()
	: KBConfigView(MacMgrView::IDD)
{
	m_macNodeType = MACLIB_NODE_NOT_INIT;

	// disable drag/drop.
	m_macLibTree.EnableDragDrop(FALSE);
}

MacMgrView::~MacMgrView()
{
}

void MacMgrView::DoDataExchange(CDataExchange* pDX)
{
	CResizableFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE_MAC, m_macLibTree);
}

BOOL MacMgrView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CResizableFormView::PreCreateWindow(cs);
}

void MacMgrView::OnInitialUpdate()
{
	CResizableFormView::OnInitialUpdate();
	ResizeParentToFit();
}

void MacMgrView::InitSubDialog()
{
	// create MachineListPage.
	m_macListPage.Create(IDD_CAMCONFIG_MACLIST_PAGE, this);
	m_macListPage.ShowWindow(SW_HIDE);

	// create MachineInfoPage.
	m_macInfoPage.Create(IDD_clMachineLibUI_MachineInfo, this);
	m_macInfoPage.ShowWindow(SW_HIDE);

	// create CutMacInfoPage.
	m_macParamPage.Create(IDD_clMachineLibUI_MachineParam, this);
	m_macParamPage.ShowWindow(SW_HIDE);

	// create ConditionEditPage.
	m_condEditPage.Create(IDD_clMachineLibUI_ConditionEdit, this);
	m_condEditPage.ShowWindow(SW_HIDE);

	// create NCConfigTypeEditPage.
	m_NCConfTypeEditPage.Create(IDD_clMachineLibUI_NcConfigType, this);
	m_NCConfTypeEditPage.ShowWindow(SW_HIDE);
}

BOOL MacMgrView::OnInitDialog() 
{
	CResizableFormView::OnInitDialog();

	// 翻译
	MultiLanguageMgr::GetInstance()->TranslateDialog(this);

	// layout the controls.
	AddAnchor(IDC_GROUP_TREE, TOP_LEFT, ANCHOR(30,100));
	AddAnchor(IDC_TREE_MAC, TOP_LEFT, ANCHOR(30,100));
	AddAnchor(IDC_STATIC_CHILDAREA, ANCHOR(30,0), BOTTOM_RIGHT);

	// init sub dialogs.
	InitSubDialog();

	// init the expert library structure tree.
	InitMacTree();

	SetWindowTheme( GetDlgItem( IDC_GROUP_TREE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_TREE_MAC )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void MacMgrView::InitMacTree()
{
	// whether used for internal.
	ProductParamPtr pProductParam = CLOptionInfo::GetInstance()->GetProductParam();
	BOOL bInternal = pProductParam->IsUseInside();

	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	MachineLibNodePtr pMacLibRootNode;
	if (bInternal)
		pMacLibRootNode = MachineItemLoader::LoadMacLibTree(pMacLibDBConnect, FALSE, FALSE);
	else
		pMacLibRootNode = MachineItemLoader::LoadMacLibTree(pMacLibDBConnect);
	m_macLibTree.Init(pMacLibRootNode);
}

void MacMgrView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void MacMgrView::HideAllSubDialog()
{
	m_macListPage.ShowWindow(SW_HIDE);
	m_macParamPage.ShowWindow(SW_HIDE);
	m_condEditPage.ShowWindow(SW_HIDE);
	m_NCConfTypeEditPage.ShowWindow(SW_HIDE);
	m_macInfoPage.ShowWindow(SW_HIDE);
}

void MacMgrView::OnSize(UINT nType, int cx, int cy)
{
	CResizableFormView::OnSize(nType, cx, cy);

	// re-position the sub dialog.
	RepositionSubDlg();
}

BOOL MacMgrView::OnEraseBkgnd( CDC *pDC )
{
	CBrush* br = &SkinManager::GetDialogBkgndBrush(); // to be verified.
	CRect rcClip;
	pDC->GetClipBox( rcClip );
	pDC->FillSolidRect( rcClip, SkinManager::GetBackgroundColor() );
	return TRUE;
}

HBRUSH MacMgrView::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void MacMgrView::RepositionSubDlg()
{
	// no node selected, of course no page to display.
	if (m_macNodeType == MACLIB_NODE_NOT_INIT)
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

	if (m_macNodeType == MACLIB_NODE_ROOT)
	{
		m_macListPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_macListPage.ShowWindow(SW_SHOW);
	}
	else if (m_macNodeType == MACLIB_NODE_MACLIB_ITEM)
	{
		m_macInfoPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_macInfoPage.ShowWindow(SW_SHOW);
	}
	else if (m_macNodeType == MACLIB_NODE_MAC_PARAM)
	{
		m_macParamPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_macParamPage.ShowWindow(SW_SHOW);
	}
	else if (m_macNodeType == MACLIB_NODE_MAC_COND)
	{
		m_condEditPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_condEditPage.ShowWindow(SW_SHOW);
	}
	else if (m_macNodeType == MACLIB_NODE_MAC_NCCONFIG)
	{
		m_NCConfTypeEditPage.SetWindowPos(pWndPos,  pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_NCConfTypeEditPage.ShowWindow(SW_SHOW);
	}
	/************************************************************************/
}

void MacMgrView::SaveCurrentPage()
{
	/************************************************************************/
	// prepare some data.

	CString strTaskName, strMsgContent;
	strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_MAC);

	HTREEITEM hOldItem = m_macLibTree.GetSelectedItem();
	if (!hOldItem)
		return;

	UINT oldDataVal = m_macLibTree.GetItemData(hOldItem);
	IMachineLibNode* pOldNodeData = (IMachineLibNode*)oldDataVal;
	MACLIB_NODE_TYPE pOldNodeType = pOldNodeData->GetMacLibNodeType();
	/************************************************************************/

	if (SaveModification(hOldItem, FALSE))
	{
		// prepare the message.
		if (pOldNodeType == MACLIB_NODE_MACLIB_ITEM)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAC_SAVED);
		else if (pOldNodeType == MACLIB_NODE_MAC_PARAM)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAC_PARAM_SAVED);
		else if (pOldNodeType == MACLIB_NODE_MAC_COND)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAC_COND_SAVED);
		else if (pOldNodeType == MACLIB_NODE_MAC_NCCONFIG)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NC_TYPE_SAVED);
	}
	else
	{
		// prepare the message.
		if (pOldNodeType == MACLIB_NODE_MACLIB_ITEM)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAC_BASE_NOT_CHANGE);
		else if (pOldNodeType == MACLIB_NODE_MAC_PARAM)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAC_PARAM_NOT_CHANGE);
		else if (pOldNodeType == MACLIB_NODE_MAC_COND)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAC_COND_NOT_CHANGE);
		else if (pOldNodeType == MACLIB_NODE_MAC_NCCONFIG)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NC_TYPE_NOT_CHANGE);
	}

	// write the log.
	LogMessagePtr pLogMsg(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_INFO, strTaskName, strMsgContent));
	LogMgr::WriteLog(pLogMsg);

	// prompt user.
	CString strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	MessageBox(strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
}

BOOL MacMgrView::DocModified()
{
	/************************************************************************/
	// prepare some data.

	CString strTaskName, strMsgContent;

	HTREEITEM hOldItem = m_macLibTree.GetSelectedItem();
	if (!hOldItem)
		return FALSE;

	UINT oldDataVal = m_macLibTree.GetItemData(hOldItem);
	IMachineLibNode* pOldNodeData = (IMachineLibNode*)oldDataVal;
	MACLIB_NODE_TYPE pOldNodeType = pOldNodeData->GetMacLibNodeType();
	/************************************************************************/


	if (pOldNodeType == MACLIB_NODE_ROOT)
	{
		// currently, cannot edit anything in this page.
		return FALSE;
	}
	else if (pOldNodeType == MACLIB_NODE_MACLIB_ITEM)
	{
		// force all edit boxes lose focus.
		m_macInfoPage.SetFocus();

		MachineItemPtr pMacItem = m_macInfoPage.GetMac();
		if (pMacItem->IsModified())
			return TRUE;
	}
	else if (pOldNodeType == MACLIB_NODE_MAC_PARAM)
	{
		// force all edit boxes lose focus.
		m_macInfoPage.SetFocus();

		// get the updated machine.
		MachineParamPtr pMacParam = m_macParamPage.GetMacParam();
		if (pMacParam->IsModified())
			return TRUE;
	}
	else if (pOldNodeType == MACLIB_NODE_MAC_COND)
	{
		// force all edit boxes lose focus.
		m_condEditPage.SetFocus();

		ConditionTypeListPtr pCondTypeList = m_condEditPage.GetMacConds();
		if (pCondTypeList->IsModified())
			return TRUE;
	}
	else if (pOldNodeType == MACLIB_NODE_MAC_NCCONFIG)
	{
		// force all edit boxes lose focus.
		m_NCConfTypeEditPage.SetFocus();

		NCConfigTypeListPtr pNCConfigTypeList = m_NCConfTypeEditPage.GetNCConfigTypeList();
		if (pNCConfigTypeList->IsModified())
			return TRUE;
	}

	return FALSE;
}

BOOL MacMgrView::SaveModification(HTREEITEM hOldItem, BOOL bNeedConfirm)
{
	BOOL bModified = FALSE;

	CString strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	UINT oldDataVal = m_macLibTree.GetItemData(hOldItem);
	IMachineLibNode* pOldNodeData = (IMachineLibNode*)oldDataVal;
	MACLIB_NODE_TYPE pOldNodeType = pOldNodeData->GetMacLibNodeType();
	LONGLONG iMacItemID = pOldNodeData->GetDataID();

	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();

	if (pOldNodeType == MACLIB_NODE_ROOT)
	{
		// currently, cannot edit anything in this page.
		;
	}
	else if (pOldNodeType == MACLIB_NODE_MACLIB_ITEM)
	{
		// force all edit boxes lose focus.
		m_macInfoPage.SetFocus();

		MachineItemPtr pMacItem = m_macInfoPage.GetMac();
		if (pMacItem->IsModified())
		{
			CString strProductName, str;
			strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_MAC_BASE);
			if ((bNeedConfirm && MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				// update database
				pMacItem->SetModifyTime(DateHelper::GetCurrentTime());
				MachineItemWriter::UpdateBasicData(pMacLibDBConnect, pMacItem);

				// update UI.
				m_macLibTree.SetItemText(hOldItem, pMacItem->GetName());
				m_macInfoPage.UpdateModifyDate();
			}
		}
	}
	else if (pOldNodeType == MACLIB_NODE_MAC_PARAM)
	{
		// force all edit boxes lose focus.
		m_macParamPage.SetFocus();

		// get the updated machine.
		MachineParamPtr pMacParam = m_macParamPage.GetMacParam();
		if (pMacParam->IsModified())
		{
			CString str;
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_MAC_PARAM);
			if ((bNeedConfirm && MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				MachineItemWriter::SaveMacParam(pMacLibDBConnect, iMacItemID, pMacParam);
				MachineItemWriter::UpdateModifyDate(pMacLibDBConnect, iMacItemID, DateHelper::GetCurrentTime());
			}
		}
	}
	else if (pOldNodeType == MACLIB_NODE_MAC_COND)
	{
		// force all edit boxes lose focus.
		m_condEditPage.SetFocus();

		ConditionTypeListPtr pCondTypeList = m_condEditPage.GetMacConds();
		if (pCondTypeList->IsModified())
		{
			CString str;
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_MAC_COND);
			if ((bNeedConfirm && MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				MachineItemWriter::SaveMacConds(pMacLibDBConnect, iMacItemID, pCondTypeList);
				MachineItemWriter::UpdateModifyDate(pMacLibDBConnect, iMacItemID, DateHelper::GetCurrentTime());
			}
		}
	}
	else if (pOldNodeType == MACLIB_NODE_MAC_NCCONFIG)
	{
		// force all edit boxes lose focus.
		m_NCConfTypeEditPage.SetFocus();

		NCConfigTypeListPtr pNCConfigTypeList = m_NCConfTypeEditPage.GetNCConfigTypeList();

		// check whether we should save the NC Config type setting.
		if (pNCConfigTypeList->IsModified())
		{
			CString str;
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_NC_TYPE);
			if ((bNeedConfirm && MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				MachineItemWriter::SaveNCConfigTypes(pMacLibDBConnect, iMacItemID, pNCConfigTypeList);
				MachineItemWriter::UpdateModifyDate(pMacLibDBConnect, iMacItemID, DateHelper::GetCurrentTime());
			}
		}
	}

	return bModified;
}

void MacMgrView::CheckModification(HTREEITEM hOldItem)
{
	UINT oldDataVal = m_macLibTree.GetItemData(hOldItem);
	IMachineLibNode* pOldNodeData = (IMachineLibNode*)oldDataVal;
	MACLIB_NODE_TYPE pOldNodeType = pOldNodeData->GetMacLibNodeType();

	if (SaveModification(hOldItem, TRUE))
	{
		CString strTaskName, strMsgContent;
		strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_MAC);
		if (pOldNodeType == MACLIB_NODE_MACLIB_ITEM)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAC_SAVED);
		else if (pOldNodeType == MACLIB_NODE_MAC_PARAM)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAC_PARAM_SAVED);
		else if (pOldNodeType == MACLIB_NODE_MAC_COND)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAC_COND_SAVED);
		else if (pOldNodeType == MACLIB_NODE_MAC_NCCONFIG)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NC_TYPE_SAVED);

		// write the log.
		LogMessagePtr pLogMsg(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_INFO, strTaskName, strMsgContent));
		LogMgr::WriteLog(pLogMsg);
	}
}

void MacMgrView::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item = pNMTreeView->itemNew;
	TVITEM oldItem = pNMTreeView->itemOld;


	/************************************************************************/
	// get the data that is stored in the new and old node.

	// the item data in new node.
	UINT dataVal = m_macLibTree.GetItemData(item.hItem);
	IMachineLibNode* pNodeData = (IMachineLibNode*)dataVal;
	m_macNodeType = pNodeData->GetMacLibNodeType();
	LONGLONG iMacItemID = pNodeData->GetDataID();

	// if "oldItem.hItem" is NULL, we know this is the first time to select a node.
	if (oldItem.hItem)
	{
		// we should check whether we should save the data on the previous page.
		CheckModification(oldItem.hItem);
	}
	/************************************************************************/


	// re-position the sub dialog.
	RepositionSubDlg();

	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	if (m_macNodeType == MACLIB_NODE_ROOT)
	{
		// whether used for internal.
		ProductParamPtr pProductParam = CLOptionInfo::GetInstance()->GetProductParam();
		BOOL bInternal = pProductParam->IsUseInside();

		vector<DataProperty> propList;
		if (bInternal)
			MachineItemLoader::GetMacItemPropList(pMacLibDBConnect, propList, FALSE, FALSE);
		else
			MachineItemLoader::GetMacItemPropList(pMacLibDBConnect, propList);
		m_macListPage.DisplayMacList(propList);
	}
	else if (m_macNodeType == MACLIB_NODE_MACLIB_ITEM)
	{
		MachineItemPtr pMacItem = MachineItemLoader::LoadMacItem(pMacLibDBConnect, iMacItemID);
		m_macInfoPage.DisplayMac(pMacItem);
	}
	else if (m_macNodeType == MACLIB_NODE_MAC_PARAM)
	{
		MachineParamPtr pMacParam = MachineItemLoader::LoadMacParam(pMacLibDBConnect, iMacItemID);
		m_macParamPage.DisplayMacParam(pMacParam);
	}
	else if (m_macNodeType == MACLIB_NODE_MAC_COND)
	{
		ConditionTypeListPtr pCondTypeList = MachineItemLoader::LoadMacConds(pMacLibDBConnect, iMacItemID);
		m_condEditPage.DisplayMacConds(pCondTypeList);
	}
	else if (m_macNodeType == MACLIB_NODE_MAC_NCCONFIG)
	{
		// load the NC config types of the machine.
		NCConfigTypeListPtr pNCConfigTypeList = MachineItemLoader::LoadMacNCConfigType(pMacLibDBConnect, iMacItemID);
		m_NCConfTypeEditPage.DisplayNCConfigTypeList(pNCConfigTypeList);
	}
}

void MacMgrView::OnRClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	/************************************************************************/
	// select tree item which r-button clicked.

	POINT pos;
	if (!GetCursorPos(&pos))
		return;
	m_macLibTree.ScreenToClient(&pos);

	// get the item which was r-clicked, if click on the blank area, quit.
	UINT uFlags;
	HTREEITEM hItem = m_macLibTree.HitTest(pos, &uFlags);
	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
		m_macLibTree.SelectItem(hItem);
	else
		return;

	// the selected node type.
	UINT dataVal = m_macLibTree.GetItemData(hItem);
	IMachineLibNode* pNodeData = (IMachineLibNode*)dataVal;
	MACLIB_NODE_TYPE nodeType = pNodeData->GetMacLibNodeType();
	/************************************************************************/

	/************************************************************************/
	// display the popup menu.

	// figure out the position where the menu will pop up.
	CRect rect;
	this->GetClientRect(&rect);
	ClientToScreen(&rect);
	m_macLibTree.ClientToScreen(&pos);

	// display different popup.
	CString str;
	CMenu menu;
	menu.CreatePopupMenu();
	if (nodeType == MACLIB_NODE_ROOT)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADD_MAC);
		menu.AppendMenu(MF_STRING, IDC_ADD_MAC, str);
	}
	else if (nodeType == MACLIB_NODE_MACLIB_ITEM)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DEL_MAC);
		menu.AppendMenu(MF_STRING, IDC_DEL_MAC, str);
	}
	menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, pos.x, pos.y, this); 
	/************************************************************************/

	*pResult = 0;
}

void MacMgrView::OnCreateMac()
{
	MachineInfoDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		// the new mac item.
		MachineItemPtr pMacItem = dlg.GetMacItem();

		// save the new mac item.
		DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
		MachineItemWriter::SaveMacItem(pMacLibDBConnect, pMacItem, TRUE);

		/************************************************************************/
		// init the node data which will be appended.

		CString str;

		// the mac item node.
		MachineLibNodePtr pMacItemNode(new MachineLibNode(pMacItem->GetName(), MACLIB_NODE_MACLIB_ITEM, pMacItem->GetID()));
		MachineLibNodeListPtr pNodeList(new MachineLibNodeList);
		pMacItemNode->SetSubNodeList(pNodeList);

		// append the mac param node.
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAC_PARAM);
		MachineLibNodePtr pMacParamNode(new MachineLibNode(str, MACLIB_NODE_MAC_PARAM, pMacItem->GetID()));
		pNodeList->push_back(pMacParamNode);

		// append the machine condition node.
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAC_CONDITION);
		MachineLibNodePtr pMacCondNode(new MachineLibNode(str, MACLIB_NODE_MAC_COND, pMacItem->GetID()));
		pNodeList->push_back(pMacCondNode);

		// append the machine nc config node.
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAC_NCCONFIG_TYPE);
		MachineLibNodePtr pMacNCConfigNode(new MachineLibNode(str, MACLIB_NODE_MAC_NCCONFIG, pMacItem->GetID()));
		pNodeList->push_back(pMacNCConfigNode);
		/************************************************************************/

		// append the mac item node.
		HTREEITEM hParentNode = m_macLibTree.GetSelectedItem();
		HTREEITEM hMacItemNode = m_macLibTree.AppendExpLibItemNode(hParentNode, pMacItemNode);
		m_macLibTree.SelectItem(hMacItemNode);
	}
}

void MacMgrView::OnDelMac()
{
	/************************************************************************/
	// prepare some data.

	HTREEITEM hNode = m_macLibTree.GetSelectedItem();
	UINT dataVal = m_macLibTree.GetItemData(hNode);
	IMachineLibNode* pMacNodeData = (IMachineLibNode*)dataVal;
	LONGLONG iMacItemID = pMacNodeData->GetDataID();

	CString str, strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	/************************************************************************/

	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();

	// cannot delete the reserved machine.
	if (MachineItemLoader::GetReserverdMacItem(pMacLibDBConnect) == iMacItemID)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DELETE_RESERVED_MAC);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// cannot delete the machine if we have generated ExpLib from it.
	if (ExpertLibItemLoader::GetExpLibItemID(pExpLibDBCon, iMacItemID) != INVALID_ID)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DELETE_CONFIGED_MAC);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// make sure to delete.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DELETE_MAC);
	if (MessageBox(str, strProductName, MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		MachineItemWriter::DeleteMacItem(pMacLibDBConnect, iMacItemID);
		m_macLibTree.DeleteItem(hNode);
	}
}

END_CUTLEADER_NAMESPACE()

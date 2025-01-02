#include "stdafx.h"
#include "ParamConfigPage.h"

#include "baseConst.h"
#include "DBConnect.h"
#include "DateHelper.h"
#include "clUtilityResource.h"
#include "clUILibResource.h"
#include "LogMessage.h"
#include "LogMgr.h"
#include "DataBaseManager.h"
#include "versionInfo.h"

#include "ConditionTypeList.h"
#include "MachineItemLoader.h"
#include "ToolAssignConfigItem.h"
#include "NCConfigItem.h"
#include "NestConfigItem.h"
#include "clExpertLibResource.h"
#include "PierceConfigItem.h"
#include "SequenceConfigItem.h"
#include "ConfigItemLoader.h"
#include "ConfigItemWriter.h"
#include "ParamConfigLoader.h"
#include "ExpertLibItemWriter.h"
#include "ExpertLibItemLoader.h"
#include "clExpertLibUIResource.h"
#include "clMachineLibResource.h"


#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


BEGIN_CUTLEADER_NAMESPACE()


IMPLEMENT_DYNAMIC(ParamConfigPage, CResizableDialog)

ParamConfigPage::ParamConfigPage(CWnd* pParent)
					: CLResizableDialog(ParamConfigPage::IDD, pParent)
{
	m_expLibNodeType = EXPLIB_NODE_NOT_INIT;
	m_iParamConfigID = INVALID_ID;
}

ParamConfigPage::~ParamConfigPage()
{
}

BEGIN_MESSAGE_MAP(ParamConfigPage, CResizableDialog)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SCHEME_ITEM, OnSelchangedItem)
	ON_WM_SIZE()
END_MESSAGE_MAP()

void ParamConfigPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_SCHEME_ITEM, m_schemeItemList);
}

void ParamConfigPage::InitSubDialog()
{
    // 刀具分配页面。
    m_toolAssignPage.Create(IDD_clExpertLibUI_ToolAssign, this);
    m_toolAssignPage.ShowWindow(SW_HIDE);

	// init Lead in/out setting page.
	m_leadInOutPage.Create(IDD_clExpertLibUI_LeadInOut, this);
	m_leadInOutPage.ShowWindow(SW_HIDE);

	// init "Pierce Location" setting page.
	m_pierceLocPage.Create(IDD_clExpertLibUI_PierceLocation, this);
	m_pierceLocPage.ShowWindow(SW_HIDE);

	// init "micro joint setting" page
	m_microJointAssignmentPage.Create(IDD_clExpertLibUI_MicroJoint_Assignment, this);
	m_microJointAssignmentPage.ShowWindow(SW_HIDE);

	// init "Corner Assignment" page
	m_cornerAssignPage.Create(IDD_clExpertLibUI_Corner, this);
	m_cornerAssignPage.ShowWindow(SW_HIDE);

	// init sequence setting page.
	m_seqSetPage.Create(IDD_clExpertLibUI_Sequence, this);
	m_seqSetPage.ShowWindow(SW_HIDE);

	// init NCConfigItem page.
	m_NCConfigPage.Create(IDD_clExpertLibUI_NC, this);
	m_NCConfigPage.ShowWindow(SW_HIDE);

	// init NestConfigItemPage page.
	m_nestParamPage.Create(IDD_clExpertLibUI_Nest, this);
	m_nestParamPage.ShowWindow(SW_HIDE);
}

void ParamConfigPage::RepositionSubDlg()
{
	// no node selected, of course no page to display.
	if (m_expLibNodeType == EXPLIB_NODE_NOT_INIT)
		return;

	// calculate the position of the child page.
	CWnd* pWndPos = GetDlgItem(IDC_STATIC_CHILDAREA);
	CRect pagePos;
	if (pWndPos)
	{
		pWndPos->GetWindowRect(&pagePos);
		ScreenToClient(&pagePos);
	}

	/************************************************************************/
	// because the layout library will re-layout the controls, so we should re-position the sub dialog.

	HideAllSubDialog();

    if (m_expLibNodeType == EXPLIB_NODE_TOOL_ASSIGN)
    {
        m_toolAssignPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
        m_toolAssignPage.ShowWindow(SW_SHOW);
    }
	else if (m_expLibNodeType == EXPLIB_NODE_LEAD_ASSIGN)
	{
		m_leadInOutPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_leadInOutPage.ShowWindow(SW_SHOW);
	}
	else if (m_expLibNodeType == EXPLIB_NODE_PIERCE_ASSIGN)
	{
		m_pierceLocPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_pierceLocPage.ShowWindow(SW_SHOW);
	}
	else if (m_expLibNodeType == EXPLIB_NODE_CUT_MJ_ASSIGN)
	{
		m_microJointAssignmentPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_microJointAssignmentPage.ShowWindow(SW_SHOW);
	}
	else if (m_expLibNodeType == EXPLIB_NODE_CORNER_ASSIGN)
	{
		m_cornerAssignPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_cornerAssignPage.ShowWindow(SW_SHOW);
	}
	else if (m_expLibNodeType == EXPLIB_NODE_GENERATE_SEQ)
	{
		m_seqSetPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_seqSetPage.ShowWindow(SW_SHOW);
	}
	else if (m_expLibNodeType == EXPLIB_NODE_GENERATE_NC)
	{
		m_NCConfigPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_NCConfigPage.ShowWindow(SW_SHOW);
	}
	else if (m_expLibNodeType == EXPLIB_NODE_NEST_PARAM)
	{
		m_nestParamPage.SetWindowPos(pWndPos, pagePos.left, pagePos.top, pagePos.Width(), pagePos.Height(), 0);
		m_nestParamPage.ShowWindow(SW_SHOW);
	}
	/************************************************************************/
}

BOOL ParamConfigPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// layout the controls.
	AddAnchor(IDC_GROUP_SCHEME_ITEM, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_LIST_SCHEME_ITEM, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_CHILDAREA, TOP_LEFT, BOTTOM_RIGHT);

	// init sub dialogs.
	InitSubDialog();

	// init the image list which will be used in listctrl.
	InitImageList();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_GROUP_SCHEME_ITEM )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH ParamConfigPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ParamConfigPage::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

	// re-position the sub dialog.
	RepositionSubDlg();
}

void ParamConfigPage::OnSelchangedItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int iIndex = -1;

	if (pNMListView->uNewState && (iIndex = m_schemeItemList.GetNextItem(iIndex, LVNI_SELECTED))!= -1)
	{
		// we should check whether we should save the data on the previous page.
		// notes:
		//   1) when call this, "m_expLibNodeType" will be the previous scheme item.
		CheckModification();

		// get the scheme item which was selected.
		if (iIndex == 0)
			m_expLibNodeType = EXPLIB_NODE_TOOL_ASSIGN;
		else if (iIndex == 1)
			m_expLibNodeType = EXPLIB_NODE_LEAD_ASSIGN;
		else if (iIndex == 2)
			m_expLibNodeType = EXPLIB_NODE_PIERCE_ASSIGN;
		else if (iIndex == 3)
			m_expLibNodeType = EXPLIB_NODE_CUT_MJ_ASSIGN;
		else if (iIndex == 4)
			m_expLibNodeType = EXPLIB_NODE_CORNER_ASSIGN;
		else if (iIndex == 5)
			m_expLibNodeType = EXPLIB_NODE_GENERATE_SEQ;
		else if (iIndex == 6)
			m_expLibNodeType = EXPLIB_NODE_GENERATE_NC;
		else if (iIndex == 7)
			m_expLibNodeType = EXPLIB_NODE_NEST_PARAM;

		// re-position the sub dialog.
		RepositionSubDlg();

		DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
		DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();

		// display the data on the different sub dialog.
        if (m_expLibNodeType == EXPLIB_NODE_TOOL_ASSIGN)
        {
			// 加载所有条件类型。
			LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, m_iParamConfigID);
			LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pDBConnect, iExpLibItemID);
			ConditionTypeListPtr pAllCondTypeList = MachineItemLoader::LoadMacConds(pMacLibDBConnect, iMacItemID);

            ToolAssignConfigItemPtr pToolAssignConfigItem = ConfigItemLoader::LoadToolAssignConfigItem(pDBConnect, m_iParamConfigID, pAllCondTypeList);
            m_toolAssignPage.DisplayToolAssignInfo(pToolAssignConfigItem, pAllCondTypeList);
        }
		else if (m_expLibNodeType == EXPLIB_NODE_LEAD_ASSIGN)
		{
			LeadConfigItemPtr pLeadConfigItem = ConfigItemLoader::LoadLeadSetting(pDBConnect, m_iParamConfigID);
			m_leadInOutPage.DisplayLeadSetting(pLeadConfigItem);
			OnPaint();
		}
		else if (m_expLibNodeType == EXPLIB_NODE_PIERCE_ASSIGN)
		{
			// load all mac conditions.
			LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, m_iParamConfigID);
			LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pDBConnect, iExpLibItemID);
			ConditionTypeListPtr pAllCondTypeList = MachineItemLoader::LoadMacConds(pMacLibDBConnect, iMacItemID);

			// query the Pierce Location setting from database and display.
			PierceConfigItemPtr pPierceSetting = ConfigItemLoader::LoadPierceSetting(pDBConnect, m_iParamConfigID);
			m_pierceLocPage.DisplayPierceSetting(pPierceSetting);
		}
		else if (m_expLibNodeType == EXPLIB_NODE_CUT_MJ_ASSIGN)
		{
			MicroJointConfigItemPtr bmjSettingPtr = ConfigItemLoader::LoadMicroJointSetting(pDBConnect, m_iParamConfigID);
			m_microJointAssignmentPage.DisplayMicroJointSetting(bmjSettingPtr);
		}
		else if (m_expLibNodeType == EXPLIB_NODE_CORNER_ASSIGN)
		{
			CornerConfigItemPtr pCorAssignInfo = ConfigItemLoader::LoadCornerConfigItem(pDBConnect, m_iParamConfigID);
			m_cornerAssignPage.DisplayCornerConfigItem(pCorAssignInfo);
			OnPaint();
		}
		else if (m_expLibNodeType == EXPLIB_NODE_GENERATE_SEQ)
		{
			SequenceConfigItemPtr seqSettingPtr = ConfigItemLoader::LoadSeqConfigItem(pDBConnect, m_iParamConfigID);
			m_seqSetPage.DisplaySeqConfigItem(seqSettingPtr);
		}
		else if (m_expLibNodeType == EXPLIB_NODE_GENERATE_NC)
		{
			LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, m_iParamConfigID);
			LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pDBConnect, iExpLibItemID);
			NCConfigTypeListPtr pNCConfigTypeList = MachineItemLoader::LoadMacNCConfigType(pMacLibDBConnect, iMacItemID);

			// load NCConfigItem info from database and display it in dialog.
			NCConfigItemPtr pNCConfigItem = ConfigItemLoader::LoadNCConfigItem(pDBConnect, m_iParamConfigID, pNCConfigTypeList);
			m_NCConfigPage.DisplayNCConfig(pNCConfigItem);
		}
		else if (m_expLibNodeType == EXPLIB_NODE_NEST_PARAM)
		{
			// load the nest param from database and display it.
			NestConfigItemPtr pNestConfigItem = ConfigItemLoader::LoadNestConfigItem(pDBConnect, m_iParamConfigID);
			m_nestParamPage.DisplayNestConfigItem(pNestConfigItem);
		}
	}
}

void ParamConfigPage::DisplayPage(LONGLONG iParamConfigID, int iCurrentIndex)
{
	m_iParamConfigID = iParamConfigID;

	// 初始化列表。
	InitListContent();
	//m_schemeItemList.SetBkColor( SkinManager::GetBackgroundColor() );
	//m_schemeItemList.set
	m_schemeItemList.SetItemState(iCurrentIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}

void ParamConfigPage::InitImageList()
{
	// Create 256 color image lists
	HIMAGELIST hList = ImageList_Create(32,32, ILC_COLOR8 |ILC_MASK , 8, 1);
	m_imageList.Attach(hList);

	// Load the large icons
	CBitmap bmp;
	bmp.LoadBitmap(IDB_EXPLIB_SCHEMEITEM);
	m_imageList.Add(&bmp, RGB(192, 192, 192));
	bmp.DeleteObject();

	// Attach them
	m_schemeItemList.SetImageList(&m_imageList, LVSIL_NORMAL);
}

void ParamConfigPage::InitListContent()
{
	m_schemeItemList.DeleteAllItems();

	CString str;

    // 刀具分配。
    LVITEM toolAssignItem;
    toolAssignItem.mask = LVIF_IMAGE | LVIF_TEXT;
    str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TOOL_ASSIGN);
    toolAssignItem.iItem = 0;
    toolAssignItem.iSubItem = 0;
    toolAssignItem.pszText = (LPTSTR)(LPCTSTR)(str);
    toolAssignItem.iImage = 3;
    m_schemeItemList.InsertItem(&toolAssignItem);

	// lead in/out.
	LVITEM leadItem;
	leadItem.mask =  LVIF_IMAGE | LVIF_TEXT;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LEAD_CONFIG);
	leadItem.iItem = 1;
	leadItem.iSubItem = 0;
	leadItem.pszText = (LPTSTR)(LPCTSTR)(str);
	leadItem.iImage = 0;
	m_schemeItemList.InsertItem(&leadItem);

	// PierceLoc
	LVITEM pierceLocItem;
	pierceLocItem.mask =  LVIF_IMAGE | LVIF_TEXT;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PIERCE_CONFIG);
	pierceLocItem.iItem = 2;
	pierceLocItem.iSubItem = 0;
	pierceLocItem.pszText = (LPTSTR)(LPCTSTR)(str);
	pierceLocItem.iImage = 1;
	m_schemeItemList.InsertItem(&pierceLocItem);

	// micro joint
	LVITEM bmjItem;
	bmjItem.mask =  LVIF_IMAGE | LVIF_TEXT;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MJ_CONFIG);
	bmjItem.iItem = 3;
	bmjItem.iSubItem = 0;
	bmjItem.pszText = (LPTSTR)(LPCTSTR)(str);
	bmjItem.iImage = 2;
	m_schemeItemList.InsertItem(&bmjItem);

	// corner assign
	LVITEM corAssignItem;
	corAssignItem.mask =  LVIF_IMAGE | LVIF_TEXT;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CORNER_CONFIG);
	corAssignItem.iItem = 5;
	corAssignItem.iSubItem = 0;
	corAssignItem.pszText = (LPTSTR)(LPCTSTR)(str);
	corAssignItem.iImage = 4;
	m_schemeItemList.InsertItem(&corAssignItem);

	// Sequence
	LVITEM seqItem;
	seqItem.mask =  LVIF_IMAGE | LVIF_TEXT;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQUENCE_CONFIG);
	seqItem.iItem = 6;
	seqItem.iSubItem = 0;
	seqItem.pszText = (LPTSTR)(LPCTSTR)(str);
	seqItem.iImage = 5;
	m_schemeItemList.InsertItem(&seqItem);

	// NCConfigItem
	LVITEM ncParamConfigItem;
	ncParamConfigItem.mask =  LVIF_IMAGE | LVIF_TEXT;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NC_CONFIG);
	ncParamConfigItem.iItem = 7;
	ncParamConfigItem.iSubItem = 0;
	ncParamConfigItem.pszText = (LPTSTR)(LPCTSTR)(str);
	ncParamConfigItem.iImage = 6;
	m_schemeItemList.InsertItem(&ncParamConfigItem);

	// NestParam
	LVITEM nestItem;
	nestItem.mask =  LVIF_IMAGE | LVIF_TEXT;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEST_PARAM);
	nestItem.iItem = 8;
	nestItem.iSubItem = 0;
	nestItem.pszText = (LPTSTR)(LPCTSTR)(str);
	nestItem.iImage = 7;
	m_schemeItemList.InsertItem(&nestItem);
}

BOOL ParamConfigPage::SaveModification(BOOL bNeedConfirm)
{
	BOOL bModified = FALSE;

	DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();

	if (m_expLibNodeType == EXPLIB_NODE_TOOL_ASSIGN)
	{
        // force all edit boxes lose focus.
        m_toolAssignPage.SetFocus();

        ToolAssignConfigItemPtr pToolAssignConfigItem = m_toolAssignPage.GetToolAssignConfigItem();
        if (pToolAssignConfigItem->IsModified())
        {
            CString str;
            str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_TOOL_CONFIG);
            if ((bNeedConfirm && MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
            {
                bModified = TRUE;

                // get relative ExpertLibItem ID.
                LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pToolAssignConfigItem->GetParentID());

                // save the change.
                pDBConnect->BeginTransaction();
                ConfigItemWriter::SaveToolAssignConfigItem(pDBConnect, pToolAssignConfigItem, FALSE);
                ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
                pDBConnect->CommitTransaction();
            }
        }
	}
	else if (m_expLibNodeType == EXPLIB_NODE_LEAD_ASSIGN)
	{
		// force all edit boxes lose focus.
		m_leadInOutPage.SetFocus();

		LeadConfigItemPtr pLeadConfigItem = m_leadInOutPage.GetLeadSetting();
		if (pLeadConfigItem->IsModified())
		{
			CString str;
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_LEAD);
			if ((bNeedConfirm && MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				// get relative ExpertLibItem ID.
				LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pLeadConfigItem->GetParentID());

				// save the change.
				pDBConnect->BeginTransaction();
				ConfigItemWriter::SaveLeadSetting(pDBConnect, pLeadConfigItem, FALSE);
				ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
				pDBConnect->CommitTransaction();
			}
		}
	}
	else if (m_expLibNodeType == EXPLIB_NODE_PIERCE_ASSIGN)
	{
		// force all edit boxes lose focus.
		m_pierceLocPage.SetFocus();

		PierceConfigItemPtr pSetting = m_pierceLocPage.GetPierceConfigItem();
		if (pSetting->IsModified())
		{
			CString str;
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_PIERCE);
			if ((bNeedConfirm && MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				// get relative ExpertLibItem ID.
				LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pSetting->GetParentID());

				// save the change.
				pDBConnect->BeginTransaction();
				ConfigItemWriter::SavePierceConfigItem(pDBConnect, pSetting, FALSE);
				ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
				pDBConnect->CommitTransaction();
			}
		}
	}
	else if (m_expLibNodeType == EXPLIB_NODE_CUT_MJ_ASSIGN)
	{
		// force all edit boxes lose focus.
		m_microJointAssignmentPage.SetFocus();

		MicroJointConfigItemPtr pBmjSetting = m_microJointAssignmentPage.GetMicroJointSetting();
		if (pBmjSetting->IsModified())
		{
			CString str;
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_MJ_ASSIGN);
			if ((bNeedConfirm && MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				// get relative ExpertLibItem ID.
				LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pBmjSetting->GetParentID());

				// save the change.
				pDBConnect->BeginTransaction();
				ConfigItemWriter::SaveMicroJointConfigItem(pDBConnect, pBmjSetting, FALSE);
				ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
				pDBConnect->CommitTransaction();
			}
		}
	}
	else if (m_expLibNodeType == EXPLIB_NODE_CORNER_ASSIGN)
	{
		// force all edit boxes lose focus.
		m_cornerAssignPage.SetFocus();

		CornerConfigItemPtr pCornerConfigItem = m_cornerAssignPage.GetCornerConfigItem();
		if (pCornerConfigItem->IsModified())
		{
			CString str;
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_CORNER_ASSIGN);
			if ((bNeedConfirm && MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				// get relative ExpertLibItem ID.
				LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pCornerConfigItem->GetParentID());

				// save the change.
				pDBConnect->BeginTransaction();
				ConfigItemWriter::SaveCornerConfigItem(pDBConnect, pCornerConfigItem, FALSE);
				ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
				pDBConnect->CommitTransaction();
			}
		}
	}
	else if (m_expLibNodeType == EXPLIB_NODE_GENERATE_SEQ)
	{
		// force all edit boxes lose focus.
		m_seqSetPage.SetFocus();

		SequenceConfigItemPtr pSeqConfigItem = m_seqSetPage.GetSeqConfigItem();
		if (pSeqConfigItem->IsModified())
		{
			CString str;
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_SEQ_CONFIG);
			if ((bNeedConfirm && MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				// get relative ExpertLibItem ID.
				LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pSeqConfigItem->GetParentID());

				// save the change.
				pDBConnect->BeginTransaction();
				ConfigItemWriter::SaveSeqConfigItem(pDBConnect, pSeqConfigItem, FALSE);
				ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
				pDBConnect->CommitTransaction();
			}
		}
	}
	else if (m_expLibNodeType == EXPLIB_NODE_GENERATE_NC)
	{
		// force all edit boxes lose focus.
		m_NCConfigPage.SetFocus();

		NCConfigItemPtr pNCConfigItem = m_NCConfigPage.GetNCConfig();
		if (pNCConfigItem->IsModified())
		{
			CString str;
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_NC_CONFIG);
			if ((bNeedConfirm && MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				// get relative ExpertLibItem ID.
				LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pNCConfigItem->GetParentID());

				// save the change.
				pDBConnect->BeginTransaction();
				ConfigItemWriter::SaveNCConfigItem(pDBConnect, pNCConfigItem, FALSE);
				ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
				pDBConnect->CommitTransaction();
			}
		}
	}
	else if (m_expLibNodeType == EXPLIB_NODE_NEST_PARAM)
	{
		// force all edit boxes lose focus.
		m_nestParamPage.SetFocus();

		NestConfigItemPtr pNestConfigItem = m_nestParamPage.GetNestConfigItem();
		if (pNestConfigItem->IsModified())
		{
			CString str;
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_NESTPARAM);
			if ((bNeedConfirm && MessageBox(str, NULL, MB_YESNO | MB_ICONQUESTION) == IDYES) || !bNeedConfirm)
			{
				bModified = TRUE;

				// get relative ExpertLibItem ID.
				LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, pNestConfigItem->GetParentID());

				// save the change.
				pDBConnect->BeginTransaction();
				ConfigItemWriter::SaveNestConfigItem(pDBConnect, pNestConfigItem, FALSE);
				ExpertLibItemWriter::UpdateModifyDate(pDBConnect, iExpLibItemID, DateHelper::GetCurrentTime(), FALSE);
				pDBConnect->CommitTransaction();
			}
		}
	}

	return bModified;
}

BOOL ParamConfigPage::DocModified()
{
	if (m_expLibNodeType == EXPLIB_NODE_TOOL_ASSIGN)
	{
		// force all edit boxes lose focus.
		m_toolAssignPage.SetFocus();

		//
		ToolAssignConfigItemPtr pToolAssignConfigItem = m_toolAssignPage.GetToolAssignConfigItem();
		if (pToolAssignConfigItem->IsModified())
		{
			return TRUE;
		}
	}
	else if (m_expLibNodeType == EXPLIB_NODE_LEAD_ASSIGN)
	{
		// force all edit boxes lose focus.
		m_leadInOutPage.SetFocus();

		// get the updated Lead in/out setting.
		LeadConfigItemPtr leadSetting = m_leadInOutPage.GetLeadSetting();

		// check whether we should save the lead in/out setting.
		if (leadSetting->IsModified())
		{
			return TRUE;
		}
	}
	else if (m_expLibNodeType == EXPLIB_NODE_PIERCE_ASSIGN)
	{
		// force all edit boxes lose focus.
		m_pierceLocPage.SetFocus();

		// get the updated PierceLoc setting.
		PierceConfigItemPtr pSetting = m_pierceLocPage.GetPierceConfigItem();

		// check whether we should save the pierce setting.
		if (pSetting->IsModified())
		{
			return TRUE;
		}
	}
	else if (m_expLibNodeType == EXPLIB_NODE_CUT_MJ_ASSIGN)
	{
		// force all edit boxes lose focus.
		m_microJointAssignmentPage.SetFocus();

		// get the updated micro joint setting.
		MicroJointConfigItemPtr pBmjSetting = m_microJointAssignmentPage.GetMicroJointSetting();

		// check whether we should save the micro joint setting.
		if (pBmjSetting->IsModified())
		{
			return TRUE;
		}
	}
	else if (m_expLibNodeType == EXPLIB_NODE_CORNER_ASSIGN)
	{
		// force all edit boxes lose focus.
		m_cornerAssignPage.SetFocus();

		// get the updated corner assign info.
		CornerConfigItemPtr pCornerConfigItem = m_cornerAssignPage.GetCornerConfigItem();

		// check whether we should save the corner assign info.
		if (pCornerConfigItem->IsModified())
		{
			return TRUE;
		}
	}
	// sheet scheme items.
	else if (m_expLibNodeType == EXPLIB_NODE_GENERATE_SEQ)
	{
		// force all edit boxes lose focus.
		m_seqSetPage.SetFocus();

		// get the updated sequence setting.
		SequenceConfigItemPtr pSeqConfigItem = m_seqSetPage.GetSeqConfigItem();

		// check whether we should save the sequence setting.
		if (pSeqConfigItem->IsModified())
		{
			return TRUE;
		}
	}
	else if (m_expLibNodeType == EXPLIB_NODE_GENERATE_NC)
	{
		// force all edit boxes lose focus.
		m_NCConfigPage.SetFocus();

		// get the updated NC setting.
		NCConfigItemPtr pNCConfigItem = m_NCConfigPage.GetNCConfig();

		// check whether we should save the NC setting.
		if (pNCConfigItem->IsModified())
		{
			return TRUE;
		}
	}
	// nest scheme items.
	else if (m_expLibNodeType == EXPLIB_NODE_NEST_PARAM)
	{
		// force all edit boxes lose focus.
		m_nestParamPage.SetFocus();

		// get the updated Nest setting.
		NestConfigItemPtr pNestConfigItem = m_nestParamPage.GetNestConfigItem();

		// check whether we should save the Nest setting.
		if (pNestConfigItem->IsModified())
		{
			return TRUE;
		}
	}

	return FALSE;
}

void ParamConfigPage::CheckModification()
{
	if (SaveModification(TRUE))
	{
		/************************************************************************/
		// write the log message.

		// the message content.
		CString strMsgContent;
		if (m_expLibNodeType == EXPLIB_NODE_LEAD_ASSIGN)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_LEAD_SAVED);
		else if (m_expLibNodeType == EXPLIB_NODE_PIERCE_ASSIGN)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PIERCE_SAVED);
		else if (m_expLibNodeType == EXPLIB_NODE_CUT_MJ_ASSIGN)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MJ_ASSIGN_SAVED);
		else if (m_expLibNodeType == EXPLIB_NODE_CORNER_ASSIGN)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CORNER_SAVED);
		else if (m_expLibNodeType == EXPLIB_NODE_GENERATE_SEQ)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SEQ_CONFIG_SAVED);
		else if (m_expLibNodeType == EXPLIB_NODE_GENERATE_NC)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NC_CONFIG_SAVED);
		else if (m_expLibNodeType == EXPLIB_NODE_NEST_PARAM)
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NESTPARAM_SAVED);

		// write the log.
		CString strTaskName;
		strTaskName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TASK_SAVE_EXPLIB);
		LogMessagePtr pLogMsg(new LogMessage(DateHelper::GetCurrentTime(), LOG_MSG_INFO, strTaskName, strMsgContent));
		LogMgr::WriteLog(pLogMsg);
		/************************************************************************/
	}
}

void ParamConfigPage::HideAllSubDialog()
{
	m_toolAssignPage.ShowWindow(SW_HIDE);
	m_leadInOutPage.ShowWindow(SW_HIDE);
	m_pierceLocPage.ShowWindow(SW_HIDE);
	m_microJointAssignmentPage.ShowWindow(SW_HIDE);
	m_cornerAssignPage.ShowWindow(SW_HIDE);
	m_seqSetPage.ShowWindow(SW_HIDE);
	m_NCConfigPage.ShowWindow(SW_HIDE);
	m_nestParamPage.ShowWindow(SW_HIDE);
}

END_CUTLEADER_NAMESPACE()

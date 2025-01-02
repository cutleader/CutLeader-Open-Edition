#include "stdafx.h"
#include "ChangeLoopToolDlg.h"

#include "MathEx.h"
#include <afxinet.h>
#include <vector>
#include <fstream>
#include "ClWindowFinder.h"
#include "DBConnect.h"
#include "DataBaseManager.h"
#include "LogMgr.h"
#include "DateHelper.h"
#include "LogMgr.h"
#include "LogMessage.h"
#include "StringUtil.h"

#include "ToolInstanceList.h"
#include "ParamConfigLoader.h"
#include "ExpertLibItemLoader.h"
#include "MachineItemLoader.h"
#include "ToolAssignConfigItem.h"
#include "ConfigItemLoader.h"
#include "ToolInstance.h"
#include "ConfigItemWriter.h"
#include "ExpertLibItemWriter.h"
#include "CLOptionInfo.h"
#include "ProductParam.h"
#include "clExpertLibUIResource.h"
#include "clMachineLibResource.h"
#include "MachineParam.h"
#include "clPartLayoutUIResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

// Defined the max tool count, if tool count in expert library is bigger than this, we should warn user.
#define MaxToolCount        10

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ChangeLoopToolDlg, CLResizableDialog)

ChangeLoopToolDlg::ChangeLoopToolDlg() : CLResizableDialog(ChangeLoopToolDlg::IDD)
{
}

ChangeLoopToolDlg::~ChangeLoopToolDlg()
{
}

BEGIN_MESSAGE_MAP(ChangeLoopToolDlg, CLResizableDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_BTN_NoCutLoopInstances, OnNoCutLoopInstances)
	ON_BN_CLICKED(IDC_BTN_CutLoopInstances, OnCutLoopInstances)
	ON_BN_CLICKED(IDC_BTN_Tool_1, OnSetTool_toSelectedLoopInstances)
	ON_BN_CLICKED(IDC_BTN_Tool_2, OnSetTool_toSelectedLoopInstances)
	ON_BN_CLICKED(IDC_BTN_Tool_3, OnSetTool_toSelectedLoopInstances)
	ON_BN_CLICKED(IDC_BTN_Tool_4, OnSetTool_toSelectedLoopInstances)
	ON_BN_CLICKED(IDC_BTN_Tool_5, OnSetTool_toSelectedLoopInstances)
	ON_BN_CLICKED(IDC_BTN_Tool_6, OnSetTool_toSelectedLoopInstances)
	ON_BN_CLICKED(IDC_BTN_Tool_7, OnSetTool_toSelectedLoopInstances)
	ON_BN_CLICKED(IDC_BTN_Tool_8, OnSetTool_toSelectedLoopInstances)
	ON_BN_CLICKED(IDC_BTN_Tool_9, OnSetTool_toSelectedLoopInstances)
	ON_BN_CLICKED(IDC_BTN_Tool_10, OnSetTool_toSelectedLoopInstances)
END_MESSAGE_MAP()

void ChangeLoopToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CLResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_NoCutLoopInstances, m_noCutLoopInstancesButton);
    DDX_Control(pDX, IDC_BTN_CutLoopInstances, m_cutLoopInstancesButton);
	DDX_Control(pDX, IDC_BTN_Tool_1, m_toolButton_1);
	DDX_Control(pDX, IDC_BTN_Tool_2, m_toolButton_2);
	DDX_Control(pDX, IDC_BTN_Tool_3, m_toolButton_3);
	DDX_Control(pDX, IDC_BTN_Tool_4, m_toolButton_4);
	DDX_Control(pDX, IDC_BTN_Tool_5, m_toolButton_5);
	DDX_Control(pDX, IDC_BTN_Tool_6, m_toolButton_6);
	DDX_Control(pDX, IDC_BTN_Tool_7, m_toolButton_7);
	DDX_Control(pDX, IDC_BTN_Tool_8, m_toolButton_8);
	DDX_Control(pDX, IDC_BTN_Tool_9, m_toolButton_9);
	DDX_Control(pDX, IDC_BTN_Tool_10, m_toolButton_10);
}

void ChangeLoopToolDlg::OnDestroy()
{
	CLResizableDialog::OnDestroy();
}

HBRUSH ChangeLoopToolDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ChangeLoopToolDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpdis)
{
	__super::OnDrawItem(nIDCtl, lpdis);
}

BOOL ChangeLoopToolDlg::OnInitDialog()
{
	__super::OnInitDialog();

	USES_CONVERSION;

	// 按钮相关设置。
	m_allToolButton.push_back(&m_toolButton_1);
	m_allToolButton.push_back(&m_toolButton_2);
	m_allToolButton.push_back(&m_toolButton_3);
	m_allToolButton.push_back(&m_toolButton_4);
	m_allToolButton.push_back(&m_toolButton_5);
	m_allToolButton.push_back(&m_toolButton_6);
	m_allToolButton.push_back(&m_toolButton_7);
	m_allToolButton.push_back(&m_toolButton_8);
	m_allToolButton.push_back(&m_toolButton_9);
	m_allToolButton.push_back(&m_toolButton_10);
	m_noCutLoopInstancesButton.SetImage(IDB_NoCutLoop);
	m_noCutLoopInstancesButton.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_DonotCutLoopInstances_Tip));
	m_cutLoopInstancesButton.SetImage(IDB_CutLoop);
	m_cutLoopInstancesButton.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_CutLoopInstances_Tip));

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

void ChangeLoopToolDlg::OnSize(UINT nType, int cx, int cy) 
{
	__super::OnSize(nType, cx, cy);
}

void ChangeLoopToolDlg::OnNoCutLoopInstances()
{
	CView* pSheetView = ClWindowFinder::GetCurrentView_1(); // get current sheet view.
	if (pSheetView)
	{
		// send message to sheet view.
		pSheetView->SendMessage(WM_TellSheetView_toChangeToolOfSelectedLoopInstances, -1, 0);
	}
}

void ChangeLoopToolDlg::OnCutLoopInstances()
{
    // get current sheet view.
    CView* pSheetView = 0;
    {
        CMDIFrameWnd* pMainFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
        CMDIChildWnd* pChildFrame = (CMDIChildWnd*)pMainFrame->GetActiveFrame();
        if (!pChildFrame)
        {
            LogMgr::DebugWarn(_T("645141"));
            return;
        }
        pSheetView = pChildFrame->GetActiveView();
        if (!pSheetView)
        {
            LogMgr::DebugWarn(_T("369717"));
            return;
        }
    }

    // send message to sheet view.
    pSheetView->SendMessage(WM_TellSheetView_toChangeToolOfSelectedLoopInstances, -2, 0);
}

void ChangeLoopToolDlg::OnSetTool_toSelectedLoopInstances()
{
	WORD iToolButtonID = LOWORD(GetCurrentMessage()->wParam);

	// get the tool index.
	int iToolIndex = -1;
	for (unsigned int i = 0; i < m_allToolButton.size(); i++)
	{
		CMFCButton* pToolButton = m_allToolButton[i];
		if (pToolButton->GetDlgCtrlID() == iToolButtonID)
		{
			iToolIndex = i;
			break;
		}
	}
	if (iToolIndex == -1)
	{
		LogMgr::DebugWarn(_T("45897"));
		return;
	}

	// get current sheet view.
	CView* pSheetView = 0;
	{
		CMDIFrameWnd* pMainFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
		CMDIChildWnd* pChildFrame = (CMDIChildWnd*)pMainFrame->GetActiveFrame();
		if (!pChildFrame)
		{
			LogMgr::DebugWarn(_T("21356"));
			return;
		}
		pSheetView = pChildFrame->GetActiveView();
		if (!pSheetView)
		{
			LogMgr::DebugWarn(_T("41001"));
			return;
		}
	}

	// send message to sheet view.
	pSheetView->SendMessage(WM_TellSheetView_toChangeToolOfSelectedLoopInstances, iToolIndex, 0);
}

void ChangeLoopToolDlg::Init(LONGLONG iParamConfigIDOfCurrentSheet)
{
	m_iParamConfigIDOfCurrentSheet = iParamConfigIDOfCurrentSheet;

	// get all tools in current expert library.
	ToolInstanceListPtr pToolInstanceList;

	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	DBConnectPtr pDBConnect = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pDBConnect, m_iParamConfigIDOfCurrentSheet);
	LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pDBConnect, iExpLibItemID);
	ConditionTypeListPtr pAllCondTypeList = MachineItemLoader::LoadMacConds(pMacLibDBConnect, iMacItemID);
	ToolAssignConfigItemPtr pToolAssignConfigItem = ConfigItemLoader::LoadToolAssignConfigItem(pDBConnect, m_iParamConfigIDOfCurrentSheet, pAllCondTypeList);
	pToolInstanceList = pToolAssignConfigItem->GetToolInstanceList();

	// whether the tool count is bigger than MaxToolCount.
	if (pToolInstanceList->size() > MaxToolCount)
	{
		LogMgr::DebugWarn(_T("643203"));
		return;
	}

	// hide some tool buttons.
	for (unsigned int i = 0; i < MaxToolCount; i++)
	{
		if (i < pToolInstanceList->size())
			m_allToolButton[i]->ShowWindow(SW_SHOW);
		else
			m_allToolButton[i]->ShowWindow(SW_HIDE);
	}

	// init each tool button.
	for (unsigned int i = 0; i < pToolInstanceList->size(); i++)
	{
		const ToolInstance* pToolInstance = pToolInstanceList->at(i).get();
		m_allToolButton[i]->m_bTransparent = FALSE;
		m_allToolButton[i]->SetFaceColor(pToolInstance->GetToolColor());
	}
}

END_CUTLEADER_NAMESPACE()

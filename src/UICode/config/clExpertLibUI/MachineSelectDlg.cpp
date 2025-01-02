#include "StdAfx.h"
#include "MachineSelectDlg.h"

#include "baseConst.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"

#include "clMachineLibResource.h"
#include "MachineItemLoader.h"

#include "clExpertLibResource.h"
#include "ExpertLibItemLoader.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(MachineSelectDlg, CDialogEx)

MachineSelectDlg::MachineSelectDlg()
				: CLDialog(MachineSelectDlg::IDD)
{
	m_strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAC_CONFIGED);
}

MachineSelectDlg::~MachineSelectDlg(void)
{
}

void MachineSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MSG, m_strMsg);
	DDX_Control(pDX, IDC_COMBO_MAC, m_ctrlMacList);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BEGIN_MESSAGE_MAP(MachineSelectDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)

	ON_WM_CTLCOLOR()

END_MESSAGE_MAP()

BOOL MachineSelectDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// init the dialog.
	Init();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_DEPEND_TREE )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

void MachineSelectDlg::OnDestroy()
{
	ReleaseComboItemData();
	CDialogEx::OnDestroy();
}

void MachineSelectDlg::OnBnClickedOk()
{
	int iIndex = m_ctrlMacList.GetCurSel();
	if (iIndex == -1)
	{
		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAC_SELECT);
		MessageBox(str, NULL, MB_OK | MB_ICONWARNING);
	}
	else
	{
		// check whether we have generated the expert library item for this machine.
		LONGLONG* pVal = (LONGLONG*)m_ctrlMacList.GetItemData(iIndex);
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		if (ExpertLibItemLoader::GetExpLibItemID(pExpLibDBCon, *pVal) != INVALID_ID)
		{
			CString str;
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MAC_HAS_CONFIGED);
			MessageBox(str, NULL, MB_OK | MB_ICONWARNING);
		}
		else
		{
			m_iMacItemID = *pVal;
			OnOK();
		}
	}
}

HBRUSH MachineSelectDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void MachineSelectDlg::Init()
{
	// load all macItem props.
	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	if (MachineItemLoader::GetMacItemPropList(pMacLibDBConnect, m_macItemPropList))
	{
		// set the style of the combo box.
		m_ctrlMacList.SetColumnCount(1);
		m_ctrlMacList.SetColumnWidth(0, 200);

		// add each item into combo box.
		for (unsigned int i = 0; i < m_macItemPropList.size(); i++)
		{
			DataProperty macItemProp = m_macItemPropList.at(i);
			m_ctrlMacList.AddString(macItemProp.GetItem(1).GetValue().bstrVal);

			// if we have generated the expert library item for this machine, set the color of the item as red.
			LONGLONG iMacItemID = macItemProp.GetItem(0).GetValue().llVal;
			if (ExpertLibItemLoader::GetExpLibItemID(pExpLibDBCon, iMacItemID) != INVALID_ID)
				m_ctrlMacList.SetItemColor(i, RGB(255,0,0));

			m_ctrlMacList.SetItemData(i, (DWORD_PTR)(new LONGLONG(iMacItemID)));
		}
	}
}

void MachineSelectDlg::ReleaseComboItemData()
{
	for (int i = 0; i < m_ctrlMacList.GetCount(); i++)
	{
		DWORD_PTR pItemData = m_ctrlMacList.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}
}

END_CUTLEADER_NAMESPACE()

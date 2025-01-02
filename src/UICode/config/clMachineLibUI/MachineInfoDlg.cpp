#include "stdafx.h"
#include "MachineInfoDlg.h"

#include "baseConst.h"
#include "DateHelper.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"

#include "clMachineLibResource.h"
#include "MachineItem.h"
#include "MachineItemLoader.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(MachineInfoDlg, CDialogEx)

BEGIN_MESSAGE_MAP(MachineInfoDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

void MachineInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);

	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

MachineInfoDlg::MachineInfoDlg(CWnd* pParent /*=NULL*/)
						 : CLDialog(MachineInfoDlg::IDD, pParent)
{
	m_strName = "New Machine";
}

MachineInfoDlg::~MachineInfoDlg()
{
}

BOOL MachineInfoDlg::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_MACHINE_INFO )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

HBRUSH MachineInfoDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void MachineInfoDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	// load the reserved mac item.
	DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
	LONGLONG iMacItemID = MachineItemLoader::GetReserverdMacItem(pMacLibDBConnect);
	MachineItemPtr pMacItem = MachineItemLoader::LoadMacItem(pMacLibDBConnect, iMacItemID);

	// clone it.
	IDataPtr pData = pMacItem->Clone();
	m_pMacItem = boost::dynamic_pointer_cast<MachineItem>(pData);

	// update some data.
	m_pMacItem->SetName(m_strName);
	m_pMacItem->SetComment(m_strComment);
	m_pMacItem->SetCreateTime(DateHelper::GetCurrentTime());
	m_pMacItem->SetModifyTime(DateHelper::GetCurrentTime());
	m_pMacItem->IsReserved(FALSE);
	m_pMacItem->IsVisible(TRUE);

	OnOK();
}

END_CUTLEADER_NAMESPACE()

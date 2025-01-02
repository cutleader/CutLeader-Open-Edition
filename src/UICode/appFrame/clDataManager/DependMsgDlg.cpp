#include "stdafx.h"
#include "DependMsgDlg.h"

#include "baseConst.h"
#include "clUtilityResource.h"
#include "clUILibResource.h"

#include "DependNode.h"
#include "DependNodeList.h"
#include "DependViewDlg.h"
#include "ClDataManagerResource.h"


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(DependMsgDlg, CDialog)

BEGIN_MESSAGE_MAP(DependMsgDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OK, OnOk)
	ON_BN_CLICKED(IDC_BTN_VIEW_DEPEND, OnViewDetail)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

DependMsgDlg::DependMsgDlg(DependNodeListPtr pDependNodeList, CWnd* pParent /*=NULL*/)
	: CLDialog(DependMsgDlg::IDD, pParent)
{
	m_strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DependedByOtherData);
	m_pDependNodeList = pDependNodeList;
}

DependMsgDlg::~DependMsgDlg()
{
}

void DependMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MSG, m_strMsg);
	DDX_Control( pDX, IDC_BTN_OK, m_btnOk);
	DDX_Control( pDX, IDC_BTN_VIEW_DEPEND, m_btnViewDepend );
}

BOOL DependMsgDlg::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	SetWindowTheme( GetDlgItem( IDC_EDIT_MSG )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

void DependMsgDlg::OnViewDetail() 
{
	DependViewDlg dlg;
	dlg.SetDependNodeList(m_pDependNodeList);
	dlg.DoModal();
}

void DependMsgDlg::OnOk()
{
	OnOK();
}

HBRUSH DependMsgDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

END_CUTLEADER_NAMESPACE()

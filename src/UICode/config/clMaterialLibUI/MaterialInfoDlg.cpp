#include "stdafx.h"
#include "MaterialInfoDlg.h"

#include "DateHelper.h"
#include "baseConst.h"

#include "Material.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(MaterialInfoDlg, CDialog)

MaterialInfoDlg::MaterialInfoDlg(CWnd* pParent /*=NULL*/)
								: CLDialog(MaterialInfoDlg::IDD, pParent)
{
	m_pMat.reset();
}

MaterialInfoDlg::~MaterialInfoDlg()
{
}

void MaterialInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MATTYPE, m_strName);
	DDX_Control(pDX, IDC_EDIT_GRAVITY, m_dGravity);
	DDX_Text(pDX, IDC_MATTYPE_COMMENT, m_strComment);
	
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(MaterialInfoDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL MaterialInfoDlg::OnInitDialog()
{
	__super::OnInitDialog();

	m_strName = _T("New Material");
	m_dGravity.Init(1.0, DIGITAL_NUM, FALSE);

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	return TRUE;
}

HBRUSH MaterialInfoDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void MaterialInfoDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	/************************************************************************/
	// create the new material type.

	m_pMat.reset(new Material());
	m_pMat->SetName(m_strName);
	m_pMat->SetGravity(m_dGravity.Get_Number());
	m_pMat->SetComment(m_strComment);

	//
	m_pMat->SetCreateTime(DateHelper::GetCurrentTime());
	m_pMat->SetModifyTime(DateHelper::GetCurrentTime());
	/************************************************************************/

	OnOK();
}

END_CUTLEADER_NAMESPACE()

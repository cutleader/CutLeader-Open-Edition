#include "stdafx.h"
#include "MaterialSizeInfoDlg.h"

#include "baseConst.h"
#include "clMaterialLibResource.h"
#include "RectMaterial.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(MaterialSizeInfoDlg, CDialog)

MaterialSizeInfoDlg::MaterialSizeInfoDlg(CWnd* pParent /*=NULL*/)
								: CLDialog(MaterialSizeInfoDlg::IDD, pParent)
{
	m_pMatSize.reset();
}

MaterialSizeInfoDlg::~MaterialSizeInfoDlg()
{
}

void MaterialSizeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_WIDTH, m_ctrlMatWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_ctrlMatHeight);
	DDX_Control(pDX, IDC_EDIT_MAT_TOTAL, m_ctrlMatCount);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
	
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(MaterialSizeInfoDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL MaterialSizeInfoDlg::OnInitDialog()
{
	__super::OnInitDialog();

	m_ctrlMatWidth.Init(50.0, DIGITAL_NUM, FALSE);
	m_ctrlMatHeight.Init(50.0, DIGITAL_NUM, FALSE);
	m_ctrlMatCount.Init(100, 0, FALSE);

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	return TRUE;
}

HBRUSH MaterialSizeInfoDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void MaterialSizeInfoDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	// create the MatSize object.
	m_pMatSize.reset(new RectMaterial());
	CString strName;
	strName.Format(_T("%dx%d"), (int)m_ctrlMatWidth.Get_Number(), (int)m_ctrlMatHeight.Get_Number());
	m_pMatSize->SetName(strName);
	m_pMatSize->SetWidth(m_ctrlMatWidth.Get_Number());
	m_pMatSize->SetHeight(m_ctrlMatHeight.Get_Number());
	m_pMatSize->SetTotalNum((int)m_ctrlMatCount.Get_Number());
	m_pMatSize->SetAvailNum((int)m_ctrlMatCount.Get_Number());
	m_pMatSize->SetComment(m_strComment);

	OnOK();
}

END_CUTLEADER_NAMESPACE()

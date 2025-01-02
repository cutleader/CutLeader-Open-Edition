#include "StdAfx.h"
#include "CreatePartFromGeometryDlg.h"

#include "baseConst.h"
#include "DataBaseManager.h"
#include "SelectDataCenterFolderDlg.h"
#include "DataCenterItemLoader.h"
#include "FolderNode.h"
#include "SkinManager.h"
#include "LogMgr.h"

#include "clPartResource.h"
#include "PatternList.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(CreatePartFromGeometryDlg, CDialogEx)

CreatePartFromGeometryDlg::CreatePartFromGeometryDlg(const PatternList* pPatterns, CWnd* pParent)
	: CLDialog(CreatePartFromGeometryDlg::IDD, pParent), m_pPatterns(pPatterns)
{
	m_strPartName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEW_PART);
}

CreatePartFromGeometryDlg::~CreatePartFromGeometryDlg(void)
{
}

BEGIN_MESSAGE_MAP(CreatePartFromGeometryDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_SAVE_PATH, OnSelectFolder)
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

void CreatePartFromGeometryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_PART_NAME, m_strPartName);
	DDX_Text(pDX, IDC_EDIT_SAVE_PATH, m_ctrlSavePath);
	DDX_Control(pDX, IDC_EDIT_PART_COUNT, m_ctrlPartNum);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_patternPreviewWnd);

	DDX_Control(pDX, IDC_BTN_SAVE_PATH, m_btnSavePath);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL CreatePartFromGeometryDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// 初始化保存目录。
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	if (IDataCenterItemPtr pDataCenterItem = DataCenterItemLoader::LoadProductTree(pMainDBCon)->SearchDataCenterItem(PartsLibraryFolderID)) {
		m_pSaveFolder = boost::dynamic_pointer_cast<FolderNode>(pDataCenterItem);
	}
	else {
		LogMgr::DebugWarn(_T("746321"));
		return FALSE;
	}
	m_ctrlSavePath = m_pSaveFolder->GetFullPath();

	// 零件数量。
	m_ctrlPartNum.Init(10, 0, false);

	UpdateData(FALSE);

	// 初始化视口。
	Rect2D geomRect(.0, 10.0, .0, 10.0);
	m_patternPreviewWnd.InitViewPort(geomRect);

	SetWindowTheme(GetDlgItem(IDC_GROUP_Preview)->GetSafeHwnd(), L"", L"");
	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	return TRUE;
}

void CreatePartFromGeometryDlg::OnDestroy()
{
	// 释放视口。
	m_patternPreviewWnd.ReleaseViewPort();

	CLDialog::OnDestroy();
}

void CreatePartFromGeometryDlg::OnPaint()
{
	CLDialog::OnPaint();

	m_patternPreviewWnd.FitRect(m_pPatterns->GetRectBox());
	m_patternPreviewWnd.SetSolidGeom2DList(m_pPatterns->Calculate2DLineArcs());
	m_patternPreviewWnd.OnPaint();
}

void CreatePartFromGeometryDlg::OnSelectFolder()
{
	SelectDataCenterFolderDlg dlg(this);
	if (dlg.DoModal() == IDOK) {
		m_pSaveFolder = dlg.GetSelectFolder();
		m_ctrlSavePath = m_pSaveFolder->GetFullPath();
		GetDlgItem(IDC_EDIT_SAVE_PATH)->SetWindowText(m_ctrlSavePath);
	}
}

void CreatePartFromGeometryDlg::OnOK()
{
	UpdateData(TRUE);
	CDialogEx::OnOK();
}

HBRUSH CreatePartFromGeometryDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "ImportDxfDwg2NestTaskDlg.h"

#include "baseConst.h"
#include "LineArc2DList.h"
#include "PathHelper.h"
#include "DataBaseManager.h"
#include "IntegerList.h"
#include "LogMgr.h"
#include "FolderNode.h"
#include "SelectDataCenterFolderDlg.h"
#include "DataCenterItemLoader.h"

#include "CLOptionInfo.h"
#include "PartEditParam.h"

#include "DxfData.h"
#include "DxfDwgConvertor.h"
#include "ClDataImportExportResource.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(ImportDxfDwg2NestTaskDlg, CFileDialog)

ImportDxfDwg2NestTaskDlg::ImportDxfDwg2NestTaskDlg(CWnd* pParentWnd) :
    CFileDialog(TRUE, _T(""), NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT | OFN_ENABLESIZING,
		MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPORT_DXFDWG_FILEEXT), pParentWnd, 0, FALSE)
{
	SetTemplate(0, IDD_clNestUI_ImportDxfDwg2NestTask);
}

ImportDxfDwg2NestTaskDlg::~ImportDxfDwg2NestTaskDlg()
{
}

void ImportDxfDwg2NestTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_SAVE_PATH, m_ctrlSavePath);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_ctrlPreviewWnd);
}

BEGIN_MESSAGE_MAP(ImportDxfDwg2NestTaskDlg, CFileDialog)
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_BTN_SAVE_PATH, OnSelectFolder)
END_MESSAGE_MAP()

BOOL ImportDxfDwg2NestTaskDlg::OnInitDialog()
{
	CFileDialog::OnInitDialog();

	// 翻译
	MultiLanguageMgr::GetInstance()->TranslateDialog(this);

	// 初始化视口。
	Rect2D geomRect(.0, 10.0, .0, 10.0);
	m_ctrlPreviewWnd.InitViewPort(geomRect);

	// 初始化保存目录。
    DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
    if (IDataCenterItemPtr pDataCenterItem = DataCenterItemLoader::LoadProductTree(pMainDBCon)->SearchDataCenterItem(PartsLibraryFolderID))
    {
        m_pSaveFolder = boost::dynamic_pointer_cast<FolderNode>(pDataCenterItem);
    }
    else
    {
        LogMgr::DebugWarn(_T("01231"));
    }
	m_ctrlSavePath = m_pSaveFolder->GetName();

	UpdateData(FALSE);

	return TRUE;
}

void ImportDxfDwg2NestTaskDlg::OnDestroy()
{
	// 释放视口。
	m_ctrlPreviewWnd.ReleaseViewPort();
}

void ImportDxfDwg2NestTaskDlg::OnSelectFolder()
{
	SelectDataCenterFolderDlg dlg(this);
    dlg.SetInitialFolderNode(m_pSaveFolder->GetID());
	if (dlg.DoModal() == IDOK)
	{
		m_pSaveFolder = dlg.GetSelectFolder();

		// get the path of the folder.
		CString strPath = m_pSaveFolder->GetItemPath();
		strPath += _T("\\");
		strPath += m_pSaveFolder->GetName();

		m_ctrlSavePath = strPath;
		UpdateData(FALSE);
	}
}

void ImportDxfDwg2NestTaskDlg::OnFileNameChange()
{
	const CString& strDefaultFontFileName = CLOptionInfo::GetInstance()->GetPartEditParam()->GetDefaultFontFileName();

	// get the basic geoms of the selected file.
	LineArc2DListPtr pLineArc2DList;
    CString strFilePath = GetPathName();
	if (PathHelper::GetFileExt(strFilePath).CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DXF_EXT)) == 0)
    {
        DxfDataPtr pDxfData = DxfDwgConvertor::ExtractPatList(strFilePath, FALSE, strDefaultFontFileName);
        pLineArc2DList = pDxfData->Calculate2DLineArcs();
    }
	else if (PathHelper::GetFileExt(strFilePath).CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DWG_EXT)) == 0)
    {
        DxfDataPtr pDxfData = DxfDwgConvertor::ExtractPatList(strFilePath, TRUE, strDefaultFontFileName);
        pLineArc2DList = pDxfData->Calculate2DLineArcs();
    }
    else // 到了这里应该就是多选了
    {
        return;
    }

	// 预览
	if (pLineArc2DList && pLineArc2DList->size() > 0)
	{
		// 初始化视口。
		Rect2D geomRect = pLineArc2DList->GetRectBox();
		m_ctrlPreviewWnd.FitRect(geomRect);
		m_ctrlPreviewWnd.SetSolidGeom2DList(pLineArc2DList);
		m_ctrlPreviewWnd.Invalidate();

		// 显示统计数据。
		CString str;
		str.Format(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPORT_DxfDwgInfo), pLineArc2DList->size(), geomRect.GetWidth(), geomRect.GetHeight());
		GetDlgItem(IDC_STATIC_DxfInfo)->SetWindowText(str);
	}

	CFileDialog::OnFileNameChange();
}

BOOL ImportDxfDwg2NestTaskDlg::OnFileNameOK()
{
	return CFileDialog::OnFileNameOK();
}

END_CUTLEADER_NAMESPACE()

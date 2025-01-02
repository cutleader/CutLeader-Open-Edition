#include "StdAfx.h"
#include "ImportDxfDwg2CanvasDlg.h"

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
#include "clBaseDataResource.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(ImportDxfDwg2CanvasDlg, CFileDialog)

ImportDxfDwg2CanvasDlg::ImportDxfDwg2CanvasDlg(CWnd* pParentWnd)
	: CFileDialog(TRUE, _T(""), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ENABLESIZING,
		MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPORT_DXFDWG_FILEEXT), pParentWnd, 0, FALSE)
{
	SetTemplate(0, IDD_clCanvasGeometryUI_ImportDxfDwg2Canvas);
	m_bInvalidDxfDwgFile = TRUE;
}

ImportDxfDwg2CanvasDlg::~ImportDxfDwg2CanvasDlg()
{
}

void ImportDxfDwg2CanvasDlg::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_ctrlPreviewWnd);
}

BEGIN_MESSAGE_MAP(ImportDxfDwg2CanvasDlg, CFileDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL ImportDxfDwg2CanvasDlg::OnInitDialog()
{
	CFileDialog::OnInitDialog();

	// 翻译
	MultiLanguageMgr::GetInstance()->TranslateDialog(this);

	// 初始化视口。
	Rect2D geomRect(.0, 10.0, .0, 10.0);
	m_ctrlPreviewWnd.InitViewPort(geomRect);

	UpdateData(FALSE);

	return TRUE;
}

void ImportDxfDwg2CanvasDlg::OnDestroy()
{
	// 释放视口。
	m_ctrlPreviewWnd.ReleaseViewPort();
}

void ImportDxfDwg2CanvasDlg::OnFileNameChange()
{
	const CString& strDefaultFontFileName = CLOptionInfo::GetInstance()->GetPartEditParam()->GetDefaultFontFileName();

	// get the basic geoms of the selected file.
	LineArc2DListPtr pLineArc2DList;
	CString strFilePath = GetPathName();
	if (PathHelper::GetFileExt(strFilePath).CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DXF_EXT)) == 0) {
		DxfDataPtr pDxfData = DxfDwgConvertor::ExtractPatList(strFilePath, FALSE, strDefaultFontFileName);
		pLineArc2DList = pDxfData->Calculate2DLineArcs();
	}
	else if (PathHelper::GetFileExt(strFilePath).CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DWG_EXT)) == 0) {
		DxfDataPtr pDxfData = DxfDwgConvertor::ExtractPatList(strFilePath, TRUE, strDefaultFontFileName);
		pLineArc2DList = pDxfData->Calculate2DLineArcs();
	}
	else { // 到了这里应该就是多选了
		return;
	}

	// 预览
	if ( pLineArc2DList )
	{
		// 初始化视口。
		Rect2D geomRect = pLineArc2DList->GetRectBox();
		m_ctrlPreviewWnd.FitRect( geomRect );
		m_ctrlPreviewWnd.SetSolidGeom2DList( pLineArc2DList ); // 设置图形。
		m_ctrlPreviewWnd.Invalidate(); // 绘制。
		if ( pLineArc2DList->size() > 0 ) {
			m_bInvalidDxfDwgFile = FALSE;
		}
		else {
			m_bInvalidDxfDwgFile = TRUE;
			DisplayInvalidDxfDwgFileHint();
		}

		// 显示统计数据。
		CString str;
		str.Format(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPORT_DxfDwgInfo), pLineArc2DList->size(), geomRect.GetWidth(), geomRect.GetHeight());
		GetDlgItem(IDC_STATIC_DxfInfo)->SetWindowText(str);
	}

	CFileDialog::OnFileNameChange();
}

BOOL ImportDxfDwg2CanvasDlg::OnFileNameOK()
{
	if( m_bInvalidDxfDwgFile ) {
		DisplayInvalidDxfDwgFileHint();
		return TRUE;
	}
	return CFileDialog::OnFileNameOK();
}

void ImportDxfDwg2CanvasDlg::DisplayInvalidDxfDwgFileHint()
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString( IDS_PRODUCT_NAME );
	MessageBox( MultiLanguageMgr::GetInstance()->TranslateString( IDS_IMP_NOT_SUPPORT_VERSION ), strProductName, MB_OK | MB_ICONWARNING );
}

END_CUTLEADER_NAMESPACE()

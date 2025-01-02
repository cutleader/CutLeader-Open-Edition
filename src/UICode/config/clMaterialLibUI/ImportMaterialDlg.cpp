#include "StdAfx.h"
#include "ImportMaterialDlg.h"

#include "baseConst.h"
#include "LineArc2DList.h"
#include "PathHelper.h"

#include "DxfData.h"
#include "DxfDwgConvertor.h"
#include "CLOptionInfo.h"
#include "PartEditParam.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(ImportMaterialDlg, CFileDialog)

ImportMaterialDlg::ImportMaterialDlg(DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
	CFileDialog(TRUE, _T(""), NULL, dwFlags, _T("DXF/DWG Files (*.dxf;*.dwg)|*.dxf;*.dwg|"), pParentWnd, 0, FALSE)
{
}

ImportMaterialDlg::~ImportMaterialDlg()
{
}

BEGIN_MESSAGE_MAP(ImportMaterialDlg, CFileDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL ImportMaterialDlg::OnInitDialog()
{
	CFileDialog::OnInitDialog();

	// 创建预览窗口。
	CRect rect;
	CWnd* parent = GetParent();
	parent->GetWindowRect(&rect);
	parent->SetWindowPos(0, rect.left, rect.top, rect.Width()+200, rect.Height(), 0);
	ScreenToClient(rect);
	CRect staticWinRect(rect.Width(), rect.top+65, rect.Width()+200-20, rect.Height()-100);
	m_previewStatic.Create(NULL, WS_CHILD|WS_VISIBLE|WS_BORDER|WS_BORDER, staticWinRect, parent);

	// 初始化视口。
	Rect2D geomRect(.0, 10.0, .0, 10.0);
	m_previewStatic.InitViewPort(geomRect);

	return TRUE;
}

void ImportMaterialDlg::OnFileNameChange()
{
	const CString& strDefaultFontFileName = CLOptionInfo::GetInstance()->GetPartEditParam()->GetDefaultFontFileName();

	// 从dxf/dwg加载图形。
	DxfDataPtr pDxfData;
	CString strFilePath = GetPathName();
	if (PathHelper::IsDxfFile(strFilePath))
		pDxfData = DxfDwgConvertor::ExtractPatList(strFilePath, FALSE, strDefaultFontFileName);
	else if (PathHelper::IsDwgFile(strFilePath))
		pDxfData = DxfDwgConvertor::ExtractPatList(strFilePath, TRUE, strDefaultFontFileName);
	else
		return;
	LineArc2DListPtr pGeo2DList = pDxfData->Calculate2DLineArcs();

	// preview line/arcs.
	if (pGeo2DList && pGeo2DList->size() > 0)
	{
		// 调整视口以适合矩形框。
		Rect2D geomRect = pGeo2DList->GetRectBox();
		m_previewStatic.FitRect(geomRect);

		// 设置图形。
		m_previewStatic.SetSolidGeom2DList(pGeo2DList);

		// 绘制。
		m_previewStatic.Invalidate();
	}

	CFileDialog::OnFileNameChange();
}

void ImportMaterialDlg::OnDestroy()
{
	// 释放视口。
	m_previewStatic.ReleaseViewPort();

	CFileDialog::OnDestroy();
}

BOOL ImportMaterialDlg::OnFileNameOK()
{
	return CFileDialog::OnFileNameOK();
}

END_CUTLEADER_NAMESPACE()

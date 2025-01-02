#include "StdAfx.h"
#include "ImportClPart2DataCenterDlg.h"

#include "baseConst.h"
#include "LineArc2DList.h"
#include "PathHelper.h"
#include "DataBaseManager.h"
#include "IntegerList.h"
#include "LogMgr.h"
#include "StringUtil.h"
#include "FolderNode.h"
#include "SelectDataCenterFolderDlg.h"
#include "DataCenterItemLoader.h"

#include "ParamConfigLoader.h"
#include "ExpertLibItemLoader.h"
#include "MachineItemLoader.h"
#include "DefaultParamConfig.h"

#include "DxfData.h"
#include "DxfDwgConvertor.h"
#include "ClDataImportExportResource.h"
#include "ImportManager.h"

using namespace std;


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(ImportClPart2DataCenterDlg, CFileDialog)

ImportClPart2DataCenterDlg::ImportClPart2DataCenterDlg(CWnd* pParentWnd) :
    CFileDialog(TRUE, _T(""), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,
        MultiLanguageMgr::GetInstance()->TranslateString(IDS_ImportClPart_FileExt), pParentWnd, 0, FALSE)
{
	SetTemplate(0, IDD_clDataImportExportUI_ImportClPart2DataCenter);
}

ImportClPart2DataCenterDlg::~ImportClPart2DataCenterDlg()
{
}

void ImportClPart2DataCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_ctrlPreviewWnd);
}

BEGIN_MESSAGE_MAP(ImportClPart2DataCenterDlg, CFileDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL ImportClPart2DataCenterDlg::OnInitDialog()
{
	CFileDialog::OnInitDialog();

	// ����
	MultiLanguageMgr::GetInstance()->TranslateDialog(this);

	// ��ʼ���ӿڡ�
	Rect2D geomRect(.0, 10.0, .0, 10.0);
	m_ctrlPreviewWnd.InitViewPort(geomRect);

	UpdateData(FALSE);

	return TRUE;
}

void ImportClPart2DataCenterDlg::OnDestroy()
{
	// �ͷ��ӿڡ�
	m_ctrlPreviewWnd.ReleaseViewPort();
}

void ImportClPart2DataCenterDlg::OnFileNameChange()
{
	// get the basic geoms of the selected file.
	LineArc2DListPtr pLineArc2DList;
    CString strFilePath = GetPathName();
	if (PathHelper::GetFileExt(strFilePath).CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_TAOPRT_EXT)) == 0)
    {
        pLineArc2DList = ImportManager::GetLineArc2Ds_ofClPart(strFilePath);
    }
    else
    {
        return;
    }

	// Ԥ��
	if (pLineArc2DList && pLineArc2DList->size() > 0)
	{
		// ��ʼ���ӿڡ�
		Rect2D geomRect = pLineArc2DList->GetRectBox();
		m_ctrlPreviewWnd.FitRect(geomRect);

		// ����ͼ�Ρ�
		m_ctrlPreviewWnd.SetSolidGeom2DList(pLineArc2DList);

		// ���ơ�
		m_ctrlPreviewWnd.Invalidate();
	}

	CFileDialog::OnFileNameChange();
}

BOOL ImportClPart2DataCenterDlg::OnFileNameOK()
{
	return CFileDialog::OnFileNameOK();
}

END_CUTLEADER_NAMESPACE()

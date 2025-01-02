#include "StdAfx.h"
#include "ImportDxfDwg2TaskDlg.h"

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

#include "DxfData.h"
#include "DxfDwgConvertor.h"
#include "ClDataImportExportResource.h"

#include "ParamConfigLoader.h"
#include "ExpertLibItemLoader.h"
#include "MachineItemLoader.h"
#include "DefaultParamConfig.h"
#include "CamConfigSelectDlg.h"
#include "CLOptionInfo.h"
#include "PartEditParam.h"

using namespace std;


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(ImportDxfDwg2TaskDlg, CFileDialog)

ImportDxfDwg2TaskDlg::ImportDxfDwg2TaskDlg(CWnd* pParentWnd) :
    CFileDialog(TRUE, _T(""), NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT| OFN_ENABLESIZING,
        MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPORT_DXFDWG_FILEEXT), pParentWnd, 0, FALSE)
{
	SetTemplate(0, IDD_clTaskUI_ImportDxfDwg);
	m_iParamConfigID = INVALID_ID;
}

ImportDxfDwg2TaskDlg::~ImportDxfDwg2TaskDlg()
{
}

void ImportDxfDwg2TaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_SAVE_PATH, m_ctrlSavePath);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_ctrlPreviewWnd);
}

BEGIN_MESSAGE_MAP(ImportDxfDwg2TaskDlg, CFileDialog)
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_BTN_SAVE_PATH, OnSelectFolder)
    ON_BN_CLICKED(IDC_BTN_SelectCamParam2, OnSelectCamParam)
END_MESSAGE_MAP()

BOOL ImportDxfDwg2TaskDlg::OnInitDialog()
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
        LogMgr::DebugWarn(_T("57690"));
    }
	m_ctrlSavePath = m_pSaveFolder->GetFullPath();

    // 加工参数
    {
        DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
        DefaultParamConfigPtr pDefaultParamConfig = ParamConfigLoader::LoadDflParamConfig(pExpLibDBCon);
        m_iExpLibItemID = pDefaultParamConfig->GetExpLibItemID();
        m_iParamConfigID = pDefaultParamConfig->GetParamConfigID();
        DataProperty expLibProp;
        if (ParamConfigLoader::GetParamConfigInfo(pExpLibDBCon, m_iParamConfigID, expLibProp))
        {
            CString str = StringUtil::ConnectStrings(expLibProp.GetItem(2).GetValue().bstrVal, _T("("), expLibProp.GetItem(3).GetValue().bstrVal, _T(")"));
            GetDlgItem(IDC_EDIT_CamParam)->SetWindowText(str);
        }
        else
        {
            LogMgr::DebugWarn(_T("34583"));
        }
    }

	UpdateData(FALSE);

	return TRUE;
}

void ImportDxfDwg2TaskDlg::OnDestroy()
{
	// 释放视口。
	m_ctrlPreviewWnd.ReleaseViewPort();
}

void ImportDxfDwg2TaskDlg::OnSelectFolder()
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

void ImportDxfDwg2TaskDlg::OnSelectCamParam()
{
    // 更换板材的加工参数。
	CamConfigSelectDlg dlg;
	dlg.InitParamConfigID(m_iParamConfigID);
	if (dlg.DoModal() == IDOK)
	{
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		DataProperty expLibProp;
		if (ParamConfigLoader::GetParamConfigInfo(pExpLibDBCon, dlg.GetParamConfigID(), expLibProp))
		{
			CString str = StringUtil::ConnectStrings(expLibProp.GetItem(2).GetValue().bstrVal, _T("("), expLibProp.GetItem(3).GetValue().bstrVal, _T(")"));
			GetDlgItem(IDC_EDIT_CamParam)->SetWindowText(str);
			m_iParamConfigID = expLibProp.GetItem(0).GetValue().llVal;
			m_iExpLibItemID = expLibProp.GetItem(1).GetValue().llVal;
		}
		else
		{
			LogMgr::DebugWarn(_T("91476"));
		}
	}
}

void ImportDxfDwg2TaskDlg::OnFileNameChange()
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

BOOL ImportDxfDwg2TaskDlg::OnFileNameOK()
{
	return CFileDialog::OnFileNameOK();
}

END_CUTLEADER_NAMESPACE()

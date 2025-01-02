#include "StdAfx.h"
#include "NewPartFromDxfDwgDlg.h"

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
#include "CamConfigSelectDlg.h"
#include "CLOptionInfo.h"
#include "PartEditParam.h"

#include "DxfData.h"
#include "DxfDwgConvertor.h"
#include "ClDataImportExportResource.h"


using namespace std;


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(NewPartFromDxfDwgDlg, CFileDialog)

NewPartFromDxfDwgDlg::NewPartFromDxfDwgDlg(CWnd* pParentWnd) :
    CFileDialog(TRUE, _T(""), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ENABLESIZING,
		MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPORT_DXFDWG_FILEEXT), pParentWnd, 0, FALSE)
{
	SetTemplate(0, IDD_clPartUI_NewPartFromDxfDwg);
	m_iParamConfigID = INVALID_ID;
}

NewPartFromDxfDwgDlg::~NewPartFromDxfDwgDlg()
{
}

void NewPartFromDxfDwgDlg::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_ctrlPreviewWnd);
}

BEGIN_MESSAGE_MAP(NewPartFromDxfDwgDlg, CFileDialog)
	ON_WM_DESTROY()

    ON_BN_CLICKED(IDC_BTN_SelectCamParam, OnSelectCamParam)
END_MESSAGE_MAP()

BOOL NewPartFromDxfDwgDlg::OnInitDialog()
{
	CFileDialog::OnInitDialog();

	// 翻译
	MultiLanguageMgr::GetInstance()->TranslateDialog(this);

	// 初始化视口。
	Rect2D geomRect(.0, 10.0, .0, 10.0);
	m_ctrlPreviewWnd.InitViewPort(geomRect);

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

void NewPartFromDxfDwgDlg::OnDestroy()
{
	// 释放视口。
	m_ctrlPreviewWnd.ReleaseViewPort();
}

void NewPartFromDxfDwgDlg::OnSelectCamParam()
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

void NewPartFromDxfDwgDlg::OnFileNameChange()
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

BOOL NewPartFromDxfDwgDlg::OnFileNameOK()
{
	return CFileDialog::OnFileNameOK();
}

END_CUTLEADER_NAMESPACE()

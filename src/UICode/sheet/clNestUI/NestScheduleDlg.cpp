#include "stdafx.h"
#include "NestScheduleDlg.h"

#include <tuple>
#include "IntegerList.h"
#include "clUtilityResource.h"
#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"
#include "DateHelper.h"
#include "PathHelper.h"
#include "clSecureResource.h"
#include "RandomGen.h"
#include "ProgressData.h"
#include "ProgressDlg.h"
#include "ImgData.h"
#include "baseConst.h"
#include "clUILibResource.h"
#include "Rect2D.h"
#include "LineArc2DList.h"
#include "clBaseDataResource.h"
#include "ClFolderHelper.h"
#include "DataBaseManager.h"
#include "LogMgr.h"
#include "MultiLanguageMgr.h"
#include "ClFileHelper.h"
#include "StringUtil.h"

#include "IMaterialSize.h"
#include "MaterialSizeList.h"
#include "MaterialLibLoader.h"
#include "clMaterialLibResource.h"
#include "MaterialProcessor.h"
#include "MachineItemLoader.h"
#include "clMachineLibResource.h"
#include "ParamConfig.h"
#include "ParamConfigSelectDlg.h"
#include "ParamConfigLoader.h"
#include "DefaultParamConfig.h"
#include "ExpertLibItemLoader.h"
#include "clExpertLibResource.h"
#include "ParamConfigDlg.h"
#include "NestConfigItem.h"
#include "ConfigItemLoader.h"
#include "CamConfigSelectDlg.h"
#include "CLOptionInfo.h"
#include "ImpExpConfig.h"

#include "SelectDataCenterFolderDlg.h"
#include "FolderNode.h"
#include "DataCenterManager.h"
#include "DataCenterItemList.h"
#include "DataCenterItemLoader.h"
#include "NestPartSelectDlg.h"
#include "clGeometryFeatureResource.h"
#include "PatternList.h"
#include "PatternLoopList.h"
#include "LoopToolData.h"
#include "LoopCutFeature.h"
#include "LoopProcessor.h"
#include "clPartResource.h"
#include "PartCadData.h"
#include "PartCamData.h"
#include "PartCamDataList.h"
#include "LoopCutFeatureLoader.h"
#include "PartItem.h"
#include "Part.h"
#include "PartItemList.h"
#include "PartManager.h"
#include "PartBuilder.h"
#include "PartLoader.h"
#include "PartWriter.h"
#include "Loop2PartDlg.h"
#include "NestTask.h"
#include "NestPart.h"
#include "NestMaterial.h"
#include "NestMaterialList.h"
#include "NestPartList.h"
#include "NestMatSelectDlg.h"
#include "clNestResource.h"
#include "NestResultDlg.h"
#include "CreateStdPartDlg.h"
#include "NestInfoDlg.h"
#include "ImpXlsPartDlg.h"
#include "NestJob.h"
#include "NestProcessor.h"
#include "LibraryData.h"
#include "AreaInfo.h"
#include "clDataImportExportResource.h"
#include "ImportManager.h"
#include "ImportDxfDwg2NestTaskDlg.h"
#include "DxfDwgConvertor.h"
#include "PartsFromAsmDxf.h"
#include "PartList.h"
#include "LoopTopologyItemList.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

using namespace std;
using namespace std::tr1;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(NestScheduleDlg, CDialogEx)

BEGIN_MESSAGE_MAP(NestScheduleDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_SelectCamParam, OnSelectCamParam)
	ON_BN_CLICKED(IDC_BTN_EDIT_PARAM, OnEditParam)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_NEST_PART, OnNestPartListchanged)
	ON_BN_CLICKED(IDC_BTN_AddPartsToNesting, OnAddPartsToNesting)
	ON_BN_CLICKED(IDC_BTN_NEST_INFO, OnSetNestInfo)
	ON_BN_CLICKED(IDC_BTN_DELETE_PART, OnRemoveFromNestList)

	ON_BN_CLICKED(IDC_BTN_ADD_MAT, OnAddMat)
	ON_BN_CLICKED(IDC_BTN_DELETE_MAT, OnDeleteMat)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_NEST_MAT, OnNestMatListchanged)

	ON_BN_CLICKED(IDC_BTN_EXECUTE_NEST, OnExecuteNest)
END_MESSAGE_MAP()

NestScheduleDlg::NestScheduleDlg(CWnd* pParent /*=NULL*/)
		: CLDialog(NestScheduleDlg::IDD, pParent)
{
	m_iExpLibItemID = INVALID_ID;
	m_iParamConfigID = INVALID_ID;
	m_bFinishAddNestPart = TRUE;
}

NestScheduleDlg::~NestScheduleDlg()
{
}

void NestScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// “参数配置”分组。
	DDX_Control(pDX, IDC_BTN_SelectCamParam, m_btnSelectCamParam);
	DDX_Control(pDX, IDC_BTN_EDIT_PARAM, m_btnEditCamParam);

	// 零件
	DDX_Control(pDX, IDC_LIST_NEST_PART, m_nestPartList);
	DDX_Control(pDX, IDC_STATIC_PREVIEW_NESTPART, m_partPreviewWnd);
	DDX_Control(pDX, IDC_BTN_AddPartsToNesting, m_addPartBtn);
	
	DDX_Control(pDX, IDC_BTN_NEST_INFO, m_ctrlNestInfo);
	DDX_Control(pDX, IDC_BTN_DELETE_PART, m_btnDelPrt);

	// 材料
	DDX_Control(pDX, IDC_LIST_NEST_MAT, m_nestMatList);
	DDX_Control(pDX, IDC_STATIC_PREVIEW_MAT, m_matPreviewWnd);
	DDX_Control(pDX, IDC_BTN_ADD_MAT, m_btnAddMat);
	DDX_Control(pDX, IDC_BTN_DELETE_MAT, m_btnDelMat);

	// 排版按钮。
	DDX_Control(pDX, IDC_BTN_EXECUTE_NEST, m_ctrlExecuteNest);
	DDX_Control(pDX, IDCANCEL, m_btnCancel );
}

BOOL NestScheduleDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// 按钮提示。
	CString strTips;
	strTips = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADD_MAT_FROM_DB);
	m_btnAddMat.SetTooltip(strTips);
	strTips = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SET_NEST_INFO_TIP);
	m_ctrlNestInfo.SetTooltip(strTips);

	// 初始化按钮菜单。
	m_btnMenu.CreatePopupMenu();
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADD_PART_FROM_DXF);
	m_btnMenu.AppendMenu(MF_STRING, IDC_BTN_ADD_DXF, str.AllocSysString());
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ADD_PART_FROM_DB);
	m_btnMenu.AppendMenu(MF_STRING, IDC_BTN_AddPartFromDB, str.AllocSysString());
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_AddStdPart);
	m_btnMenu.AppendMenu(MF_STRING, IDC_BTN_AddPartFromStdPart, str.AllocSysString());
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMP_RECT_PART_FROM_XLS);
	m_btnMenu.AppendMenu(MF_STRING, IDC_BTN_IMP_STD_XLS, str.AllocSysString());
	m_addPartBtn.m_hMenu = m_btnMenu.GetSafeHmenu();
	m_addPartBtn.m_bStayPressed = TRUE;
	m_addPartBtn.m_bDefaultClick = FALSE;

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
			LogMgr::DebugWarn(_T("46853"));
		}
	}

	// 初始化表格样式。
	InitPartGridStyle();
	InitMatGridStyle();

	// 初始化视口。
	Rect2D geomRect(.0, 10.0, .0, 10.0);
	m_matPreviewWnd.InitViewPort(geomRect);

	// 添加默认的材料。
	AddDflMat();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_CONFIG )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_EXPLIB_ITEM )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_PARAM_CONFIG )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_NEST_PART )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_NEST_MAT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_LIST_NEST_PART )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

void NestScheduleDlg::OnDestroy()
{
	RlsPartGridData();
	RlsMatGridData();

	// 释放视口。
	m_matPreviewWnd.ReleaseViewPort();

	CDialogEx::OnDestroy();
}

void NestScheduleDlg::OnPaint()
{
	CDialogEx::OnPaint();

	PreviewNestPart();
	PreviewNestMat();
}

HBRUSH NestScheduleDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void NestScheduleDlg::OnSelectCamParam()
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

void NestScheduleDlg::OnEditParam()
{
	ParamConfigDlg dlg(this);
	dlg.Init(m_iParamConfigID, 7);
	dlg.DoModal();
}

void NestScheduleDlg::AddPartFromDB()
{
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	NestPartSelectDlg dlg;
	dlg.Init(DataCenterItemLoader::LoadProductTree(pMainDBCon));
	if (dlg.DoModal() == IDOK)
	{
		// 得到选中的零件。
		IntegerListPtr pSelPartIDList = dlg.GetSelectedPart();

		// 禁止消息。
		m_bFinishAddNestPart = FALSE;

		// 逐个加入选中的零件。
		for (unsigned int i = 0; i < pSelPartIDList->size(); i++)
		{
			LONGLONG iPartID = pSelPartIDList->at(i);

			// 检查是否添加了这个零件。
			if (PartAdded(iPartID))
				continue;

            // 检查这个零件有没有和当前参数配置对应的工艺数据。
            LONGLONG iCamDataID = CheckPart(m_iParamConfigID, iPartID);

			// 插入一行。
			int iRowCount = m_nestPartList.GetItemCount();
			m_nestPartList.InsertItem(iRowCount, _T(""));
			m_nestPartList.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(iPartID)));

			// 序号。
			CString strNum;
			if ((iRowCount+1) < 10)
				strNum.Format(_T("0%d"), iRowCount+1);
			else
				strNum.Format(_T("%d"), iRowCount+1);
			m_nestPartList.SetItemText(iRowCount, 1, strNum);

			// 零件名称。
			DataProperty partProp;
			DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
			DataCenterItemLoader::GetDataProp(pMainDBCon, iPartID, partProp);
			CString strPartName = (LPCTSTR)partProp.GetItem(0).GetValue().bstrVal;
			m_nestPartList.SetItemText(iRowCount, 2, strPartName);

			// 零件尺寸。
			DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(iPartID);
			PartCadDataPtr pPartCadData = PartLoader::LoadPartCadData(pPartDBCon);
			Rect2D outerRect = pPartCadData->GetPatternLoopList()->GetRectBox();
			CString strPartSize;
			strPartSize.Format(_T("%.3f * %.3f"), outerRect.GetWidth(), outerRect.GetHeight());
			m_nestPartList.SetItemText(iRowCount, 3, strPartSize);

			// 排版优先级。
			CString strPartPriority;
			strPartPriority.Format(INT_FORMAT, NestPriority::MaxVal());
			m_nestPartList.SetItemText(iRowCount, 4, strPartPriority);

			// 排版个数。
			CString strPartCount;
			strPartCount.Format(INT_FORMAT, 1);
			m_nestPartList.SetItemText(iRowCount, 5, strPartCount);

			// 旋转角度。
			CString strRotAng;
			strRotAng = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_FREE);
			m_nestPartList.SetItemText(iRowCount, 6, strRotAng);

            // 外轮廓刀具信息
			IPatternLoopPtr pBoundaryPatternLoop = pPartCadData->GetLoopTopologyItems()->GetBoundaryLoopEx();
            if (!pBoundaryPatternLoop)
            {
                LogMgr::DebugWarn(_T("NestScheduleDlg::OnAddPartFromDB want add a part without the closed boundary loop!"));
            }
            tuple<CString, double> toolInfo = LoopCutFeatureLoader::LoadToolInfo_of_loop(pPartDBCon, iCamDataID, pBoundaryPatternLoop->GetID());
            CString strToolInfo;
            strToolInfo.Format(_T("%.3f(%s)"), get<1>(toolInfo), get<0>(toolInfo));
            m_nestPartList.SetItemText(iRowCount, 7, strToolInfo);
		}

		// 打开消息。
		m_bFinishAddNestPart = TRUE;

		/************************************************************************/
		// select the last row.

		int iCount = m_nestPartList.GetItemCount();

		// de-select the selected rows.
		for (int i = 0; i < iCount; i++)
		{
			if (m_nestPartList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
				m_nestPartList.SetItemState(i, 0, LVIS_SELECTED);
		}

		// select the last row.
		m_nestPartList.EnsureVisible(iCount-1, FALSE);
		m_nestPartList.SetItemState(iCount-1, LVIS_SELECTED, LVIS_SELECTED);
		m_nestPartList.SetFocus();
		/************************************************************************/
	}
}

void NestScheduleDlg::AddPartsFromDxfDwg()
{
	CString strFileName;
	ImportDxfDwg2NestTaskDlg dlg(this);
	int iBufferSize = 100 * 1000 * MAX_PATH;
	dlg.m_ofn.lpstrFile = strFileName.GetBuffer(iBufferSize);
	dlg.m_ofn.nMaxFile = iBufferSize;
	if (dlg.DoModal() == IDOK)
	{
		vector<CString> dxfdwgFullPaths;

		// 遍历每个dxf/dwg文件
		POSITION pos = dlg.GetStartPosition();
		while (pos != NULL)
		{
			CString strDxfDwgFilePath = dlg.GetNextPathName(pos);
			dxfdwgFullPaths.push_back(strDxfDwgFilePath);
		}

		const CString& strDataCenterPath = dlg.GetSavePath()->GetFullPath();
		AddDxfdwgToNest(dxfdwgFullPaths, strDataCenterPath);
	}
	strFileName.ReleaseBuffer();
}

void NestScheduleDlg::OnAddPartsToNesting()
{
	switch (m_addPartBtn.m_nMenuResult)
	{
	case IDC_BTN_ADD_DXF:
	{
		AddPartsFromDxfDwg();
	}
	break;

	case IDC_BTN_AddPartFromDB:
	{
		AddPartFromDB();
	}
	break;

	case IDC_BTN_AddPartFromStdPart:
	{
		CreateStdPart();
	}
	break;

	case IDC_BTN_IMP_STD_XLS:
	{
		ImportRectPart();
	}
	break;
	}
}

void NestScheduleDlg::OnSetNestInfo()
{
	// 检查有没有选中零件。
	int iSelCount = 0;
	for (int i = 0; i < m_nestPartList.GetItemCount(); i++)
	{
		if (m_nestPartList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
			iSelCount++;
	}
	if (iSelCount == 0)
	{
		CString strProductName, str;
		strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NEST_INFO);
		MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	// 设置排版数量。
	NestInfoDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		// 排版数量。
		if (dlg.IsEnablePartCount())
		{
			int iNestCount = dlg.GetNestCount();
			for (int i = 0; i < m_nestPartList.GetItemCount(); i++)
			{
				if (m_nestPartList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
				{
					CString strPartCount;
					strPartCount.Format(INT_FORMAT, iNestCount);
					m_nestPartList.SetItemText(i, 5, strPartCount);
				}
			}
		}

		// 旋转角度。
		if (dlg.IsEnableRotAng())
		{
			PartRotateType iRotAng = dlg.GetRotAng();
			CString strRotAng = NestPart::GetName_ofPartRotateStyle(iRotAng);
			for (int i = 0; i < m_nestPartList.GetItemCount(); i++)
			{
				if (m_nestPartList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
					m_nestPartList.SetItemText(i, 6, strRotAng);
			}
		}
	}
}

void NestScheduleDlg::OnRemoveFromNestList()
{
	// 禁止消息。
	m_bFinishAddNestPart = FALSE;

	for (int i = 0; i < m_nestPartList.GetItemCount(); i++)
	{
		if (m_nestPartList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			// release item data.
			DWORD_PTR pItemData = m_nestPartList.GetItemData(i);
			delete (LONGLONG*)pItemData;

			// delete the item.
			m_nestPartList.DeleteItem(i);
			i--;
		}
	}

	// re-index the first column.
	for (int i = 0; i < m_nestPartList.GetItemCount(); i++)
	{
		CString strNum;
		if ((i+1) < 10)
			strNum.Format(_T("0%d"), i+1);
		else
			strNum.Format(_T("%d"), i+1);
		m_nestPartList.SetItemText(i, 1, strNum);
	}

	// 打开消息。
	m_bFinishAddNestPart = TRUE;

	// select the last row.
	int iCount = m_nestPartList.GetItemCount();
	if (iCount > 0)
	{
		m_nestPartList.EnsureVisible(iCount-1, FALSE);
		m_nestPartList.SetItemState(iCount-1, LVIS_SELECTED, LVIS_SELECTED);
		m_nestPartList.SetFocus();
	}
	else
	{
		PreviewNestPart();
	}
}

void NestScheduleDlg::OnNestPartListchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!m_bFinishAddNestPart)
		return;

	// preview
	PreviewNestPart();

	*pResult = 0;
}

void NestScheduleDlg::OnAddMat()
{
	NestMatSelectDlg dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		// 添加到材料表格。
		MaterialSizeListPtr pMaterialSizeList = dlg.GetSelectedMaterialSizes();
		for (unsigned int i = 0; i < pMaterialSizeList->size(); i++)
		{
			// check whether this material is added.
			IMaterialSizePtr pMaterialSize = pMaterialSizeList->at(i);
			if (MatAdded(pMaterialSize->GetID()))
				continue;

			// insert a new row.
			AddNestMat(pMaterialSize);
		}

		/************************************************************************/
		// select the last row.

		int iCount = m_nestMatList.GetItemCount();

		// de-select the selected rows.
		for (int i = 0; i < iCount; i++)
		{
			if (m_nestMatList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
				m_nestMatList.SetItemState(i, 0, LVIS_SELECTED);
		}

		// select the last row.
		m_nestMatList.EnsureVisible(iCount-1, FALSE);
		m_nestMatList.SetItemState(iCount-1, LVIS_SELECTED, LVIS_SELECTED);
		m_nestMatList.SetFocus();
		/************************************************************************/
	}
}

void NestScheduleDlg::OnDeleteMat()
{
	for (int i = 0; i < m_nestMatList.GetItemCount(); i++)
	{
		if (m_nestMatList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			// release item data.
			DWORD_PTR pItemData = m_nestMatList.GetItemData(i);
			delete (LONGLONG*)pItemData;

			// delete the item.
			m_nestMatList.DeleteItem(i);
			i--;
		}
	}

	// re-index the first column.
	for (int i = 0; i < m_nestMatList.GetItemCount(); i++)
	{
		CString strNum;
		if ((i+1) < 10)
			strNum.Format(_T("0%d"), i+1);
		else
			strNum.Format(_T("%d"), i+1);
		m_nestMatList.SetItemText(i, 1, strNum);
	}

	// select the last row.
	int iCount = m_nestMatList.GetItemCount();
	if (iCount > 0)
	{
		m_nestMatList.EnsureVisible(iCount-1, FALSE);
		m_nestMatList.SetItemState(iCount-1, LVIS_SELECTED, LVIS_SELECTED);
		m_nestMatList.SetFocus();
	}
	else
	{
		// 清空预览。
		LineArc2DListPtr pGeom2Ds;
		pGeom2Ds.reset();
		m_matPreviewWnd.SetSolidGeom2DList(pGeom2Ds);
		m_matPreviewWnd.SetDashGeom2DList(pGeom2Ds);
		m_matPreviewWnd.OnPaint();
	}
}

void NestScheduleDlg::OnNestMatListchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	// preview
	PreviewNestMat();

	*pResult = 0;
}

void NestScheduleDlg::OnExecuteNest()
{
	CString strTitle, strMsg;
	strTitle = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// 创建排版任务。
	NestTaskPtr pNestTask(new NestTask);
	{
		pNestTask->SetParamConfigID(m_iParamConfigID);

		// 从列表得到排版零件信息。
		NestPartListPtr pNestPartList = GetNestPartList();
		if (pNestPartList->size() == 0)
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NO_NESTPART);
			MessageBox(strMsg, strTitle, MB_ICONWARNING);
			return;
		}
		pNestTask->SetNestPartList(pNestPartList);

		// 从列表得到排版材料信息。
		NestMaterialListPtr pNestMatList = GetNestMatList();
		if (pNestMatList->size() == 0)
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NO_NESTMAT);
			MessageBox(strMsg, strTitle, MB_ICONWARNING);
			return;
		}
		pNestTask->SetNestMatList(pNestMatList);
	}

	// 计算零件的面积信息。
	AreaInfo areaInfo;
	{
		map<__int64, double> areaData;
		map<__int64, Polygon2DPtr> partOutPolyData;
		double dAllArea = .0;
		NestPartListPtr pNestPartList = pNestTask->GetNestPartList();
		for (unsigned int i = 0; i < pNestPartList->size(); i++)
		{
			NestPartPtr pNestPart = pNestPartList->at(i);
			pair<double, Polygon2DPtr> area = PartManager::GetPartAreaEx(pNestPart->GetPartItem()->GetPart()->GetCadData());
			areaData[pNestPart->GetPartItem()->GetPart()->GetID()] = area.first;
			partOutPolyData[pNestPart->GetPartItem()->GetPart()->GetID()] = area.second;
			dAllArea += area.first*pNestPart->GetNestCount();
		}
		areaInfo.partAreaData = areaData;
		areaInfo.partOutPolyData = partOutPolyData;
		areaInfo.dAllPartArea = dAllArea;
	}

	// 准备库信息。
	LibraryDataPtr pLibraryData;
	{
		// 机器信息
		DBConnectPtr pMacLibDBConnect = DataBaseManager::GetInstance()->GetMacLibDBConnect();
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		LONGLONG iExpLibItemID = ParamConfigLoader::GetExpLibItemID(pExpLibDBCon, pNestTask->GetParamConfigID());
		LONGLONG iMacItemID = ExpertLibItemLoader::GetMacIDByExpLibItem(pExpLibDBCon, iExpLibItemID);
		MachineParamPtr pMacParam = MachineItemLoader::LoadMacParam(pMacLibDBConnect, iMacItemID);

		// 参数配置。
		NestConfigItemPtr pNestConfigItem = ConfigItemLoader::LoadNestConfigItem(pExpLibDBCon, pNestTask->GetParamConfigID());
		SequenceConfigItemPtr pSeqConfigItem = ConfigItemLoader::LoadSeqConfigItem(pExpLibDBCon, pNestTask->GetParamConfigID());

		// 材料
		MaterialSizeListPtr pMaterialSizeList(new MaterialSizeList);
		for (unsigned int i = 0; i < pNestTask->GetNestMatList()->size(); i++)
		{
			pMaterialSizeList->push_back(pNestTask->GetNestMatList()->at(i)->GetMatSize());
		}

		pLibraryData.reset(new LibraryData(pMacParam, pMaterialSizeList, pNestConfigItem, pSeqConfigItem));
	}

    // 要是启动共边排版，检查参与排版的各个零件的刀具是否一样。
    if (pLibraryData->m_pNestConfigItem->IsEnableComcut() && m_nestPartList.GetItemCount() > 1)
    {
        CString strToolInfo = m_nestPartList.GetItemText(0, 7);
        for (int i = 1; i < m_nestPartList.GetItemCount(); i++)
        {
            CString strTmpToolInfo = m_nestPartList.GetItemText(i, 7);
            if (strTmpToolInfo != strToolInfo)
            {
                strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_TOOLSIZE_WARN);
                MessageBox(strMsg, strTitle, MB_ICONWARNING);
                break;
            }
        }
        
    }

	// 启动排版。
	NestProcessorPtr pNestProcessor(new NestProcessor(pNestTask, areaInfo, pLibraryData));
	pNestProcessor->StartNest();

	// 显示排版过程。
	NestResultDlg nestResultDlg;
	nestResultDlg.Init(pNestTask, pNestProcessor, areaInfo, pLibraryData);
	if (nestResultDlg.DoModal() == IDOK)
	{
		m_pNestJob = nestResultDlg.GetNestJob();
		EndDialog(IDC_BTN_EXECUTE_NEST);
	}
}

void NestScheduleDlg::AddDxfdwgToNest(const vector<CString>& dxfdwgFullPaths, const CString& strDataCenterPath)
{
	// 所有导入的零件项。
	PartItemListPtr pPartItemList(new PartItemList);
	map<LONGLONG, int> partCount; // <零件ID，零件数量>，当某个零件在dxf中有多个且被合并了后，才设置这个值，肯定大于1。

	for (unsigned int i = 0; i < dxfdwgFullPaths.size(); i++)
	{
		const CString& strDxfDwgFilePath = dxfdwgFullPaths[i];

		// 导入dxf/dwg文件为PartCadData对象，此时不要生成阵列回路，否则会影响拓扑关系识别。
		BOOL bIsDwg = (PathHelper::GetFileExt(strDxfDwgFilePath).CompareNoCase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DWG_EXT)) == 0 ? TRUE : FALSE);
		PartCadDataPtr pPartCadData = DxfDwgConvertor::DxfDwg2CadData(strDxfDwgFilePath, bIsDwg);

		// 理出轮廓的拓扑关系，从中分析出各个零件。
		if (!LoopProcessor::NeedToSeperate(LoopProcessor::CalcLoopTopStruct(pPartCadData->GetPatternLoopList().get()).get()))
		{
			// 要是没有封闭的外边框，就不导入这个零件。
			if (pPartCadData->GetLoopTopologyItems()->GetBoundaryLoop(pPartCadData->GetPatternLoopList().get()) == INVALID_IDX)
			{
				CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NEST_OPEN_BOUNDARY);
				str.Replace(_T("WILLBEREPLACED1"), strDxfDwgFilePath);
				MessageBox(str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
				continue;
			}

			// 利用“零件cad数据”和零件方案来构建零件对象。
			UINT iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
			PartPtr pPart = PartBuilder::BuildPart(PathHelper::GetFileName(strDxfDwgFilePath), pPartCadData, m_iParamConfigID, iAssignLimit, TRUE);
			PartItemPtr pPartItem(new PartItem(pPart, pPart->GetCamDataList()->at(0)));
			pPartItemList->push_back(pPartItem);
		}
		else
		{
			// 这个对话框用于从回路识别零件。
			Loop2PartDlg dlg;
			dlg.Init(pPartCadData, strDxfDwgFilePath, m_iParamConfigID);
			if (dlg.DoModal() == IDC_BTN_ACCEPT)
			{
				PartsFromAsmDxfPtr pPartsFromAsmDxf = dlg.Get_PartsFromAsmDxf();
				const map<LONGLONG, vector<pair<LONGLONG, Matrix2D>>>& matrixData_of_sameShapePart = pPartsFromAsmDxf->GetMatrixsOfSameShapePart();
				for (unsigned int i = 0; i < pPartsFromAsmDxf->GetPartList()->size(); i++)
				{
					PartPtr pPart = pPartsFromAsmDxf->GetPartList()->operator [](i);

					// 要是没有封闭的外边框，就不导入这个零件。
					if (pPart->GetCadData()->GetLoopTopologyItems()->GetBoundaryLoop(pPart->GetCadData()->GetPatternLoopList().get()) == INVALID_IDX)
					{
						CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NEST_OPEN_BOUNDARY);
						str.Replace(_T("WILLBEREPLACED1"), pPart->GetName());
						MessageBox(str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
						continue;
					}

					PartItemPtr pPartItem(new PartItem(pPart, pPart->GetCamDataList()->at(0)));
					pPartItemList->push_back(pPartItem);

					map<LONGLONG, vector<pair<LONGLONG, Matrix2D>>>::const_iterator iter = matrixData_of_sameShapePart.find(pPart->GetID());
					if (iter != matrixData_of_sameShapePart.end())
					{
						partCount[pPart->GetID()] = iter->second.size();
					}
				}
			}
		}
	}

	// 保存零件到数据库。
	vector<PartPtr> newPartList;
	for (unsigned int i = 0; i < pPartItemList->size(); i++)
	{
		PartItemPtr pPartItem = pPartItemList->at(i);
		PartPtr pPart = pPartItem->GetPart();
		pPart->SetCreateTime(DateHelper::GetCurrentTime());
		pPart->SetModifyTime(DateHelper::GetCurrentTime());
		newPartList.push_back(pPart);
	}
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	FolderNodePtr pDataCenterRootFolder = DataCenterItemLoader::LoadProductTree(pMainDBCon);
	IDataCenterItemPtr pDataCenterItem = pDataCenterRootFolder->SearchDataCenterItemByFullPath(strDataCenterPath);
	if (!pDataCenterItem)
	{
		LogMgr::DebugWarn(_T("693541"));
		return;
	}
	FolderNodePtr pFolderNode = boost::dynamic_pointer_cast<FolderNode>(pDataCenterItem);
	if (!pFolderNode)
	{
		LogMgr::DebugWarn(_T("321720"));
		return;
	}
	PartManager::SaveNewPartList(newPartList, pFolderNode);

	// 禁止消息。
	m_bFinishAddNestPart = FALSE;

	// 逐个加入选中的零件。
	for (unsigned int i = 0; i < pPartItemList->size(); i++)
	{
		PartItemPtr pPartItem = pPartItemList->at(i);
		// 检查是否添加了这个零件。
		if (PartAdded(pPartItem->GetPart()->GetID()))
			continue;

		// 要是这个零件在dxf中有多个，且被合并了，那这里的数量就应该大于1。
		int iCount = 1;
		map<LONGLONG, int>::const_iterator iter = partCount.find(pPartItem->GetPart()->GetID());
		if (iter != partCount.end())
		{
			iCount = iter->second + 1;
		}

		AddNestPartToGrid(pPartItem.get(), NestPriority::MaxVal(), iCount, PartRotate_Free);
	}

	// 打开消息。
	m_bFinishAddNestPart = TRUE;

	// select the last row.
	{
		int iCount = m_nestPartList.GetItemCount();

		// de-select the selected rows.
		for (int i = 0; i < iCount; i++)
		{
			if (m_nestPartList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
				m_nestPartList.SetItemState(i, 0, LVIS_SELECTED);
		}

		// select the last row.
		m_nestPartList.EnsureVisible(iCount - 1, FALSE);
		m_nestPartList.SetItemState(iCount - 1, LVIS_SELECTED, LVIS_SELECTED);
		m_nestPartList.SetFocus();
	}
}

void NestScheduleDlg::InitPartGridStyle()
{
	CString str;

	// Requires one never uses column 0
	m_nestPartList.InsertHiddenLabelColumn();

	// the num column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
	CGridColumnTrait* pNUMCol1 = new CGridColumnTraitText;
	m_nestPartList.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNUMCol1);

	// the part name column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTNAME);
	CGridColumnTrait* pNameCo2 = new CGridColumnTraitText;
	m_nestPartList.InsertColumnTrait(2, str, LVCFMT_LEFT, 120, 0, pNameCo2);

	// “零件尺寸”列。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_SIZE);
	CGridColumnTrait* pPartSizeCol = new CGridColumnTraitText;
	m_nestPartList.InsertColumnTrait(3, str, LVCFMT_LEFT, 150, 0, pPartSizeCol);

	// the nest priority column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NESTMGR_NEST_PRIORITY);
	CGridColumnTraitCombo* pNameCo3 = new CGridColumnTraitCombo;
	pNameCo3->SetStyle(pNameCo3->GetStyle() | CBS_DROPDOWNLIST);
	StringListPtr pList1 = NestPriority::GetAllPriority();
	for (unsigned int i = 0; i < pList1->size(); i++)
		pNameCo3->AddItem(i, pList1->at(i));
	m_nestPartList.InsertColumnTrait(4, str, LVCFMT_LEFT, 70, 0, pNameCo3);

	// the nest count column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COUNT);
	CGridColumnTrait* pNameCo4 = new CGridColumnTraitEdit(INTEDITBOX);
	m_nestPartList.InsertColumnTrait(5, str, LVCFMT_LEFT, 50, 0, pNameCo4);

	// "Rotate Angle" column.
	CGridColumnTraitCombo* pRotAngCol = new CGridColumnTraitCombo;
	pRotAngCol->SetStyle(pRotAngCol->GetStyle() | CBS_DROPDOWNLIST);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_FREE);
	pRotAngCol->AddItem(0, str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_90_INCREMENT);
	pRotAngCol->AddItem(1, str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_0_FIXED);
	pRotAngCol->AddItem(2, str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_90_FIXED);
	pRotAngCol->AddItem(3, str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_180_FIXED);
	pRotAngCol->AddItem(4, str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_270_FIXED);
	pRotAngCol->AddItem(5, str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_HORIZONTAL);
	pRotAngCol->AddItem(6, str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_VERTICAL);
	pRotAngCol->AddItem(7, str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NEST_ROT_ANG);
	m_nestPartList.InsertColumnTrait(6, str, LVCFMT_LEFT, 130, 0, pRotAngCol);

    // “外轮廓刀具信息”列。
    str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NESTPART_OUTER_LOOPTOOL);
    CGridColumnTrait* pBoundaryLoopToolCol = new CGridColumnTraitText;
    m_nestPartList.InsertColumnTrait(7, str, LVCFMT_LEFT, 130, 0, pBoundaryLoopToolCol);
}

void NestScheduleDlg::PreviewNestPart()
{
	// 得到选中项。
	int iIndex = INVALID_IDX;
	for (int i = 0; i < m_nestPartList.GetItemCount(); i++)
	{
		if (m_nestPartList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
			iIndex = i;
	}

	// 得到预览数据。
	ImgDataPtr pImgData;
	pImgData.reset();
	if (iIndex != INVALID_IDX)
	{
		LONGLONG* pPartID = (LONGLONG*)m_nestPartList.GetItemData(iIndex);
		DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(*pPartID);
		pImgData = PartLoader::LoadPreviewData(pPartDBCon);
	}

	// 预览。
	ImgData::DrawImgData(pImgData.get(), &m_partPreviewWnd, SkinManager::GetBackgroundColor());
}

LONGLONG NestScheduleDlg::CheckPart(LONGLONG iParamConfigID, LONGLONG iPartID)
{
	// 检查零件是不是有和当前选中的“参数配置”配套的工艺数据。
	LONGLONG iCamDataID, iExpLibItemID;
	if (!PartManager::HasCamDatasetA(iPartID, iParamConfigID, iCamDataID, iExpLibItemID))
	{
		// 创建新的工艺数据并保存。
		DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(iPartID);
		PartCadDataPtr pPartCadData = PartLoader::LoadPartCadData(pPartDBCon);
		UINT iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
		PartCamDataPtr pNewPartCamData = PartBuilder::BuildPartCamData(pPartCadData, iParamConfigID, iAssignLimit, FALSE);
		PartWriter::SaveCamData(pPartDBCon, pNewPartCamData);
		iCamDataID = pNewPartCamData->GetID();
	}

	return iCamDataID;
}

void NestScheduleDlg::RlsPartGridData()
{
	for (int i = 0; i < m_nestPartList.GetItemCount(); i++)
	{
		DWORD_PTR pItemData = m_nestPartList.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}
}

BOOL NestScheduleDlg::PartAdded(LONGLONG iPartID)
{
	for (int i = 0; i < m_nestPartList.GetItemCount(); i++)
	{
		LONGLONG* pPartID = (LONGLONG*)m_nestPartList.GetItemData(i);
		if (*pPartID == iPartID)
			return TRUE;
	}

	return FALSE;
}

NestPartListPtr NestScheduleDlg::GetNestPartList()
{
	NestPartListPtr pNestPartList(new NestPartList);

	for (int i = 0; i < m_nestPartList.GetItemCount(); i++)
	{
		LONGLONG* pPartID = (LONGLONG*)m_nestPartList.GetItemData(i);

		// 加载零件项。
		LONGLONG iCamDataID = CheckPart(m_iParamConfigID, *pPartID);
		DBConnectPtr pPartDBCon = DataBaseManager::ConnectPartDB(*pPartID);
		PartItemPtr pPartItem = PartLoader::LoadPartItem(pPartDBCon, *pPartID, iCamDataID);

		// 新建“排版零件”。
		NestPartPtr pNestPart(new NestPart(pPartItem));
		pNestPartList->push_back(pNestPart);

		// 排版优先级。
		int iNestPriority = _wtoi(m_nestPartList.GetItemText(i, 4));
		NestPriority nestPriority(iNestPriority);
		pNestPart->SetPriority(nestPriority);

		// 排版个数。
		int iNestCount = _wtoi(m_nestPartList.GetItemText(i, 5));
		pNestPart->SetNestCount(iNestCount);

		/************************************************************************/		
		// 旋转角度。

		CString str1, str2, str3, str4, str5, str6, str7, str8;
		str1 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_FREE);
		str2 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_90_INCREMENT);
		str3 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_0_FIXED);
		str4 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_90_FIXED);
		str5 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_180_FIXED);
		str6 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_270_FIXED);
		str7 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_HORIZONTAL);
		str8 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_VERTICAL);

		PartRotateType iPartRotAng = PartRotate_Free;
		CString str = m_nestPartList.GetItemText(i, 6);
		if (str == str1)
			iPartRotAng = PartRotate_Free;
		else if (str == str2)
			iPartRotAng = PartRotate_90Increment;
		else if (str == str3)
			iPartRotAng = PartRotate_0Fixed;
		else if (str == str4)
			iPartRotAng = PartRotate_90Fixed;
		else if (str == str5)
			iPartRotAng = PartRotate_180Fixed;
		else if (str == str6)
			iPartRotAng = PartRotate_270Fixed;
		else if (str == str7)
		{
			iPartRotAng = PartRotate_0And180Fixed;
		}
		else if (str == str8)
		{
			iPartRotAng = PartRotate_90And270Fixed;
		}
		pNestPart->SetRotStyle(iPartRotAng);
		/************************************************************************/
	}

	return pNestPartList;
}

void NestScheduleDlg::CreateStdPart()
{
	CreateStdPartDlg dlg;
	dlg.SetConfigParamID(m_iParamConfigID);
	if (dlg.DoModal() == IDOK)
	{
		PartPtr pPart = dlg.GetNewPart();
		FolderNodePtr pFolderNode = dlg.GetSaveFolder();
		int iPartCount = dlg.GetPartCount();

		// 保存零件到数据库。
		PartManager::SaveNewPart(pPart, pFolderNode);

		/************************************************************************/
		// 插入一行。

		int iRowCount = m_nestPartList.GetItemCount();
		m_nestPartList.InsertItem(iRowCount, _T(""));
		m_nestPartList.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pPart->GetID())));

		// 序号。
		CString strNum;
		if ((iRowCount+1) < 10)
			strNum.Format(_T("0%d"), iRowCount+1);
		else
			strNum.Format(_T("%d"), iRowCount+1);
		m_nestPartList.SetItemText(iRowCount, 1, strNum);

		// 零件名称。
		m_nestPartList.SetItemText(iRowCount, 2, pPart->GetName());

		// 零件尺寸。
		PartCadDataPtr pPartCadData = pPart->GetCadData();
		Rect2D outerRect = pPartCadData->GetPatternLoopList()->GetRectBox();
		CString strPartSize;
		strPartSize.Format(_T("%.3f * %.3f"), outerRect.GetWidth(), outerRect.GetHeight());
		m_nestPartList.SetItemText(iRowCount, 3, strPartSize);

		// 排版优先级。
		CString strPartPriority;
		strPartPriority.Format(INT_FORMAT, NestPriority::MaxVal());
		m_nestPartList.SetItemText(iRowCount, 4, strPartPriority);

		// 排版个数。
		CString strPartCount;
		strPartCount.Format(INT_FORMAT, iPartCount);
		m_nestPartList.SetItemText(iRowCount, 5, strPartCount);

		// 旋转角度。
		CString strRotAng;
		strRotAng = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_FREE);
		m_nestPartList.SetItemText(iRowCount, 6, strRotAng);

		// 外轮廓刀具信息
		const LoopTopologyItemList* pLoopTopologyItems = pPart->GetCadData()->GetLoopTopologyItems();
		LoopCutFeaturePtr pBoundaryLoopFeature = pPart->GetCamDataList()->at(0)->GetBoundaryLoopFeature(pLoopTopologyItems);
		LoopToolDataPtr pLoopTool = pBoundaryLoopFeature->GetLoopTool();
		CString strToolInfo;
		strToolInfo.Format(_T("%.3f(%s)"), pLoopTool->GetToolWidth(), pLoopTool->GetToolName());
		m_nestPartList.SetItemText(iRowCount, 7, strToolInfo);
		/************************************************************************/

		/************************************************************************/
		// select the last row.

		int iCount = m_nestPartList.GetItemCount();

		// de-select the selected rows.
		for (int i = 0; i < iCount; i++)
		{
			if (m_nestPartList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
				m_nestPartList.SetItemState(i, 0, LVIS_SELECTED);
		}

		// select the last row.
		m_nestPartList.EnsureVisible(iCount-1, FALSE);
		m_nestPartList.SetItemState(iCount-1, LVIS_SELECTED, LVIS_SELECTED);
		m_nestPartList.SetFocus();
		/************************************************************************/
	}
}

void NestScheduleDlg::ImportRectPart()
{
	ImpXlsPartDlg dlg;
	dlg.SetConfigParamID(m_iParamConfigID);
	if (dlg.DoModal() == IDOK)
	{
		NestPartListPtr pNestParts = dlg.GetNestParts();

		// 禁止消息。
		m_bFinishAddNestPart = FALSE;

		// 插入每行。
		for (unsigned int i = 0; i < pNestParts->size(); i++)
		{
			NestPartPtr pNestPart = pNestParts->at(i);
			AddNestPartToGrid(pNestPart->GetPartItem(), pNestPart->GetPriority(), pNestPart->GetNestCount(), pNestPart->GetRotStyle());
		}

		// 打开消息。
		m_bFinishAddNestPart = TRUE;

		/************************************************************************/
		// select the last row.

		int iCount = m_nestPartList.GetItemCount();

		// de-select the selected rows.
		for (int i = 0; i < iCount; i++)
		{
			if (m_nestPartList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
				m_nestPartList.SetItemState(i, 0, LVIS_SELECTED);
		}

		// select the last row.
		m_nestPartList.EnsureVisible(iCount-1, FALSE);
		m_nestPartList.SetItemState(iCount-1, LVIS_SELECTED, LVIS_SELECTED);
		m_nestPartList.SetFocus();
		/************************************************************************/
	}
}

void NestScheduleDlg::AddNestPartToGrid(const PartItem* pPartItem, const NestPriority& iNestPriority, int iCount, PartRotateType iRotAng)
{
	int iRowCount = m_nestPartList.GetItemCount();
	m_nestPartList.InsertItem(iRowCount, _T(""));
	m_nestPartList.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pPartItem->GetPart()->GetID())));

	// 序号。
	CString strNum;
	if ((iRowCount+1) < 10)
		strNum.Format(_T("0%d"), iRowCount+1);
	else
		strNum.Format(_T("%d"), iRowCount+1);
	m_nestPartList.SetItemText(iRowCount, 1, strNum);

	// 零件名称。
	m_nestPartList.SetItemText(iRowCount, 2, pPartItem->GetPart()->GetName());

	// 零件尺寸。
	PartCadDataPtr pPartCadData = pPartItem->GetPart()->GetCadData();
	Rect2D outerRect = pPartCadData->GetPatternLoopList()->GetRectBox();
	CString strPartSize;
	strPartSize.Format(_T("%.3f * %.3f"), outerRect.GetWidth(), outerRect.GetHeight());
	m_nestPartList.SetItemText(iRowCount, 3, strPartSize);

	// 排版优先级。
	CString strPartPriority;
	strPartPriority.Format(INT_FORMAT, iNestPriority);
	m_nestPartList.SetItemText(iRowCount, 4, strPartPriority);

	// 排版个数。
	CString strPartCount;
	strPartCount.Format(INT_FORMAT, iCount);
	m_nestPartList.SetItemText(iRowCount, 5, strPartCount);

	// 旋转角度。
	CString strRotAng = NestPart::GetName_ofPartRotateStyle(iRotAng);
	m_nestPartList.SetItemText(iRowCount, 6, strRotAng);

	// 外轮廓刀具信息
	const LoopTopologyItemList* pLoopTopologyItems = pPartItem->GetPart()->GetCadData()->GetLoopTopologyItems();
	LoopCutFeaturePtr pBoundaryLoopFeature = pPartItem->GetCamData()->GetBoundaryLoopFeature(pLoopTopologyItems);
	LoopToolDataPtr pLoopTool = pBoundaryLoopFeature->GetLoopTool();
	CString strToolInfo;
	strToolInfo.Format(_T("%.3f(%s)"), pLoopTool->GetToolWidth(), pLoopTool->GetToolName());
	m_nestPartList.SetItemText(iRowCount, 7, strToolInfo);
}

void NestScheduleDlg::InitMatGridStyle()
{
	CString str;

	// Requires one never uses column 0
	m_nestMatList.InsertHiddenLabelColumn();

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
	CGridColumnTrait* pNUMCol1 = new CGridColumnTraitText;
	m_nestMatList.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNUMCol1);

	// "material name" column
	CGridColumnTrait* pMatThickCol = new CGridColumnTraitText;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
	m_nestMatList.InsertColumnTrait(2, str, LVCFMT_LEFT, 130, 0, pMatThickCol);

	// "width"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_WIDTH);
	CGridColumnTrait* pWidthCol = new CGridColumnTraitText;
	m_nestMatList.InsertColumnTrait(3, str, LVCFMT_LEFT, 90, 0, pWidthCol);

	// "height"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_HEIGHT);
	CGridColumnTrait* pHeightCol = new CGridColumnTraitText;
	m_nestMatList.InsertColumnTrait(4, str, LVCFMT_LEFT, 90, 0, pHeightCol);

	// "count"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAT_TOTAL_NUM);
	CGridColumnTrait* pCountCol = new CGridColumnTraitText;
	m_nestMatList.InsertColumnTrait(5, str, LVCFMT_LEFT, 80, 0, pCountCol);

	// "available number"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAT_AVAILABLE_NUM);
	CGridColumnTrait* pAvailCol = new CGridColumnTraitText;
	m_nestMatList.InsertColumnTrait(6, str, LVCFMT_LEFT, 80, 0, pAvailCol);

	// "matsize type" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MATSIZE_TYPE);
	CGridColumnTrait* pMatSizeTypeCol = new CGridColumnTraitText;
	m_nestMatList.InsertColumnTrait(7, str, LVCFMT_LEFT, 90, 0, pMatSizeTypeCol);
}

void NestScheduleDlg::PreviewNestMat()
{
	// 得到选中项。
	int iIndex = INVALID_IDX;
	for (int i = 0; i < m_nestMatList.GetItemCount(); i++)
	{
		if (m_nestMatList.GetItemState(i, LVIS_SELECTED) & LVIS_SELECTED)
			iIndex = i;
	}

	if (iIndex != INVALID_IDX)
	{
		LONGLONG* pMatSizeID = (LONGLONG*)m_nestMatList.GetItemData(iIndex);
		DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
		IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(pMatLibDBCon, *pMatSizeID);
		if (pMaterialSize)
		{
			// 初始化视口。
			Rect2DPtr pOutRect(new Rect2D(0, pMaterialSize->GetWidth(), 0, pMaterialSize->GetHeight()));
			LineArc2DListPtr pGeom2DList = pOutRect->GetLineS();
			Rect2D geomRect = pGeom2DList->GetRectBox();
			m_matPreviewWnd.FitRect(geomRect);

			// 设置材料外框。
			m_matPreviewWnd.SetSolidGeom2DList(MaterialProcessor::GetBorderLineArcs(pMaterialSize.get()));

			// 绘制材料的已用区域。
			LineArc2DListPtr pUselessRegGeo2D = MaterialProcessor::GetUselessRegions(pMaterialSize.get());
			m_matPreviewWnd.SetDashGeom2DList(pUselessRegGeo2D);

			// 绘制。
			m_matPreviewWnd.OnPaint();
		}
	}
}

void NestScheduleDlg::RlsMatGridData()
{
	for (int i = 0; i < m_nestMatList.GetItemCount(); i++)
	{
		DWORD_PTR pItemData = m_nestMatList.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}
}

BOOL NestScheduleDlg::MatAdded(LONGLONG iMaterialSizeID)
{
	for (int i = 0; i < m_nestMatList.GetItemCount(); i++)
	{
		LONGLONG* pMatSizeID = (LONGLONG*)m_nestMatList.GetItemData(i);
		if (*pMatSizeID == iMaterialSizeID)
			return TRUE;
	}

	return FALSE;
}

NestMaterialListPtr NestScheduleDlg::GetNestMatList()
{
	NestMaterialListPtr pNestMatList(new NestMaterialList);

	// 遍历材料表格。
	for (int i = 0; i < m_nestMatList.GetItemCount(); i++)
	{
		LONGLONG* pMatSizeID = (LONGLONG*)m_nestMatList.GetItemData(i);

		// 从数据库加载材料。
		DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
		IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(pMatLibDBCon, *pMatSizeID);

		// 创建“排版材料”。
		NestMaterialPtr pNestMat(new NestMaterial(pMaterialSize));
		pNestMatList->push_back(pNestMat);
	}

	return pNestMatList;
}

void NestScheduleDlg::AddDflMat()
{
	// 默认的材料。
	DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
	IMaterialSizePtr pMaterialSize = MaterialLibLoader::LoadMatSize(pMatLibDBCon, MaterialLibLoader::GetDflMatSize(pMatLibDBCon));

	// insert a new row.
	AddNestMat(pMaterialSize);
	m_nestMatList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
}

void NestScheduleDlg::AddNestMat(IMaterialSizePtr pMaterialSize)
{
	int iRowCount = m_nestMatList.GetItemCount();
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.iItem = iRowCount;
	lvItem.iSubItem = 0;
	lvItem.pszText = _T("");
	lvItem.lParam = (DWORD_PTR)(new LONGLONG(pMaterialSize->GetID()));
	m_nestMatList.InsertItem(&lvItem);

	// "num" column
	CString strNum;
	if ((iRowCount+1) < 10)
		strNum.Format(_T("0%d"), iRowCount+1);
	else
		strNum.Format(_T("%d"), iRowCount+1);
	m_nestMatList.SetItemText(iRowCount, 1, strNum);

	// "material name" column
	m_nestMatList.SetItemText(iRowCount, 2, pMaterialSize->GetName());

	// "material width" column
	CString strMatWid;
	strMatWid.Format(FLOAT_FORMAT_4, pMaterialSize->GetWidth());
	m_nestMatList.SetItemText(iRowCount, 3, strMatWid);

	// "material height" column
	CString strMatHeight;
	strMatHeight.Format(FLOAT_FORMAT_4, pMaterialSize->GetHeight());
	m_nestMatList.SetItemText(iRowCount, 4, strMatHeight);

	// "total" column
	CString strMatTotal;
	strMatTotal.Format(INT_FORMAT, pMaterialSize->GetTotalNum());
	m_nestMatList.SetItemText(iRowCount, 5, strMatTotal);

	// "available num" column
	CString strAvailable;
	strAvailable.Format(INT_FORMAT, pMaterialSize->GetAvailNum());
	m_nestMatList.SetItemText(iRowCount, 6, strAvailable);

	// “材料类型”列。
	CString strMatSizeType;
	if (pMaterialSize->GetType() == MAT_SIZE_RECT)
		strMatSizeType = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MATSIZE_RECT);
	else if (pMaterialSize->GetType() == MAT_SIZE_POLY)
		strMatSizeType = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MATSIZE_POLY);
	m_nestMatList.SetItemText(iRowCount, 7, strMatSizeType);
}

END_CUTLEADER_NAMESPACE()

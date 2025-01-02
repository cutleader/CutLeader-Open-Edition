#include "StdAfx.h"
#include "ImpXlsPartDlg.h"

#include "libxl.h"
using namespace libxl;

#include <map>
#include "clUtilityResource.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "PathHelper.h"
#include "baseConst.h"
#include "ClFolderHelper.h"
#include "ProductFileDefine.h"
#include "DataBaseManager.h"
#include "clBaseDataResource.h"
#include "SelectDataCenterFolderDlg.h"
#include "DataCenterItemLoader.h"
#include "FolderNode.h"
#include "ExtPropType.h"
#include "ExtPropTypeList.h"
#include "ExtPropVal.h"
#include "ExtPropValList.h"

#include "CLOptionInfo.h"
#include "ImpExpConfig.h"
#include "PartEditParam.h"
#include "CLOptionWriter.h"
#include "CLOptionSheet.h"
#include "LibXlHelper.h"

#include "PatternLoopList.h"
#include "PartCadData.h"
#include "PartItem.h"
#include "PartCamDataList.h"
#include "Part.h"
#include "PartBuilder.h"
#include "PartManager.h"
#include "clDataImportExportResource.h"
#include "NestPart.h"
#include "NestPartList.h"
#include "clNestResource.h"
#include "ImportManager.h"
#include "DxfDwgConvertor.h"
#include "LoopTopologyItemList.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(ImpXlsPartDlg, CDialogEx)

ImpXlsPartDlg::ImpXlsPartDlg(CWnd* pParent /*=NULL*/)
		: CLDialog(ImpXlsPartDlg::IDD, pParent)
{
	m_pNestPartList.reset(new NestPartList);
}

ImpXlsPartDlg::~ImpXlsPartDlg(void)
{
}

BEGIN_MESSAGE_MAP(ImpXlsPartDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_SAVE_PATH, OnSelectFolder)
	ON_BN_CLICKED(IDC_BTN_SEL_XLS, OnSelectXlsFile)
	ON_BN_CLICKED(IDC_BTN_DEL_ROW, OnDeleteRow)
	ON_BN_CLICKED(IDC_CHECK_LOAD_FROM_PATH, OnCheckFromFile)
	ON_BN_CLICKED(IDC_BTN_MGR_EXTPROP, OnMgrExtProp)
	ON_BN_CLICKED(IDOK, OnOK)

	ON_MESSAGE(WM_GET_CELL_EDITOR_DATA, GetCellEditorData)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_EXTPROP_MAP, OnEndLabelEdit)
END_MESSAGE_MAP()

void ImpXlsPartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_XLS_PATH, m_editXLSPath);
	DDX_Control(pDX, IDC_EDIT_START_ROW, m_editStartRow);
	DDX_Text(pDX, IDC_EDIT_SAVE_PATH, m_editSavePath);
	DDX_Control(pDX, IDC_LIST_XLS_DATA, m_listXlsData);
	DDX_Control(pDX, IDC_COMBO_ROT_ANG, m_ctrlRotateAng);

	DDX_Control(pDX, IDC_COMBO_PART_NAME, m_comboPartName);
	DDX_Control(pDX, IDC_COMBO_PART_WIDTH, m_comboPartWidth);
	DDX_Control(pDX, IDC_COMBO_PART_HEIGHT, m_comboPartHeight);
	DDX_Control(pDX, IDC_COMBO_PART_COUNT, m_comboPartCount);
	DDX_Control(pDX, IDC_COMBO_PART_PATH, m_comboPartPath);
	DDX_Check(pDX, IDC_CHECK_LOAD_FROM_PATH, m_bLoadPartFromPath);

	DDX_Control(pDX, IDC_LIST_EXTPROP_MAP, m_listExtProp);
	
	DDX_Control(pDX, IDC_BTN_SEL_XLS, m_btnDelSelXLS);
	DDX_Control(pDX, IDC_BTN_SAVE_PATH, m_btnDelSavePath);
	DDX_Control(pDX, IDC_BTN_DEL_ROW, m_btnDelRow);
	DDX_Control(pDX, IDC_BTN_MGR_EXTPROP, m_btnMgrExtProp);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL ImpXlsPartDlg::OnInitDialog()
{
	__super::OnInitDialog();

	ImpExpConfigPtr pImpExpConfig = CLOptionInfo::GetInstance()->GetImpExpConfig();
	m_editStartRow.Init(pImpExpConfig->GetStartRow(), 0, false);
	
	// 旋转角度。
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_FREE);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_90_INCREMENT);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_0_FIXED);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_90_FIXED);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_180_FIXED);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_270_FIXED);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_HORIZONTAL);
	m_ctrlRotateAng.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_ROTATE_VERTICAL);
	m_ctrlRotateAng.AddString(str);
	m_ctrlRotateAng.SetCurSel(2);

	// 初始化保存目录。
	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();
	m_pSaveFolder = DataCenterItemLoader::LoadProductTree(pMainDBCon);
	m_editSavePath = m_pSaveFolder->GetName();

	// 是否从文件加载零件。
	m_bLoadPartFromPath = pImpExpConfig->IsLoadPartFromPath();
	if (m_bLoadPartFromPath)
	{
		GetDlgItem(IDC_COMBO_PART_PATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PART_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PART_HEIGHT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_PART_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PART_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PART_HEIGHT)->EnableWindow(TRUE);
	}

	/************************************************************************/
	// 初始化“扩展属性”列表。

	// Requires one never uses column 0
	m_listExtProp.InsertHiddenLabelColumn();

	// 属性名称列。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
	CGridColumnTrait* pNameCol = new CGridColumnTraitText;
	m_listExtProp.InsertColumnTrait(1, str, LVCFMT_LEFT, 90, 0, pNameCol);

	// 数据类型列。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DATATYPE);
	CGridColumnTrait* pDataTypeCol = new CGridColumnTraitText;
	m_listExtProp.InsertColumnTrait(2, str, LVCFMT_LEFT, 90, 0, pDataTypeCol);

	// excel列。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLUMN);
	CGridColumnTrait* pExcelColCol = new CGridColumnTraitText;
	m_listExtProp.InsertColumnTrait(3, str, LVCFMT_LEFT, 80, 0, pExcelColCol);

	// we should decide the cell editor for this column at run-time.
	m_listExtProp.AddDynamicCol(3);

	// 显示扩展属性。
	int nItem = 0;
	PartEditParamPtr pPartEditParam = CLOptionInfo::GetInstance()->GetPartEditParam();
	ExtPropTypeListPtr pExtPropTypes = pPartEditParam->GetExtPropTypeList();
	m_listExtProp.DeleteAllItems();
	for (unsigned int i = 0; i < pExtPropTypes->size(); i++)
	{
		ExtPropTypePtr pExtPropType = pExtPropTypes->at(i);
		nItem = m_listExtProp.InsertItem(++nItem, _T(""));
		m_listExtProp.SetItemText(nItem, 1, pExtPropType->GetName());
		m_listExtProp.SetItemText(nItem, 2, pExtPropType->GetDataTypeName());
		m_listExtProp.SetItemText(nItem, 3, _T(""));
	}
	/************************************************************************/

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_RECT_PARTS_IN_EXCEL )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_LIST_XLS_DATA )->GetSafeHwnd(), L"", L"" );	
	SetWindowTheme( GetDlgItem( IDC_COMBO_ROT_ANG )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_EXCEL_COLUMN_MAPPING )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_EXTEND_PROP_MAPPING )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_LOAD_FROM_PATH )->GetSafeHwnd(), L"", L"" );

	SetWindowTheme( GetDlgItem( IDC_COMBO_PART_NAME )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_PART_WIDTH )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_PART_HEIGHT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_PART_COUNT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_PART_PATH )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

void ImpXlsPartDlg::OnSelectFolder()
{
	SelectDataCenterFolderDlg dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		m_pSaveFolder = dlg.GetSelectFolder();

		// get the path of the folder.
		CString strPath = m_pSaveFolder->GetItemPath();
		strPath += _T("\\");
		strPath += m_pSaveFolder->GetName();

		m_editSavePath = strPath;
		UpdateData(FALSE);
	}
}

HBRUSH ImpXlsPartDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ImpXlsPartDlg::OnSelectXlsFile()
{
	CString strFilter;
	strFilter = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPORT_XLS_EXT);
	CFileDialog openFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter);
	if (openFileDlg.DoModal() == IDOK)
	{
		CString strFilePath = openFileDlg.GetPathName();
		m_editXLSPath = strFilePath;
		UpdateData(FALSE);

		// 把excel文件中数据显示在列表中。
		InitXlsDataList(strFilePath);

		// 初始化映射关系。
		if (m_listXlsData.GetItemCount() > 0)
		{
			// 初始化可用的列名。
			m_comboPartName.ResetContent();
			m_comboPartWidth.ResetContent();
			m_comboPartHeight.ResetContent();
			m_comboPartCount.ResetContent();
			m_comboPartPath.ResetContent();
			vector<CString> columeNames = GetColumnNames();
			for (unsigned int i = 0; i < columeNames.size(); i++)
			{
				m_comboPartName.AddString(columeNames.at(i));
				m_comboPartWidth.AddString(columeNames.at(i));
				m_comboPartHeight.AddString(columeNames.at(i));
				m_comboPartCount.AddString(columeNames.at(i));
				m_comboPartPath.AddString(columeNames.at(i));
			}

			// 设置选中的列名。
			ImpExpConfigPtr pImpExpConfig = CLOptionInfo::GetInstance()->GetImpExpConfig();
			if (m_comboPartName.GetCount() >= pImpExpConfig->GetPartNameCol())
				m_comboPartName.SetCurSel(pImpExpConfig->GetPartNameCol()-1);
			else
				m_comboPartName.SetCurSel(0);
			if (m_comboPartWidth.GetCount() >= pImpExpConfig->GetPartWidthCol())
				m_comboPartWidth.SetCurSel(pImpExpConfig->GetPartWidthCol()-1);
			else
				m_comboPartWidth.SetCurSel(0);
			if (m_comboPartHeight.GetCount() >= pImpExpConfig->GetPartHeightCol())
				m_comboPartHeight.SetCurSel(pImpExpConfig->GetPartHeightCol()-1);
			else
				m_comboPartHeight.SetCurSel(0);
			if (m_comboPartCount.GetCount() >= pImpExpConfig->GetPartCountCol())
				m_comboPartCount.SetCurSel(pImpExpConfig->GetPartCountCol()-1);
			else
				m_comboPartCount.SetCurSel(0);
			if (m_comboPartPath.GetCount() >= pImpExpConfig->GetPartPathCol())
				m_comboPartPath.SetCurSel(pImpExpConfig->GetPartPathCol()-1);
			else
				m_comboPartPath.SetCurSel(0);
		}
	}
}

void ImpXlsPartDlg::OnDeleteRow()
{
	for (int i = 0; i < m_listXlsData.GetItemCount(); i++)
	{
		if (m_listXlsData.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			// delete the item.
			m_listXlsData.DeleteItem(i);
			i--;
		}
	}

	// 重新设置行号。
	for (int i = 0; i < m_listXlsData.GetItemCount(); i++)
	{
		CString strNum;
		if ((i+1) < 10)
			strNum.Format(_T("0%d"), i+1);
		else
			strNum.Format(_T("%d"), i+1);
		m_listXlsData.SetItemText(i, 0, strNum);
	}
}

void ImpXlsPartDlg::OnCheckFromFile()
{
	UpdateData(TRUE);

	if (m_bLoadPartFromPath)
	{
		GetDlgItem(IDC_COMBO_PART_PATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PART_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PART_HEIGHT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_PART_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PART_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PART_HEIGHT)->EnableWindow(TRUE);
	}
}

void ImpXlsPartDlg::OnMgrExtProp()
{
	CLOptionSheet clOptionSheet;
	clOptionSheet.InitStartPage(CONFIG_PART_EDIT);
	clOptionSheet.DoModal();

	// 从列表删除一些扩展属性。
	PartEditParamPtr pPartEditParam = CLOptionInfo::GetInstance()->GetPartEditParam();
	ExtPropTypeListPtr pExtPropTypes = pPartEditParam->GetExtPropTypeList();
	for (int i = 0; i < m_listExtProp.GetItemCount(); i++)
	{
		CString strExtPropName = m_listExtProp.GetItemText(i, 1);
		if (!pExtPropTypes->GetItemByName(strExtPropName))
		{
			m_listExtProp.DeleteItem(i);
			i--;
		}
	}

	// 增加新的扩展属性到列表。
	for (unsigned int i = 0; i < pExtPropTypes->size(); i++)
	{
		ExtPropTypePtr pExtPropType = pExtPropTypes->at(i);

		// 这个扩展属性是不是新加的。
		BOOL bExist = FALSE;
		for (int j = 0; j < m_listExtProp.GetItemCount(); j++)
		{
			CString strExtPropName = m_listExtProp.GetItemText(i, 1);
			if (pExtPropType->GetName() == strExtPropName)
			{
				bExist = TRUE;
				break;
			}
		}

		// 添加新行。
		if (!bExist)
		{
			int nItem = m_listExtProp.GetItemCount();
			nItem = m_listExtProp.InsertItem(++nItem, _T(""));
			m_listExtProp.SetItemText(nItem, 1, pExtPropType->GetName());
			m_listExtProp.SetItemText(nItem, 2, pExtPropType->GetDataTypeName());
			m_listExtProp.SetItemText(nItem, 3, _T(""));
		}
	}
}

void ImpXlsPartDlg::OnOK()
{
	UpdateData(TRUE);
	CString str, strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// 列表为空时。
	if (m_listXlsData.GetItemCount() == 0)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SEL_VALID_XLS);
		MessageBox(str, strProductName, MB_ICONWARNING);
		return;
	}

	// 起始行。
	int iStartRow = 0;
	if ((int)m_editStartRow.Get_Number() > 0)
		iStartRow = (int)m_editStartRow.Get_Number()-1;

	// 得到所有零件名称，宽度，高度，数量。
	vector<CString> partNames;
	vector<CString> partPaths;
	vector<double> partWidths, partHeights;
	vector<int> partCounts;
	int iPartNameCol = m_comboPartName.GetCurSel();
	int iPartWidthCol = m_comboPartWidth.GetCurSel();
	int iPartHeightCol = m_comboPartHeight.GetCurSel();
	int iPartCountCol = m_comboPartCount.GetCurSel();
	int iPartPathCol = m_comboPartPath.GetCurSel();
	for (int i = iStartRow; i < m_listXlsData.GetItemCount(); i++)
	{
		CString strRow;
		strRow.Format(_T("%d"), i+1);

		// 零件名称。
		CString strData = m_listXlsData.GetItemText(i, iPartNameCol+1);
		if (strData.GetLength() > 0)
			partNames.push_back(strData);
		else
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PART_NAME_COL_ERROR);
			str.Replace(_T("WILLBEREPLACED1"), strRow);
			MessageBox(str, strProductName, MB_ICONWARNING);
			return;
		}

		if (m_bLoadPartFromPath)
		{
			// 零件路径。
			CString strData = m_listXlsData.GetItemText(i, iPartPathCol+1);
			CFileFind fileFind;
			if (fileFind.FindFile(strData))
				partPaths.push_back(strData);
			else
			{
				str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PART_PATH_COL_ERROR);
				str.Replace(_T("WILLBEREPLACED1"), strRow);
				MessageBox(str, strProductName, MB_ICONWARNING);
				return;
			}
		}
		else
		{
			// 零件宽度。
			strData = m_listXlsData.GetItemText(i, iPartWidthCol+1);
			double dWidth = _wtof(strData);
			if (dWidth > .0)
				partWidths.push_back(dWidth);
			else
			{
				str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PART_WIDTH_COL_ERROR);
				str.Replace(_T("WILLBEREPLACED1"), strRow);
				MessageBox(str, strProductName, MB_ICONWARNING);
				return;
			}

			// 零件高度。
			strData = m_listXlsData.GetItemText(i, iPartHeightCol+1);
			double dHeight = _wtof(strData);
			if (dHeight > .0)
				partHeights.push_back(dHeight);
			else
			{
				str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PART_HEIGHT_COL_ERROR);
				str.Replace(_T("WILLBEREPLACED1"), strRow);
				MessageBox(str, strProductName, MB_ICONWARNING);
				return;
			}
		}

		// 零件数量。
		strData = m_listXlsData.GetItemText(i, iPartCountCol+1);
		int iCount = _wtoi(strData);
		if (iCount > 0)
			partCounts.push_back(iCount);
		else
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_PART_COUNT_COL_ERROR);
			str.Replace(_T("WILLBEREPLACED1"), strRow);
			MessageBox(str, strProductName, MB_ICONWARNING);
			return;
		}
	}

	// 检查有没有可导入的数据。
	if (partNames.size() == 0)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NO_DATA_IMPORT);
		MessageBox(str, strProductName, MB_ICONWARNING);
		return;
	}

	// 保存每个零件并创建“待排零件”对象。
	vector<ExtPropValListPtr> extPropVals = GetPartExtProps(iStartRow); // 得到所有零件的扩展属性。
	for (unsigned int i = 0; i < partNames.size(); i++)
	{
		// 创建零件并保存。
		PartPtr pPart;
		if (m_bLoadPartFromPath)
		{
			// dxf还是dwg文件。
			BOOL bDwg = TRUE;
			CString strExt = PathHelper::GetFileExt(partPaths.at(i));
			CString strTmp;
			strTmp = MultiLanguageMgr::GetInstance()->TranslateString(IDS_IMPEXP_DXF_EXT);
			if (strExt.CompareNoCase(strTmp) == 0)
				bDwg = FALSE;

			// 加载零件。
			PartCadDataPtr pPartCadData = DxfDwgConvertor::DxfDwg2CadData(partPaths.at(i), bDwg);
			if (pPartCadData->GetLoopTopologyItems()->GetBoundaryLoop(pPartCadData->GetPatternLoopList().get()) != INVALID_IDX)
			{
				UINT iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
				pPart = PartBuilder::BuildPart(partNames.at(i), pPartCadData, m_iParamConfigID, iAssignLimit, TRUE);
			}
			else
			{
				continue;
			}
		}
		else
		{
			double dWidth = partWidths.at(i);
			double dHeight = partHeights.at(i);
			UINT iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
			pPart = PartBuilder::BuildRectPart(partNames.at(i), dWidth, dHeight, m_iParamConfigID, iAssignLimit);
		}
		pPart->SetExtPropValList(extPropVals[i]);
		PartManager::SaveNewPart(pPart, m_pSaveFolder, FALSE);

		// 创建待排零件。
		PartItemPtr pPartItem(new PartItem(pPart, pPart->GetCamDataList()->at(0)));
		NestPartPtr pNestPart(new NestPart(pPartItem));
		pNestPart->SetNestCount(partCounts.at(i));
		pNestPart->SetPriority(NestPriority::MaxVal());
		pNestPart->SetRotStyle((PartRotateType)(m_ctrlRotateAng.GetCurSel()));
		m_pNestPartList->push_back(pNestPart);
	}

	// 保存映射关系。
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	ImpExpConfigPtr pImpExpConfig = pCLOptionInfo->GetImpExpConfig();
	pImpExpConfig->SetStartRow((int)m_editStartRow.Get_Number());
	pImpExpConfig->SetPartNameCol(m_comboPartName.GetCurSel()+1);
	pImpExpConfig->SetPartWidthCol(m_comboPartWidth.GetCurSel()+1);
	pImpExpConfig->SetPartHeightCol(m_comboPartHeight.GetCurSel()+1);
	pImpExpConfig->SetPartCountCol(m_comboPartCount.GetCurSel()+1);
	pImpExpConfig->SetPartPathCol(m_comboPartPath.GetCurSel()+1);
	pImpExpConfig->IsLoadPartFromPath(m_bLoadPartFromPath);
	CString strPath = ClFolderHelper::GetOptionPath();
	strPath += USER_OPTION_FILE;
	CLOptionWriter::SaveCLOption(pCLOptionInfo, strPath);

	CDialogEx::OnOK();
}

LRESULT ImpXlsPartDlg::GetCellEditorData(WPARAM wParam, LPARAM lParam)
{
	CellEditorData* pCellEditorData = reinterpret_cast<CellEditorData*>(wParam);
	if (pCellEditorData->hwnd == m_listExtProp.m_hWnd)
	{
		pCellEditorData->m_cellEditorType = COMBOBOX;
		pCellEditorData->m_optionValList.clear();
		pCellEditorData->m_optionValList.push_back(EMPTY_STRING);
		vector<CString> columeNames = GetColumnNames();
		for (unsigned int i = 0; i < columeNames.size(); i++)
			pCellEditorData->m_optionValList.push_back(columeNames.at(i));
	}

	return true;
}

void ImpXlsPartDlg::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);
	
	// 暂不需要做什么。
}

void ImpXlsPartDlg::InitXlsDataList(CString strXlsPath)
{
	// 清除列表。
	m_listXlsData.DeleteAllItems();
	int iCurColNum = m_listXlsData.GetColumnCount();
	for (int i = 0; i < iCurColNum; i++)
		m_listXlsData.DeleteColumn(0);

	// 创建book对象。
	Book* pBook;
	CString strExt = PathHelper::GetFileExt(strXlsPath);
	if (strExt.CompareNoCase(_T(".xls")) == 0)
		pBook = xlCreateBook();
	else if (strExt.CompareNoCase(_T(".xlsx")) == 0)
		pBook = xlCreateXMLBook();
    LibXlHelper::Init(pBook);

	// 加载并显示excel文件中数据。
	if (pBook->load(strXlsPath))
	{
		libxl::Sheet* pSheet = pBook->getSheet(0);
		if (pSheet)
		{
			// 表的行数和列数。
			int iStartColumn = pSheet->firstCol();
			int iColumnCount = pSheet->lastCol();
			int iStartRow = pSheet->firstRow();
			int iRowCount = pSheet->lastRow();

			// 添加Num列。
			int iColIdx = 0;
			CString strNum;
			strNum = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
			m_listXlsData.InsertColumn(iColIdx, strNum, LVCFMT_LEFT, 50);
			iColIdx++;

			// 添加每个列头。
			for (int i = iStartColumn; i < iColumnCount; i++)
			{
				CString str, strColumnName;
				str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COLUMN);
				strColumnName.Format(_T("%s %d"), str, i+1);
				m_listXlsData.InsertColumn(iColIdx, strColumnName, LVCFMT_LEFT, 100);
				iColIdx++;
			}

			// 插入每行数据。
			int iRow = 0;
			for (int i = iStartRow; i < iRowCount; i++)
			{
				m_listXlsData.InsertItem(iRow, _T(""));

				// 设置行号。
				int iCol = 0;
				CString strNum;
				if ((iRow+1) < 10)
					strNum.Format(_T("0%d"), iRow+1);
				else
					strNum.Format(_T("%d"), iRow+1);
				m_listXlsData.SetItemText(iRow, iCol, strNum);
				iCol++;

				// 这一行的内容。
				for (int j = iStartColumn; j < iColumnCount; j++)
				{
					CString strData;
					CellType iCellType = pSheet->cellType(i, j);
					if (iCellType == CELLTYPE_NUMBER)
						strData.Format(FLOAT_FORMAT_2, pSheet->readNum(i, j));
					else if (iCellType == CELLTYPE_STRING)
						strData = pSheet->readStr(i, j);
					m_listXlsData.SetItemText(iRow, iCol, strData);
					iCol++;
				}

				iRow++;
			}
		}
	}

	pBook->release();
}

vector<CString> ImpXlsPartDlg::GetColumnNames()
{
	vector<CString> columeNames;

	for (int i = 1; i < m_listXlsData.GetColumnCount(); i++)
	{
		CString strColumnName = m_listXlsData.GetColumnHeading(i);
		columeNames.push_back(strColumnName);
	}

	return columeNames;
}

vector<ExtPropValListPtr> ImpXlsPartDlg::GetPartExtProps(int iStartRow)
{
	vector<ExtPropValListPtr> partExtProps;

	// 列名和列的对应<列名，第几列>。
	map<CString, int> colNameMap;
	vector<CString> columeNames = GetColumnNames();
	for (unsigned int i = 0; i < columeNames.size(); i++)
		colNameMap[columeNames.at(i)] = i+1; // 第一列用不着。

	// 扩展属性和列的对应<列名，扩展属性>。
	map<CString, ExtPropTypePtr> extPropMap;
	PartEditParamPtr pPartEditParam = CLOptionInfo::GetInstance()->GetPartEditParam();
	ExtPropTypeListPtr pExtPropTypes = pPartEditParam->GetExtPropTypeList();
	for (int i = 0; i < m_listExtProp.GetItemCount(); i++)
	{
		CString strCol = m_listExtProp.GetItemText(i, 3);
		if (strCol.GetLength() > 0)
		{
			CString strExtPropName = m_listExtProp.GetItemText(i, 1);
			ExtPropTypePtr pExtPropType = pExtPropTypes->GetItemByName(strExtPropName);
			extPropMap[strCol] = pExtPropType;
		}
	}

	// 每个零件的扩展属性值。
	for (int i = iStartRow; i < m_listXlsData.GetItemCount(); i++)
	{
		// 这个零件的各个扩展属性。
		ExtPropValListPtr pExtPropVals(new ExtPropValList);
		for (map<CString, ExtPropTypePtr>::iterator iter = extPropMap.begin(); iter != extPropMap.end(); iter++)
		{
			CString strCol = iter->first;
			int iColIndex = colNameMap[strCol];

			// 创建属性值。
			ExtPropValPtr pExtPropVal(new ExtPropVal);
			pExtPropVal->SetExtPropType(iter->second);
			_bstr_t strVal = m_listXlsData.GetItemText(i, iColIndex);
			pExtPropVal->SetVal(strVal);
			pExtPropVals->push_back(pExtPropVal);
		}
		partExtProps.push_back(pExtPropVals);
	}

	return partExtProps;
}

END_CUTLEADER_NAMESPACE()

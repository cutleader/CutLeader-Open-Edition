#include "StdAfx.h"
#include "Loop2PartDlg.h"

#include "baseConst.h"
#include "ClUtilityResource.h"
#include "CGridColumnTraitText.h"
#include "LineArc2DList.h"
#include "ClBaseDataResource.h"
#include "PathHelper.h"

#include "CLOptionInfo.h"
#include "ImpExpConfig.h"

#include "LoopProcessor.h"
#include "PatternLoopList.h"
#include "PartTopologyItemList.h"
#include "PartBuilder.h"
#include "ClPartResource.h"
#include "PartsFromAsmDxf.h"
#include "PartList.h"
#include "PartCadData.h"
#include "PartManager.h"
#include "LoopTopologyItemList.h"
#include "TextStruct.h"
#include "TextStructList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(Loop2PartDlg, CDialogEx)

Loop2PartDlg::Loop2PartDlg()
	: CLDialog(Loop2PartDlg::IDD)
{
	m_strMsg_1 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_LOOP2PART_MSG_1);
	m_bSelectAll = TRUE;
	m_bRecognizeSamePart = TRUE;
	m_bImportAsSinglePart = FALSE;
	m_bEnableEvent = FALSE;

	m_pPartsFromAsmDxf.reset(new PartsFromAsmDxf);
}

Loop2PartDlg::~Loop2PartDlg(void)
{
}

void Loop2PartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MSG_1, m_strMsg_1);
	DDX_Control(pDX, IDC_LIST_PART, m_partGrid);
	DDX_Check(pDX, IDC_CHECK_SEL_ALL, m_bSelectAll);
	DDX_Check(pDX, IDC_CHECK_RECOGNIZE_SAME_PART, m_bRecognizeSamePart);
	DDX_Check(pDX, IDC_CHECK_ImportAsSinglePart, m_bImportAsSinglePart);
	DDX_Control(pDX, IDC_STATIC_PREVIEW_8, m_previewWnd);

	DDX_Control(pDX, IDC_BTN_ACCEPT, m_btnOK);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(Loop2PartDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PART, OnItemchangedList)
	ON_BN_CLICKED(IDC_CHECK_SEL_ALL, OnSelectAll)
	ON_BN_CLICKED(IDC_CHECK_RECOGNIZE_SAME_PART, OnRecognizeSamePart)
	ON_BN_CLICKED(IDC_CHECK_ImportAsSinglePart, OnImportAsSinglePart)

	ON_BN_CLICKED(IDC_BTN_ACCEPT, OnAccept)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnCancel)
END_MESSAGE_MAP()

HBRUSH Loop2PartDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void Loop2PartDlg::OnDestroy()
{
	// 释放视口。
	m_previewWnd.ReleaseViewPort();

	CDialogEx::OnDestroy();
}

void Loop2PartDlg::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!m_bEnableEvent)
		return;

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int item = pNMLV->iItem;
	if (item == -1)
		return;

	// 零件ID。
	LONGLONG* pItemData = (LONGLONG*)m_partGrid.GetItemData(item);
	if (!pItemData)
		return;

	// 绘制选中零件。
	for (unsigned int i = 0; i < m_partList.size(); i++)
	{
		PatternLoopListPtr pPatternLoopList = get<1>(m_partList[i]);
		if (pPatternLoopList->GetID() == *pItemData)
		{
			LineArc2DListPtr pSelGeom2DList = pPatternLoopList->Calculate2DLineArcs();
			m_previewWnd.SetSelGeom2DList(pSelGeom2DList);
			m_previewWnd.Invalidate();
			break;
		}
	}

	*pResult = 0;
}

void Loop2PartDlg::OnSelectAll()
{
	UpdateData(TRUE);

	// 禁止消息。
	m_bEnableEvent = FALSE;

	for (int i = 0; i < m_partGrid.GetItemCount(); i++)
	{
		if (m_bSelectAll)
			m_partGrid.SetCheck(i, TRUE);
		else
			m_partGrid.SetCheck(i, FALSE);
	}

	// 打开消息。
	m_bEnableEvent = TRUE;
}

void Loop2PartDlg::OnRecognizeSamePart()
{
	UpdateData(TRUE);
}

void Loop2PartDlg::OnImportAsSinglePart()
{
	UpdateData(TRUE);

	if (m_bImportAsSinglePart)
	{
		GetDlgItem(IDC_LIST_PART)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SEL_ALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_RECOGNIZE_SAME_PART)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_LIST_PART)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SEL_ALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_RECOGNIZE_SAME_PART)->EnableWindow(TRUE);
	}
}

void Loop2PartDlg::OnAccept()
{
	UpdateData(TRUE);

	PartListPtr pPartList(new PartList);
	map<LONGLONG, vector<pair<long long, Matrix2D>>> matrixData_of_sameShapePart;
	if (m_bImportAsSinglePart) // 不拆分，作为一个零件导入
	{
		UINT iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
		PartPtr pPart = PartBuilder::BuildPart(PathHelper::GetFileName(m_strDxfDwgPath), m_pPartCadData, m_iParamConfigID, iAssignLimit, TRUE);
		pPartList->push_back(pPart);
	}
	else
	{
		vector<tuple<CString, PatternLoopListPtr, LoopTopologyItemListPtr, TextStructListPtr>> selectedPartList;
		for (int i = 0; i < m_partGrid.GetItemCount(); i++)
		{
			if (m_partGrid.GetCheck(i))
			{
				LONGLONG* pPartID = (LONGLONG*)m_partGrid.GetItemData(i);
				for (unsigned int j = 0; j < m_partList.size(); j++)
				{
					if (get<1>(m_partList.at(j))->GetID() == *pPartID)
					{
						CString strPartName = get<0>(m_partList.at(j));
						PatternLoopListPtr pPatternLoopList = get<1>(m_partList.at(j));
						LoopTopologyItemListPtr pLoopTopologyItems = get<2>(m_partList.at(j));
						TextStructListPtr pTextStructs = get<3>(m_partList.at(j));
						selectedPartList.push_back(make_tuple(strPartName, pPatternLoopList, pLoopTopologyItems, pTextStructs));
						break;
					}
				}
			}
		}

		if (selectedPartList.size() == 0)
		{
			CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_LOOP2PART_SEL_PART);
			MessageBox(str, strProductName, MB_OK | MB_ICONWARNING);
			return;
		}

		// 创建所有零件
		for (unsigned int i = 0; i < selectedPartList.size(); i++)
		{
			const tuple<CString, PatternLoopListPtr, LoopTopologyItemListPtr, TextStructListPtr>& part = selectedPartList.at(i);
			PatternLoopListPtr pPatternLoops = get<1>(part);
			LoopTopologyItemListPtr pLoopTopologyItems = get<2>(part);
			TextStructListPtr pTextStructs = get<3>(part);
			PartCadDataPtr pPartCadData(new PartCadData(pPatternLoops->GetPatList(), pPatternLoops, pLoopTopologyItems));
			pPartCadData->SetTextStructs(pTextStructs);
			UINT iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
			PartPtr pPart = PartBuilder::BuildPart(get<0>(part), pPartCadData, m_iParamConfigID, iAssignLimit, TRUE);
			pPartList->push_back(pPart);
		}

		// 是否识别相同的零件
		if (m_bRecognizeSamePart)
		{
			pPartList = PartManager::MergeCadData_of_sameShapePart(pPartList.get(), matrixData_of_sameShapePart);
		}
	}

	// 初始化返回的数据。
	m_pPartsFromAsmDxf->SetPartList(pPartList);
	m_pPartsFromAsmDxf->SetMatrixsOfSameShapePart(matrixData_of_sameShapePart);

	EndDialog(IDC_BTN_ACCEPT);
}

void Loop2PartDlg::OnCancel()
{
	EndDialog(IDC_BTN_CANCEL);
}

BOOL Loop2PartDlg::OnInitDialog()
{
	__super::OnInitDialog();

	GetDlgItem(IDC_EDIT_DxfDwgFullPath)->SetWindowText(m_strDxfDwgPath);

	// 识别出的零件集合。
	vector<pair<PatternLoopListPtr, LoopTopologyItemListPtr>> partList;
	PartCadDataPtr pPartCadData_new = PartBuilder::CloneCadData(m_pPartCadData.get());
	LoopTopologyItemListPtr pLoopTopItemList = LoopProcessor::CalcLoopTopStruct(pPartCadData_new->GetPatternLoopList().get());
	PartTopologyItemListPtr pPartTopItemList(new PartTopologyItemList);
	PartBuilder::CalcPartTopStruct_2(pLoopTopItemList.get(), pPartTopItemList);
	pPartTopItemList->GetAllParts(partList);
	for (unsigned int i = 0; i < partList.size(); i++)
	{
		PatternLoopListPtr pPatternLoopList = partList[i].first;
		LoopTopologyItemListPtr pLoopTopologyItems = partList[i].second;

		// 搜集哪些文字对象要放在这个零件中。
		TextStructListPtr pTextStructs(new TextStructList);
		for (unsigned int j = 0; j < pPartCadData_new->GetTextStructs()->size(); j++)
		{
			TextStructPtr pTextStruct = pPartCadData_new->GetTextStructs()->operator[](j);
			const PatternLoopList* pInvolvedPatternLoops = pTextStruct->GetInvolvedPatternLoops().get();
			bool bTextInThisPart = true;
			for (unsigned int k = 0; k < pInvolvedPatternLoops->size(); k++)
			{
				if (!pPatternLoopList->GetLoopByID(pInvolvedPatternLoops->operator[](k)->GetID()))
				{
					bTextInThisPart = false;
					break;
				}
			}
			if (bTextInThisPart)
				pTextStructs->push_back(pTextStruct);
		}

		CString strPartName;
		strPartName.Format(_T("%s-%d"), PathHelper::GetFileName(m_strDxfDwgPath), i + 1);
		m_partList.push_back(make_tuple(strPartName, pPatternLoopList, pLoopTopologyItems, pTextStructs));
	}

	// 初始化预览窗口。
	{
		// 从所有回路得到的基本图形集合。
		LineArc2DListPtr pLineArc2DList(new LineArc2DList);
		for (unsigned int i = 0; i < partList.size(); i++)
		{
			PatternLoopListPtr pPatternLoopList = partList[i].first;
			LineArc2DListPtr pTmpLineArc2Ds = pPatternLoopList->Calculate2DLineArcs();
			pLineArc2DList->insert(pLineArc2DList->end(), pTmpLineArc2Ds->begin(), pTmpLineArc2Ds->end());
		}

		// 初始化视口。
		Rect2D geomRect = pLineArc2DList->GetRectBox();
		m_previewWnd.InitViewPort(geomRect);
		m_previewWnd.SetSolidGeom2DList(pLineArc2DList);
	}

	// 初始化表格。
	InitGrid();

	m_bEnableEvent = TRUE;

	SetBackgroundColor(SkinManager::GetBackgroundColor());

	SetWindowTheme(GetDlgItem(IDC_STATIC_RECOGNIZED_PART)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_STATIC_PREVIEW_12)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_CHECK_SEL_ALL)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_CHECK_RECOGNIZE_SAME_PART)->GetSafeHwnd(), L"", L"");

	return TRUE;
}

void Loop2PartDlg::Init(PartCadDataPtr pPartCadData, CString strDxfDwgPath, LONGLONG iParamConfigID)
{
	m_pPartCadData = pPartCadData;
	m_strDxfDwgPath = strDxfDwgPath;
	m_iParamConfigID = iParamConfigID;
}

void Loop2PartDlg::InitGrid()
{
	CString str;

	// 初始化样式。
	{
		// place check box at first column.
		CGridColumnTrait* pStatusCol = new CGridColumnTraitText;
		m_partGrid.InsertColumnTrait(0, _T(""), LVCFMT_LEFT, 30, 0, pStatusCol);

		// "num" column
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
		CGridColumnTrait* pNUMCol = new CGridColumnTraitText;
		m_partGrid.InsertColumnTrait(1, str, LVCFMT_LEFT, 50, 0, pNUMCol);

		// "part name" column
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
		CGridColumnTrait* pNameCol = new CGridColumnTraitText;
		m_partGrid.InsertColumnTrait(2, str, LVCFMT_LEFT, 110, 0, pNameCol);

		// “零件尺寸”列。
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_SIZE);
		CGridColumnTrait* pPartSizeCol = new CGridColumnTraitText;
		m_partGrid.InsertColumnTrait(3, str, LVCFMT_LEFT, 140, 0, pPartSizeCol);

		// set other style.
		m_partGrid.SetExtendedStyle(LVS_EX_CHECKBOXES | m_partGrid.GetExtendedStyle());
		m_partGrid.EnableVisualStyles(TRUE);
	}

	// 初始化数据。
	for (unsigned int i = 0; i < m_partList.size(); i++)
	{
		// insert a new row.
		LVITEM lvItem;
		lvItem.mask = LVIF_TEXT | LVIF_PARAM;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.pszText = _T("");
		m_partGrid.InsertItem(&lvItem);

		// "num" column
		CString strNum;
		strNum.Format(_T("%d"), i + 1);
		m_partGrid.SetItemText(i, 1, strNum);

		// 零件名称。
		CString strPartName;
		strPartName.Format(_T("%s-%d"), PathHelper::GetFileName(m_strDxfDwgPath), i + 1);
		m_partGrid.SetItemText(i, 2, get<0>(m_partList.at(i)));

		// 零件尺寸。
		PatternLoopListPtr pPatternLoopList = get<1>(m_partList.at(i));
		Rect2D outerRect = pPatternLoopList->GetRectBox();
		CString strPartSize;
		strPartSize.Format(_T("%.3f * %.3f"), outerRect.GetWidth(), outerRect.GetHeight());
		m_partGrid.SetItemText(i, 3, strPartSize);

		// 设置数据。
		m_partGrid.SetItemData(i, (DWORD_PTR)(new LONGLONG(get<1>(m_partList.at(i))->GetID())));

		// 选中该行。
		m_partGrid.SetCheck(i, TRUE);
	}
}

END_CUTLEADER_NAMESPACE()

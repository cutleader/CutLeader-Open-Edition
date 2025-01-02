#include "stdafx.h"
#include "TaskPartsNestResultDlg.h"

#include "baseConst.h"
#include "ClUtilityResource.h"
#include "LineArc2DList.h"
#include "StringUtil.h"
#include "ClBaseDataResource.h"
#include "LogMgr.h"

#include "NestConfigItem.h"
#include "ClMaterialLibResource.h"
#include "IMaterialSize.h"
#include "MaterialProcessor.h"
#include "MaterialSizeList.h"
#include "CLOptionInfo.h"
#include "ProductParam.h"
#include "PolygonMaterial.h"

#include "ClNestResource.h"
#include "LibraryData.h"
#include "NestJobMgr.h"
#include "NestJob.h"
#include "SheetList.h"
#include "Sheet.h"
#include "SheetMaterialList.h"
#include "SheetMaterial.h"
#include "AllResultDlg.h"
#include "NestProcessor.h"
#include "ClTaskResource.h"
#include "NestTask.h"
#include "TaskItem.h"
#include "NestPartList.h"
#include "NestPart.h"
#include "PartItem.h"
#include "Part.h"
#include "TaskPart.h"
#include "ClSheetResource.h"
#include "Helper_4_NestingTaskParts.h"
#include "TaskPartsNestResult.h"
#include "NestResultOnSheetMaterial.h"
#include "NestResultListOnSheetMaterial.h"
#include "PartPlacementList.h"
#include "PartPlacementManager.h"


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(TaskPartsNestResultDlg, CDialogEx)

BEGIN_MESSAGE_MAP(TaskPartsNestResultDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
	ON_WM_TIMER()

    ON_WM_MOUSEWHEEL()
    ON_WM_MOUSEMOVE()
    ON_WM_MBUTTONDOWN()

    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ExistingSheets, OnSelchangedExistingSheet)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_NewSheets, OnSelchangedNewSheet)

    ON_BN_CLICKED(IDC_BTN_AllResult, OnAllResult)
    ON_BN_CLICKED(IDC_BTN_STOP, OnStopNest)

	ON_BN_CLICKED(IDC_BTN_AcceptResult, OnAccept)
    ON_BN_CLICKED(IDC_BTN_CANCEL, OnQuit)
END_MESSAGE_MAP()

void TaskPartsNestResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_LIST_NestedParts, m_nestedPartsGrid);
    DDX_Control(pDX, IDC_LIST_ExistingSheets, m_existingSheetsGrid);
	DDX_Control(pDX, IDC_LIST_NewSheets, m_newSheetsGrid);
    DDX_Text(pDX, IDC_EDIT_STATUS, m_strStatus);
    DDX_Text(pDX, IDC_EDIT_RET_RST_NUM, m_strRetRstNum);
    DDX_Control(pDX, IDC_PIC_PROGRESS, m_ctrlTaskProgress);
	DDX_Control(pDX, IDC_STATIC_SheetPreview, m_previewWnd);

    DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
    DDX_Control(pDX, IDC_BTN_AllResult, m_btnAllResult);
    DDX_Control(pDX, IDC_BTN_AcceptResult, m_btnOK);
    DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
}

TaskPartsNestResultDlg::TaskPartsNestResultDlg(CWnd* pParent /*=NULL*/)
				: CLDialog(TaskPartsNestResultDlg::IDD, pParent)
{
	m_iRetRstNum = 0;
	m_pNestResult.reset(new NestResult);
    m_bDisableGridSelectChangeEvent = FALSE;
}

TaskPartsNestResultDlg::~TaskPartsNestResultDlg()
{
}

BOOL TaskPartsNestResultDlg::OnInitDialog()
{
	__super::OnInitDialog();

    // no result returned yet.
    m_strStatus = _T("Running");
    m_strRetRstNum.Format(_T("%d"), m_iRetRstNum);

    // init the progress bar.
    if (m_ctrlTaskProgress.Load(MAKEINTRESOURCE(IDR_GIF_PROGRESS), _T("Gif")))
    {
        m_ctrlTaskProgress.Draw();
    }

    // 初始化表格样式
    InitGridStyle();

    // 初始化视口。
    Rect2D geomRect(.0, 10.0, .0, 10.0);
    m_previewWnd.InitViewPort(geomRect);

    // 只有内部使用时才能查看历史排版。
    if (CLOptionInfo::GetInstance()->GetProductParam()->IsUseInside())
    {
        ((CWnd*)GetDlgItem(IDC_BTN_AllResult))->ShowWindow(SW_SHOW);
    }
    else
    {
        ((CWnd*)GetDlgItem(IDC_BTN_AllResult))->ShowWindow(SW_HIDE);
    }

	// start the watcher.
	m_nestRstWatcher.Init(m_pNestProcessor, m_pNestResult, m_pLibraryData->m_pNestConfigItem->GetAllowedTime());
	m_nestRstWatcher.Start();

	// start the timer.
	m_timer = SetTimer(1, 1000, 0);

	SetBackgroundColor(SkinManager::GetBackgroundColor());
    SetWindowTheme(GetDlgItem(IDC_STATIC_Parts)->GetSafeHwnd(), L"", L"");
    SetWindowTheme(GetDlgItem(IDC_STATIC_Sheets_1)->GetSafeHwnd(), L"", L"");
    SetWindowTheme(GetDlgItem(IDC_STATIC_Sheets_2)->GetSafeHwnd(), L"", L"");
    SetWindowTheme(GetDlgItem(IDC_STATIC_Progress)->GetSafeHwnd(), L"", L"");
    SetWindowTheme(GetDlgItem(IDC_STATIC_PREVIEW)->GetSafeHwnd(), L"", L"");

	return TRUE;
}

void TaskPartsNestResultDlg::OnAllResult()
{
    AllResultDlg dlg;
    dlg.Init(m_pNestResult->GetNestResultCount());
    if (dlg.DoModal() == IDOK)
    {
        unsigned int iSelectedResultIndex = dlg.GetSelectedResultIndex();
		SheetListPtr pSheets = m_pNestResult->GetNestResultByIndex(iSelectedResultIndex);
        m_pTaskPartsNestResult = Helper_4_NestingTaskParts::GenerateTaskPartsNestResult(pSheets.get(), m_pTaskItem.get(), m_pSheetMaterials.get());
        DisplayNestResult();
    }
}

void TaskPartsNestResultDlg::OnStopNest()
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	CString strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_STOP_NEST);
	int iRet = MessageBox(strMsg, strProductName, MB_YESNO | MB_ICONQUESTION);
    if (iRet == IDYES)
    {
        m_pNestProcessor->StopNest();
    }
}

void TaskPartsNestResultDlg::OnAccept()
{
	// 是否要炸开零件放置。
	if (!m_pLibraryData->m_pNestConfigItem->IsCreateGrid())
	{
		for (unsigned int i = 0; i < m_pTaskPartsNestResult->GetNestResultListOnSheetMaterial()->size(); i++)
		{
			const NestResultOnSheetMaterial* pNestResultOnSheetMaterial = m_pTaskPartsNestResult->GetNestResultListOnSheetMaterial()->operator [](i).get();
            PartPlacementManager::ExplodeGridPart(pNestResultOnSheetMaterial->GetPartPmtList(), pNestResultOnSheetMaterial->GetPartInstList());
		}
		for (unsigned int i = 0; i < m_pTaskPartsNestResult->GetNewSheetList()->size(); i++)
		{
			SheetPtr pNewSheet_fromNesting = m_pTaskPartsNestResult->GetNewSheetList()->operator [](i);
			PartPlacementListPtr pPartPlacements = pNewSheet_fromNesting->GetPartPlacements();
			PartInstanceListPtr pPartInstances = pNewSheet_fromNesting->GetPartInstances();
			PartPlacementManager::ExplodeGridPart(pPartPlacements, pPartInstances);
		}
	}

	CDialogEx::OnOK();
}

void TaskPartsNestResultDlg::OnQuit()
{
	CDialogEx::OnCancel();
}

void TaskPartsNestResultDlg::OnPaint()
{
	PreviewSheet();

	CDialogEx::OnPaint();
}

void TaskPartsNestResultDlg::OnDestroy()
{
	ReleaseData_in_grid();
	m_previewWnd.ReleaseViewPort();

	CDialogEx::OnDestroy();
}

BOOL TaskPartsNestResultDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect r;
    GetClientRect(&r);
    if ((HBRUSH)m_brush == NULL)
        m_brush.CreateSolidBrush(SkinManager::GetBackgroundColor());
    pDC->FillRect(&r, &m_brush);

    return TRUE;
}

HBRUSH TaskPartsNestResultDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void TaskPartsNestResultDlg::OnTimer(UINT nIDEvent)
{
	// update the elapsed time.
	CString strElapsedTime;
	strElapsedTime.Format(_T("%d"), m_pNestResult->GetElapsedTime());
	((CWnd*)GetDlgItem(IDC_EDIT_COST_TIME))->SetWindowText(strElapsedTime);

	// whether we displayed all results and kill the timer.
	bool bFinished = false;
	if (m_pNestResult->TaskFinished() && m_iRetRstNum == m_pNestResult->GetNestResultCount())
	{
		bFinished = true;
	}

	if (bFinished)
	{
		// stop the timer.
		KillTimer(m_timer);

		m_strStatus = _T("Stopped");
		UpdateData(FALSE);
		((CWnd*)GetDlgItem(IDC_BTN_STOP))->EnableWindow(FALSE);
		if (m_iRetRstNum > 0)
		{
			((CWnd*)GetDlgItem(IDC_BTN_AcceptResult))->EnableWindow(TRUE);
			((CWnd*)GetDlgItem(IDC_BTN_AllResult))->EnableWindow(TRUE);
		}
		((CWnd*)GetDlgItem(IDC_BTN_CANCEL))->EnableWindow(TRUE);

		// 停止进度条动画。
		m_ctrlTaskProgress.UnLoad();
		m_ctrlTaskProgress.ShowWindow(SW_HIDE);
	}
	else
	{
		if (m_iRetRstNum < m_pNestResult->GetNestResultCount()) // 来了一个新的排版结果
		{
			// get the next result.
			m_iRetRstNum++;
			SheetListPtr pSheets = m_pNestResult->GetNestResultByIndex(m_iRetRstNum - 1);
            m_pTaskPartsNestResult = Helper_4_NestingTaskParts::GenerateTaskPartsNestResult(pSheets.get(), m_pTaskItem.get(), m_pSheetMaterials.get());

			// display the nest result.
			{
				m_strRetRstNum.Format(_T("%d"), m_iRetRstNum);
				DisplayNestResult();
			}
		}
	}
}

BOOL TaskPartsNestResultDlg::OnMouseWheel(UINT nFlags, short iDelta, CPoint pt)
{
    // the rect of the polygon view window.
    CRect polyWndRect;
    m_previewWnd.GetWindowRect(&polyWndRect);

    // check whether the current cursor is on the material view window.
    if (polyWndRect.PtInRect(pt))
        m_previewWnd.ZoomViewPort(iDelta, pt);

    return TRUE;
}

void TaskPartsNestResultDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // check whether the current cursor is on the material view window.
    CRect polyWndRect;
    m_previewWnd.GetWindowRect(&polyWndRect);
    ScreenToClient(&polyWndRect);
    if (polyWndRect.PtInRect(point))
    {
        m_previewWnd.SetFocus();

        if (nFlags & MK_MBUTTON)
        {
            CPoint ptOffSet = point - m_previewWnd.GetPanReferPt();
            m_previewWnd.PanViewPort(ptOffSet.x, ptOffSet.y);
            m_previewWnd.SetPanReferPt(point);
        }
    }
}

void TaskPartsNestResultDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
    m_previewWnd.SetPanReferPt(point);
}

void TaskPartsNestResultDlg::OnSelchangedExistingSheet(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // 取消选中
    for (int i = 0; i < m_newSheetsGrid.GetItemCount(); i++)
    {
        if (m_newSheetsGrid.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
        {
            m_newSheetsGrid.SetItemState(i, 0, LVIS_SELECTED);
        }
    }

    if (!m_bDisableGridSelectChangeEvent && pNMListView->iItem >= 0)
    {
        InitPreviewData(pNMListView->iItem, true);
        PreviewSheet();
    }

    *pResult = 0;
}

void TaskPartsNestResultDlg::OnSelchangedNewSheet(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // 取消选中
    for (int i = 0; i < m_existingSheetsGrid.GetItemCount(); i++)
    {
        if (m_existingSheetsGrid.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
        {
            m_existingSheetsGrid.SetItemState(i, 0, LVIS_SELECTED);
        }
    }

    if (!m_bDisableGridSelectChangeEvent && pNMListView->iItem >= 0)
    {
        InitPreviewData(pNMListView->iItem, false);
        PreviewSheet();
    }

    *pResult = 0;
}

void TaskPartsNestResultDlg::InitGridStyle()
{
    CString str;

    // 零件排版结果
    {
        m_nestedPartsGrid.InsertColumn(0, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM), LVCFMT_LEFT, 40);
        m_nestedPartsGrid.InsertColumn(1, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME), LVCFMT_LEFT, 80);
        m_nestedPartsGrid.InsertColumn(2, MultiLanguageMgr::GetInstance()->TranslateString(IDS_RemainingQuantity), LVCFMT_LEFT, 120);
        m_nestedPartsGrid.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    }

	// 现有板材
	{
		m_existingSheetsGrid.InsertColumn(0, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM), LVCFMT_LEFT, 40);
		m_existingSheetsGrid.InsertColumn(1, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME), LVCFMT_LEFT, 80);
		m_existingSheetsGrid.InsertColumn(2, MultiLanguageMgr::GetInstance()->TranslateString(IDS_COUNT), LVCFMT_LEFT, 80);
		m_existingSheetsGrid.InsertColumn(3, MultiLanguageMgr::GetInstance()->TranslateString(IDS_MATSIZE_USED), LVCFMT_LEFT, 100);
		m_existingSheetsGrid.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	}

    // 新生成板材
    {
        m_newSheetsGrid.InsertColumn(0, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM), LVCFMT_LEFT, 40);
        m_newSheetsGrid.InsertColumn(1, MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME), LVCFMT_LEFT, 80);
        m_newSheetsGrid.InsertColumn(2, MultiLanguageMgr::GetInstance()->TranslateString(IDS_COUNT), LVCFMT_LEFT, 80);
        m_newSheetsGrid.InsertColumn(3, MultiLanguageMgr::GetInstance()->TranslateString(IDS_MATSIZE_USED), LVCFMT_LEFT, 100);
        m_newSheetsGrid.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    }
}

void TaskPartsNestResultDlg::Init(TaskItemPtr pTaskItem, NestTaskPtr pNestTask, NestProcessorPtr pNestProcessor, LibraryDataPtr pLibraryData, SheetMaterialListPtr pSheetMaterials)
{
    m_pTaskItem = pTaskItem;
	m_pNestTask = pNestTask;
	m_pNestProcessor = pNestProcessor;
	m_pLibraryData = pLibraryData;
	m_pSheetMaterials = pSheetMaterials;
}

void TaskPartsNestResultDlg::DisplayNestResult()
{
	// 显示零件统计信息。
	{
        m_nestedPartsGrid.DeleteAllItems();
        const NestPartList* pNestPartList = m_pNestTask->GetNestPartList().get();
        for (unsigned int i = 0; i < pNestPartList->size(); i++)
        {
            const NestPart* pNestPart = pNestPartList->at(i).get();
            const Part* pPart = pNestPart->GetPartItem()->GetPart().get();

            int iCount = m_nestedPartsGrid.GetItemCount();
            m_nestedPartsGrid.InsertItem(iCount, _T(""));
            m_nestedPartsGrid.SetItemText(iCount, 0, StringUtil::Integer2String(i + 1));
            m_nestedPartsGrid.SetItemText(iCount, 1, pPart->GetName());

            // 零件剩余数量
            int iPlanningCount = m_pTaskItem->GetTaskPartByPartID(pPart->GetID())->GetPlanningCount();
            int iNestedCount_before = m_pTaskItem->GetSheetList()->GetPartNestedCount(pPart->GetID());
            int iNestedCount_thisTime = m_pTaskPartsNestResult->GetPartNestedCount(pPart->GetID());
            int iRemainingQuantity = iPlanningCount - iNestedCount_before - iNestedCount_thisTime;
            if (iRemainingQuantity < 0)
            {
                LogMgr::DebugWarn(_T("10246"), this);
                return;
            }
            m_nestedPartsGrid.SetItemText(iCount, 2, StringUtil::Integer2String(iRemainingQuantity));
        }
	}

	// 显示各个板材。
	{
        m_bDisableGridSelectChangeEvent = TRUE;

        ReleaseData_in_grid();
		m_existingSheetsGrid.DeleteAllItems();
		m_newSheetsGrid.DeleteAllItems();
        for (unsigned int i = 0; i < m_pTaskPartsNestResult->GetNestResultListOnSheetMaterial()->size(); i++)
        {
            const NestResultOnSheetMaterial* pNestResultOnSheetMaterial = m_pTaskPartsNestResult->GetNestResultListOnSheetMaterial()->operator [](i).get();
            int iRowCount = m_existingSheetsGrid.GetItemCount();
            m_existingSheetsGrid.InsertItem(iRowCount, _T(""));
            m_existingSheetsGrid.SetItemText(iRowCount, 0, StringUtil::Integer2String(i + 1));
            m_existingSheetsGrid.SetItemText(iRowCount, 1, pNestResultOnSheetMaterial->GetSheetMaterial()->GetSheet()->GetName());
            m_existingSheetsGrid.SetItemText(iRowCount, 2, StringUtil::Integer2String(pNestResultOnSheetMaterial->GetSheetMaterial()->GetSheet()->GetSheetCount()));
            m_existingSheetsGrid.SetItemText(iRowCount, 3, pNestResultOnSheetMaterial->GetSheetMaterial()->GetMatSize()->GetName());
            m_existingSheetsGrid.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pNestResultOnSheetMaterial->GetID())));
        }
        for (unsigned int i = 0; i < m_pTaskPartsNestResult->GetNewSheetList()->size(); i++)
        {
            const Sheet* pNewSheet = m_pTaskPartsNestResult->GetNewSheetList()->operator [](i).get();
            int iRowCount = m_newSheetsGrid.GetItemCount();
            m_newSheetsGrid.InsertItem(iRowCount, _T(""));
            m_newSheetsGrid.SetItemText(iRowCount, 0, StringUtil::Integer2String(i + 1));
            m_newSheetsGrid.SetItemText(iRowCount, 1, pNewSheet->GetName());
            m_newSheetsGrid.SetItemText(iRowCount, 2, StringUtil::Integer2String(pNewSheet->GetSheetCount()));
            IMaterialSizePtr pMaterialSize = m_pLibraryData->m_pMatSizeList->GetItemByID(pNewSheet->GetMaterialSizeID());
            m_newSheetsGrid.SetItemText(iRowCount, 3, pMaterialSize->GetName());
            m_newSheetsGrid.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pNewSheet->GetID())));
        }

        m_bDisableGridSelectChangeEvent = FALSE;

		// 选择第一行。
        if (m_existingSheetsGrid.GetItemCount() > 0)
        {
            m_existingSheetsGrid.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
            m_existingSheetsGrid.SetFocus();
        }
        else if (m_newSheetsGrid.GetItemCount() > 0)
        {
            m_newSheetsGrid.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
            m_newSheetsGrid.SetFocus();
        }
	}

	// 初始化预览数据。
	if (m_existingSheetsGrid.GetItemCount() > 0)
	{
		InitPreviewData(0, true);
	}
    else if (m_newSheetsGrid.GetItemCount() > 0)
    {
        InitPreviewData(0, false);
    }

	UpdateData(FALSE);
}

void TaskPartsNestResultDlg::InitPreviewData(int iRow, bool bFromExistingSheetsGrid)
{
    if (bFromExistingSheetsGrid)
    {
        LONGLONG* pNestResultOnSheetMaterialID = (LONGLONG*)m_existingSheetsGrid.GetItemData(iRow);
        const NestResultOnSheetMaterial* pNestResultOnSheetMaterial = m_pTaskPartsNestResult->GetNestResultListOnSheetMaterial()->GetItemByID(*pNestResultOnSheetMaterialID).get();
        m_pLineArc2Ds_of_part = pNestResultOnSheetMaterial->GetPartPmtList()->Calculate2DLineArcs();
        const IMaterialSize* pMaterialSize = pNestResultOnSheetMaterial->GetSheetMaterial()->GetMatSize();
        m_pLineArc2Ds_of_mat = MaterialProcessor::GetBorderLineArcs(pMaterialSize);

        // 无效区域
        m_pDashLineArc2Ds_of_mat = MaterialProcessor::GetUselessRegions(pMaterialSize);
        const SheetMaterial* pSheetMaterial = pNestResultOnSheetMaterial->GetSheetMaterial();
        m_pDashLineArc2Ds_of_mat->insert(m_pDashLineArc2Ds_of_mat->end(), pSheetMaterial->GetLineArc2Ds_ofParts()->begin(), pSheetMaterial->GetLineArc2Ds_ofParts()->end());
    }
    else
    {
        LONGLONG* pSheetID = (LONGLONG*)m_newSheetsGrid.GetItemData(iRow);
        const Sheet* pSheet = m_pTaskPartsNestResult->GetNewSheetList()->GetSheetByID(*pSheetID).get();
        m_pLineArc2Ds_of_part = pSheet->Calculate2DLineArcs();
        const IMaterialSize* pMaterialSize = m_pLibraryData->m_pMatSizeList->GetItemByID(pSheet->GetMaterialSizeID()).get();
        m_pLineArc2Ds_of_mat = MaterialProcessor::GetBorderLineArcs(pMaterialSize);
        m_pDashLineArc2Ds_of_mat = MaterialProcessor::GetUselessRegions(pMaterialSize);
    }
}

void TaskPartsNestResultDlg::PreviewSheet()
{
	if (m_pLineArc2Ds_of_mat && m_pLineArc2Ds_of_mat->size() > 0 && m_previewWnd.m_hWnd)
	{
		// 调整视口以适合板材外框。
		Rect2D sheetRect = m_pLineArc2Ds_of_mat->GetRectBox();
		m_previewWnd.FitRect(sheetRect);

		// 设置图形。
		LineArc2DListPtr pSolidLineArc2Ds(new LineArc2DList);
		pSolidLineArc2Ds->insert(pSolidLineArc2Ds->end(), m_pLineArc2Ds_of_part->begin(), m_pLineArc2Ds_of_part->end());
		pSolidLineArc2Ds->insert(pSolidLineArc2Ds->end(), m_pLineArc2Ds_of_mat->begin(), m_pLineArc2Ds_of_mat->end());
		m_previewWnd.SetSolidGeom2DList(pSolidLineArc2Ds);
		if (m_pDashLineArc2Ds_of_mat)
		{
			m_previewWnd.SetDashGeom2DList(m_pDashLineArc2Ds_of_mat);
		}

		// 绘制。
		m_previewWnd.Invalidate();

		UpdateData(FALSE);
	}
}

void TaskPartsNestResultDlg::ReleaseData_in_grid()
{
	for (int i = 0; i < m_existingSheetsGrid.GetItemCount(); i++)
	{
		DWORD_PTR pItemData = m_existingSheetsGrid.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}
	for (int i = 0; i < m_newSheetsGrid.GetItemCount(); i++)
	{
		DWORD_PTR pItemData = m_newSheetsGrid.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}
}

END_CUTLEADER_NAMESPACE()

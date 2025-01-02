#include "StdAfx.h"
#include "PartPlacementPropertyBar.h"

#include "DBConnect.h"
#include "clUtilityResource.h"
#include "NumberProp.h"
#include "baseConst.h"
#include "StringUtil.h"
#include "Rect2D.h"
#include "CommandManager.h"
#include "clUILibResource.h"
#include "DataBaseManager.h"
#include "DataItem.h"
#include "DataCenterItemLoader.h"
#include "clBaseDataResource.h"

#include "PatternList.h"
#include "clGeometryFeatureResource.h"
#include "LoopToolData.h"
#include "LoopCutFeature.h"
#include "PartCamData.h"
#include "PartItem.h"
#include "Part.h"
#include "PartCadData.h"
#include "PartPlacementList.h"
#include "clPartLayoutResource.h"
#include "PartLayoutEditor.h"
#include "PartPlacement.h"
#include "GridSpaceModifyCommand.h"
#include "PartPlacementPropertyEditCommand.h"
#include "PartGridGapDlg.h"
#include "PartPlacementManager.h"
#include "SkinManager.h"
#include "clPartLayoutUIResource.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementPropertyBar* PartPlacementPropertyBar::m_pPartPmtPropBar = NULL;

BEGIN_MESSAGE_MAP(PartPlacementPropertyBar, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()

	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChange)

	ON_BN_CLICKED(IDC_SET_GRID_GAP, OnSetGridGap)
	ON_UPDATE_COMMAND_UI(IDC_SET_GRID_GAP, OnUpdateClickBtn)
END_MESSAGE_MAP()

PartPlacementPropertyBar::PartPlacementPropertyBar(void)
{
	m_pPartPlacementList.reset();
}

PartPlacementPropertyBar::~PartPlacementPropertyBar(void)
{
}

PartPlacementPropertyBar* PartPlacementPropertyBar::GetInstance(CWnd* pParent)
{
	if (m_pPartPmtPropBar == NULL)
	{
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROPBAR_EDITPRTPMT);
		m_pPartPmtPropBar = new PartPlacementPropertyBar();
		m_pPartPmtPropBar->m_pMainWnd = pParent;
		if (!m_pPartPmtPropBar->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200), TRUE, IDC_PROPBAR_EDITPRTPMT,
			WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)) {
			return NULL;
		}
	}

	return m_pPartPmtPropBar;
}

int PartPlacementPropertyBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create controls.
	CreatePropWnd();
	CreateSetGapBtn();

	// layout the windows in this pane.
	AdjustLayout();

	return 0;
}

void PartPlacementPropertyBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// layout the windows in this pane.
	AdjustLayout();
}

BOOL PartPlacementPropertyBar::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	GetClientRect(&r);
	if ((HBRUSH)m_brush == NULL)
		m_brush.CreateSolidBrush(SkinManager::GetBackgroundColor());
	pDC->FillRect(&r, &m_brush);

	return TRUE;
}

LRESULT PartPlacementPropertyBar::OnPropertyChange(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* prop = (CMFCPropertyGridProperty *)lParam;
	const COleVariant& var = prop->GetValue();

	DataPropItem item = m_propInfo[(CObject*)prop];

	// check whether the input is valid.
	if (item.GetItemName() == IDS_PROP_REPEAT_NUM_X || item.GetItemName() == IDS_PROP_REPEAT_NUM_Y)
	{
		if (var.intVal < 1) { // 输入了非法值。
			CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_REPEAT_NUM_SMALL_EX);
			::MessageBox(m_pMainWnd->m_hWnd, str, strProductName, MB_OK | MB_ICONWARNING);
			DisplayPartPmt(m_pPartPlacementList, m_pPartInstanceList, m_pViewPort); // refresh part pmt data.
			return S_OK;
		}
	}

	// 执行命令。
	DataPropItem newPropItem(item.GetItemName(), var);
	DataPropItem oldPropItem(item.GetItemName(), item.GetValue());
	ICommandPtr pCommand(new PartPlacementPropertyEditCommand(GetEditorData(), m_pPartPlacementList->at(0), m_pPartInstanceList, newPropItem, oldPropItem));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	// 刷新界面。
	if (item.GetItemName() == IDS_PROP_ANGLE || item.GetItemName() == IDS_PROP_REPEAT_NUM_X || item.GetItemName() == IDS_PROP_REPEAT_NUM_Y ||
		item.GetItemName() == IDS_PROP_SPACE_X || item.GetItemName() == IDS_PROP_SPACE_Y) { // 修改了这些属性后，需要刷新属性面板。
		DisplayPartPmt(m_pPartPlacementList, m_pPartInstanceList, m_pViewPort);
	}
	GetRelativeView()->Invalidate();

	return S_OK;
}

void PartPlacementPropertyBar::OnSetGridGap()
{
	if (m_pPartPlacementList->size() == 1)
	{
		PartPlacementPtr pPartPlacement = m_pPartPlacementList->at(0);
		if (pPartPlacement->IsGrid())
		{
			PartGridGapDlg dlg(m_pMainWnd);
			if (dlg.DoModal() == IDOK)
			{
				// 通过阵列新的零件间距，计算出行/列间距。
                BOOL bEnableComcut = FALSE;
				double dPartDis = dlg.GetPartDis(bEnableComcut);
                if (bEnableComcut) {
					const LoopTopologyItemList* pLoopTopologyItems = pPartPlacement->GetPart()->GetCadData()->GetLoopTopologyItems();
                    LoopCutFeaturePtr pBoundaryLoopFeature = pPartPlacement->GetCamData()->GetBoundaryLoopFeature(pLoopTopologyItems);
                    dPartDis = pBoundaryLoopFeature->GetLoopTool()->GetToolWidth();
                }
				pair<double, double> spaceData = PartPlacementManager::CalcGridPartSpacing(pPartPlacement, dPartDis);

				// 执行命令。
				ICommandPtr pCommand(new GridSpaceModifyCommand(GetEditorData(), pPartPlacement, pPartPlacement->GetSpacingX(), pPartPlacement->GetSpacingY(), spaceData.first, spaceData.second));
				pCommand->Do();
				CommandManager::GetInstance()->AddCommand(pCommand);

				// 刷新属性面板。
				DisplayPartPmt(m_pPartPlacementList, m_pPartInstanceList, m_pViewPort);
			}
		}
	}
}

void PartPlacementPropertyBar::OnUpdateClickBtn(CCmdUI* pCmdUI)
{
	if (m_pPartPlacementList && m_pPartPlacementList->size() == 1)
	{
		PartPlacementPtr pPartPlacement = m_pPartPlacementList->at(0);
		if (pPartPlacement->IsGrid()) {
			pCmdUI->Enable();
			return;
		}
	}

	pCmdUI->Enable(FALSE);
}

void PartPlacementPropertyBar::DisplayPartPmt(PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, GlViewPortPtr pViewPort)
{
	m_pPartPlacementList = pPartPlacements;
	m_pPartInstanceList = pPartInstances;
	m_pViewPort = pViewPort;

	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE); // 不显示表头。
	m_wndPropList.EnableDescriptionArea(FALSE); // 不显示这个区域，下方会创建一个按钮。
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	if (m_pPartPlacementList->size() == 1)
	{
		PartPlacementPtr pPartPlacement = m_pPartPlacementList->at(0);

		// get the property of the part placement
		DataProperty partPmtProp = pPartPlacement->GetProperty();

		// init "Part Placement Data" category
		vector<DataPropItem> partPmtPropItems;
		partPmtProp.GetItemsByType(IDS_CATEGORY_PARTPMT_DATA, partPmtPropItems);
		InitPropGroup(IDS_CATEGORY_PARTPMT_DATA, partPmtPropItems);

		// init "grid data" category
		vector<DataPropItem> gridPropItems;
		partPmtProp.GetItemsByType(IDS_CATEGORY_PARTPMT_GRID, gridPropItems);
		if (gridPropItems.size() > 0) {
			InitPropGroup(IDS_CATEGORY_PARTPMT_GRID, gridPropItems);
		}
	}
	else
	{
		// 零件数量。
		{
			CString strPropName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PartInstanceCount);
			CString strPropValue = StringUtil::Integer2String(m_pPartPlacementList->GetPartInstanceCount());
			CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(strPropName, strPropValue);
			pItem->AllowEdit(FALSE);
			m_wndPropList.AddProperty(pItem);
		}

		// 零件尺寸。
		{
			CString strPropName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PartInstanceSize);
			Rect2D partPlacementsRect = m_pPartPlacementList->GetRectBox();
			CString strPropValue;
			strPropValue.Format(_T("%.2f X %.2f"), partPlacementsRect.GetWidth(), partPlacementsRect.GetHeight());
			CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(strPropName, strPropValue);
			pItem->AllowEdit(FALSE);
			m_wndPropList.AddProperty(pItem);
		}
	}

	if (!IsVisible())
		ShowPane(TRUE, FALSE, TRUE);
}

void PartPlacementPropertyBar::UpdatePmtProp()
{
	if (m_pPartPlacementList->size() == 1) {
		PartPlacementPtr pPartPlacement = m_pPartPlacementList->at(0);

		// 得到“零件放置”的属性。
		DataProperty partPmtProp = pPartPlacement->GetProperty();
		vector<DataPropItem> partPmtPropItems;
		partPmtProp.GetItemsByType(IDS_CATEGORY_PARTPMT_DATA, partPmtPropItems);

		// 更新每个属性值。
		CMFCPropertyGridProperty* pGeneralGroup = m_wndPropList.GetProperty(0);
		for (int i = 0; i < pGeneralGroup->GetSubItemsCount(); i++) {
			pGeneralGroup->GetSubItem(i)->SetValue(partPmtPropItems.at(i).GetValue());
		}
	} else {
		// 零件数量。
		CString strPropValue = StringUtil::Integer2String(m_pPartPlacementList->GetPartInstanceCount());
		m_wndPropList.GetProperty(0)->SetValue(strPropValue);

		// 零件尺寸。
		Rect2D partPlacementsRect = m_pPartPlacementList->GetRectBox();
		strPropValue.Format(_T("%.2f X %.2f"), partPlacementsRect.GetWidth(), partPlacementsRect.GetHeight());
		m_wndPropList.GetProperty(1)->SetValue(strPropValue);
	}
}

void PartPlacementPropertyBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	// 属性表和按钮占用1/3的面板高度。
	int iHeight1 = rectClient.Height() * 9 / 30;
	int iHeight2 = rectClient.Height() * 1 / 30;
	int iDeduction = 0; // the space between two windows.

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), iHeight1-iDeduction, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetCustomColors( SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(), SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(),
		SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(), SkinManager::GetTextColor() );
	m_btnSetGridGap.SetWindowPos(NULL, rectClient.left + 3, iHeight1, rectClient.Width() - 6, iHeight2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL PartPlacementPropertyBar::CreatePropWnd()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
		return -1;

	return TRUE;
}

BOOL PartPlacementPropertyBar::CreateSetGapBtn()
{
	CRect rectDummy (0, 0, 0, 0);
	m_btnSetGridGap.Create(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SET_GRID_GAP), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rectDummy, this, IDC_SET_GRID_GAP);

	return TRUE;
}

void PartPlacementPropertyBar::InitPropGroup(int iGroupName, vector<DataPropItem>& propItems)
{
	CString strGroupName;
	strGroupName = MultiLanguageMgr::GetInstance()->TranslateString(iGroupName);

	// go through each prop item.
	CMFCPropertyGridProperty* pGeneralGroup = new CMFCPropertyGridProperty(strGroupName);
	for (unsigned int i = 0; i < propItems.size(); i++)
	{
		DataPropItem item = propItems[i];

		// the name
		CString propName;
		propName = MultiLanguageMgr::GetInstance()->TranslateString(item.GetItemName());

		// 新建属性项。
		CMFCPropertyGridProperty* pProp = NULL;
		if (item.IsNumber())
			pProp = new NumberProp(propName, item.GetValue(), item.GetFormat(), NULL, 0, item.HasButton(), item.CanNegative());
		else
			pProp = new CMFCPropertyGridProperty(propName, item.GetValue(), _T(""));

		// 是否只读。
		if (item.IsReadOnly())
			pProp->Enable(FALSE);

		pGeneralGroup->AddSubItem(pProp);
		m_propInfo[(CObject*)pProp] = item;
	}

	m_wndPropList.AddProperty(pGeneralGroup);
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "CanvasPatternPropertyBar.h"

#include "LineArc2DList.h"
#include "clUtilityResource.h"
#include "clUILibResource.h"
#include "DataProperty.h"
#include "NumberProp.h"
#include "IntegerList.h"
#include "CommandManager.h"
#include "clBaseDataResource.h"
#include "StringUtil.h"

#include "CanvasPatternPropertyEditCommand.h"
#include "IPattern.h"
#include "PatternList.h"
#include "clGeometryFeatureResource.h"
#include "SkinManager.h"
#include "clCanvasGeometryUIResource.h"
#include "TextStructList.h"
#include "clGeometryFeatureUIResource.h"
#include "PartCadData.h"
#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "PolygonPatternLoop.h"
#include "ChangeLoopSizeDlg.h"
#include "CanvasLoopSizeChangeCommand.h"
#include "PatternLoopCache.h"
#include "PatternLoopNodeList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternPropertyBar* CanvasPatternPropertyBar::m_pCanvasPatternPropertyBar = NULL;

CanvasPatternPropertyBar::CanvasPatternPropertyBar(void)
{
	m_pPatList.reset();
}

CanvasPatternPropertyBar::~CanvasPatternPropertyBar(void)
{
}

EditorData CanvasPatternPropertyBar::GetEditorData() const
{
    CString str;
    str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CanvasGeometry);

    EditorData editorData(GetRelativeView(), str);
    return editorData;
}

void CanvasPatternPropertyBar::ShowBar(BOOL bShow, BOOL bDelay)
{
	// 隐藏该面板时，需要重置这些数据。
	// 注：
	// 1) 场景1：底图上选中一些轮廓，然后关闭任务，此时会发现不少内存没有释放，原因就是该面板还引用了那些数据。
	if (!bShow) {
		m_pPartCadData.reset();
		m_pPartCamData.reset();
		m_pPatList.reset();
		m_pTextStructs.reset();
		m_pPolygonPatternLoop_rect.reset();
	}

	__super::ShowBar(bShow, bDelay);
}

// get the singleton object
CanvasPatternPropertyBar* CanvasPatternPropertyBar::GetInstance(CWnd* pParent)
{
	if(m_pCanvasPatternPropertyBar == NULL)
	{
		m_pCanvasPatternPropertyBar = new CanvasPatternPropertyBar();
		m_pCanvasPatternPropertyBar->m_pMainWnd = pParent;

		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROPBAR_PATPROP);

		if (!m_pCanvasPatternPropertyBar->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200),
			TRUE, IDC_CanvasPatternPropertyBar, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)) {
			return NULL;
		}
	}

	return m_pCanvasPatternPropertyBar;
}

BEGIN_MESSAGE_MAP(CanvasPatternPropertyBar, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChange)

	ON_BN_CLICKED(IDC_BTN_SetRectLinesSize, OnSetRectLinesSize)
	ON_UPDATE_COMMAND_UI(IDC_BTN_SetRectLinesSize, OnUpdateClickBtn)
END_MESSAGE_MAP()

void CanvasPatternPropertyBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	// 属性表和按钮占用1/3的面板高度。
	int iHeight1 = rectClient.Height() * 9 / 30;
	int iHeight2 = rectClient.Height() * 1 / 30;
	int iDeduction = 0; // the space between two windows.

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), iHeight1 - iDeduction, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetCustomColors(SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(), SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(),
		SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(), SkinManager::GetTextColor());
	m_btnSetRectLinesSize.SetWindowPos(NULL, rectClient.left + 3, iHeight1, rectClient.Width() - 6, iHeight2, SWP_NOACTIVATE | SWP_NOZORDER);
}

int CanvasPatternPropertyBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 创建属性表。
	{
		CRect rectDummy;
		rectDummy.SetRectEmpty();
		if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
			return -1;
	}

	// 创建按钮。
	{
		CRect rectDummy(0, 0, 0, 0);
		m_btnSetRectLinesSize.Create(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetLoopSize), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectDummy, this, IDC_BTN_SetRectLinesSize);
	}

	// 布局控件。
	AdjustLayout();

	return 0;
}

void CanvasPatternPropertyBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

BOOL CanvasPatternPropertyBar::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	GetClientRect(&r);
	if ((HBRUSH)m_brush == NULL)
		m_brush.CreateSolidBrush(SkinManager::GetBackgroundColor());
	pDC->FillRect(&r, &m_brush);

	return TRUE;
}

void CanvasPatternPropertyBar::InitPropGroup(int iGroupName, vector<DataPropItem>& propItems)
{
	CString strGroupName, propName;
	strGroupName = MultiLanguageMgr::GetInstance()->TranslateString(iGroupName);

	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(strGroupName);

	// go through each data prop item
	for (unsigned int i = 0; i < propItems.size(); i++)
	{
		CMFCPropertyGridProperty* pProp = NULL;

		//
		DataPropItem dataPropItem = propItems[i];
		propName = MultiLanguageMgr::GetInstance()->TranslateString(dataPropItem.GetItemName());

		// create CMFCPropertyGridProperty object.
		if (dataPropItem.IsOption())
		{
			StringListPtr optListPtr = dataPropItem.GetOptions();
			CString str = (*optListPtr)[dataPropItem.GetValue().lVal];
			pProp = new CMFCPropertyGridProperty(propName, str, _T(""));

			for (unsigned int i = 0; i < optListPtr->size(); i++)
				pProp->AddOption((*optListPtr)[i]);
			pProp->AllowEdit(FALSE);
		}
		else if (dataPropItem.IsNumber())
		{
			// this item is number-item
			pProp = new NumberProp(propName, dataPropItem.GetValue(), dataPropItem.GetFormat(), NULL, 0, dataPropItem.HasButton(), dataPropItem.CanNegative());

			// whether read only.
			if (dataPropItem.IsReadOnly())
				pProp->AllowEdit(FALSE);
		}
		else
		{
			pProp = new CMFCPropertyGridProperty(propName, dataPropItem.GetValue(), _T(""));

			// whether read only.
			if (dataPropItem.IsReadOnly())
				pProp->AllowEdit(FALSE);
		}

		// add the data prop item to the group.
		pGroup->AddSubItem(pProp);

		// keep track the prop info
		m_propInfo[(CObject*)pProp] = dataPropItem;
	}

	m_wndPropList.AddProperty(pGroup);
}

void CanvasPatternPropertyBar::DisplayPatProp(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, PatternListPtr pPatList, TextStructListPtr pTextStructs)
{
	m_pPartCadData = pPartCadData;
	m_pPartCamData = pPartCamData;
	m_pPatList = pPatList;
	m_pTextStructs = pTextStructs;

	// 判断当前是不是只选中了矩形线，即四条组成矩形的线。
	m_bOnlyRectangleLinesSelected = CheckOnlyRectangleLinesSelected(m_pPolygonPatternLoop_rect);

	// 设置表格的样式。
	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE); // 不显示表头。
	m_wndPropList.EnableDescriptionArea(FALSE); // 不显示这个区域，下方会创建一个按钮。
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	if (m_pPatList->size() == 1)
	{
		// get the property of the pattern
		IPatternPtr pPattern = m_pPatList->at(0);
		DataProperty patProp = pPattern->GetProperty();

		// init general category
		vector<DataPropItem> genPropItems;
		patProp.GetItemsByType(IDS_CATEGORY_GENERAL, genPropItems);
		InitPropGroup(IDS_CATEGORY_GENERAL, genPropItems);

		// init geometry category
		vector<DataPropItem> geomPropItems;
		patProp.GetItemsByType(IDS_CATEGORY_GEOMETRY, geomPropItems);
		InitPropGroup(IDS_CATEGORY_GEOMETRY, geomPropItems);
	}
	else
	{
		// 轮廓数量。
		{
			PatternLoopListPtr pPatternLoops = m_pPartCadData->GetPatternLoopList()->GetLoopWithinPats(m_pPatList.get());
			CString strPropValue = StringUtil::Integer2String(pPatternLoops->size());
			CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(MultiLanguageMgr::GetInstance()->TranslateString(IDS_LoopCount), strPropValue);
			pItem->AllowEdit(FALSE);
			m_wndPropList.AddProperty(pItem);
		}

		// 图元数量。
		{
			CString strPropName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GeometryCount);
			CString strPropValue = StringUtil::Integer2String(m_pPatList->size());
			CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(strPropName, strPropValue);
			pItem->AllowEdit(FALSE);
			m_wndPropList.AddProperty(pItem);
		}

		// 文字个数。
		{
			CString strPropName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TextCount);
			CString strPropValue = StringUtil::Integer2String(m_pTextStructs->size());
			CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(strPropName, strPropValue);
			pItem->AllowEdit(FALSE);
			m_wndPropList.AddProperty(pItem);
		}

		// 图形尺寸。
		{
			CString strPropName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GeometrySize);
			Rect2D geomRect = m_pPatList->GetRectBox();
			CString strPropValue;
			strPropValue.Format(_T("%.2f X %.2f"), geomRect.GetWidth(), geomRect.GetHeight());
			CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(strPropName, strPropValue);
			pItem->AllowEdit(FALSE);
			m_wndPropList.AddProperty(pItem);
		}

		// 图形总长。
		{
			CString strPropName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GeometryLength);
			double dGeometryLength = m_pPatList->Calculate2DLineArcs()->GetTotalLength();
			CString strPropValue;
			strPropValue.Format(_T("%.2f"), dGeometryLength);
			CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(strPropName, strPropValue);
			pItem->AllowEdit(FALSE);
			m_wndPropList.AddProperty(pItem);
		}
	}

	if (!this->IsVisible()) {
		this->ShowPane(TRUE, FALSE, TRUE);
	}
}

void CanvasPatternPropertyBar::UpdateProp()
{
	if (m_pPatList->size() == 1) // 这里更新第二个group中每行的值。
	{
		IPatternPtr pPattern = m_pPatList->at(0);

		//
		vector<DataPropItem> geomPropItems;
		DataProperty patProp = pPattern->GetProperty();
		patProp.GetItemsByType(IDS_CATEGORY_GEOMETRY, geomPropItems);

		// 更新每个属性值。
		CMFCPropertyGridProperty* pGroup = m_wndPropList.GetProperty(1);
		for (int i = 0; i < pGroup->GetSubItemsCount(); i++) {
			pGroup->GetSubItem(i)->SetValue(geomPropItems.at(i).GetValue());
		}
	} else {
		// 轮廓数量。
		PatternLoopListPtr pPatternLoops = m_pPartCadData->GetPatternLoopList()->GetLoopWithinPats(m_pPatList.get());
		m_wndPropList.GetProperty(0)->SetValue(StringUtil::Integer2String(pPatternLoops->size()));

		// 图元数量。
		m_wndPropList.GetProperty(1)->SetValue(StringUtil::Integer2String(m_pPatList->size()));

		// 文字个数。
		m_wndPropList.GetProperty(2)->SetValue(StringUtil::Integer2String(m_pTextStructs->size()));

		// 图形尺寸。
		{
			Rect2D geomRect = m_pPatList->GetRectBox();
			CString strPropValue;
			strPropValue.Format(_T("%.2f X %.2f"), geomRect.GetWidth(), geomRect.GetHeight());
			m_wndPropList.GetProperty(3)->SetValue(strPropValue);
		}

		// 图形总长。
		m_wndPropList.GetProperty(4)->SetValue(StringUtil::Double2String(m_pPatList->Calculate2DLineArcs()->GetTotalLength(), _T("%.2f")));
	}
}

LRESULT CanvasPatternPropertyBar::OnPropertyChange(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* prop = (CMFCPropertyGridProperty *)lParam;
	COleVariant var = prop->GetValue();

	DataPropItem item = m_propInfo[(CObject*)prop];
	if (item.GetItemName() == IDS_PROP_REPEAT_NUM || item.GetItemName() == IDS_PROP_REPEAT_NUM_X || item.GetItemName() == IDS_PROP_REPEAT_NUM_Y)
	{
		if (var.intVal < 2)
		{
			CString str, strProductName;
			strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_REPEAT_NUM_SMALL);
			::MessageBox(m_pMainWnd->m_hWnd, str, strProductName, MB_OK | MB_ICONWARNING);

			// refresh pattern data.
			DisplayPatProp(m_pPartCadData, m_pPartCamData, m_pPatList, m_pTextStructs);

			return S_OK;
		}
	}

	// 修改图形属性。
	DataPropItem newPropItem(item.GetItemName(), var);
	ICommandPtr pCommand(new CanvasPatternPropertyEditCommand(GetEditorData(), m_pPartCadData, m_pPartCamData, m_pPatList->at(0)->GetID(), newPropItem));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	// 刷新界面。
	GetRelativeView()->Invalidate();

	return S_OK;
}

void CanvasPatternPropertyBar::OnSetRectLinesSize()
{
	const Rect2D& loopRect = m_pPolygonPatternLoop_rect->GetCacheData()->GetLoopRect();
	ChangeLoopSizeDlg dlg;
	dlg.SetSize(loopRect.GetWidth(), loopRect.GetHeight());
	if (dlg.DoModal() == IDOK)
	{
		double dLoopWidth = .0, dLoopHeight = .0;
		dlg.GetSize(dLoopWidth, dLoopHeight);

		// 执行修改命令。
		ICommandPtr pCommand(new CanvasLoopSizeChangeCommand(GetEditorData(), m_pPartCadData, m_pPartCamData, m_pPolygonPatternLoop_rect, dLoopWidth, dLoopHeight));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 更新显示。
		DisplayPatProp(m_pPartCadData, m_pPartCamData, m_pPatList, m_pTextStructs);
		GetRelativeView()->Invalidate();
	}
}

void CanvasPatternPropertyBar::OnUpdateClickBtn(CCmdUI* pCmdUI)
{
	if (m_bOnlyRectangleLinesSelected) {
		pCmdUI->Enable(TRUE);
	}
	else {
		pCmdUI->Enable(FALSE);
	}
}

bool CanvasPatternPropertyBar::CheckOnlyRectangleLinesSelected(PolygonPatternLoopPtr& pPolygonPatternLoop_rect)
{
	// 不能选中文字。
	if (m_pTextStructs->size() > 0) {
		return false;
	}

	// 只能选中四条直线。
	if (m_pPatList->size() != 4) {
		return false;
	}
	for (unsigned int i = 0; i < m_pPatList->size(); i++) {
		if (m_pPatList->operator[](i)->GetPatternType() != PATTERN_LINE) {
			return false;
		}
	}

	// 这四条直线要组成一个封闭多边形轮廓。
	PatternLoopListPtr pPatternLoops = m_pPartCadData->GetPatternLoopList()->GetLoopWithinPats(m_pPatList.get());
	if (pPatternLoops->size() != 1 || pPatternLoops->front()->GetPatternLoopType() != PatternLoop_Polygon ||
		pPatternLoops->front()->IsOpenPath() || pPatternLoops->front()->GetPatternLoopNodes()->size() != 4) {
		return false;
	}

	// 该封闭多边形轮廓的四个内角必须均为90度。
	PolygonPatternLoopPtr pPolygonPatternLoop_tmp = boost::dynamic_pointer_cast<PolygonPatternLoop>(pPatternLoops->front());
	for (unsigned int i = 0; i < 4; i++) {
		double dCornerInsideAngle = pPolygonPatternLoop_tmp->GetTwoPatternInsideAngle(i);
		if (fabs(dCornerInsideAngle - PID2) > GEOM_TOLERANCE) {
			return false;
		}
	}

	pPolygonPatternLoop_rect = pPolygonPatternLoop_tmp;
	return true;
}

END_CUTLEADER_NAMESPACE()

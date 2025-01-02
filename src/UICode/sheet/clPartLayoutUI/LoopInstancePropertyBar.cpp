#include "StdAfx.h"
#include "LoopInstancePropertyBar.h"

#include "Rect2D.h"
#include "LogMgr.h"
#include "StringUtil.h"
#include "clUtilityResource.h"

#include "clPartLayoutUIResource.h"
#include "clGeometryFeatureUIResource.h"
#include "LoopInstanceList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

LoopInstancePropertyBar* LoopInstancePropertyBar::m_pLoopInstancePropertyBar = NULL;

BEGIN_MESSAGE_MAP(LoopInstancePropertyBar, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

LoopInstancePropertyBar::LoopInstancePropertyBar(void)
{
}

LoopInstancePropertyBar::~LoopInstancePropertyBar(void)
{
}

LoopInstancePropertyBar* LoopInstancePropertyBar::GetInstance(CWnd* pParent)
{
	if (m_pLoopInstancePropertyBar == NULL)
	{
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LoopInstanceProperty);
		m_pLoopInstancePropertyBar = new LoopInstancePropertyBar();
		m_pLoopInstancePropertyBar->m_pMainWnd = pParent;
		if (!m_pLoopInstancePropertyBar->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200), TRUE, IDC_PROPBAR_LoopInstance,
			WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)) {
			return NULL;
		}
	}

	return m_pLoopInstancePropertyBar;
}

int LoopInstancePropertyBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create controls.
	CreatePropWnd();

	// layout the windows in this pane.
	AdjustLayout();

	return 0;
}

void LoopInstancePropertyBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// layout the windows in this pane.
	AdjustLayout();
}

BOOL LoopInstancePropertyBar::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	GetClientRect(&r);
	if ((HBRUSH)m_brush == NULL)
		m_brush.CreateSolidBrush(SkinManager::GetBackgroundColor());
	pDC->FillRect(&r, &m_brush);

	return TRUE;
}

void LoopInstancePropertyBar::DisplayProperty(const LoopInstanceList* pLoopInstances)
{
	if (!pLoopInstances || pLoopInstances->size() == 0) {
		LogMgr::DebugWarn(_T("4600976"));
		return;
	}

	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE); // 不显示表头。
	m_wndPropList.EnableDescriptionArea(FALSE); // 不显示这个区域，下方会创建一个按钮。
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	// 轮廓数量。
	{
		CString strPropName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LoopCount);
		CString strPropValue = StringUtil::Integer2String(pLoopInstances->size());
		CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(strPropName, strPropValue);
		pItem->AllowEdit(FALSE);
		m_wndPropList.AddProperty(pItem);
	}

	// 轮廓尺寸。
	{
		CString strPropName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SIZE);
		Rect2D loopInstancesRect = pLoopInstances->GetRectBox();
		CString strPropValue;
		strPropValue.Format(_T("%.2f X %.2f"), loopInstancesRect.GetWidth(), loopInstancesRect.GetHeight());
		CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(strPropName, strPropValue);
		pItem->AllowEdit(FALSE);
		m_wndPropList.AddProperty(pItem);
	}

	if (!IsVisible())
		ShowPane(TRUE, FALSE, TRUE);
}

void LoopInstancePropertyBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetCustomColors(SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(), SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(),
		SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(), SkinManager::GetTextColor());
}

BOOL LoopInstancePropertyBar::CreatePropWnd()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
		return -1;

	return TRUE;
}

END_CUTLEADER_NAMESPACE()

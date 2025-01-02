#include "StdAfx.h"
#include "LoopLeadPropertyBar.h"

#include "clUtilityResource.h"
#include "DataHelper.h"
#include "NumberProp.h"
#include "CheckBoxProp.h"

#include "CommandManager.h"
#include "clUILibResource.h"
#include "baseConst.h"

#include "clGeometryFeatureResource.h"
#include "IPatternLoop.h"

#include "ILead.h"
#include "LineLead.h"
#include "LineArcLead.h"
#include "HookLead.h"
#include "TwoLineLead.h"
#include "ArcLead.h"
#include "LeadList.h"
#include "ILoopStartCutPt.h"
#include "GeometryStartCutPt.h"
#include "CornerStartCutPt.h"
#include "LoopStartCutCache.h"
#include "LoopStartCutData.h"
#include "LoopStartCutDataList.h"
#include "clCutFeatureResource.h"
#include "ConditionNodeList.h"
#include "ConditionNode.h"
#include "EndCutPtModifyCommand.h"
#include "LeadModifyCommand.h"
#include "LeadManager.h"

#include "ConditionTypeList.h"
#include "LeadConfigItem.h"
#include "SkinManager.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

LoopLeadPropertyBar* LoopLeadPropertyBar::m_pLoopLeadPropBar = NULL;

LoopLeadPropertyBar::LoopLeadPropertyBar(void)
{
	m_bIgnore_OnPropertyChangeMsg = false;
}

LoopLeadPropertyBar::~LoopLeadPropertyBar(void)
{
}

LoopLeadPropertyBar* LoopLeadPropertyBar::GetInstance(CWnd* pParent)
{
	if (m_pLoopLeadPropBar == NULL) {
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROPBAR_STARTCUTPROP);
		m_pLoopLeadPropBar = new LoopLeadPropertyBar();
		m_pLoopLeadPropBar->m_pMainWnd = pParent;
		if (!m_pLoopLeadPropBar->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200), TRUE, IDC_PROPBAR_LOOPLEAD,
			WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)) {
			return NULL;
		}
	}

	return m_pLoopLeadPropBar;
}

BEGIN_MESSAGE_MAP(LoopLeadPropertyBar, XBarBase)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()

	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChange)
END_MESSAGE_MAP()

int LoopLeadPropertyBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// create the list.
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
		return -1;

	// create the button.
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SET_STARTCUT_POINT);
	m_setStartPtBtn.Create(str, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rectDummy, this, IDC_SET_STARTCUT_POINT);

	AdjustLayout();

	return 0;
}

void LoopLeadPropertyBar::OnDestroy()
{
	CutFeaturePropertyBar::OnDestroy();
}

void LoopLeadPropertyBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
}

void LoopLeadPropertyBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

LRESULT LoopLeadPropertyBar::OnPropertyChange(WPARAM wParam, LPARAM lParam)
{
	if (m_bIgnore_OnPropertyChangeMsg)
	{
		return S_OK;
	}

	CMFCPropertyGridProperty* prop = (CMFCPropertyGridProperty*)lParam;
	COleVariant var = prop->GetValue();

	// 四个条件节点分组的名称。
	CString str1, str2, str3, str4, str5, str6;
	str1 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CATEGORY_LEADINCOND);
	str2 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CATEGORY_LEADOUTCOND);
	str3 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CATEGORY_DOPIERCECOND);
	str4 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CATEGORY_UNPIERCECOND);
	str5 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CATEGORY_LEADINDATA);
	str6 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CATEGORY_LEADOUTDATA);

	// 这个属性项的父亲项。
	CMFCPropertyGridProperty* pParentProp = prop->GetParent();

	// 这个属性项代表的含义。
	int iPropItemName = m_propItemInfo[(CObject*)prop];

	// 修改过切值。
	if (iPropItemName == IDS_PROP_LEAD_OVERCUT)
	{
		ICommandPtr pCommand(new EndCutPtModifyCommand(GetEditorData(), m_pLoopStaEndCutList, var.dblVal));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}

	// 修改引入引出线类型。
	if (iPropItemName == IDS_PROP_LEAD_TYPE)
	{
		LeadType emLeadInOut = LeadInOutData::GetTypeByName(var.bstrVal);
		LeadInOutType iLeadType;
		if (pParentProp->GetName() == str5)
			iLeadType = LeadInOut_In;
		else if (pParentProp->GetName() == str6)
			iLeadType = LeadInOut_Out;

		// 修改回路的引入引出线。
		LeadListPtr pNewLeadList(new LeadList);
		for (unsigned int i = 0; i < m_pLoopStaEndCutList->size(); i++)
			pNewLeadList->push_back(LeadManager::BuildLead(iLeadType, emLeadInOut, FALSE, m_pLeadSetting.get()));
		ICommandPtr pCommand(new LeadModifyCommand(GetEditorData(), iLeadType, m_pLoopStaEndCutList, pNewLeadList));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}

	// 修改引入引出线的数据。
	if (iPropItemName == IDS_PROP_LEAD_LENGTH || iPropItemName == IDS_PROP_LEAD_ANGLE ||
		iPropItemName == IDS_PROP_LEAD_ARCRAD || iPropItemName == IDS_PROP_LEAD_ARC_ANGLE ||
		iPropItemName == IDS_PROP_LEAD_LINESPLIT)
	{
		LeadInOutType iLeadType;
		if (pParentProp->GetName() == str5)
			iLeadType = LeadInOut_In;
		else if (pParentProp->GetName() == str6)
			iLeadType = LeadInOut_Out;

		double dNewVal = var.dblVal;
		if (iPropItemName == IDS_PROP_LEAD_ANGLE || iPropItemName == IDS_PROP_LEAD_ARC_ANGLE)
			dNewVal *= D2R;

		// 修改回路的引入引出线。
		LeadListPtr pNewLeadList(new LeadList);
		for (unsigned int i = 0; i < m_pLoopStaEndCutList->size(); i++)
		{
			LoopStartCutDataPtr pLoopStartCutData = m_pLoopStaEndCutList->at(i);
			ILeadPtr pNewLead;
			if (iLeadType == LeadInOut_In)
				pNewLead = LeadManager::BuildLead(pLoopStartCutData->GetLeadIn().get(), iPropItemName, dNewVal);
			else if (iLeadType == LeadInOut_Out)
				pNewLead = LeadManager::BuildLead(pLoopStartCutData->GetLeadOut().get(), iPropItemName, dNewVal);
			pNewLeadList->push_back(pNewLead);
		}
		ICommandPtr pCommand(new LeadModifyCommand(GetEditorData(), iLeadType, m_pLoopStaEndCutList, pNewLeadList));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}

	// 更新显示。
	GetRelativeView()->Invalidate();
	m_bIgnore_OnPropertyChangeMsg = true;
	DisplayLoopLead(GetEditorData(), m_pLoopStaEndCutList, m_startPtList, m_pLeadSetting);
	m_bIgnore_OnPropertyChangeMsg = false;

	return S_OK;
}

void LoopLeadPropertyBar::DisplayLoopLead(const EditorData& editorData, LoopStartCutDataListPtr pLoopStaEndCutList, vector<Point2D> startPtList, LeadConfigItemPtr pLeadConfigItem)
{
	m_editorData = editorData;
	m_pLoopStaEndCutList = pLoopStaEndCutList;
	m_startPtList = startPtList;
	m_pLeadSetting = pLeadConfigItem;

	// init something about the prop grid ctrl.
	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	// 显示“切割开始和结束点”分组。
	DisplayStaEndPt();

	// 显示“引入/引出线设置”分组。
	bool bSomeLoop_isCutOn = false; // 是不是有的轮廓用了cut-on方式
	for (unsigned int i = 0; i < m_pLoopStaEndCutList->size(); i++)
	{
		if (m_pLoopStaEndCutList->at(i)->GetLoopCutSide() == PatternLoopCutSide_On)
		{
			bSomeLoop_isCutOn = true;
			break;
		}
	}
	if ((m_pLoopStaEndCutList->CheckSECutPtType(SECUT_PT_GEOM) || m_pLoopStaEndCutList->CheckSECutPtType(SECUT_PT_CORNER)) && !bSomeLoop_isCutOn)
	{
		DisplayLeadGroup(LeadInOut_In);
		DisplayLeadGroup(LeadInOut_Out);
	}

	if (!IsVisible())
    {
		ShowPane(TRUE, FALSE, TRUE);
    }
}

void LoopLeadPropertyBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient, rectCombo;
	GetClientRect(rectClient);

	rectCombo.SetRectEmpty();
	int cyCmb = rectCombo.Size().cy;
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, 
							   rectClient.Width(), rectClient.Height() - cyCmb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetCustomColors( SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(), SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(),
		SkinManager::GetBackgroundColor(), SkinManager::GetTextColor(), SkinManager::GetTextColor() );
}

void LoopLeadPropertyBar::DisplayStaEndPt()
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CATEGORY_STARTPOINT);
	CMFCPropertyGridProperty* pStartEndPtGroup = new CMFCPropertyGridProperty(str);

	LoopStartCutDataPtr pLoopStartCutData = m_pLoopStaEndCutList->at(0);
	Point2D startPt = m_startPtList.at(0);

	// 显示起切点位置。
	BOOL bHightlight = FALSE;
	if (m_pLoopStaEndCutList->size() > 1)
		bHightlight = TRUE;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_STARTPOINT_X);
	AddDoublePropItem(pStartEndPtGroup, str, startPt.X(), DIGITAL_NUM, FALSE, bHightlight);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_STARTPOINT_Y);
	AddDoublePropItem(pStartEndPtGroup, str, startPt.Y(), DIGITAL_NUM, FALSE, bHightlight);

	// 只有所有的起切点都在闭合回路上时才显示过切。
	if (m_pLoopStaEndCutList->CheckSECutPtType(SECUT_PT_GEOM))
	{
		GeometryStartCutPtPtr pGeomSECutPt = boost::dynamic_pointer_cast<GeometryStartCutPt>(pLoopStartCutData->GetLoopStartCutPt());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_OVERCUT);
		XGridProperty* pProp;
		if (m_pLoopStaEndCutList->HasSameOverCut(pGeomSECutPt->GetOverCut()))
			pProp = AddDoublePropItem(pStartEndPtGroup, str, pGeomSECutPt->GetOverCut(), DIGITAL_NUM);
		else
			pProp = AddDoublePropItem(pStartEndPtGroup, str, pGeomSECutPt->GetOverCut(), DIGITAL_NUM, TRUE, TRUE);
		m_propItemInfo[pProp] = IDS_PROP_LEAD_OVERCUT;
	}
	else if (m_pLoopStaEndCutList->CheckSECutPtType(SECUT_PT_CORNER))
	{
		CornerStartCutPtPtr pCornerSECutPt = boost::dynamic_pointer_cast<CornerStartCutPt>(pLoopStartCutData->GetLoopStartCutPt());
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_OVERCUT);
		XGridProperty* pProp;
		if (m_pLoopStaEndCutList->HasSameOverCut(pCornerSECutPt->GetOverCut()))
			pProp = AddDoublePropItem(pStartEndPtGroup, str, pCornerSECutPt->GetOverCut(), DIGITAL_NUM);
		else
			pProp = AddDoublePropItem(pStartEndPtGroup, str, pCornerSECutPt->GetOverCut(), DIGITAL_NUM, TRUE, TRUE);
		m_propItemInfo[pProp] = IDS_PROP_LEAD_OVERCUT;
	}

	m_wndPropList.AddProperty(pStartEndPtGroup);
}

void LoopLeadPropertyBar::DisplayLeadGroup(LeadInOutType iLeadType)
{
	CString str;
	if (iLeadType == LeadInOut_In)
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CATEGORY_LEADINDATA);
	else if (iLeadType == LeadInOut_Out)
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CATEGORY_LEADOUTDATA);
	CMFCPropertyGridProperty* pLeadGroup = new CMFCPropertyGridProperty(str);

	LeadListPtr pLeadList;
	if (iLeadType == LeadInOut_In)
		pLeadList = m_pLoopStaEndCutList->GetLeadList(LeadInOut_In);
	else if (iLeadType == LeadInOut_Out)
		pLeadList = m_pLoopStaEndCutList->GetLeadList(LeadInOut_Out);
	ILeadPtr pLead = pLeadList->at(0);

	// 显示引入/引出线类型。
	BOOL bEnableLeadData = TRUE; // 是否不能编辑引入引出数据。
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_TYPE);
	CString strLeadInType = LeadInOutData::GetNameByType(pLead->GetLeadType());
	XGridProperty* pLeadTypeProp;
	if (pLeadList->CheckLeadType(pLead->GetLeadType())) // 大家的类型是不是一样。
	{
		if (iLeadType == LeadInOut_In)
			pLeadTypeProp = AddComboPropItem(pLeadGroup, str, strLeadInType, LeadInOutData::GetAllLeadInType());
		else if (iLeadType == LeadInOut_Out)
			pLeadTypeProp = AddComboPropItem(pLeadGroup, str, strLeadInType, LeadInOutData::GetAllLeadOutType());
	}
	else
	{
		bEnableLeadData = FALSE;
		if (iLeadType == LeadInOut_In)
			pLeadTypeProp = AddComboPropItem(pLeadGroup, str, strLeadInType, LeadInOutData::GetAllLeadInType(), TRUE);
		else if (iLeadType == LeadInOut_Out)
			pLeadTypeProp = AddComboPropItem(pLeadGroup, str, strLeadInType, LeadInOutData::GetAllLeadOutType(), TRUE);
	}
	m_propItemInfo[pLeadTypeProp] = IDS_PROP_LEAD_TYPE;

	// 只要有起切点在闭合回路的角上，就不能编辑组合框。
	for (LoopStartCutDataList::iterator iter = m_pLoopStaEndCutList->begin(); iter != m_pLoopStaEndCutList->end(); iter++)
	{
		LoopStartCutDataPtr pLoopStartCutData = (LoopStartCutDataPtr)(*iter);
		if (pLoopStartCutData->GetLoopStartCutPt()->GetSECutPtType() == SECUT_PT_CORNER)
		{
			pLeadTypeProp->Enable(FALSE);
			break;
		}
	}

	// 显示引入/引出线数据。
	if (m_pLoopStaEndCutList->CheckSECutPtType(SECUT_PT_GEOM)) // 当所有的起切点都在闭合回路的非角点上时。
	{
		if (pLead->GetLeadType() == Lead_Line)
		{
			LineLeadPtr pLineLead = boost::dynamic_pointer_cast<LineLead>(pLead);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_LENGTH);
			XGridProperty* pLenProp = AddDoublePropItem(pLeadGroup, str, pLineLead->GetLength(), DIGITAL_NUM, bEnableLeadData);
			if (bEnableLeadData && !LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_LENGTH, pLineLead->GetLength()))
				pLenProp->HighlightText(TRUE);
			m_propItemInfo[pLenProp] = IDS_PROP_LEAD_LENGTH;

			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_ANGLE);
			XGridProperty* pAngProp = AddDoublePropItem(pLeadGroup, str, pLineLead->GetAngle()*R2D, DIGITAL_NUM, bEnableLeadData);
			if (bEnableLeadData && !LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_ANGLE, pLineLead->GetAngle()))
				pAngProp->HighlightText(TRUE);
			m_propItemInfo[pAngProp] = IDS_PROP_LEAD_ANGLE;
		}
		else if (pLead->GetLeadType() == Lead_Arc)
		{
			ArcLeadPtr pArcLead = boost::dynamic_pointer_cast<ArcLead>(pLead);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_ARCRAD);
			XGridProperty* pRadProp = AddDoublePropItem(pLeadGroup, str, pArcLead->GetRadius(), DIGITAL_NUM, bEnableLeadData);
			if (bEnableLeadData && !LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_ARCRAD, pArcLead->GetRadius()))
				pRadProp->HighlightText(TRUE);
			m_propItemInfo[pRadProp] = IDS_PROP_LEAD_ARCRAD;

			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_ARC_ANGLE);
			XGridProperty* pAngProp = AddDoublePropItem(pLeadGroup, str, pArcLead->GetSweepAngle()*R2D, DIGITAL_NUM, bEnableLeadData);
			if (bEnableLeadData && !LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_ARC_ANGLE, pArcLead->GetSweepAngle()))
				pAngProp->HighlightText(TRUE);
			m_propItemInfo[pAngProp] = IDS_PROP_LEAD_ARC_ANGLE;
		}
		else if (pLead->GetLeadType() == Lead_TwoLines)
		{
			TwoLineLeadPtr pTwoLineLead = boost::dynamic_pointer_cast<TwoLineLead>(pLead);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_LENGTH);
			XGridProperty* pLenProp = AddDoublePropItem(pLeadGroup, str, pTwoLineLead->GetLength(), DIGITAL_NUM, bEnableLeadData);
			if (bEnableLeadData && !LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_LENGTH, pTwoLineLead->GetLength()))
				pLenProp->HighlightText(TRUE);
			m_propItemInfo[pLenProp] = IDS_PROP_LEAD_LENGTH;

			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_ANGLE);
			XGridProperty* pAngProp = AddDoublePropItem(pLeadGroup, str, pTwoLineLead->GetAngle()*R2D, DIGITAL_NUM, bEnableLeadData);
			if (bEnableLeadData && !LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_ANGLE, pTwoLineLead->GetAngle()))
				pAngProp->HighlightText(TRUE);
			m_propItemInfo[pAngProp] = IDS_PROP_LEAD_ANGLE;

			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_LINESPLIT);
			XGridProperty* pSplitProp = AddDoublePropItem(pLeadGroup, str, pTwoLineLead->GetSplit(), DIGITAL_NUM, bEnableLeadData);
			if (bEnableLeadData && !LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_LINESPLIT, pTwoLineLead->GetSplit()))
				pSplitProp->HighlightText(TRUE);
			m_propItemInfo[pSplitProp] = IDS_PROP_LEAD_LINESPLIT;
		}
		else if (pLead->GetLeadType() == Lead_LineArc)
		{
			LineArcLeadPtr pLineArcLead = boost::dynamic_pointer_cast<LineArcLead>(pLead);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_LENGTH);
			XGridProperty* pLenProp = AddDoublePropItem(pLeadGroup, str, pLineArcLead->GetLength(), DIGITAL_NUM, bEnableLeadData);
			if (bEnableLeadData && !LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_LENGTH, pLineArcLead->GetLength()))
				pLenProp->HighlightText(TRUE);
			m_propItemInfo[pLenProp] = IDS_PROP_LEAD_LENGTH;

			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_ARC_ANGLE);
			XGridProperty* pAngProp = AddDoublePropItem(pLeadGroup, str, pLineArcLead->GetArcAngle()*R2D, DIGITAL_NUM, bEnableLeadData);
			if (bEnableLeadData && !LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_ARC_ANGLE, pLineArcLead->GetArcAngle()))
				pAngProp->HighlightText(TRUE);
			m_propItemInfo[pAngProp] = IDS_PROP_LEAD_ARC_ANGLE;
		}
		else if (pLead->GetLeadType() == Lead_Hook)
		{
			HookLeadPtr pHookLead = boost::dynamic_pointer_cast<HookLead>(pLead);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_LENGTH);
			XGridProperty* pLenProp = AddDoublePropItem(pLeadGroup, str, pHookLead->GetLength(), DIGITAL_NUM, bEnableLeadData);
			if (bEnableLeadData && !LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_LENGTH, pHookLead->GetLength()))
				pLenProp->HighlightText(TRUE);
			m_propItemInfo[pLenProp] = IDS_PROP_LEAD_LENGTH;

			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_ARCRAD);
			XGridProperty* pRadProp = AddDoublePropItem(pLeadGroup, str, pHookLead->GetRadius(), DIGITAL_NUM, bEnableLeadData);
			if (bEnableLeadData && !LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_ARCRAD, pHookLead->GetRadius()))
				pRadProp->HighlightText(TRUE);
			m_propItemInfo[pRadProp] = IDS_PROP_LEAD_ARCRAD;
		}
	}
	else if (m_pLoopStaEndCutList->CheckSECutPtType(SECUT_PT_CORNER)) // 当所有的起切点都在闭合回路的角点上时。
	{
        // 只要有一个过切值大于0，就不能修改引出线长度。
        bool bCanModifyLeadOutLength = true;
        if (iLeadType == LeadInOut_Out)
        {
            for (LoopStartCutDataList::iterator iter = m_pLoopStaEndCutList->begin(); iter != m_pLoopStaEndCutList->end(); iter++)
            {
                LoopStartCutDataPtr pLoopStartCutData = (LoopStartCutDataPtr)(*iter);
                CornerStartCutPtPtr pCornerSECutPt = boost::dynamic_pointer_cast<CornerStartCutPt>(pLoopStartCutData->GetLoopStartCutPt());
                if (fabs(pCornerSECutPt->GetOverCut()) > GEOM_TOLERANCE)
                {
                    bCanModifyLeadOutLength = false;
                    break;
                }
            }
        }

		if (pLead->GetLeadType() == Lead_Line)
		{
			LineLeadPtr pLineLead = boost::dynamic_pointer_cast<LineLead>(pLead);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_LENGTH);
			XGridProperty* pLenProp = AddDoublePropItem(pLeadGroup, str, pLineLead->GetLength(), DIGITAL_NUM, bEnableLeadData);
			if (!LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_LENGTH, pLineLead->GetLength()))
            {
				pLenProp->HighlightText(TRUE);
            }
            if (!bCanModifyLeadOutLength)
            {
                pLenProp->Enable(FALSE);
            }
			m_propItemInfo[pLenProp] = IDS_PROP_LEAD_LENGTH;
		}
		else if (pLead->GetLeadType() == Lead_Arc)
		{
			ArcLeadPtr pArcLead = boost::dynamic_pointer_cast<ArcLead>(pLead);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROP_LEAD_ARC_ANGLE);
			XGridProperty* pAngProp = AddDoublePropItem(pLeadGroup, str, pArcLead->GetSweepAngle()*R2D, DIGITAL_NUM, bEnableLeadData);
			if (!LeadManager::HaveSameVal(pLeadList.get(), IDS_PROP_LEAD_ARC_ANGLE, pArcLead->GetSweepAngle()))
            {
				pAngProp->HighlightText(TRUE);
            }
            if (!bCanModifyLeadOutLength)
            {
                pAngProp->Enable(FALSE);
            }
			m_propItemInfo[pAngProp] = IDS_PROP_LEAD_ARC_ANGLE;
		}
	}

	m_wndPropList.AddProperty(pLeadGroup);
}

END_CUTLEADER_NAMESPACE()

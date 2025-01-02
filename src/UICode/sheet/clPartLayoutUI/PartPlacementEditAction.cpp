#include "StdAfx.h"
#include "PartPlacementEditAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "DummyAction.h"
#include "clUILibResource.h"
#include "ValueInputManager.h"
#include "LogMgr.h"
#include "ClWindowFinder.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "IMaterialSize.h"

#include "clPartLayoutResource.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartPlacementSelectAction.h"
#include "PartInstanceList.h"
#include "PartInstance.h"
#include "Part.h"
#include "CanvasGeometryEditAction.h"
#include "PartLayoutHelper.h"
#include "LoopInstance.h"
#include "LoopInstanceList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementEditAction::PartPlacementEditAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacementList, PartInstanceListPtr pPartInstanceList,
	PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort)
{
    m_pMatSize = pMaterialSize;
    m_pPartPlacementList = pPartPlacementList;
    m_pPartInstanceList = pPartInstanceList;
    m_pPartGroupList = pPartGroups;
    m_pViewPort = pViewPort;

	// 缓存每种零件对应的几何轮廓拓扑数据。
	m_loopTopologyItemData = PartLayoutHelper::GetLoopTopologyItemDataOfPartInstances(pPartInstanceList.get());
}

PartPlacementEditAction::~PartPlacementEditAction(void)
{
}

EditorData PartPlacementEditAction::GetEditorData() const
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_LAYOUT);

	EditorData editorData(CWnd::FromHandle(m_pViewPort->GetWnd()), str);
	return editorData;
}

BOOL PartPlacementEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	ActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		if (m_pMatSize->GetType() == MAT_SIZE_Canvas)
			CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction();
		else // 注意，这里返回到零件选择action，也就是最初的action。
			PostMessage2EnterPartPlacementSelectAction();
	}

	return TRUE;
}

void PartPlacementEditAction::PostMessage2EnterPartPlacementSelectAction()
{
	ClWindowFinder::GetCurrentView_1()->PostMessage(WM_EnterSelectPartPlacementOrLoopInstanceAction, 2, 0);
}

void PartPlacementEditAction::PostMessage2EnterLoopInstanceSelectAction()
{
	ClWindowFinder::GetCurrentView_1()->PostMessage(WM_EnterSelectPartPlacementOrLoopInstanceAction, 1, 0);
}

void PartPlacementEditAction::PostMessage2EnterPartPlacementSelectAction(const PartPlacementList* pPartPlacements_preSelect)
{
    // 要预选中的零件放置的ID。
    if (!pPartPlacements_preSelect || pPartPlacements_preSelect->size() == 0) {
        LogMgr::DebugWarn(_T("356846"));
        return;
    }
    vector<LONGLONG>* pPartPlacementIDs_preSelect = new vector<LONGLONG>();
    for (unsigned int i = 0; i < pPartPlacements_preSelect->size(); i++) {
        const PartPlacement* pPartPlacement = pPartPlacements_preSelect->operator [](i).get();
        pPartPlacementIDs_preSelect->push_back(pPartPlacement->GetID());
    }

	ClWindowFinder::GetCurrentView_1()->PostMessage(WM_EnterSelectPartPlacementAction, (WPARAM)pPartPlacementIDs_preSelect, 0);
}

void PartPlacementEditAction::PostMessage2EnterPartPlacementSelectAction(const PartPlacement* pPartPlacement_preSelect)
{
    // 要预选中的零件放置的ID。
    if (!pPartPlacement_preSelect) {
        LogMgr::DebugWarn(_T("658742"));
        return;
    }
    vector<LONGLONG>* pPartPlacementIDs_preSelect = new vector<LONGLONG>();
    pPartPlacementIDs_preSelect->push_back(pPartPlacement_preSelect->GetID());

	ClWindowFinder::GetCurrentView_1()->PostMessage(WM_EnterSelectPartPlacementAction, (WPARAM)pPartPlacementIDs_preSelect, 0);
}

void PartPlacementEditAction::PostMessage2EnterLoopInstanceSelectAction(const LoopInstanceList* pLoopInstances_preSelect)
{
	// 要预选中的轮廓实例。
	if (!pLoopInstances_preSelect) {
		LogMgr::DebugWarn(_T("309674"));
		return;
	}
	vector<pair<LONGLONG, LONGLONG>>* pIdPairs_preSelect = new vector<pair<LONGLONG, LONGLONG>>();
	for (unsigned int i = 0; i < pLoopInstances_preSelect->size(); i++) {
		const LoopInstance* pLoopInstance = pLoopInstances_preSelect->operator[](i).get();
		pIdPairs_preSelect->push_back(make_pair(pLoopInstance->GetPartInstance()->GetID(), pLoopInstance->GetPatternLoopID()));
	}

	ClWindowFinder::GetCurrentView_1()->PostMessage(WM_EnterSelectLoopInstanceAction, (WPARAM)pIdPairs_preSelect, 0);
}

END_CUTLEADER_NAMESPACE()

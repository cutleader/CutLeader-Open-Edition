#include "StdAfx.h"
#include "CanvasGeometryEditAction.h"

#include <LoopInstance.h>
#include <LoopInstanceCache.h>

#include "baseConst.h"
#include "GlViewPort.h"
#include "DummyAction.h"
#include "clUILibResource.h"
#include "ValueInputManager.h"
#include "LogMgr.h"
#include "BasicGeometryDrawer.h"
#include "ClWindowFinder.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PartPlacement.h"
#include "Part.h"
#include "clGeometryFeatureResource.h"
#include "clCanvasGeometryUIResource.h"
#include "IPattern.h"
#include "PatternList.h"
#include "IPatternLoop.h"
#include "PartCadData.h"
#include "PatternLoopList.h"
#include "LoopToolData.h"
#include "LoopToolDataList.h"
#include "LoopCutFeatureList.h"
#include "PartCamData.h"
#include "PatternCache.h"
#include "LoopDrawer.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "LoopInstanceList.h"
#include "PartInstance.h"
#include "GeometryFeatureHelper.h"
#include "LoopCutFeature.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasGeometryEditAction::CanvasGeometryEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort)
{
    m_pViewPort = pViewPort;
    m_pPartPlacement = pPartPlacement;
}

CanvasGeometryEditAction::~CanvasGeometryEditAction(void)
{
}

EditorData CanvasGeometryEditAction::GetEditorData() const
{
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CanvasGeometry);
	EditorData editorData(CWnd::FromHandle(m_pViewPort->GetWnd()), str);
	return editorData;
}

BOOL CanvasGeometryEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	ActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar)) {
		PostMessage2EnterCanvasLoopSelectAction();
	}

	return TRUE;
}

SnapInputPtr CanvasGeometryEditAction::GetSnapInput() const
{
	PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
	DWORD dwSnapStyle = SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapCenterPtOf2DArc | SnapClosestPtTo2DLineArc | SnapClosestPtToOneAssistLine |
		SnapCrossPtOfTwo2DLineArc | SnapCrossPtOfTwoAssistLines;
	SnapInputPtr pSnapInput(new SnapInput(dwSnapStyle, pPartCadData->GetPatternList(), Matrix2D()));
	return pSnapInput;
}

void CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction()
{
	ClWindowFinder::GetCurrentView_1()->PostMessage(WM_EnterCanvasLoopSelectAction, 0, 0);
}

void CanvasGeometryEditAction::PostMessage2EnterCanvasPatternSelectAction_PreSelectOnePattern(const IPattern* pPattern)
{
	// 要预选中的几何特征(图元)。
	if (!pPattern) {
		LogMgr::DebugWarn(_T("564352"));
		return;
	}
	LONGLONG* pPatternID_preSelect = new LONGLONG(pPattern->GetID());

	ClWindowFinder::GetCurrentView_1()->PostMessage(WM_EnterCanvasPatternSelectAction_PreSelectOnePattern, (WPARAM)pPatternID_preSelect, 0);
}

void CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(const PatternLoopList* pPatternLoops, const TextStructList* pTextStructs)
{
	if (!pPatternLoops || pPatternLoops->size() == 0) {
		LogMgr::DebugWarn(_T("440090"));
		return;
	}
	if (!pTextStructs) {
		LogMgr::DebugWarn(_T("098421"));
		return;
	}

	// 要预选中的几何轮廓的ID。
	vector<LONGLONG>* pPatternLoopIDs_preSelect = new vector<LONGLONG>();
	for (unsigned int i = 0; i < pPatternLoops->size(); i++) {
		const IPatternLoop* pPatternLoop = pPatternLoops->operator [](i).get();
		pPatternLoopIDs_preSelect->push_back(pPatternLoop->GetID());
	}

	// 要预选中的文字对象的ID。
	vector<LONGLONG>* pTextStructIDs_preSelect = new vector<LONGLONG>();
	for (unsigned int i = 0; i < pTextStructs->size(); i++) {
		pTextStructIDs_preSelect->push_back(pTextStructs->operator [](i)->GetID());
	}

	ClWindowFinder::GetCurrentView_1()->PostMessage(WM_EnterCanvasLoopSelectAction_PreSelectLoopAndText, (WPARAM)pPatternLoopIDs_preSelect, (LPARAM)pTextStructIDs_preSelect);
}

void CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(const LoopInstanceList* pLoopInstances)
{
	if (!pLoopInstances || pLoopInstances->size() == 0) {
		LogMgr::DebugWarn(_T("874009"));
		return;
	}

	// 要预选中的几何轮廓的ID。
	vector<LONGLONG>* pPatternLoopIDs_preSelect = new vector<LONGLONG>();
	PatternLoopListPtr pPatternLoops_preSelect(new PatternLoopList);
	for (unsigned int i = 0; i < pLoopInstances->size(); i++) {
		const LoopInstance* pLoopInstance = pLoopInstances->operator[](i).get();
		pPatternLoopIDs_preSelect->push_back(pLoopInstance->GetCacheData()->GetPatternLoop()->GetID());
		pPatternLoops_preSelect->push_back(pLoopInstance->GetCacheData()->GetPatternLoop());
	}

	// 要预选中的文字对象的ID。
	vector<LONGLONG>* pTextStructIDs_preSelect = new vector<LONGLONG>();
	const PartCadData* pPartCadData = pLoopInstances->front()->GetPartInstance()->GetPartPlacement()->GetPart()->GetCadData().get();
	TextStructListPtr pInvolvedTexts = GeometryFeatureHelper::FindInvolvedTexts(pPartCadData, pPatternLoops_preSelect.get());
	for (unsigned int i = 0; i < pInvolvedTexts->size(); i++) {
		pTextStructIDs_preSelect->push_back(pInvolvedTexts->operator [](i)->GetID());
	}

	ClWindowFinder::GetCurrentView_1()->PostMessage(WM_EnterCanvasLoopSelectAction_PreSelectLoopAndText, (WPARAM)pPatternLoopIDs_preSelect, (LPARAM)pTextStructIDs_preSelect);
}

void CanvasGeometryEditAction::DrawActivePatterns(const PatternList* pPatterns, const map<LONGLONG, IPatternLoopPtr>& loopsOfPatterns)
{
	map<LONGLONG, COLORREF> colorOfLoops; // 缓存轮廓的绘制颜色，这样同一轮廓中的几何特征就可以公用一个颜色，可以提高这段代码的效率。
	BasicGeometryDrawer basicGeometryDrawer(m_pViewPort);
	for (unsigned int i = 0; i < pPatterns->size(); i++)
	{
		const IPattern* pPattern = pPatterns->operator [](i).get();

		// 几何特征对应的几何轮廓。
		map<LONGLONG, IPatternLoopPtr>::const_iterator loopIter = loopsOfPatterns.find(pPattern->GetID());
		if (loopIter == loopsOfPatterns.end()) {
			LogMgr::DebugWarn(_T("653892"));
			return;
		}
		const IPatternLoop* pPatternLoop = loopIter->second.get();

		// 确定该几何特征的显示颜色。
		COLORREF iColor;
		map<LONGLONG, COLORREF>::const_iterator colorIter = colorOfLoops.find(pPatternLoop->GetID());
		if (colorIter != colorOfLoops.end()) { // 该几何特征所在的轮廓的显示颜色已经计算过了。
			iColor = colorIter->second;
		}
		else {
			const LoopCutFeature* pLoopCutFeature = m_pPartPlacement->GetCamData()->GetLoopFeatureList()->FindByPatternLoopID(pPatternLoop->GetID()).get();
			if (pLoopCutFeature->GetLoopTool()->GetIsToolDiabled()) {
				iColor = *CLOptionInfo::GetInstance()->GetDisplayParam()->GetNoCutLoopColor();
			}
			else {
				iColor = pLoopCutFeature->GetLoopTool()->GetToolColor();
			}
			colorOfLoops[pPatternLoop->GetID()] = iColor;
		}

		// backup the old draw config.
		ROP_MODE ropMode;
		m_pViewPort->GetROP(ropMode);
		COLORREF dwOldColor;
		m_pViewPort->GetDrawColor(dwOldColor);
		int iOldLineWidth = m_pViewPort->GetLineWidth();

		// 绘制。
		m_pViewPort->SetDrawColor(iColor);
		m_pViewPort->SetROP(ROP_COPY);
		m_pViewPort->SetLineWidth(2);
		basicGeometryDrawer.Draw2DLineArcs(pPattern->GetCacheData()->GetBaseGeom().get());

		// restore the drawer
		m_pViewPort->SetROP(ropMode);
		m_pViewPort->SetDrawColor(dwOldColor);
		m_pViewPort->SetLineWidth(iOldLineWidth);
	}
}

void CanvasGeometryEditAction::DrawSelectedPatterns(const PatternList* pPatterns, const map<LONGLONG, IPatternLoopPtr>& loopsOfPatterns)
{
	map<LONGLONG, COLORREF> colorOfLoops; // 缓存轮廓的绘制颜色，这样同一轮廓中的几何特征就可以公用一个颜色，可以提高这段代码的效率。
	BasicGeometryDrawer basicGeometryDrawer(m_pViewPort);
	for (unsigned int i = 0; i < pPatterns->size(); i++)
	{
		const IPattern* pPattern = pPatterns->operator [](i).get();

		// 几何特征对应的几何轮廓。
		map<LONGLONG, IPatternLoopPtr>::const_iterator loopIter = loopsOfPatterns.find(pPattern->GetID());
		if (loopIter == loopsOfPatterns.end()) {
			LogMgr::DebugWarn(_T("490094"));
			return;
		}
		const IPatternLoop* pPatternLoop = loopIter->second.get();

		// 确定该几何特征的显示颜色。
		COLORREF iColor;
		map<LONGLONG, COLORREF>::const_iterator colorIter = colorOfLoops.find(pPatternLoop->GetID());
		if (colorIter != colorOfLoops.end()) { // 该几何特征所在的轮廓的显示颜色已经计算过了。
			iColor = colorIter->second;
		}
		else {
			const LoopCutFeature* pLoopCutFeature = m_pPartPlacement->GetCamData()->GetLoopFeatureList()->FindByPatternLoopID(pPatternLoop->GetID()).get();
			if (pLoopCutFeature->GetLoopTool()->GetIsToolDiabled()) {
				iColor = *CLOptionInfo::GetInstance()->GetDisplayParam()->GetNoCutLoopColor();
			}
			else {
				iColor = pLoopCutFeature->GetLoopTool()->GetToolColor();
			}
			colorOfLoops[pPatternLoop->GetID()] = iColor;
		}

		// backup the old draw config.
		bool bOldStipple = false;
		int iOldRepeat = 1;
		unsigned short iOldPattern = 0xffff;
		m_pViewPort->GetLineStipple(bOldStipple, iOldRepeat, iOldPattern);
		ROP_MODE ropMode;
		m_pViewPort->GetROP(ropMode);
		COLORREF dwOldColor;
		m_pViewPort->GetDrawColor(dwOldColor);
		int iOldLineWidth = m_pViewPort->GetLineWidth();

		// 绘制。
		m_pViewPort->SetLineStipple(true, 2, 0xcccc);
		m_pViewPort->SetDrawColor(iColor);
		m_pViewPort->SetROP(ROP_XOR);
		m_pViewPort->SetLineWidth(1);
		basicGeometryDrawer.Draw2DLineArcs(pPattern->GetCacheData()->GetBaseGeom().get());

		// restore the drawer
		m_pViewPort->SetLineStipple(bOldStipple, iOldRepeat, iOldPattern);
		m_pViewPort->SetROP(ropMode);
		m_pViewPort->SetDrawColor(dwOldColor);
		m_pViewPort->SetLineWidth(iOldLineWidth);
	}
}

void CanvasGeometryEditAction::DrawActivePatternLoop(const IPatternLoop* pPatternLoop)
{
	// 确定显示颜色。
	COLORREF iColor;
	const LoopToolData* pLoopToolData = m_pPartPlacement->GetCamData()->GetLoopFeatureList()->GetLoopToolList()->GetLoopTool_by_loopID(pPatternLoop->GetID()).get();
	if (pLoopToolData->GetIsToolDiabled())
		iColor = *CLOptionInfo::GetInstance()->GetDisplayParam()->GetNoCutLoopColor();
	else
		iColor = pLoopToolData->GetToolColor();

	LoopDrawer loopDrawer(m_pViewPort);
	loopDrawer.DrawActiveLoop(pPatternLoop, iColor);
}

void CanvasGeometryEditAction::DrawSelectedPatternLoop(const IPatternLoop* pPatternLoop)
{
	// 确定显示颜色。
	COLORREF iColor;
	const LoopToolData* pLoopToolData = m_pPartPlacement->GetCamData()->GetLoopFeatureList()->GetLoopToolList()->GetLoopTool_by_loopID(pPatternLoop->GetID()).get();
	if (pLoopToolData->GetIsToolDiabled())
		iColor = *CLOptionInfo::GetInstance()->GetDisplayParam()->GetNoCutLoopColor();
	else
		iColor = pLoopToolData->GetToolColor();

	LoopDrawer loopDrawer(m_pViewPort);
	loopDrawer.DrawSelectedLoop(pPatternLoop, iColor);
}

END_CUTLEADER_NAMESPACE()

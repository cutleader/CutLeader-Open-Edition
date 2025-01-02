#include "StdAfx.h"
#include "LoopPositionDetector.h"

#include "Polygon2D.h"
#include "Polygon2DList.h"
#include "IntegerList.h"
#include "LogMgr.h"
#include "Polygon2DBuilder.h"
#include "Polygon2DProcessor.h"
#include "LineArc2DList.h"
#include "GlViewPort.h"
#include "Line2DList.h"
#include "LineArc2DProcessor.h"
#include "Line2D.h"
#include "PolyViewDlg.h"
#include "ValueInputManager.h"

#include "CLOptionInfo.h"
#include "SheetEditParam.h"
#include "DisplayParam.h"

#include "CanvasZoneList.h"
#include "CanvasZone.h"
#include "PatternLoopList.h"
#include "PatternLoopCache.h"
#include "IPatternLoop.h"
#include "LoopTopologyItem.h"
#include "LoopTopologyItemList.h"
#include "LoopProcessor.h"
#include "PartPlacement.h"
#include "PartCadData.h"
#include "PartCamData.h"
#include "Part.h"
#include "LoopCutFeatureList.h"
#include "LoopToolData.h"
#include "LoopCutFeature.h"
#include "PartPlacementManager.h"
#include "LoopStartCutProcessor.h"
#include "clCanvasGeometryUIResource.h"
#include "LoopToolDataList.h"
#include "PatternList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

LoopPositionDetector::LoopPositionDetector(GlViewPortPtr pViewPort, LoopPositionDetectActionBar* pLoopPositionDetectActionBar)
	: m_pViewPort4ActionBar(pViewPort), m_pLoopPositionDetectActionBar(pLoopPositionDetectActionBar)
{
}

LoopPositionDetector::~LoopPositionDetector(void)
{
}

void LoopPositionDetector::UpdateDataBeforeMove(MovingLoopsType emMovingLoopsType, const PartPlacement* pUniquePartPlacement, PatternLoopListPtr pMovingPatLoops,
	LoopToolDataListPtr pToolDataOfMovingPatLoops, const Point2D& refPtOfMovingPatLoops)
{
	m_emMovingLoopsType = emMovingLoopsType;
	m_pUniquePartPlacement = pUniquePartPlacement;
	m_pMovingPatLoops = pMovingPatLoops;
	m_pToolDataOfMovingPatLoops = pToolDataOfMovingPatLoops;
	m_refPt = refPtOfMovingPatLoops;
	m_refPtOfMovingPatLoops = refPtOfMovingPatLoops;

	m_config = m_pLoopPositionDetectActionBar->GetLoopPositionConfig();

	m_pMovingPatterns = boost::dynamic_pointer_cast<PatternList>(pMovingPatLoops->GetPatList()->Clone());
	m_pMovingPatterns->UpdateCache();
	m_rectOfMovingPatLoops = pMovingPatLoops->GetRectBox(); // 移动的几何轮廓的外接矩形。

	// 计算第一层移动的几何轮廓的多边形。开放的几何轮廓会被过滤掉。
	m_pMovingPatLoops_FirstLayer.reset(new PatternLoopList);
	m_pFirstLayerPolys.reset(new Polygon2DList);
	m_pFirstLayerPolys_Expanded.reset(new Polygon2DList);
	LoopTopologyItemListPtr pLoopTopItems = LoopProcessor::CalcLoopTopStruct(pMovingPatLoops.get());
	for (unsigned int i = 0; i < pLoopTopItems->size(); i++) {
		const LoopTopologyItem* pLoopTopItem = pLoopTopItems->operator[](i).get();
		if (pLoopTopItem->GetPatternLoop()->IsOpenPath())
			continue;
		m_pMovingPatLoops_FirstLayer->push_back(pLoopTopItem->GetPatternLoop());
		Polygon2DListPtr pPolys = Polygon2DBuilder::BuildPolyListFromLineArc(pLoopTopItem->GetPatternLoop()->GetCacheData()->GetLoopBaseGeom().get(), CUTARC_FACTOR);
		if (pPolys->size() == 0) {
			LogMgr::DebugWarn(_T("753892"));
			continue;
		}
		Polygon2DProcessor::SleekPolygon(pPolys->front(), OPTPOLY_FACTOR);
		m_pFirstLayerPolys->push_back(pPolys->front());

		// 膨胀多边形。
		double dExpandValue = GetExpandValue4MovingPatLoops(pUniquePartPlacement, pLoopTopItem->GetPatternLoop().get(), m_emMovingLoopsType,
			m_pToolDataOfMovingPatLoops.get(), m_config.m_dLoopDistance, m_config.m_bEnableComCut);
		Polygon2DPtr pNewPoly = pPolys->front()->Clone();
		Polygon2DProcessor::EnlargePolygon(pNewPoly, dExpandValue, CUTARC_FACTOR, OPTPOLY_FACTOR);
		m_pFirstLayerPolys_Expanded->push_back(pNewPoly);
	}

	// 计算出底图上那些不动的闭合轮廓。
	const PartCadData* pPartCadData = pUniquePartPlacement->GetPart()->GetCadData().get();
	m_pLoopTopItems4FixedPatLoops = CalcLoopTopItems4FixedPatLoops(emMovingLoopsType, pPartCadData->GetPatternLoopList().get(), pMovingPatLoops.get());

	// 更新底图区域集合。
	m_pCanvasZones = CalcCanvasZones(m_pLoopTopItems4FixedPatLoops.get(), m_rectOfMovingPatLoops.GetWidth(), m_rectOfMovingPatLoops.GetHeight());

	// 重置这些变量。
	m_innerStickPolys.clear();
	m_outerStickPolys.clear();
	m_pStickPt.reset();
	m_pStickedCanvasZone.reset();
	m_transformMatrix = Matrix2D();
}

void LoopPositionDetector::UpdateDataBeforeMove_2(MovingLoopsType emMovingLoopsType, const PartPlacement* pUniquePartPlacement, PatternLoopListPtr pMovingPatLoops,
	const LoopTopologyItemList* pLoopTopologyItems, LoopToolDataListPtr pToolDataOfMovingPatLoops, const Point2D& refPtOfMovingPatLoops)
{
	m_emMovingLoopsType = emMovingLoopsType;
	m_pUniquePartPlacement = pUniquePartPlacement;
	m_pMovingPatLoops = pMovingPatLoops;
	m_pToolDataOfMovingPatLoops = pToolDataOfMovingPatLoops;
	m_refPt = refPtOfMovingPatLoops;
	m_refPtOfMovingPatLoops = refPtOfMovingPatLoops;

	m_config = m_pLoopPositionDetectActionBar->GetLoopPositionConfig();

	m_pMovingPatterns = boost::dynamic_pointer_cast<PatternList>(pMovingPatLoops->GetPatList()->Clone());
	m_pMovingPatterns->UpdateCache();
	m_rectOfMovingPatLoops = pMovingPatLoops->GetRectBox(); // 移动的几何轮廓的外接矩形。

	// 计算第一层移动的几何轮廓的多边形。开放的几何轮廓会被过滤掉。
	m_pMovingPatLoops_FirstLayer.reset(new PatternLoopList);
	m_pFirstLayerPolys.reset(new Polygon2DList);
	m_pFirstLayerPolys_Expanded.reset(new Polygon2DList);
	for (unsigned int i = 0; i < pLoopTopologyItems->size(); i++) {
		const LoopTopologyItem* pLoopTopItem = pLoopTopologyItems->operator[](i).get();
		if (pLoopTopItem->GetPatternLoop()->IsOpenPath())
			continue;
		m_pMovingPatLoops_FirstLayer->push_back(pLoopTopItem->GetPatternLoop());
		Polygon2DListPtr pPolys = Polygon2DBuilder::BuildPolyListFromLineArc(pLoopTopItem->GetPatternLoop()->GetCacheData()->GetLoopBaseGeom().get(), CUTARC_FACTOR);
		if (pPolys->size() == 0) {
			LogMgr::DebugWarn(_T("753892"));
			continue;
		}
		Polygon2DProcessor::SleekPolygon(pPolys->front(), OPTPOLY_FACTOR);
		m_pFirstLayerPolys->push_back(pPolys->front());

		// 膨胀多边形。
		double dExpandValue = GetExpandValue4MovingPatLoops(pUniquePartPlacement, pLoopTopItem->GetPatternLoop().get(), m_emMovingLoopsType,
			m_pToolDataOfMovingPatLoops.get(), m_config.m_dLoopDistance, m_config.m_bEnableComCut);
		Polygon2DPtr pNewPoly = pPolys->front()->Clone();
		Polygon2DProcessor::EnlargePolygon(pNewPoly, dExpandValue, CUTARC_FACTOR, OPTPOLY_FACTOR);
		m_pFirstLayerPolys_Expanded->push_back(pNewPoly);
	}

	// 计算出底图上那些不动的闭合轮廓。
	const PartCadData* pPartCadData = pUniquePartPlacement->GetPart()->GetCadData().get();
	m_pLoopTopItems4FixedPatLoops = CalcLoopTopItems4FixedPatLoops(emMovingLoopsType, pPartCadData->GetPatternLoopList().get(), pMovingPatLoops.get());

	// 更新底图区域集合。
	m_pCanvasZones = CalcCanvasZones(m_pLoopTopItems4FixedPatLoops.get(), m_rectOfMovingPatLoops.GetWidth(), m_rectOfMovingPatLoops.GetHeight());

	// 重置这些变量。
	m_innerStickPolys.clear();
	m_outerStickPolys.clear();
	m_pStickPt.reset();
	m_pStickedCanvasZone.reset();
	m_transformMatrix = Matrix2D();
}

void LoopPositionDetector::UpdateDataWhenRotate(UINT nChar)
{
	const SheetEditParam* pSheetEditParam = CLOptionInfo::GetInstance()->GetSheetEditParam().get();
	if (ValueInputManager::GetInstance()->IsInputAdd(nChar)) {
		UpdateWhenRotate(-pSheetEditParam->GetRotateStep()*D2R);
	}
	else if (ValueInputManager::GetInstance()->IsInputDes(nChar)) {
		UpdateWhenRotate(pSheetEditParam->GetRotateStep()*D2R);
	}
	else if (!ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputS(nChar)) { // 逆时针旋转90度。
		UpdateWhenRotate(PID2);
	}
	else if (!ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputW(nChar)) { // 顺时针旋转90度。
		UpdateWhenRotate(-PID2);
	}
}

void LoopPositionDetector::UpdateWhenRotate(double dRotateAngle)
{
	// 旋转移动中的轮廓。
	Matrix2D rotateMatrix;
	rotateMatrix.RotateBy(m_rectOfMovingPatLoops.GetCenterPt(), dRotateAngle);
	m_pMovingPatterns->TransformAndUpdateCache(rotateMatrix);
	m_transformMatrix *= rotateMatrix;
	m_pFirstLayerPolys->Transform(rotateMatrix);
	m_pFirstLayerPolys_Expanded->Transform(rotateMatrix);

	// 把旋转后轮廓的中心点校对到鼠标点。注意，m_refPtOfMovingPartPmts在移动时已经更新过了。
	Matrix2D transferMatrix;
	transferMatrix.SetTransfer(Vector2D(m_pMovingPatterns->GetRectBox().GetCenterPt(), m_refPtOfMovingPatLoops));
	m_pMovingPatterns->TransformAndUpdateCache(transferMatrix);
	m_transformMatrix *= transferMatrix;
	m_pFirstLayerPolys->Transform(transferMatrix);
	m_pFirstLayerPolys_Expanded->Transform(transferMatrix);

	// 更新移动的几何轮廓外接矩形。
	m_rectOfMovingPatLoops = m_pMovingPatterns->GetRectBox();

	// 重置一些变量。
	m_innerStickPolys.clear();
	m_outerStickPolys.clear();
	m_pStickPt.reset();
	m_pStickedCanvasZone.reset();
	m_pStickPoly.reset();
	m_pAlignmentAssistLine.reset();
}

void LoopPositionDetector::UpdateDataWhenMove(const Point2D& cursorPosition)
{
	// 先检查一些不能为空的数据。
	if (!m_pUniquePartPlacement || !m_pMovingPatLoops || !m_pFirstLayerPolys || !m_pCanvasZones) {
		LogMgr::DebugWarn(_T("409882"));
		return;
	}

	// 重置一些变量。
	m_pAlignmentAssistLine.reset();

	// 在移动过程中如果参数变化了，需要更新一些数据。一般在移动的过程中修改参数，更新数据的方式就是看起来像从当前位置再次启动移动。
	LoopPositionConfig config = m_pLoopPositionDetectActionBar->GetLoopPositionConfig();
	if (config.IsDifferent(m_config))
	{
		m_config = config;

		Vector2D transferVect(m_refPt, cursorPosition);
		m_pMovingPatterns = boost::dynamic_pointer_cast<PatternList>(m_pMovingPatLoops->GetPatList()->Clone());
		m_pMovingPatterns->UpdateCache();
		m_pMovingPatterns->TransferAndUpdateCache(transferVect);
		m_transformMatrix.Reset();
		m_transformMatrix.SetTransfer(transferVect);

		// 更新移动的轮廓的外接矩形。
		m_rectOfMovingPatLoops = m_pMovingPatLoops->GetRectBox();
		m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());

		// 更新移动参考点。
		m_refPtOfMovingPatLoops = m_refPt;
		m_refPtOfMovingPatLoops.Transfer(transferVect);

		// 更新第一层移动的几何轮廓的多边形。开放的几何轮廓会被过滤掉。
		{
			m_pFirstLayerPolys->clear();
			m_pFirstLayerPolys_Expanded->clear();
			LoopTopologyItemListPtr pLoopTopItems = LoopProcessor::CalcLoopTopStruct(m_pMovingPatLoops.get());
			for (unsigned int i = 0; i < pLoopTopItems->size(); i++)
			{
				const LoopTopologyItem* pLoopTopItem = pLoopTopItems->operator[](i).get();
				if (pLoopTopItem->GetPatternLoop()->IsOpenPath())
					continue;
				Polygon2DListPtr pPolys = Polygon2DBuilder::BuildPolyListFromLineArc(pLoopTopItem->GetPatternLoop()->GetCacheData()->GetLoopBaseGeom().get(), CUTARC_FACTOR);
				if (pPolys->size() == 0) { LogMgr::DebugWarn(_T("449977")); continue; }
				Polygon2DProcessor::SleekPolygon(pPolys->front(), OPTPOLY_FACTOR);
				m_pFirstLayerPolys->push_back(pPolys->front());

				// 膨胀多边形。
				double dExpandValue = GetExpandValue4MovingPatLoops(m_pUniquePartPlacement, pLoopTopItem->GetPatternLoop().get(), m_emMovingLoopsType,
					m_pToolDataOfMovingPatLoops.get(), m_config.m_dLoopDistance, m_config.m_bEnableComCut);
				Polygon2DPtr pNewPoly = pPolys->front()->Clone();
				Polygon2DProcessor::EnlargePolygon(pNewPoly, dExpandValue, CUTARC_FACTOR, OPTPOLY_FACTOR);
				m_pFirstLayerPolys_Expanded->push_back(pNewPoly);
			}
			m_pFirstLayerPolys->Transfer(transferVect);
			m_pFirstLayerPolys_Expanded->Transfer(transferVect);
		}

		// 重置一些变量。
		m_innerStickPolys.clear();
		m_outerStickPolys.clear();
		m_pStickPt.reset();
		m_pStickedCanvasZone.reset();
	}

	if (m_pFirstLayerPolys->size() == 0) { // 此时选中的应该都是开放几何轮廓，那么就取消距离检测。
		m_pStickPt.reset();
		m_pStickedCanvasZone.reset();
		return;
	}

	// 这里先拦截一下，如果选中的轮廓不满足做共边的条件，那么就取消距离检测。其实在这之前对第一层移动的轮廓进行膨胀时就可以拦截掉，不过这里拦截也可以。
	if (m_config.m_bEnableComCut && !IsMovingPatLoopsCanComCut(m_pUniquePartPlacement, m_pMovingPatLoops_FirstLayer.get(), m_emMovingLoopsType, m_pToolDataOfMovingPatLoops.get())) {
		m_pStickPt.reset();
		m_pStickedCanvasZone.reset();
		return;
	}

	// 检测距离吸附点。
	if (!m_pStickedCanvasZone) // 此时没有距离吸附点。
	{
		Vector2D transferVect(m_refPtOfMovingPatLoops, cursorPosition); // 几何轮廓的移动向量。
		m_pMovingPatterns->TransferAndUpdateCache(transferVect);
		m_transformMatrix.Transfer(transferVect);
		m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());
		m_pFirstLayerPolys->Transfer(transferVect);
		m_pFirstLayerPolys_Expanded->Transfer(transferVect);

		if (m_config.m_bEnableLoopDistanceDetect) {
			DetectStickPt4LoopDistance_1(cursorPosition);
		}
		if (m_pStickedCanvasZone) { // 检测到了距离吸附点，更新一些数据。
			Vector2D transferVect(cursorPosition, *m_pStickPt); // 几何轮廓的移动向量。
			m_pMovingPatterns->TransferAndUpdateCache(transferVect);
			m_transformMatrix.Transfer(transferVect);
			m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());
			m_pFirstLayerPolys->Transfer(transferVect);
			m_pFirstLayerPolys_Expanded->Transfer(transferVect);
			m_refPtOfMovingPatLoops = *m_pStickPt; // 更新移动参考点。
		}
		else { // 没有检测到距离吸附点。
			m_refPtOfMovingPatLoops = cursorPosition; // 更新移动参考点。
		}
	}
	else { // 此时有距离吸附点。
		if (m_config.m_bEnableLoopDistanceDetect) {
			DetectStickPt4LoopDistance_2(cursorPosition);
		}
		if (m_pStickedCanvasZone) { // 再一次检测到了距离吸附点，更新一些数据。
			Vector2D transferVect(m_refPtOfMovingPatLoops, *m_pStickPt); // 几何轮廓的移动向量。
			m_pMovingPatterns->TransferAndUpdateCache(transferVect);
			m_transformMatrix.Transfer(transferVect);
			m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());
			m_pFirstLayerPolys->Transfer(transferVect);
			m_pFirstLayerPolys_Expanded->Transfer(transferVect);
			m_refPtOfMovingPatLoops = *m_pStickPt; // 更新移动参考点。
		}
		else { // 逃离了距离吸附，更新一些数据。
			Vector2D transferVect(m_refPtOfMovingPatLoops, cursorPosition); // 几何轮廓的移动向量。
			m_pMovingPatterns->TransferAndUpdateCache(transferVect);
			m_transformMatrix.Transfer(transferVect);
			m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());
			m_pFirstLayerPolys->Transfer(transferVect);
			m_pFirstLayerPolys_Expanded->Transfer(transferVect);
			m_refPtOfMovingPatLoops = cursorPosition; // 更新移动参考点。
		}
	}

	// 检测对齐吸附点。
	{
		if (m_config.m_bEnableLoopAlignmentDetect) {
			DetectStickPt4Alignment(cursorPosition);
		}
		if (m_pAlignmentAssistLine) {
			Vector2D transferVect(m_refPtOfMovingPatLoops, *m_pStickPt); // 几何轮廓的移动向量。
			m_pMovingPatterns->TransferAndUpdateCache(transferVect);
			m_transformMatrix.Transfer(transferVect);
			m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());
			m_pFirstLayerPolys->Transfer(transferVect);
			m_pFirstLayerPolys_Expanded->Transfer(transferVect);
			m_refPtOfMovingPatLoops = *m_pStickPt; // 更新移动参考点。
		}
		else { // 没有检测到对齐点，这里不需要做更新了，否则不对了。
			//Vector2D transferVect(m_refPtOfMovingPatLoops, cursorPosition); // 几何轮廓的移动向量。
			//m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());
			//m_pFirstLayerPolys->Transfer(transferVect);
			//m_pFirstLayerPolys_Expanded->Transfer(transferVect);
			//m_refPtOfMovingPatLoops = cursorPosition; // 更新移动参考点。
		}
	}
}

void LoopPositionDetector::DrawAssistInfo(const CString& strExtraTip)
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	COLORREF dwOldColor;
	m_pViewPort4ActionBar->GetDrawColor(dwOldColor);
	m_pViewPort4ActionBar->SetDrawColor(*pDisplayParam->GetDrawAssistColor());

	// 绘制提示文字。
	{
		double dCursorPtX, dCursorPtY;
		m_pViewPort4ActionBar->GetCursorPos(dCursorPtX, dCursorPtY);
		double dOffset = 8.0;
		m_pViewPort4ActionBar->DToW(dOffset);

		CString strTip;
		if (m_pStickedCanvasZone && m_pAlignmentAssistLine)
			strTip = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Find_Stick_Align_Position);
		else if (m_pStickedCanvasZone)
			strTip = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Find_StickPosition);
		else if (m_pAlignmentAssistLine)
			strTip = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Find_AlignPosition);

		// 额外要显示的提示。
		if (strExtraTip.GetLength() > 0)
		{
			strTip += _T(" (");
			strTip += strExtraTip;
			strTip += _T(")");
		}

		m_pViewPort4ActionBar->DrawString(strTip.GetString(), dCursorPtX + dOffset, dCursorPtY - dOffset * 2.0, .0, .0);
	}

	// 绘制对齐辅助线。
	if (m_pAlignmentAssistLine)
	{
		bool bStipple = false;
		int nRepeat = 0;
		unsigned short iPattern = 1;
		m_pViewPort4ActionBar->GetLineStipple(bStipple, nRepeat, iPattern);
		m_pViewPort4ActionBar->SetLineStipple(true, 0, 0xf0f0);
		const Point2D& startPt = m_pAlignmentAssistLine->GetStartPoint();
		const Point2D& endPt = m_pAlignmentAssistLine->GetEndPoint();
		m_pViewPort4ActionBar->DrawLinePoint2D(startPt.X(), startPt.Y(), endPt.X(), endPt.Y());
		m_pViewPort4ActionBar->SetLineStipple(false, nRepeat, iPattern);
	}

	m_pViewPort4ActionBar->SetDrawColor(dwOldColor);
}

bool LoopPositionDetector::IsMovingPatLoopsCanComCut(const PartPlacement* pUniquePartPlacement, const PatternLoopList* pMovingPatLoops_FirstLayer,
	MovingLoopsType emMovingLoopsType, const LoopToolDataList* pToolDataOfMovingPatLoops)
{
	const LoopCutFeatureList* pLoopCutFeatures = pUniquePartPlacement->GetCamData()->GetLoopFeatureList().get();

	// 选中的轮廓要求都是阳切。
	if (emMovingLoopsType == MoveNewLoop) // 正在往底图导入新的图元，因为把这些新图元当成一个零件，此时认为第一层轮廓都是阳切。
		;
	else
	{
		for (unsigned int i = 0; i < pMovingPatLoops_FirstLayer->size(); i++)
		{
			const IPatternLoop* pPatternLoop = pMovingPatLoops_FirstLayer->operator[](i).get();
			const LoopStartCutData* pLoopStartCutData = pLoopCutFeatures->FindByPatternLoopID(pPatternLoop->GetID())->GetLoopStartCutData().get();
			PatternLoopCutOffsetType emPatternLoopCutOffsetType = LoopStartCutProcessor::CalculateLoopCutOffset(pLoopStartCutData);
			if (emPatternLoopCutOffsetType != PatternLoopCutOffset_Outside)
				return false;
		}
	}

	// 选中的轮廓需要使用相同的刀具尺寸。
	set<double> toolSizeList;
	for (unsigned int i = 0; i < pMovingPatLoops_FirstLayer->size(); i++)
	{
		const IPatternLoop* pPatternLoop = pMovingPatLoops_FirstLayer->operator[](i).get();
		if (emMovingLoopsType == MoveNewLoop)
			toolSizeList.insert(pToolDataOfMovingPatLoops->GetLoopTool_by_loopID(pPatternLoop->GetID())->GetToolWidth());
		else
			toolSizeList.insert(pLoopCutFeatures->FindByPatternLoopID(pPatternLoop->GetID())->GetLoopTool()->GetToolWidth());
	}
	if (toolSizeList.size() != 1)
		return false;

	return true;
}

bool LoopPositionDetector::IsMovingLoopsCanComCutWithCanvasZoneInnerPoly(const PartPlacement* pUniquePartPlacement, const LoopTopologyItemList* pLoopTopItems4FixedPatLoops,
	const PatternLoopList* pMovingPatLoops, MovingLoopsType emMovingLoopsType, const LoopToolDataList* pToolDataOfMovingPatLoops, LONGLONG iPatLoopID4CanvasZonePoly)
{
	const PartCadData* pPartCadData = pUniquePartPlacement->GetPart()->GetCadData().get();
	const LoopCutFeatureList* pLoopCutFeatures = pUniquePartPlacement->GetCamData()->GetLoopFeatureList().get();

	// 底图区域内轮廓要在奇数层。
	{
		int iLayerIndex = pLoopTopItems4FixedPatLoops->GetLayerIndexOfPatternLoop(iPatLoopID4CanvasZonePoly);
		if (iLayerIndex % 2 != 0)
			return false;
	}

	// 移动的轮廓的刀具尺寸要和底图区域内轮廓的相同。
	{
		double dToolWidthOfMovingLoop = .0;
		if (emMovingLoopsType == MoveNewLoop)
			dToolWidthOfMovingLoop = pToolDataOfMovingPatLoops->GetLoopTool_by_loopID(pMovingPatLoops->front()->GetID())->GetToolWidth();
		else
			dToolWidthOfMovingLoop = pLoopCutFeatures->FindByPatternLoopID(pMovingPatLoops->front()->GetID())->GetLoopTool()->GetToolWidth();
		const LoopToolData* pLoopToolData_2 = pLoopCutFeatures->FindByPatternLoopID(iPatLoopID4CanvasZonePoly)->GetLoopTool().get();
		if (fabs(dToolWidthOfMovingLoop - pLoopToolData_2->GetToolWidth()) > GEOM_TOLERANCE)
			return false;
	}

	// 底图区域内轮廓要阳切。
	{
		const LoopStartCutData* pLoopStartCutData = pLoopCutFeatures->FindByPatternLoopID(iPatLoopID4CanvasZonePoly)->GetLoopStartCutData().get();
		PatternLoopCutOffsetType emPatternLoopCutOffsetType = LoopStartCutProcessor::CalculateLoopCutOffset(pLoopStartCutData);
		if (emPatternLoopCutOffsetType != PatternLoopCutOffset_Outside)
			return false;
	}

	return true;
}

bool LoopPositionDetector::CheckAlignment(const CanvasZone* pCanvasZone, const PatternLoopList* pAllPatternLoops, const Rect2D& rectOfMovingPatLoops, double dStickDistance,
	Rect2D& alignRect, RECT_SIDE& emRectSide_1, RECT_SIDE& emRectSide_2)
{
	bool bRet = false;

	double dMinDistance = INFINITE_VAL;
	const Polygon2DList* pCanvasZoneInnerPolys = pCanvasZone->GetInnerPolys().get();
	for (unsigned int i = 0; i < pCanvasZoneInnerPolys->size(); i++)
	{
		const Polygon2D* pCanvasZoneInnerPoly = pCanvasZoneInnerPolys->operator[](i).get();
		LONGLONG iPatternLoopID = pCanvasZone->GetPatLoopIDs4InnerPolys()->at(i);
		const Rect2D& innerPolyRect = pAllPatternLoops->GetLoopByID(iPatternLoopID)->GetCacheData()->GetLoopRect();
		if (rectOfMovingPatLoops.DistanceTo(innerPolyRect) < rectOfMovingPatLoops.GetLongEdge()) // 选出离移动的几何轮廓较近的底图区域内轮廓。
		{
			tuple<bool, RECT_SIDE, RECT_SIDE, double> alignData = CheckTwoRectAlignment(innerPolyRect, rectOfMovingPatLoops, dStickDistance);
			if (get<0>(alignData) && get<3>(alignData) < dMinDistance)
			{
				bRet = true;

				alignRect = innerPolyRect;
				emRectSide_1 = get<1>(alignData);
				emRectSide_2 = get<2>(alignData);
				dMinDistance = get<3>(alignData);
			}
		}
	}

	return bRet;
}

Line2DPtr LoopPositionDetector::CreateAlignmentAssistLine(const Rect2D& alignRect, RECT_SIDE emRectSide, const GlViewPort* pViewPort)
{
	Line2DPtr pAlignmentAssistLine;

	// 得到视图区域的长宽。
	double dWorldRange, dScreenLength, dScreenWidth, dLeftBottomX, dLeftBottomY;
	pViewPort->GetDrawingArea(dWorldRange, dScreenLength, dScreenWidth, dLeftBottomX, dLeftBottomY);
	pViewPort->DToW(dScreenLength);
	pViewPort->DToW(dScreenWidth);

	if (emRectSide == RECT_LEFT) // 生成一个竖直线。
		pAlignmentAssistLine.reset(new Line2D(Point2D(alignRect.GetXMin(), dLeftBottomY), Point2D(alignRect.GetXMin(), dLeftBottomY + dScreenWidth)));
	else if (emRectSide == RECT_RIGHT) // 生成一个竖直线。
		pAlignmentAssistLine.reset(new Line2D(Point2D(alignRect.GetXMax(), dLeftBottomY), Point2D(alignRect.GetXMax(), dLeftBottomY + dScreenWidth)));
	else if (emRectSide == RECT_BOTTOM) // 生成一个水平线。
		pAlignmentAssistLine.reset(new Line2D(Point2D(dLeftBottomX, alignRect.GetYMin()), Point2D(dLeftBottomX + dScreenLength, alignRect.GetYMin())));
	else if (emRectSide == RECT_TOP) // 生成一个水平线。
		pAlignmentAssistLine.reset(new Line2D(Point2D(dLeftBottomX, alignRect.GetYMax()), Point2D(dLeftBottomX + dScreenLength, alignRect.GetYMax())));

	return pAlignmentAssistLine;
}

Vector2D LoopPositionDetector::CalcTransferVect4Alignment(const Rect2D& alignRect, RECT_SIDE emRectSide_1, const Rect2D& oldRect, RECT_SIDE emRectSide_2)
{
	Vector2D transferVect;

	if (emRectSide_1 == RECT_LEFT)
	{
		if (emRectSide_2 == RECT_LEFT)
			transferVect.Components(alignRect.GetXMin() - oldRect.GetXMin(), .0);
		else if (emRectSide_2 == RECT_RIGHT)
			transferVect.Components(alignRect.GetXMin() - oldRect.GetXMax(), .0);
	}
	else if (emRectSide_1 == RECT_RIGHT)
	{
		if (emRectSide_2 == RECT_LEFT)
			transferVect.Components(alignRect.GetXMax() - oldRect.GetXMin(), .0);
		else if (emRectSide_2 == RECT_RIGHT)
			transferVect.Components(alignRect.GetXMax() - oldRect.GetXMax(), .0);
	}
	else if (emRectSide_1 == RECT_BOTTOM)
	{
		if (emRectSide_2 == RECT_BOTTOM)
			transferVect.Components(.0, alignRect.GetYMin() - oldRect.GetYMin());
		else if (emRectSide_2 == RECT_TOP)
			transferVect.Components(.0, alignRect.GetYMin() - oldRect.GetYMax());
	}
	else if (emRectSide_1 == RECT_TOP)
	{
		if (emRectSide_2 == RECT_BOTTOM)
			transferVect.Components(.0, alignRect.GetYMax() - oldRect.GetYMin());
		else if (emRectSide_2 == RECT_TOP)
			transferVect.Components(.0, alignRect.GetYMax() - oldRect.GetYMax());
	}

	return transferVect;
}

void LoopPositionDetector::DetectStickPt4Alignment(const Point2D& cursorPosition)
{
	if (m_pAlignmentAssistLine) // 此时不应该存在对齐点。
	{
		LogMgr::DebugWarn(_T("448066"));
		return;
	}

	double dStickDistance = CLOptionInfo::GetInstance()->GetSheetEditParam()->GetStickDis();
	m_pViewPort4ActionBar->DToW(dStickDistance);

	// 先挑中一个底图区域，判断选中的轮廓和它的内轮廓之间的对齐关系。
	const PatternLoopList* pAllPatternLoops = m_pUniquePartPlacement->GetPart()->GetCadData()->GetPatternLoopList().get();
	CanvasZonePtr pCanvasZone = m_pCanvasZones->GetContainer(m_pFirstLayerPolys.get(), m_rectOfMovingPatLoops, pAllPatternLoops);
	if (!pCanvasZone)
		return;

	// 根据移动的轮廓的外接矩形，找出对齐信息。
	Rect2D alignRect;
	RECT_SIDE emRectSide_1, emRectSide_2;
	if (CheckAlignment(pCanvasZone.get(), pAllPatternLoops, m_rectOfMovingPatLoops, dStickDistance, alignRect, emRectSide_1, emRectSide_2)) // 检测到了对齐。
	{
		// 创建对齐辅助线，表示找到了对齐吸附点。
		m_pAlignmentAssistLine = CreateAlignmentAssistLine(alignRect, emRectSide_1, m_pViewPort4ActionBar.get());

		// 为了对齐要移动的向量。
		Vector2D transferVect = CalcTransferVect4Alignment(alignRect, emRectSide_1, m_rectOfMovingPatLoops, emRectSide_2);

		if (!m_pStickedCanvasZone) // 此前不存在“距离吸附点”。
		{
			m_pStickPt.reset(new Point2D(cursorPosition));
			m_pStickPt->Transfer(transferVect);
		}
		else // 此前已存在“距离吸附点”。
		{
			if (!m_pStickPoly) // 此时m_pStickPoly应该有值。
			{
				LogMgr::DebugWarn(_T("309452"));
				return;
			}

			// 判断“距离吸附点”还有没有效。如果找到的“对齐吸附点”破坏了之前的轮廓距离，下面争取校正一下。
			Point2D currentStickPt = *m_pStickPt;
			currentStickPt.Transfer(transferVect);
			if (m_pStickPoly->GetPointPos(currentStickPt) != ON_POLY) // 找到的“对齐吸附点”破坏了之前的轮廓距离条件。
			{
				// to be commented.
				{
					//PolyViewDlg polyViewDlg(_T("1"), NULL, TRUE);
					//polyViewDlg.AddPolygon(m_pStickPoly, RGB(255, 0, 0));
					//polyViewDlg.AddPoint(m_pStickPt, RGB(255, 0, 0));
					//Point2DPtr pCurrentStickPt(new Point2D(currentStickPt));
					//polyViewDlg.AddPoint(pCurrentStickPt, RGB(0, 0, 0));
					//polyViewDlg.DoModal();
				}

				// 争取校正一下。
				if (fabs(transferVect.X()) > GEOM_TOLERANCE) // 向左或者向右对齐了。
				{
					Point2DListPtr pCrossPts_1 = Polygon2DProcessor::GetCrossPtsWithLine(m_pStickPoly.get(), currentStickPt, POSITION_UP, FALSE);
					Point2DListPtr pCrossPts_2 = Polygon2DProcessor::GetCrossPtsWithLine(m_pStickPoly.get(), currentStickPt, POSITION_DOWN, FALSE);
					Point2DListPtr pCrossPts(new Point2DList);
					pCrossPts->insert(pCrossPts->end(), pCrossPts_1->begin(), pCrossPts_1->end());
					pCrossPts->insert(pCrossPts->end(), pCrossPts_2->begin(), pCrossPts_2->end());
					if (pCrossPts->size() == 0) // 校正失败。
					{
						m_pStickedCanvasZone.reset();
						m_pStickPoly.reset();
						m_pStickPt.reset(new Point2D(currentStickPt));
					}
					else // 校正成功。
					{
						Point2D newStickPt = pCrossPts->GetPtClosestTo(currentStickPt);
						m_pStickPt.reset(new Point2D(newStickPt));
					}
				}
				else if (fabs(transferVect.Y()) > GEOM_TOLERANCE) // 向上或者向下对齐了。
				{
					Point2DListPtr pCrossPts_1 = Polygon2DProcessor::GetCrossPtsWithLine(m_pStickPoly.get(), currentStickPt, POSITION_LEFT, FALSE);
					Point2DListPtr pCrossPts_2 = Polygon2DProcessor::GetCrossPtsWithLine(m_pStickPoly.get(), currentStickPt, POSITION_RIGHT, FALSE);
					Point2DListPtr pCrossPts(new Point2DList);
					pCrossPts->insert(pCrossPts->end(), pCrossPts_1->begin(), pCrossPts_1->end());
					pCrossPts->insert(pCrossPts->end(), pCrossPts_2->begin(), pCrossPts_2->end());
					if (pCrossPts->size() == 0) // 校正失败。
					{
						m_pStickedCanvasZone.reset();
						m_pStickPoly.reset();
						m_pStickPt.reset(new Point2D(currentStickPt));
					}
					else // 校正成功。
					{
						Point2D newStickPt = pCrossPts->GetPtClosestTo(currentStickPt);
						m_pStickPt.reset(new Point2D(newStickPt));
					}
				}
			}
			else // 找到的“对齐吸附点”也满足之前的轮廓距离条件。
				m_pStickPt.reset(new Point2D(currentStickPt));
		}
	}
	else // 没有检测到对齐。
	{
		if (!m_pStickedCanvasZone) // 此前不存在“距离吸附点”。
			m_pStickPt.reset();
	}
}

tuple<bool, RECT_SIDE, RECT_SIDE, double> LoopPositionDetector::CheckTwoRectAlignment(const Rect2D& rect_1, const Rect2D& rect_2, double dStickDistance)
{
	RECT_SIDE emRectSide_1, emRectSide_2;
	double dMinDistance = INFINITE_VAL;

	// 对比两个矩形的水平边。
	{
		double dDistance_1 = fabs(rect_1.GetYMin() - rect_2.GetYMin());
		if (dDistance_1 < dMinDistance)
		{
			emRectSide_1 = RECT_BOTTOM;
			emRectSide_2 = RECT_BOTTOM;
			dMinDistance = dDistance_1;
		}

		double dDistance_2 = fabs(rect_1.GetYMax() - rect_2.GetYMax());
		if (dDistance_2 < dMinDistance)
		{
			emRectSide_1 = RECT_TOP;
			emRectSide_2 = RECT_TOP;
			dMinDistance = dDistance_2;
		}
	}

	// 对比两个矩形的竖直边。
	{
		double dDistance_1 = fabs(rect_1.GetXMin() - rect_2.GetXMin());
		if (dDistance_1 < dMinDistance)
		{
			emRectSide_1 = RECT_LEFT;
			emRectSide_2 = RECT_LEFT;
			dMinDistance = dDistance_1;
		}

		double dDistance_2 = fabs(rect_1.GetXMax() - rect_2.GetXMax());
		if (dDistance_2 < dMinDistance)
		{
			emRectSide_1 = RECT_RIGHT;
			emRectSide_2 = RECT_RIGHT;
			dMinDistance = dDistance_2;
		}
	}

	tuple<bool, RECT_SIDE, RECT_SIDE, double> alignData;
	if (dMinDistance < dStickDistance)
		alignData = make_tuple(true, emRectSide_1, emRectSide_2, dMinDistance);
	else
		alignData = make_tuple(false, emRectSide_1, emRectSide_2, dMinDistance);
	return alignData;
}

void LoopPositionDetector::DetectStickPt4LoopDistance_1(const Point2D& cursorPosition)
{
	if (m_pStickedCanvasZone) // 此时不应该处于距离吸附状态。
	{
		LogMgr::DebugWarn(_T("468821"));
		return;
	}

	double dStickDistance = CLOptionInfo::GetInstance()->GetSheetEditParam()->GetStickDis();
	m_pViewPort4ActionBar->DToW(dStickDistance);

	// 先挑中一个底图区域，判断选中的轮廓和它的吸附关系。
	const PatternLoopList* pAllPatternLoops = m_pUniquePartPlacement->GetPart()->GetCadData()->GetPatternLoopList().get();
	CanvasZonePtr pCanvasZone = m_pCanvasZones->GetContainer(m_pFirstLayerPolys.get(), m_rectOfMovingPatLoops, pAllPatternLoops);
	if (!pCanvasZone)
		return;

	// 计算选中的几何轮廓和pCanvasZone外轮廓的“内靠碰轨迹”。
	Polygon2DListPtr pInnerStickPolys(new Polygon2DList);
	if (!m_config.m_bEnableComCut) // 不能和包含自己的轮廓共边。
	{
		const Polygon2D* pOuterPoly = pCanvasZone->GetOuterPoly().get();
		pInnerStickPolys = m_innerStickPolys[pOuterPoly->GetPolygonID()];
		if (!pInnerStickPolys)
		{
			double dCompressValue = GetCompressValue4CanvasZoneOuterPoly(m_pUniquePartPlacement, pCanvasZone.get(), m_config.m_dLoopDistance, m_config.m_bEnableComCut);
			pInnerStickPolys = CalcInnerStickPolys(cursorPosition, pCanvasZone.get(), dCompressValue);
			if (pInnerStickPolys)
				m_innerStickPolys[pOuterPoly->GetPolygonID()] = pInnerStickPolys;
			else
				return; // 选中的零件无法放到底图区域内部。
		}
	}

	// 计算选中的几何轮廓和离的较近的底图区域内轮廓的“外靠碰轨迹”。
	Polygon2DListPtr pOuterStickPolys(new Polygon2DList);
	{
		const Polygon2DList* pCanvasZoneInnerPolys = pCanvasZone->GetInnerPolys().get();
		const IntegerList* pPatLoopIDs4InnerPolys = pCanvasZone->GetPatLoopIDs4InnerPolys().get();
		for (unsigned int i = 0; i < pCanvasZoneInnerPolys->size(); i++)
		{
			const Polygon2D* pCanvasZoneInnerPoly = pCanvasZoneInnerPolys->operator[](i).get();
			LONGLONG iPatternLoopID = pPatLoopIDs4InnerPolys->at(i);

			// 判断能不能和这个底图区域内轮廓进行共边。
			if (m_config.m_bEnableComCut && !IsMovingLoopsCanComCutWithCanvasZoneInnerPoly(m_pUniquePartPlacement, m_pLoopTopItems4FixedPatLoops.get(),
				m_pMovingPatLoops.get(), m_emMovingLoopsType, m_pToolDataOfMovingPatLoops.get(), iPatternLoopID))
				continue;

			const Rect2D& innerPolyRect = pAllPatternLoops->GetLoopByID(iPatternLoopID)->GetCacheData()->GetLoopRect();
			double dExpandValue = GetExpandValue4CanvasZoneInnerPoly(m_pUniquePartPlacement, iPatternLoopID, m_config.m_dLoopDistance, m_config.m_bEnableComCut);
			if (m_rectOfMovingPatLoops.DistanceTo(innerPolyRect) < (dExpandValue * 2.0 + dStickDistance)) // 选出离移动的几何轮廓较近的底图区域内轮廓。
			{
				Polygon2DPtr pOuterStickPoly = m_outerStickPolys[pCanvasZoneInnerPoly->GetPolygonID()];
				if (!pOuterStickPoly)
				{
					pOuterStickPoly = CalcOuterStickPolys(cursorPosition, pCanvasZoneInnerPoly, dExpandValue);
					m_outerStickPolys[pCanvasZoneInnerPoly->GetPolygonID()] = pOuterStickPoly;
				}
				pOuterStickPolys->push_back(pOuterStickPoly);
			}
		}
	}

	// 如果没有计算出靠碰轨迹，那么就取消距离检测。
	if (pInnerStickPolys->size() == 0 && pOuterStickPolys->size() == 0)
	{
		m_pStickedCanvasZone.reset();
		m_pStickPt.reset();
		return;
	}

	// 移动的几何轮廓不能和底图区域外轮廓和附近的底图区域内轮廓相交。
	for (unsigned int i = 0; i < pInnerStickPolys->size(); i++)
	{
		if (pInnerStickPolys->at(i)->GetPointPos(cursorPosition) == OUTSIDE_POLY)
			return;
	}
	for (unsigned int i = 0; i < pOuterStickPolys->size(); i++)
	{
		if (pOuterStickPolys->at(i)->GetPointPos(cursorPosition) == INSIDE_POLY)
			return;
	}

	// 检查是不是吸附住了。
	{
		Polygon2DListPtr pStickPolys(new Polygon2DList);
		pStickPolys->insert(pStickPolys->end(), pInnerStickPolys->begin(), pInnerStickPolys->end());
		pStickPolys->insert(pStickPolys->end(), pOuterStickPolys->begin(), pOuterStickPolys->end());
		if (pStickPolys->size() == 0)
		{
			LogMgr::DebugWarn(_T("884278"));
			return;
		}

		int iPolygonIndex = INVALID_IDX;
		Point2D closestPt = pStickPolys->GetClosestPt_2(cursorPosition, iPolygonIndex);
		double dDis = closestPt.DistanceTo(cursorPosition);
		if (fabs(dDis) < dStickDistance)
		{
			m_pStickedCanvasZone = pCanvasZone;
			m_pStickPt.reset(new Point2D(closestPt));
			m_pStickPoly = pStickPolys->operator[](iPolygonIndex)->Clone();
		}
	}
}

void LoopPositionDetector::DetectStickPt4LoopDistance_2(const Point2D& cursorPosition)
{
	if (!m_pStickedCanvasZone) // 此时应该处于距离吸附状态。
	{
		LogMgr::DebugWarn(_T("475742"));
		return;
	}

	const SheetEditParam* pSheetEditParam = CLOptionInfo::GetInstance()->GetSheetEditParam().get();
	double dStickDistance = pSheetEditParam->GetStickDis();
	m_pViewPort4ActionBar->DToW(dStickDistance);
	double dEscapeDistance = pSheetEditParam->GetEscapeDis();
	m_pViewPort4ActionBar->DToW(dEscapeDistance);

	// 当前吸附点靠近的“内靠碰轨迹”。
	Polygon2DPtr pCloseInnerStickPoly;
	if (!m_config.m_bEnableComCut) // 不能和包含自己的轮廓共边。
	{
		// 选中的几何轮廓和底图区域外轮廓的内靠碰轨迹。这个轨迹已经计算过了，直接从缓存获取即可。
		const Polygon2D* pOuterPoly = m_pStickedCanvasZone->GetOuterPoly().get();
		const Polygon2DList* pInnerStickPolys = m_innerStickPolys[pOuterPoly->GetPolygonID()].get();
		for (unsigned int i = 0; i < pInnerStickPolys->size(); i++)
		{
			Polygon2DPtr pInnerStickPoly = pInnerStickPolys->at(i);
			Point2D pt = pInnerStickPoly->GetClosestPt(m_refPtOfMovingPatLoops);
			if (pt.DistanceTo(m_refPtOfMovingPatLoops) < dStickDistance)
			{
				pCloseInnerStickPoly = pInnerStickPoly;
				break;
			}
		}
	}

	// 当前吸附点靠近的“外靠碰轨迹”，把这些“外靠碰轨迹”合并成一个。
	Polygon2DPtr pCloseOuterStickPoly;
	{
		// 计算“选中的几何轮廓和离的较近的底图区域内轮廓”的“外靠碰轨迹”。
		Polygon2DListPtr pOuterStickPolys(new Polygon2DList);
		{
			const PatternLoopList* pAllPatternLoops = m_pUniquePartPlacement->GetPart()->GetCadData()->GetPatternLoopList().get();
			const Polygon2DList* pCanvassZoneInnerPolys = m_pStickedCanvasZone->GetInnerPolys().get();
			const IntegerList* pPatLoopIDs4InnerPolys = m_pStickedCanvasZone->GetPatLoopIDs4InnerPolys().get();
			for (unsigned int i = 0; i < pCanvassZoneInnerPolys->size(); i++)
			{
				const Polygon2D* pCanvasZoneInnerPoly = pCanvassZoneInnerPolys->operator[](i).get();
				LONGLONG iPatternLoopID = pPatLoopIDs4InnerPolys->at(i);

				// 判断能不能和这个底图区域内轮廓进行共边。
				if (m_config.m_bEnableComCut && !IsMovingLoopsCanComCutWithCanvasZoneInnerPoly(m_pUniquePartPlacement, m_pLoopTopItems4FixedPatLoops.get(),
					m_pMovingPatLoops.get(), m_emMovingLoopsType, m_pToolDataOfMovingPatLoops.get(), iPatternLoopID))
					continue;

				const Rect2D& innerPolyRect = pAllPatternLoops->GetLoopByID(iPatternLoopID)->GetCacheData()->GetLoopRect();
				double dExpandValue = GetExpandValue4CanvasZoneInnerPoly(m_pUniquePartPlacement, iPatternLoopID, m_config.m_dLoopDistance, m_config.m_bEnableComCut);
				if (m_rectOfMovingPatLoops.DistanceTo(innerPolyRect) < (dExpandValue * 2.0 + dStickDistance)) // 选出离移动的轮廓较近的底图区域内轮廓。
				{
					Polygon2DPtr pOuterStickPoly = m_outerStickPolys[pCanvasZoneInnerPoly->GetPolygonID()];
					if (!pOuterStickPoly)
					{
						// 注意，此时轮廓处于吸附状态，要用m_refPtOfMovingPatLoops作为参考点来计算外靠碰多边形，而不能使用cursorPosition。
						pOuterStickPoly = CalcOuterStickPolys(m_refPtOfMovingPatLoops, pCanvasZoneInnerPoly, dExpandValue);
						m_outerStickPolys[pCanvasZoneInnerPoly->GetPolygonID()] = pOuterStickPoly;
					}
					pOuterStickPolys->push_back(pOuterStickPoly);
				}
			}
		}

		Polygon2DListPtr pCloseOuterStickPolys(new Polygon2DList);
		for (unsigned int i = 0; i < pOuterStickPolys->size(); i++)
		{
			Polygon2DPtr pOuterStickPoly = pOuterStickPolys->at(i);
			Point2D pt = pOuterStickPoly->GetClosestPt(m_refPtOfMovingPatLoops);
			if (pt.DistanceTo(m_refPtOfMovingPatLoops) < dStickDistance)
				pCloseOuterStickPolys->push_back(pOuterStickPoly);
		}
		if (pCloseOuterStickPolys->size() > 0)
			pCloseOuterStickPoly = Polygon2DProcessor::MergePolygon(pCloseOuterStickPolys, OPTPOLY_FACTOR);
	}

	// 判断吸附状态。
	if (pCloseInnerStickPoly && pCloseOuterStickPoly)
	{
		if (pCloseInnerStickPoly->IntersectWith(*pCloseOuterStickPoly))
		{
			Line2DListPtr pLineList1 = pCloseInnerStickPoly->GetLineList();
			Line2DListPtr pLineList2 = pCloseOuterStickPoly->GetLineList();
			Polygon2DListPtr pPolys = Polygon2DProcessor::CutOffPolygon(pCloseInnerStickPoly.get(), pCloseOuterStickPoly.get(), pLineList1.get(), pLineList2.get());
			if (pPolys->size() > 0)
			{
				int iPolygonIndex = INVALID_IDX;
				Point2D pt = pPolys->GetClosestPt_2(cursorPosition, iPolygonIndex);
				if (pt.DistanceTo(cursorPosition) > dEscapeDistance) // 从吸附状态逃逸掉了。
				{
					m_pStickedCanvasZone.reset();
					m_pStickPt.reset();
					m_pStickPoly.reset();
				}
				else
				{
					m_pStickPt.reset(new Point2D(pt));
					m_pStickPoly = pPolys->operator[](iPolygonIndex)->Clone();
				}
			}
			else // 切割多边形失败。
			{
				Point2D pt = pCloseInnerStickPoly->GetClosestPt(cursorPosition);
				if (pt.DistanceTo(cursorPosition) > dEscapeDistance) // 从吸附状态逃逸掉了。
				{
					m_pStickedCanvasZone.reset();
					m_pStickPt.reset();
					m_pStickPoly.reset();
				}
				else
				{
					m_pStickPt.reset(new Point2D(pt));
					m_pStickPoly = pCloseInnerStickPoly->Clone();
				}
			}
		}
		else // 两个多边形不相交（还没碰到这种情况）。
		{
			Point2D pt = pCloseInnerStickPoly->GetClosestPt(cursorPosition);
			if (pt.DistanceTo(cursorPosition) > dEscapeDistance) // 从吸附状态逃逸掉了。
			{
				m_pStickedCanvasZone.reset();
				m_pStickPt.reset();
				m_pStickPoly.reset();
			}
			else
			{
				m_pStickPt.reset(new Point2D(pt));
				m_pStickPoly = pCloseInnerStickPoly->Clone();
			}
		}
	}
	else if (pCloseInnerStickPoly && !pCloseOuterStickPoly)
	{
		Point2D pt = pCloseInnerStickPoly->GetClosestPt(cursorPosition);
		if (pt.DistanceTo(cursorPosition) > dEscapeDistance) // 从吸附状态逃逸掉了。
		{
			m_pStickedCanvasZone.reset();
			m_pStickPt.reset();
			m_pStickPoly.reset();
		}
		else
		{
			m_pStickPt.reset(new Point2D(pt));
			m_pStickPoly = pCloseInnerStickPoly->Clone();
		}
	}
	else if (!pCloseInnerStickPoly && pCloseOuterStickPoly)
	{
		Point2D pt = pCloseOuterStickPoly->GetClosestPt(cursorPosition);
		if (pt.DistanceTo(cursorPosition) > dEscapeDistance) // 从吸附状态逃逸掉了。
		{
			m_pStickedCanvasZone.reset();
			m_pStickPt.reset();
			m_pStickPoly.reset();
		}
		else
		{
			m_pStickPt.reset(new Point2D(pt));
			m_pStickPoly = pCloseOuterStickPoly->Clone();
		}
	}
	else if (!pCloseInnerStickPoly && !pCloseOuterStickPoly) // 从吸附状态逃逸掉了。
	{
		m_pStickedCanvasZone.reset();
		m_pStickPt.reset();
		m_pStickPoly.reset();
	}
}

Polygon2DPtr LoopPositionDetector::CalcOuterStickPolys(const Point2D& cursorPosition, const Polygon2D* pCanvassZoneInnerPoly, double dExpandValue)
{
	Polygon2DPtr pOuterStickPoly;

	// 得到底图区域内轮廓膨胀之后的多边形。
	Polygon2DPtr pNewHolePoly = pCanvassZoneInnerPoly->Clone();
	Polygon2DProcessor::EnlargePolygon(pNewHolePoly, dExpandValue, CUTARC_FACTOR, OPTPOLY_FACTOR);

	// 底图区域内轮廓和移动的轮廓的外临界多边形。
	Polygon2DListPtr pOuterNFPs(new Polygon2DList);
	for (unsigned int i = 0; i < m_pFirstLayerPolys_Expanded->size(); i++)
	{
		const Polygon2D* pFirstLayerOuterPoly = m_pFirstLayerPolys_Expanded->operator [](i).get();

		// 计算“外临界多边形”。
		Polygon2DPtr pOutNFP(new Polygon2D);
		Polygon2DProcessor::BuildNFP(pNewHolePoly.get(), pFirstLayerOuterPoly, pOutNFP);
		Polygon2DProcessor::SleekPolygon(pOutNFP, 0);

		// 把该外临界多边形转化为基于参考点的外临界多边形。
		for (unsigned int k = 0; k < pOutNFP->size(); k++)
			*(pOutNFP->begin() + k) = pOutNFP->at(k) + cursorPosition;

		pOuterNFPs->push_back(pOutNFP);
	}

	// 计算选中的轮廓和底图区域内轮廓的“外靠碰轨迹”。
	pOuterStickPoly = Polygon2DProcessor::MergePolygon(pOuterNFPs, OPTPOLY_FACTOR);

	return pOuterStickPoly;
}

Polygon2DListPtr LoopPositionDetector::CalcInnerStickPolys(const Point2D& cursorPosition, const CanvasZone* pCanvasZone, double dCompressValue)
{
	Polygon2DListPtr pInnerStickPolys;

	// 压缩底图区域的外轮廓。
	Polygon2DPtr pNewOuterPoly;
	if (dCompressValue > 0)
	{
		Polygon2DListPtr pPoly2DList = Polygon2DProcessor::CompressPolygon(pCanvasZone->GetOuterPoly()->Clone(), dCompressValue);
		if (pPoly2DList->size() == 0) // 此时可能压缩算法错误，或者是压缩量太大
			return pInnerStickPolys;
		pNewOuterPoly = pPoly2DList->at(0);
	}
	else
	{
		pNewOuterPoly = pCanvasZone->GetOuterPoly()->Clone();
		Polygon2DProcessor::EnlargePolygon(pNewOuterPoly, -dCompressValue, CUTARC_FACTOR, OPTPOLY_FACTOR);
	}

	// 得到底图区域外轮廓和各个几何轮廓的内临界多边形。
	vector<Polygon2DListPtr> innerNFPs;
	{
		Line2DListPtr pLineList1 = pNewOuterPoly->GetLineList();
		for (unsigned int i = 0; i < m_pFirstLayerPolys_Expanded->size(); i++)
		{
			const Polygon2D* pFirstLayerOuterPoly = m_pFirstLayerPolys_Expanded->operator[](i).get();
			Line2DListPtr pLineList2 = pFirstLayerOuterPoly->GetLineList();
			Polygon2DListPtr pInnerNFP = Polygon2DProcessor::BuildInnerNFP(pNewOuterPoly.get(), pLineList1.get(), pFirstLayerOuterPoly, pLineList2.get(), cursorPosition);
			if (pInnerNFP->size() == 0)
				return pInnerStickPolys; // 选中的几何轮廓无法放到底图区域外边框的内部。
			else
				innerNFPs.push_back(pInnerNFP);
		}
	}

	// 计算几何轮廓和底图区域外轮廓的“内靠碰轨迹”。
	if (innerNFPs.size() == 1)
		pInnerStickPolys = innerNFPs.at(0);
	else
	{
		// 对“内临界多边形”两两求交。
		Polygon2DListPtr pInnerNFP1, pInnerNFP2; // pInnerNFP1中存放每次求交的结果。
		for (unsigned int m = 0; m < innerNFPs.size() - 1; m++)
		{
			if (m == 0)
				pInnerNFP1 = innerNFPs.at(m);
			pInnerNFP2 = innerNFPs.at(m + 1);

			// 求2个内临界多边形的交集。
			Polygon2DListPtr pCrossPolys(new Polygon2DList);
			for (unsigned int i = 0; i < pInnerNFP1->size(); i++)
			{
				Polygon2DPtr pPoly1 = pInnerNFP1->at(i);
				Line2DListPtr pLineList1 = pPoly1->GetLineList();
				for (unsigned int j = 0; j < pInnerNFP2->size(); j++)
				{
					Polygon2DPtr pPoly2 = pInnerNFP2->at(j);
					if (pPoly1->IncludePolygon(*pPoly2))
						pCrossPolys->push_back(pPoly2);
					else if (pPoly2->IncludePolygon(*pPoly1))
						pCrossPolys->push_back(pPoly1);
					else
					{
						Line2DListPtr pLineList2 = pPoly2->GetLineList();

						// 这2个内临界多边形必须要相交。
						if (!pPoly1->IntersectWith(*pPoly2))
							continue;

						// 求交。
						Line2DListPtr pAllLine(new Line2DList);
						pAllLine->insert(pAllLine->begin(), pLineList1->begin(), pLineList1->end());
						pAllLine->insert(pAllLine->begin(), pLineList2->begin(), pLineList2->end());
						LineArc2DProcessor::CutLineByCross(pAllLine);
						Polygon2DPtr pCrossPoly = Polygon2DBuilder::FindInnerBoundary(pAllLine);
						pCrossPolys->push_back(pCrossPoly);
					}
				}
			}

			if (pCrossPolys->size() == 0) // 只要有2个内临界多边形不相交，这些几何轮廓就无法和底图区域外轮廓实现“内靠碰”。
				return pInnerStickPolys;
			else
				pInnerNFP1 = pCrossPolys;
		}

		pInnerStickPolys = pInnerNFP1;
	}

	return pInnerStickPolys;
}

double LoopPositionDetector::GetExpandValue4MovingPatLoops(const PartPlacement* pUniquePartPlacement, const IPatternLoop* pMovingPatternLoop,
	MovingLoopsType emMovingLoopsType, const LoopToolDataList* pToolDataOfMovingPatLoops, double dLoopDistance, bool bSupportComCut)
{
	double dExpandValue = .0;

	if (bSupportComCut)
	{
		if (emMovingLoopsType == MoveNewLoop)
		{
			const LoopToolData* pLoopToolData = pToolDataOfMovingPatLoops->GetLoopTool_by_loopID(pMovingPatternLoop->GetID()).get();
			dExpandValue = pLoopToolData->GetToolWidth() / 2.0;
		}
		else
		{
			const LoopCutFeatureList* pLoopCutFeatures = pUniquePartPlacement->GetCamData()->GetLoopFeatureList().get();
			const LoopCutFeature* pLoopCutFeature = pLoopCutFeatures->FindByPatternLoopID(pMovingPatternLoop->GetID()).get();
			double dToolSize = pLoopCutFeature->GetLoopTool()->GetToolWidth();
			dExpandValue = dToolSize / 2.0;
		}
	}
	else
	{
		dExpandValue = dLoopDistance / 2.0;
	}

	return dExpandValue;
}

double LoopPositionDetector::GetCompressValue4CanvasZoneOuterPoly(const PartPlacement* pUniquePartPlacement, const CanvasZone* pCanvasZone, double dLoopDistance, bool bSupportComCut)
{
	double dCompressValue = .0;

	// 如果这个底图区域是最外层的，它的外轮廓是个无穷大的多边形，这里不需要处理。
	if (pCanvasZone->GetPatLoopID4OuterPoly() == INVALID_IDX)
		return dCompressValue;

	if (bSupportComCut)
	{
		const LoopCutFeatureList* pLoopCutFeatures = pUniquePartPlacement->GetCamData()->GetLoopFeatureList().get();
		const LoopCutFeature* pLoopCutFeature = pLoopCutFeatures->FindByPatternLoopID(pCanvasZone->GetPatLoopID4OuterPoly()).get();
		double dToolSize = pLoopCutFeature->GetLoopTool()->GetToolWidth();
		dCompressValue = dLoopDistance - dToolSize / 2.0;
	}
	else
	{
		dCompressValue = dLoopDistance / 2.0;
	}

	return dCompressValue;
}

double LoopPositionDetector::GetExpandValue4CanvasZoneInnerPoly(const PartPlacement* pUniquePartPlacement, LONGLONG iPatLoopID4InnerPoly, double dLoopDistance, bool bSupportComCut)
{
	double dExpandValue = .0;
	if (bSupportComCut)
	{
		const LoopCutFeatureList* pLoopCutFeatures = pUniquePartPlacement->GetCamData()->GetLoopFeatureList().get();
		const LoopCutFeature* pLoopCutFeature = pLoopCutFeatures->FindByPatternLoopID(iPatLoopID4InnerPoly).get();
		double dToolSize = pLoopCutFeature->GetLoopTool()->GetToolWidth();
		dExpandValue = dToolSize / 2.0;
	}
	else
	{
		dExpandValue = dLoopDistance / 2.0;
	}

	return dExpandValue;
}

LoopTopologyItemListPtr LoopPositionDetector::CalcLoopTopItems4FixedPatLoops(MovingLoopsType emMovingLoopsType, const PatternLoopList* pAllPatternLoops,
	const PatternLoopList* pMovingPatternLoops)
{
	// 得到固定不动的几何轮廓。
	PatternLoopListPtr pFixedPatternLoops(new PatternLoopList);
	for (unsigned int i = 0; i < pAllPatternLoops->size(); i++)
	{
		IPatternLoopPtr pPatternLoop = pAllPatternLoops->operator[](i);
		if (pPatternLoop->IsOpenPath())
			continue;
		if (emMovingLoopsType == CopyExsitingLoop)
			pFixedPatternLoops->push_back(pPatternLoop);
		if (emMovingLoopsType == MoveNewLoop)
			pFixedPatternLoops->push_back(pPatternLoop);
		else if (emMovingLoopsType == MoveExsitingLoop)
		{
			if (!pMovingPatternLoops->GetLoopByID(pPatternLoop->GetID()))
				pFixedPatternLoops->push_back(pPatternLoop);
		}
	}

	// 计算出不动的几何轮廓的拓扑关系。
	LoopTopologyItemListPtr pLoopTopItems4FixedPatLoops = LoopProcessor::CalcLoopTopStruct(pFixedPatternLoops.get());
	return pLoopTopItems4FixedPatLoops;
}

CanvasZoneListPtr LoopPositionDetector::CalcCanvasZones(const LoopTopologyItemList* pLoopTopItems4FixedPatLoops, double dMovingPatLoopWidth, double dMovingPatLoopHeight)
{
	CanvasZoneListPtr pCanvasZones(new CanvasZoneList);

	// 根据几何轮廓拓扑关系计算出底图区域集合。
	// 注：
	// 1) 这里先通过一个循环得到最外层底图区域。然后通过递归再得到其他的底图区域。
	{
		CanvasZonePtr pOuterCanvasZone(new CanvasZone);
		pCanvasZones->push_back(pOuterCanvasZone);

		// 最外层底图区域的那个超大的外轮廓。
		Polygon2DPtr pOuterPoly(new Polygon2D);
		pOuterPoly->push_back(Point2D(-100000.0, -100000.0));
		pOuterPoly->push_back(Point2D(100000.0, -100000.0));
		pOuterPoly->push_back(Point2D(100000.0, 100000.0));
		pOuterPoly->push_back(Point2D(-100000.0, 100000.0));
		pOuterCanvasZone->SetOuterPoly(pOuterPoly);
		pOuterCanvasZone->SetPatLoopID4OuterPoly(INVALID_IDX);

		// 循环第一层几何轮廓，并递归下去。
		map<LONGLONG, Polygon2DPtr> polygonsOfPatLoop;
		for (unsigned int i = 0; i < pLoopTopItems4FixedPatLoops->size(); i++)
		{
			const LoopTopologyItem* pLoopTopItem = pLoopTopItems4FixedPatLoops->operator[](i).get();

			// 这个内孔的多边形。
			Polygon2DListPtr pPolys = Polygon2DBuilder::BuildPolyListFromLineArc(pLoopTopItem->GetPatternLoop()->GetCacheData()->GetLoopBaseGeom().get(), CUTARC_FACTOR);
			if (pPolys->size() == 0)
			{
				LogMgr::DebugWarn(_T("483106"));
				continue;
			}
			Polygon2DProcessor::SleekPolygon(pPolys->front(), OPTPOLY_FACTOR);
			polygonsOfPatLoop[pLoopTopItem->GetPatternLoop()->GetID()] = pPolys->front();
			pOuterCanvasZone->GetInnerPolys()->push_back(pPolys->front());
			pOuterCanvasZone->GetPatLoopIDs4InnerPolys()->AddInt(pLoopTopItem->GetPatternLoop()->GetID());

			// 递归。
			CalcCanvasZonesBypLoopTopItems(pLoopTopItem, pCanvasZones, polygonsOfPatLoop);
		}
	}

	// 根据移动的几何轮廓的尺寸，删除比较小的底图区域。
	for (CanvasZoneList::iterator iter = pCanvasZones->begin(); iter != pCanvasZones->end();)
	{
		const CanvasZone* pCanvasZone = iter->get();
		LONGLONG iPatLoopID4OuterPoly = pCanvasZone->GetPatLoopID4OuterPoly();
		if (iPatLoopID4OuterPoly == INVALID_IDX)
			iter++;
		else
		{
			const LoopTopologyItem* pLoopTopologyItem = pLoopTopItems4FixedPatLoops->FindTopologyItemByPatternLoop(iPatLoopID4OuterPoly);
			const IPatternLoop* pPatternLoop = pLoopTopologyItem->GetPatternLoop().get();
			const Rect2D& patternLoopRect = pPatternLoop->GetCacheData()->GetLoopRect();
			if (patternLoopRect.GetWidth() < dMovingPatLoopWidth || patternLoopRect.GetHeight() < dMovingPatLoopHeight)
				iter = pCanvasZones->erase(iter);
			else
				iter++;
		}
	}

	// 预览计算出的底图区域。to be commented.
	//for (unsigned int i = 0; i < pCanvasZones->size(); i++)
	//{
	//	PolyViewDlg polyViewDlg(_T("显示底图区域。"), NULL, FALSE);
	//	const CanvasZone* pCanvasZone = pCanvasZones->operator[](i).get();
	//	polyViewDlg.AddPolygon(pCanvasZone->GetOuterPoly(), RGB(255,0,0));
	//	for (unsigned int j = 0; j < pCanvasZone->GetInnerPolys()->size(); j++)
	//		polyViewDlg.AddPolygon(pCanvasZone->GetInnerPolys()->operator[](j), RGB(0,255,0));
	//	polyViewDlg.DoModal();
	//}

	return pCanvasZones;
}

void LoopPositionDetector::CalcCanvasZonesBypLoopTopItems(const LoopTopologyItem* pLoopTopItem, CanvasZoneListPtr pCanvasZones, map<LONGLONG, Polygon2DPtr>& polygonsOfPatLoop)
{
	// 如不考虑递归，这个函数创建这个底图区域。
	CanvasZonePtr pCanvasZone(new CanvasZone);
	pCanvasZones->push_back(pCanvasZone);

	// 底图区域的的外轮廓。
	map<LONGLONG, Polygon2DPtr>::const_iterator iter = polygonsOfPatLoop.find(pLoopTopItem->GetPatternLoop()->GetID());
	if (iter == polygonsOfPatLoop.end()) // 不应该出现这种情况，在上一层递归中应该计算过了该几何轮廓的多边形表达。
	{
		LogMgr::DebugWarn(_T("448841"));
		return;
	}
	pCanvasZone->SetOuterPoly(iter->second);
	pCanvasZone->SetPatLoopID4OuterPoly(pLoopTopItem->GetPatternLoop()->GetID());

	const LoopTopologyItemList* pSubLoopTopItems = pLoopTopItem->GetSubItemList().get();
	for (unsigned int i = 0; i < pSubLoopTopItems->size(); i++)
	{
		const LoopTopologyItem* pSubLoopTopItem = pSubLoopTopItems->operator[](i).get();

		// 这个内孔的多边形。
		Polygon2DListPtr pPolys = Polygon2DBuilder::BuildPolyListFromLineArc(pSubLoopTopItem->GetPatternLoop()->GetCacheData()->GetLoopBaseGeom().get(), CUTARC_FACTOR);
		if (pPolys->size() == 0)
		{
			LogMgr::DebugWarn(_T("488976"));
			continue;
		}
		Polygon2DProcessor::SleekPolygon(pPolys->front(), OPTPOLY_FACTOR);
		polygonsOfPatLoop[pSubLoopTopItem->GetPatternLoop()->GetID()] = pPolys->front();
		pCanvasZone->GetInnerPolys()->push_back(pPolys->front());
		pCanvasZone->GetPatLoopIDs4InnerPolys()->AddInt(pSubLoopTopItem->GetPatternLoop()->GetID());

		// 递归。
		CalcCanvasZonesBypLoopTopItems(pSubLoopTopItem, pCanvasZones, polygonsOfPatLoop);
	}
}

END_CUTLEADER_NAMESPACE()

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
	m_rectOfMovingPatLoops = pMovingPatLoops->GetRectBox(); // �ƶ��ļ�����������Ӿ��Ρ�

	// �����һ���ƶ��ļ��������Ķ���Ρ����ŵļ��������ᱻ���˵���
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

		// ���Ͷ���Ρ�
		double dExpandValue = GetExpandValue4MovingPatLoops(pUniquePartPlacement, pLoopTopItem->GetPatternLoop().get(), m_emMovingLoopsType,
			m_pToolDataOfMovingPatLoops.get(), m_config.m_dLoopDistance, m_config.m_bEnableComCut);
		Polygon2DPtr pNewPoly = pPolys->front()->Clone();
		Polygon2DProcessor::EnlargePolygon(pNewPoly, dExpandValue, CUTARC_FACTOR, OPTPOLY_FACTOR);
		m_pFirstLayerPolys_Expanded->push_back(pNewPoly);
	}

	// �������ͼ����Щ�����ıպ�������
	const PartCadData* pPartCadData = pUniquePartPlacement->GetPart()->GetCadData().get();
	m_pLoopTopItems4FixedPatLoops = CalcLoopTopItems4FixedPatLoops(emMovingLoopsType, pPartCadData->GetPatternLoopList().get(), pMovingPatLoops.get());

	// ���µ�ͼ���򼯺ϡ�
	m_pCanvasZones = CalcCanvasZones(m_pLoopTopItems4FixedPatLoops.get(), m_rectOfMovingPatLoops.GetWidth(), m_rectOfMovingPatLoops.GetHeight());

	// ������Щ������
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
	m_rectOfMovingPatLoops = pMovingPatLoops->GetRectBox(); // �ƶ��ļ�����������Ӿ��Ρ�

	// �����һ���ƶ��ļ��������Ķ���Ρ����ŵļ��������ᱻ���˵���
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

		// ���Ͷ���Ρ�
		double dExpandValue = GetExpandValue4MovingPatLoops(pUniquePartPlacement, pLoopTopItem->GetPatternLoop().get(), m_emMovingLoopsType,
			m_pToolDataOfMovingPatLoops.get(), m_config.m_dLoopDistance, m_config.m_bEnableComCut);
		Polygon2DPtr pNewPoly = pPolys->front()->Clone();
		Polygon2DProcessor::EnlargePolygon(pNewPoly, dExpandValue, CUTARC_FACTOR, OPTPOLY_FACTOR);
		m_pFirstLayerPolys_Expanded->push_back(pNewPoly);
	}

	// �������ͼ����Щ�����ıպ�������
	const PartCadData* pPartCadData = pUniquePartPlacement->GetPart()->GetCadData().get();
	m_pLoopTopItems4FixedPatLoops = CalcLoopTopItems4FixedPatLoops(emMovingLoopsType, pPartCadData->GetPatternLoopList().get(), pMovingPatLoops.get());

	// ���µ�ͼ���򼯺ϡ�
	m_pCanvasZones = CalcCanvasZones(m_pLoopTopItems4FixedPatLoops.get(), m_rectOfMovingPatLoops.GetWidth(), m_rectOfMovingPatLoops.GetHeight());

	// ������Щ������
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
	else if (!ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputS(nChar)) { // ��ʱ����ת90�ȡ�
		UpdateWhenRotate(PID2);
	}
	else if (!ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputW(nChar)) { // ˳ʱ����ת90�ȡ�
		UpdateWhenRotate(-PID2);
	}
}

void LoopPositionDetector::UpdateWhenRotate(double dRotateAngle)
{
	// ��ת�ƶ��е�������
	Matrix2D rotateMatrix;
	rotateMatrix.RotateBy(m_rectOfMovingPatLoops.GetCenterPt(), dRotateAngle);
	m_pMovingPatterns->TransformAndUpdateCache(rotateMatrix);
	m_transformMatrix *= rotateMatrix;
	m_pFirstLayerPolys->Transform(rotateMatrix);
	m_pFirstLayerPolys_Expanded->Transform(rotateMatrix);

	// ����ת�����������ĵ�У�Ե����㡣ע�⣬m_refPtOfMovingPartPmts���ƶ�ʱ�Ѿ����¹��ˡ�
	Matrix2D transferMatrix;
	transferMatrix.SetTransfer(Vector2D(m_pMovingPatterns->GetRectBox().GetCenterPt(), m_refPtOfMovingPatLoops));
	m_pMovingPatterns->TransformAndUpdateCache(transferMatrix);
	m_transformMatrix *= transferMatrix;
	m_pFirstLayerPolys->Transform(transferMatrix);
	m_pFirstLayerPolys_Expanded->Transform(transferMatrix);

	// �����ƶ��ļ���������Ӿ��Ρ�
	m_rectOfMovingPatLoops = m_pMovingPatterns->GetRectBox();

	// ����һЩ������
	m_innerStickPolys.clear();
	m_outerStickPolys.clear();
	m_pStickPt.reset();
	m_pStickedCanvasZone.reset();
	m_pStickPoly.reset();
	m_pAlignmentAssistLine.reset();
}

void LoopPositionDetector::UpdateDataWhenMove(const Point2D& cursorPosition)
{
	// �ȼ��һЩ����Ϊ�յ����ݡ�
	if (!m_pUniquePartPlacement || !m_pMovingPatLoops || !m_pFirstLayerPolys || !m_pCanvasZones) {
		LogMgr::DebugWarn(_T("409882"));
		return;
	}

	// ����һЩ������
	m_pAlignmentAssistLine.reset();

	// ���ƶ���������������仯�ˣ���Ҫ����һЩ���ݡ�һ�����ƶ��Ĺ������޸Ĳ������������ݵķ�ʽ���ǿ�������ӵ�ǰλ���ٴ������ƶ���
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

		// �����ƶ�����������Ӿ��Ρ�
		m_rectOfMovingPatLoops = m_pMovingPatLoops->GetRectBox();
		m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());

		// �����ƶ��ο��㡣
		m_refPtOfMovingPatLoops = m_refPt;
		m_refPtOfMovingPatLoops.Transfer(transferVect);

		// ���µ�һ���ƶ��ļ��������Ķ���Ρ����ŵļ��������ᱻ���˵���
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

				// ���Ͷ���Ρ�
				double dExpandValue = GetExpandValue4MovingPatLoops(m_pUniquePartPlacement, pLoopTopItem->GetPatternLoop().get(), m_emMovingLoopsType,
					m_pToolDataOfMovingPatLoops.get(), m_config.m_dLoopDistance, m_config.m_bEnableComCut);
				Polygon2DPtr pNewPoly = pPolys->front()->Clone();
				Polygon2DProcessor::EnlargePolygon(pNewPoly, dExpandValue, CUTARC_FACTOR, OPTPOLY_FACTOR);
				m_pFirstLayerPolys_Expanded->push_back(pNewPoly);
			}
			m_pFirstLayerPolys->Transfer(transferVect);
			m_pFirstLayerPolys_Expanded->Transfer(transferVect);
		}

		// ����һЩ������
		m_innerStickPolys.clear();
		m_outerStickPolys.clear();
		m_pStickPt.reset();
		m_pStickedCanvasZone.reset();
	}

	if (m_pFirstLayerPolys->size() == 0) { // ��ʱѡ�е�Ӧ�ö��ǿ��ż�����������ô��ȡ�������⡣
		m_pStickPt.reset();
		m_pStickedCanvasZone.reset();
		return;
	}

	// ����������һ�£����ѡ�е����������������ߵ���������ô��ȡ�������⡣��ʵ����֮ǰ�Ե�һ���ƶ���������������ʱ�Ϳ������ص���������������Ҳ���ԡ�
	if (m_config.m_bEnableComCut && !IsMovingPatLoopsCanComCut(m_pUniquePartPlacement, m_pMovingPatLoops_FirstLayer.get(), m_emMovingLoopsType, m_pToolDataOfMovingPatLoops.get())) {
		m_pStickPt.reset();
		m_pStickedCanvasZone.reset();
		return;
	}

	// �����������㡣
	if (!m_pStickedCanvasZone) // ��ʱû�о��������㡣
	{
		Vector2D transferVect(m_refPtOfMovingPatLoops, cursorPosition); // �����������ƶ�������
		m_pMovingPatterns->TransferAndUpdateCache(transferVect);
		m_transformMatrix.Transfer(transferVect);
		m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());
		m_pFirstLayerPolys->Transfer(transferVect);
		m_pFirstLayerPolys_Expanded->Transfer(transferVect);

		if (m_config.m_bEnableLoopDistanceDetect) {
			DetectStickPt4LoopDistance_1(cursorPosition);
		}
		if (m_pStickedCanvasZone) { // ��⵽�˾��������㣬����һЩ���ݡ�
			Vector2D transferVect(cursorPosition, *m_pStickPt); // �����������ƶ�������
			m_pMovingPatterns->TransferAndUpdateCache(transferVect);
			m_transformMatrix.Transfer(transferVect);
			m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());
			m_pFirstLayerPolys->Transfer(transferVect);
			m_pFirstLayerPolys_Expanded->Transfer(transferVect);
			m_refPtOfMovingPatLoops = *m_pStickPt; // �����ƶ��ο��㡣
		}
		else { // û�м�⵽���������㡣
			m_refPtOfMovingPatLoops = cursorPosition; // �����ƶ��ο��㡣
		}
	}
	else { // ��ʱ�о��������㡣
		if (m_config.m_bEnableLoopDistanceDetect) {
			DetectStickPt4LoopDistance_2(cursorPosition);
		}
		if (m_pStickedCanvasZone) { // ��һ�μ�⵽�˾��������㣬����һЩ���ݡ�
			Vector2D transferVect(m_refPtOfMovingPatLoops, *m_pStickPt); // �����������ƶ�������
			m_pMovingPatterns->TransferAndUpdateCache(transferVect);
			m_transformMatrix.Transfer(transferVect);
			m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());
			m_pFirstLayerPolys->Transfer(transferVect);
			m_pFirstLayerPolys_Expanded->Transfer(transferVect);
			m_refPtOfMovingPatLoops = *m_pStickPt; // �����ƶ��ο��㡣
		}
		else { // �����˾�������������һЩ���ݡ�
			Vector2D transferVect(m_refPtOfMovingPatLoops, cursorPosition); // �����������ƶ�������
			m_pMovingPatterns->TransferAndUpdateCache(transferVect);
			m_transformMatrix.Transfer(transferVect);
			m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());
			m_pFirstLayerPolys->Transfer(transferVect);
			m_pFirstLayerPolys_Expanded->Transfer(transferVect);
			m_refPtOfMovingPatLoops = cursorPosition; // �����ƶ��ο��㡣
		}
	}

	// �����������㡣
	{
		if (m_config.m_bEnableLoopAlignmentDetect) {
			DetectStickPt4Alignment(cursorPosition);
		}
		if (m_pAlignmentAssistLine) {
			Vector2D transferVect(m_refPtOfMovingPatLoops, *m_pStickPt); // �����������ƶ�������
			m_pMovingPatterns->TransferAndUpdateCache(transferVect);
			m_transformMatrix.Transfer(transferVect);
			m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());
			m_pFirstLayerPolys->Transfer(transferVect);
			m_pFirstLayerPolys_Expanded->Transfer(transferVect);
			m_refPtOfMovingPatLoops = *m_pStickPt; // �����ƶ��ο��㡣
		}
		else { // û�м�⵽����㣬���ﲻ��Ҫ�������ˣ����򲻶��ˡ�
			//Vector2D transferVect(m_refPtOfMovingPatLoops, cursorPosition); // �����������ƶ�������
			//m_rectOfMovingPatLoops.Transform(transferVect.X(), transferVect.Y());
			//m_pFirstLayerPolys->Transfer(transferVect);
			//m_pFirstLayerPolys_Expanded->Transfer(transferVect);
			//m_refPtOfMovingPatLoops = cursorPosition; // �����ƶ��ο��㡣
		}
	}
}

void LoopPositionDetector::DrawAssistInfo(const CString& strExtraTip)
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	COLORREF dwOldColor;
	m_pViewPort4ActionBar->GetDrawColor(dwOldColor);
	m_pViewPort4ActionBar->SetDrawColor(*pDisplayParam->GetDrawAssistColor());

	// ������ʾ���֡�
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

		// ����Ҫ��ʾ����ʾ��
		if (strExtraTip.GetLength() > 0)
		{
			strTip += _T(" (");
			strTip += strExtraTip;
			strTip += _T(")");
		}

		m_pViewPort4ActionBar->DrawString(strTip.GetString(), dCursorPtX + dOffset, dCursorPtY - dOffset * 2.0, .0, .0);
	}

	// ���ƶ��븨���ߡ�
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

	// ѡ�е�����Ҫ�������С�
	if (emMovingLoopsType == MoveNewLoop) // ��������ͼ�����µ�ͼԪ����Ϊ����Щ��ͼԪ����һ���������ʱ��Ϊ��һ�������������С�
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

	// ѡ�е�������Ҫʹ����ͬ�ĵ��߳ߴ硣
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

	// ��ͼ����������Ҫ�������㡣
	{
		int iLayerIndex = pLoopTopItems4FixedPatLoops->GetLayerIndexOfPatternLoop(iPatLoopID4CanvasZonePoly);
		if (iLayerIndex % 2 != 0)
			return false;
	}

	// �ƶ��������ĵ��߳ߴ�Ҫ�͵�ͼ��������������ͬ��
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

	// ��ͼ����������Ҫ���С�
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
		if (rectOfMovingPatLoops.DistanceTo(innerPolyRect) < rectOfMovingPatLoops.GetLongEdge()) // ѡ�����ƶ��ļ��������Ͻ��ĵ�ͼ������������
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

	// �õ���ͼ����ĳ���
	double dWorldRange, dScreenLength, dScreenWidth, dLeftBottomX, dLeftBottomY;
	pViewPort->GetDrawingArea(dWorldRange, dScreenLength, dScreenWidth, dLeftBottomX, dLeftBottomY);
	pViewPort->DToW(dScreenLength);
	pViewPort->DToW(dScreenWidth);

	if (emRectSide == RECT_LEFT) // ����һ����ֱ�ߡ�
		pAlignmentAssistLine.reset(new Line2D(Point2D(alignRect.GetXMin(), dLeftBottomY), Point2D(alignRect.GetXMin(), dLeftBottomY + dScreenWidth)));
	else if (emRectSide == RECT_RIGHT) // ����һ����ֱ�ߡ�
		pAlignmentAssistLine.reset(new Line2D(Point2D(alignRect.GetXMax(), dLeftBottomY), Point2D(alignRect.GetXMax(), dLeftBottomY + dScreenWidth)));
	else if (emRectSide == RECT_BOTTOM) // ����һ��ˮƽ�ߡ�
		pAlignmentAssistLine.reset(new Line2D(Point2D(dLeftBottomX, alignRect.GetYMin()), Point2D(dLeftBottomX + dScreenLength, alignRect.GetYMin())));
	else if (emRectSide == RECT_TOP) // ����һ��ˮƽ�ߡ�
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
	if (m_pAlignmentAssistLine) // ��ʱ��Ӧ�ô��ڶ���㡣
	{
		LogMgr::DebugWarn(_T("448066"));
		return;
	}

	double dStickDistance = CLOptionInfo::GetInstance()->GetSheetEditParam()->GetStickDis();
	m_pViewPort4ActionBar->DToW(dStickDistance);

	// ������һ����ͼ�����ж�ѡ�е�����������������֮��Ķ����ϵ��
	const PatternLoopList* pAllPatternLoops = m_pUniquePartPlacement->GetPart()->GetCadData()->GetPatternLoopList().get();
	CanvasZonePtr pCanvasZone = m_pCanvasZones->GetContainer(m_pFirstLayerPolys.get(), m_rectOfMovingPatLoops, pAllPatternLoops);
	if (!pCanvasZone)
		return;

	// �����ƶ�����������Ӿ��Σ��ҳ�������Ϣ��
	Rect2D alignRect;
	RECT_SIDE emRectSide_1, emRectSide_2;
	if (CheckAlignment(pCanvasZone.get(), pAllPatternLoops, m_rectOfMovingPatLoops, dStickDistance, alignRect, emRectSide_1, emRectSide_2)) // ��⵽�˶��롣
	{
		// �������븨���ߣ���ʾ�ҵ��˶��������㡣
		m_pAlignmentAssistLine = CreateAlignmentAssistLine(alignRect, emRectSide_1, m_pViewPort4ActionBar.get());

		// Ϊ�˶���Ҫ�ƶ���������
		Vector2D transferVect = CalcTransferVect4Alignment(alignRect, emRectSide_1, m_rectOfMovingPatLoops, emRectSide_2);

		if (!m_pStickedCanvasZone) // ��ǰ�����ڡ����������㡱��
		{
			m_pStickPt.reset(new Point2D(cursorPosition));
			m_pStickPt->Transfer(transferVect);
		}
		else // ��ǰ�Ѵ��ڡ����������㡱��
		{
			if (!m_pStickPoly) // ��ʱm_pStickPolyӦ����ֵ��
			{
				LogMgr::DebugWarn(_T("309452"));
				return;
			}

			// �жϡ����������㡱����û��Ч������ҵ��ġ����������㡱�ƻ���֮ǰ���������룬������ȡУ��һ�¡�
			Point2D currentStickPt = *m_pStickPt;
			currentStickPt.Transfer(transferVect);
			if (m_pStickPoly->GetPointPos(currentStickPt) != ON_POLY) // �ҵ��ġ����������㡱�ƻ���֮ǰ����������������
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

				// ��ȡУ��һ�¡�
				if (fabs(transferVect.X()) > GEOM_TOLERANCE) // ����������Ҷ����ˡ�
				{
					Point2DListPtr pCrossPts_1 = Polygon2DProcessor::GetCrossPtsWithLine(m_pStickPoly.get(), currentStickPt, POSITION_UP, FALSE);
					Point2DListPtr pCrossPts_2 = Polygon2DProcessor::GetCrossPtsWithLine(m_pStickPoly.get(), currentStickPt, POSITION_DOWN, FALSE);
					Point2DListPtr pCrossPts(new Point2DList);
					pCrossPts->insert(pCrossPts->end(), pCrossPts_1->begin(), pCrossPts_1->end());
					pCrossPts->insert(pCrossPts->end(), pCrossPts_2->begin(), pCrossPts_2->end());
					if (pCrossPts->size() == 0) // У��ʧ�ܡ�
					{
						m_pStickedCanvasZone.reset();
						m_pStickPoly.reset();
						m_pStickPt.reset(new Point2D(currentStickPt));
					}
					else // У���ɹ���
					{
						Point2D newStickPt = pCrossPts->GetPtClosestTo(currentStickPt);
						m_pStickPt.reset(new Point2D(newStickPt));
					}
				}
				else if (fabs(transferVect.Y()) > GEOM_TOLERANCE) // ���ϻ������¶����ˡ�
				{
					Point2DListPtr pCrossPts_1 = Polygon2DProcessor::GetCrossPtsWithLine(m_pStickPoly.get(), currentStickPt, POSITION_LEFT, FALSE);
					Point2DListPtr pCrossPts_2 = Polygon2DProcessor::GetCrossPtsWithLine(m_pStickPoly.get(), currentStickPt, POSITION_RIGHT, FALSE);
					Point2DListPtr pCrossPts(new Point2DList);
					pCrossPts->insert(pCrossPts->end(), pCrossPts_1->begin(), pCrossPts_1->end());
					pCrossPts->insert(pCrossPts->end(), pCrossPts_2->begin(), pCrossPts_2->end());
					if (pCrossPts->size() == 0) // У��ʧ�ܡ�
					{
						m_pStickedCanvasZone.reset();
						m_pStickPoly.reset();
						m_pStickPt.reset(new Point2D(currentStickPt));
					}
					else // У���ɹ���
					{
						Point2D newStickPt = pCrossPts->GetPtClosestTo(currentStickPt);
						m_pStickPt.reset(new Point2D(newStickPt));
					}
				}
			}
			else // �ҵ��ġ����������㡱Ҳ����֮ǰ����������������
				m_pStickPt.reset(new Point2D(currentStickPt));
		}
	}
	else // û�м�⵽���롣
	{
		if (!m_pStickedCanvasZone) // ��ǰ�����ڡ����������㡱��
			m_pStickPt.reset();
	}
}

tuple<bool, RECT_SIDE, RECT_SIDE, double> LoopPositionDetector::CheckTwoRectAlignment(const Rect2D& rect_1, const Rect2D& rect_2, double dStickDistance)
{
	RECT_SIDE emRectSide_1, emRectSide_2;
	double dMinDistance = INFINITE_VAL;

	// �Ա��������ε�ˮƽ�ߡ�
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

	// �Ա��������ε���ֱ�ߡ�
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
	if (m_pStickedCanvasZone) // ��ʱ��Ӧ�ô��ھ�������״̬��
	{
		LogMgr::DebugWarn(_T("468821"));
		return;
	}

	double dStickDistance = CLOptionInfo::GetInstance()->GetSheetEditParam()->GetStickDis();
	m_pViewPort4ActionBar->DToW(dStickDistance);

	// ������һ����ͼ�����ж�ѡ�е�����������������ϵ��
	const PatternLoopList* pAllPatternLoops = m_pUniquePartPlacement->GetPart()->GetCadData()->GetPatternLoopList().get();
	CanvasZonePtr pCanvasZone = m_pCanvasZones->GetContainer(m_pFirstLayerPolys.get(), m_rectOfMovingPatLoops, pAllPatternLoops);
	if (!pCanvasZone)
		return;

	// ����ѡ�еļ���������pCanvasZone�������ġ��ڿ����켣����
	Polygon2DListPtr pInnerStickPolys(new Polygon2DList);
	if (!m_config.m_bEnableComCut) // ���ܺͰ����Լ����������ߡ�
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
				return; // ѡ�е�����޷��ŵ���ͼ�����ڲ���
		}
	}

	// ����ѡ�еļ�����������ĽϽ��ĵ�ͼ�����������ġ��⿿���켣����
	Polygon2DListPtr pOuterStickPolys(new Polygon2DList);
	{
		const Polygon2DList* pCanvasZoneInnerPolys = pCanvasZone->GetInnerPolys().get();
		const IntegerList* pPatLoopIDs4InnerPolys = pCanvasZone->GetPatLoopIDs4InnerPolys().get();
		for (unsigned int i = 0; i < pCanvasZoneInnerPolys->size(); i++)
		{
			const Polygon2D* pCanvasZoneInnerPoly = pCanvasZoneInnerPolys->operator[](i).get();
			LONGLONG iPatternLoopID = pPatLoopIDs4InnerPolys->at(i);

			// �ж��ܲ��ܺ������ͼ�������������й��ߡ�
			if (m_config.m_bEnableComCut && !IsMovingLoopsCanComCutWithCanvasZoneInnerPoly(m_pUniquePartPlacement, m_pLoopTopItems4FixedPatLoops.get(),
				m_pMovingPatLoops.get(), m_emMovingLoopsType, m_pToolDataOfMovingPatLoops.get(), iPatternLoopID))
				continue;

			const Rect2D& innerPolyRect = pAllPatternLoops->GetLoopByID(iPatternLoopID)->GetCacheData()->GetLoopRect();
			double dExpandValue = GetExpandValue4CanvasZoneInnerPoly(m_pUniquePartPlacement, iPatternLoopID, m_config.m_dLoopDistance, m_config.m_bEnableComCut);
			if (m_rectOfMovingPatLoops.DistanceTo(innerPolyRect) < (dExpandValue * 2.0 + dStickDistance)) // ѡ�����ƶ��ļ��������Ͻ��ĵ�ͼ������������
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

	// ���û�м���������켣����ô��ȡ�������⡣
	if (pInnerStickPolys->size() == 0 && pOuterStickPolys->size() == 0)
	{
		m_pStickedCanvasZone.reset();
		m_pStickPt.reset();
		return;
	}

	// �ƶ��ļ����������ܺ͵�ͼ�����������͸����ĵ�ͼ�����������ཻ��
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

	// ����ǲ�������ס�ˡ�
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
	if (!m_pStickedCanvasZone) // ��ʱӦ�ô��ھ�������״̬��
	{
		LogMgr::DebugWarn(_T("475742"));
		return;
	}

	const SheetEditParam* pSheetEditParam = CLOptionInfo::GetInstance()->GetSheetEditParam().get();
	double dStickDistance = pSheetEditParam->GetStickDis();
	m_pViewPort4ActionBar->DToW(dStickDistance);
	double dEscapeDistance = pSheetEditParam->GetEscapeDis();
	m_pViewPort4ActionBar->DToW(dEscapeDistance);

	// ��ǰ�����㿿���ġ��ڿ����켣����
	Polygon2DPtr pCloseInnerStickPoly;
	if (!m_config.m_bEnableComCut) // ���ܺͰ����Լ����������ߡ�
	{
		// ѡ�еļ��������͵�ͼ�������������ڿ����켣������켣�Ѿ�������ˣ�ֱ�Ӵӻ����ȡ���ɡ�
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

	// ��ǰ�����㿿���ġ��⿿���켣��������Щ���⿿���켣���ϲ���һ����
	Polygon2DPtr pCloseOuterStickPoly;
	{
		// ���㡰ѡ�еļ�����������ĽϽ��ĵ�ͼ�������������ġ��⿿���켣����
		Polygon2DListPtr pOuterStickPolys(new Polygon2DList);
		{
			const PatternLoopList* pAllPatternLoops = m_pUniquePartPlacement->GetPart()->GetCadData()->GetPatternLoopList().get();
			const Polygon2DList* pCanvassZoneInnerPolys = m_pStickedCanvasZone->GetInnerPolys().get();
			const IntegerList* pPatLoopIDs4InnerPolys = m_pStickedCanvasZone->GetPatLoopIDs4InnerPolys().get();
			for (unsigned int i = 0; i < pCanvassZoneInnerPolys->size(); i++)
			{
				const Polygon2D* pCanvasZoneInnerPoly = pCanvassZoneInnerPolys->operator[](i).get();
				LONGLONG iPatternLoopID = pPatLoopIDs4InnerPolys->at(i);

				// �ж��ܲ��ܺ������ͼ�������������й��ߡ�
				if (m_config.m_bEnableComCut && !IsMovingLoopsCanComCutWithCanvasZoneInnerPoly(m_pUniquePartPlacement, m_pLoopTopItems4FixedPatLoops.get(),
					m_pMovingPatLoops.get(), m_emMovingLoopsType, m_pToolDataOfMovingPatLoops.get(), iPatternLoopID))
					continue;

				const Rect2D& innerPolyRect = pAllPatternLoops->GetLoopByID(iPatternLoopID)->GetCacheData()->GetLoopRect();
				double dExpandValue = GetExpandValue4CanvasZoneInnerPoly(m_pUniquePartPlacement, iPatternLoopID, m_config.m_dLoopDistance, m_config.m_bEnableComCut);
				if (m_rectOfMovingPatLoops.DistanceTo(innerPolyRect) < (dExpandValue * 2.0 + dStickDistance)) // ѡ�����ƶ��������Ͻ��ĵ�ͼ������������
				{
					Polygon2DPtr pOuterStickPoly = m_outerStickPolys[pCanvasZoneInnerPoly->GetPolygonID()];
					if (!pOuterStickPoly)
					{
						// ע�⣬��ʱ������������״̬��Ҫ��m_refPtOfMovingPatLoops��Ϊ�ο����������⿿������Σ�������ʹ��cursorPosition��
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

	// �ж�����״̬��
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
				if (pt.DistanceTo(cursorPosition) > dEscapeDistance) // ������״̬���ݵ��ˡ�
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
			else // �и�����ʧ�ܡ�
			{
				Point2D pt = pCloseInnerStickPoly->GetClosestPt(cursorPosition);
				if (pt.DistanceTo(cursorPosition) > dEscapeDistance) // ������״̬���ݵ��ˡ�
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
		else // ��������β��ཻ����û���������������
		{
			Point2D pt = pCloseInnerStickPoly->GetClosestPt(cursorPosition);
			if (pt.DistanceTo(cursorPosition) > dEscapeDistance) // ������״̬���ݵ��ˡ�
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
		if (pt.DistanceTo(cursorPosition) > dEscapeDistance) // ������״̬���ݵ��ˡ�
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
		if (pt.DistanceTo(cursorPosition) > dEscapeDistance) // ������״̬���ݵ��ˡ�
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
	else if (!pCloseInnerStickPoly && !pCloseOuterStickPoly) // ������״̬���ݵ��ˡ�
	{
		m_pStickedCanvasZone.reset();
		m_pStickPt.reset();
		m_pStickPoly.reset();
	}
}

Polygon2DPtr LoopPositionDetector::CalcOuterStickPolys(const Point2D& cursorPosition, const Polygon2D* pCanvassZoneInnerPoly, double dExpandValue)
{
	Polygon2DPtr pOuterStickPoly;

	// �õ���ͼ��������������֮��Ķ���Ρ�
	Polygon2DPtr pNewHolePoly = pCanvassZoneInnerPoly->Clone();
	Polygon2DProcessor::EnlargePolygon(pNewHolePoly, dExpandValue, CUTARC_FACTOR, OPTPOLY_FACTOR);

	// ��ͼ�������������ƶ������������ٽ����Ρ�
	Polygon2DListPtr pOuterNFPs(new Polygon2DList);
	for (unsigned int i = 0; i < m_pFirstLayerPolys_Expanded->size(); i++)
	{
		const Polygon2D* pFirstLayerOuterPoly = m_pFirstLayerPolys_Expanded->operator [](i).get();

		// ���㡰���ٽ����Ρ���
		Polygon2DPtr pOutNFP(new Polygon2D);
		Polygon2DProcessor::BuildNFP(pNewHolePoly.get(), pFirstLayerOuterPoly, pOutNFP);
		Polygon2DProcessor::SleekPolygon(pOutNFP, 0);

		// �Ѹ����ٽ�����ת��Ϊ���ڲο�������ٽ����Ρ�
		for (unsigned int k = 0; k < pOutNFP->size(); k++)
			*(pOutNFP->begin() + k) = pOutNFP->at(k) + cursorPosition;

		pOuterNFPs->push_back(pOutNFP);
	}

	// ����ѡ�е������͵�ͼ�����������ġ��⿿���켣����
	pOuterStickPoly = Polygon2DProcessor::MergePolygon(pOuterNFPs, OPTPOLY_FACTOR);

	return pOuterStickPoly;
}

Polygon2DListPtr LoopPositionDetector::CalcInnerStickPolys(const Point2D& cursorPosition, const CanvasZone* pCanvasZone, double dCompressValue)
{
	Polygon2DListPtr pInnerStickPolys;

	// ѹ����ͼ�������������
	Polygon2DPtr pNewOuterPoly;
	if (dCompressValue > 0)
	{
		Polygon2DListPtr pPoly2DList = Polygon2DProcessor::CompressPolygon(pCanvasZone->GetOuterPoly()->Clone(), dCompressValue);
		if (pPoly2DList->size() == 0) // ��ʱ����ѹ���㷨���󣬻�����ѹ����̫��
			return pInnerStickPolys;
		pNewOuterPoly = pPoly2DList->at(0);
	}
	else
	{
		pNewOuterPoly = pCanvasZone->GetOuterPoly()->Clone();
		Polygon2DProcessor::EnlargePolygon(pNewOuterPoly, -dCompressValue, CUTARC_FACTOR, OPTPOLY_FACTOR);
	}

	// �õ���ͼ�����������͸����������������ٽ����Ρ�
	vector<Polygon2DListPtr> innerNFPs;
	{
		Line2DListPtr pLineList1 = pNewOuterPoly->GetLineList();
		for (unsigned int i = 0; i < m_pFirstLayerPolys_Expanded->size(); i++)
		{
			const Polygon2D* pFirstLayerOuterPoly = m_pFirstLayerPolys_Expanded->operator[](i).get();
			Line2DListPtr pLineList2 = pFirstLayerOuterPoly->GetLineList();
			Polygon2DListPtr pInnerNFP = Polygon2DProcessor::BuildInnerNFP(pNewOuterPoly.get(), pLineList1.get(), pFirstLayerOuterPoly, pLineList2.get(), cursorPosition);
			if (pInnerNFP->size() == 0)
				return pInnerStickPolys; // ѡ�еļ��������޷��ŵ���ͼ������߿���ڲ���
			else
				innerNFPs.push_back(pInnerNFP);
		}
	}

	// ���㼸�������͵�ͼ�����������ġ��ڿ����켣����
	if (innerNFPs.size() == 1)
		pInnerStickPolys = innerNFPs.at(0);
	else
	{
		// �ԡ����ٽ����Ρ������󽻡�
		Polygon2DListPtr pInnerNFP1, pInnerNFP2; // pInnerNFP1�д��ÿ���󽻵Ľ����
		for (unsigned int m = 0; m < innerNFPs.size() - 1; m++)
		{
			if (m == 0)
				pInnerNFP1 = innerNFPs.at(m);
			pInnerNFP2 = innerNFPs.at(m + 1);

			// ��2�����ٽ����εĽ�����
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

						// ��2�����ٽ����α���Ҫ�ཻ��
						if (!pPoly1->IntersectWith(*pPoly2))
							continue;

						// �󽻡�
						Line2DListPtr pAllLine(new Line2DList);
						pAllLine->insert(pAllLine->begin(), pLineList1->begin(), pLineList1->end());
						pAllLine->insert(pAllLine->begin(), pLineList2->begin(), pLineList2->end());
						LineArc2DProcessor::CutLineByCross(pAllLine);
						Polygon2DPtr pCrossPoly = Polygon2DBuilder::FindInnerBoundary(pAllLine);
						pCrossPolys->push_back(pCrossPoly);
					}
				}
			}

			if (pCrossPolys->size() == 0) // ֻҪ��2�����ٽ����β��ཻ����Щ�����������޷��͵�ͼ����������ʵ�֡��ڿ�������
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

	// ��������ͼ�����������ģ������������Ǹ������Ķ���Σ����ﲻ��Ҫ����
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
	// �õ��̶������ļ���������
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

	// ����������ļ������������˹�ϵ��
	LoopTopologyItemListPtr pLoopTopItems4FixedPatLoops = LoopProcessor::CalcLoopTopStruct(pFixedPatternLoops.get());
	return pLoopTopItems4FixedPatLoops;
}

CanvasZoneListPtr LoopPositionDetector::CalcCanvasZones(const LoopTopologyItemList* pLoopTopItems4FixedPatLoops, double dMovingPatLoopWidth, double dMovingPatLoopHeight)
{
	CanvasZoneListPtr pCanvasZones(new CanvasZoneList);

	// ���ݼ����������˹�ϵ�������ͼ���򼯺ϡ�
	// ע��
	// 1) ������ͨ��һ��ѭ���õ�������ͼ����Ȼ��ͨ���ݹ��ٵõ������ĵ�ͼ����
	{
		CanvasZonePtr pOuterCanvasZone(new CanvasZone);
		pCanvasZones->push_back(pOuterCanvasZone);

		// ������ͼ������Ǹ��������������
		Polygon2DPtr pOuterPoly(new Polygon2D);
		pOuterPoly->push_back(Point2D(-100000.0, -100000.0));
		pOuterPoly->push_back(Point2D(100000.0, -100000.0));
		pOuterPoly->push_back(Point2D(100000.0, 100000.0));
		pOuterPoly->push_back(Point2D(-100000.0, 100000.0));
		pOuterCanvasZone->SetOuterPoly(pOuterPoly);
		pOuterCanvasZone->SetPatLoopID4OuterPoly(INVALID_IDX);

		// ѭ����һ�㼸�����������ݹ���ȥ��
		map<LONGLONG, Polygon2DPtr> polygonsOfPatLoop;
		for (unsigned int i = 0; i < pLoopTopItems4FixedPatLoops->size(); i++)
		{
			const LoopTopologyItem* pLoopTopItem = pLoopTopItems4FixedPatLoops->operator[](i).get();

			// ����ڿ׵Ķ���Ρ�
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

			// �ݹ顣
			CalcCanvasZonesBypLoopTopItems(pLoopTopItem, pCanvasZones, polygonsOfPatLoop);
		}
	}

	// �����ƶ��ļ��������ĳߴ磬ɾ���Ƚ�С�ĵ�ͼ����
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

	// Ԥ��������ĵ�ͼ����to be commented.
	//for (unsigned int i = 0; i < pCanvasZones->size(); i++)
	//{
	//	PolyViewDlg polyViewDlg(_T("��ʾ��ͼ����"), NULL, FALSE);
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
	// �粻���ǵݹ飬����������������ͼ����
	CanvasZonePtr pCanvasZone(new CanvasZone);
	pCanvasZones->push_back(pCanvasZone);

	// ��ͼ����ĵ���������
	map<LONGLONG, Polygon2DPtr>::const_iterator iter = polygonsOfPatLoop.find(pLoopTopItem->GetPatternLoop()->GetID());
	if (iter == polygonsOfPatLoop.end()) // ��Ӧ�ó����������������һ��ݹ���Ӧ�ü�����˸ü��������Ķ���α�
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

		// ����ڿ׵Ķ���Ρ�
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

		// �ݹ顣
		CalcCanvasZonesBypLoopTopItems(pSubLoopTopItem, pCanvasZones, polygonsOfPatLoop);
	}
}

END_CUTLEADER_NAMESPACE()

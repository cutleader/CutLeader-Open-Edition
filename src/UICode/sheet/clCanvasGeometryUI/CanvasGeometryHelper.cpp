#include "StdAfx.h"
#include "CanvasGeometryHelper.h"

#include "LogMgr.h"
#include "DataBaseManager.h"
#include "Polygon2D.h"
#include "Polygon2DList.h"
#include "LineArc2DList.h"
#include "clBaseDataResource.h"
#include "CommandManager.h"

#include "ParamConfigLoader.h"

#include "PartCadData.h"
#include "PatternLoopList.h"
#include "IPatternLoop.h"
#include "PatternLoopCache.h"
#include "PatternCache.h"
#include "PatternList.h"
#include "IPattern.h"
#include "PatternLoopNodeList.h"
#include "PatternLoopNode.h"
#include "LoopProcessor.h"
#include "LoopCutFeature.h"
#include "PartCamData.h"
#include "LoopCutFeatureList.h"
#include "LoopToolData.h"
#include "LoopStartCutData.h"
#include "ILoopStartCutPt.h"
#include "CornerStartCutPt.h"
#include "LoopCornerData.h"
#include "CornerList.h"
#include "ICorner.h"
#include "LoopMicroJoint.h"
#include "MicroJointList.h"
#include "MicroJoint.h"
#include "LoopStartCutProcessor.h"
#include "LoopBuilder.h"
#include "LoopCutFeatureManager.h"
#include "StdPatternLoop.h"
#include "PolygonPatternLoop.h"
#include "LinePattern.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "GeometryFeatureHelper.h"
#include "PartPlacement.h"
#include "Part.h"
#include "clGeometryFeatureResource.h"
#include "CanvasLoopAlignCommand.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

GeometryGridParam::GeometryGridParam()
{
	m_dRowSpacing = 2.0;
	m_dColumnSpacing = 2.0;
	m_iRowNum = 2;
	m_iColumnNum = 2;
	m_emGeometryGridDir = GRID_RIGHT_TOP;
}

GeometryGridParam::~GeometryGridParam(void)
{
}

tuple<PatternListPtr, PatternLoopListPtr, TextStructListPtr> CanvasGeometryHelper::ClonePatternLoops2NewPosition(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	const PatternLoopList* pPatternLoops_willCopy, const vector<LONGLONG>& textStructs_willCopy, const Matrix2D& transformMatrix)
{
	PatternListPtr pAllPatterns = pPartCadData->GetPatternList();
	PatternLoopListPtr pAllPatternLoops = pPartCadData->GetPatternLoopList();
	PatternListPtr pOldPatterns = pPatternLoops_willCopy->GetPatList();

	// 更新零件的几何特征。
	PatternListPtr pNewPatterns; // 克隆出的新的几何特征。
	PatternLoopListPtr pNewPatternLoops; // 克隆出的新的几何轮廓。
	TextStructListPtr pNewTextStructs(new TextStructList); // 克隆出的新的文字对象。
	{
		// 克隆图形集合。
		pNewPatterns = boost::dynamic_pointer_cast<PatternList>(pOldPatterns->Clone());
		pNewPatterns->Transform(transformMatrix);
		pNewPatterns->UpdateCache();
		pAllPatterns->insert(pAllPatterns->end(), pNewPatterns->begin(), pNewPatterns->end());

		// 克隆几何回路。
		pNewPatternLoops = boost::dynamic_pointer_cast<PatternLoopList>(pPatternLoops_willCopy->Clone());
		for (unsigned int i = 0; i < pNewPatternLoops->size(); i++) // 更新回路节点中引用的几何特征。
		{
			PatternLoopNodeListPtr pNewPatternLoopNodes = pNewPatternLoops->operator[](i)->GetPatternLoopNodes();
			const PatternLoopNodeList* pOldPatternLoopNodes = pPatternLoops_willCopy->operator[](i)->GetPatternLoopNodes().get();
			for (unsigned int j = 0; j < pNewPatternLoopNodes->size(); j++)
			{
				PatternLoopNodePtr pNewPatternLoopNode = pNewPatternLoopNodes->at(j);
				const IPattern* pOldPattern = pNewPatternLoopNode->GetPattern().get();
				int iPatternIndex = pOldPatterns->GetPatternIndex(pOldPattern);
				if (iPatternIndex == INVALID_IDX)
					LogMgr::DebugWarn(_T("497143"));
				pNewPatternLoopNode->SetPattern(pNewPatterns->operator[](iPatternIndex));
			}
		}
		LoopProcessor::UpdateCache(pNewPatternLoops);
		pAllPatternLoops->insert(pAllPatternLoops->end(), pNewPatternLoops->begin(), pNewPatternLoops->end());

		// 更新零件中所有几何轮廓的拓扑关系。
		LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get());
		pPartCadData->SetLoopTopologyItems(pLoopTopologyItems);

		// 克隆文字对象。
		if (textStructs_willCopy.size() > 0)
		{
			for (unsigned int i = 0; i < textStructs_willCopy.size(); i++)
			{
				const TextStruct* pTextStruct = pPartCadData->GetTextStructs()->GetTextStructByID(textStructs_willCopy[i]).get();
				TextStructPtr pNewTextStruct = boost::dynamic_pointer_cast<TextStruct>(pTextStruct->Clone());
				Matrix2D newTransformMatrix = pNewTextStruct->GetTransformMatrix();
				newTransformMatrix *= transformMatrix;
				pNewTextStruct->SetTransformMatrix(newTransformMatrix);
				pPartCadData->GetTextStructs()->push_back(pNewTextStruct);
				pNewTextStructs->push_back(pNewTextStruct);

				PatternListPtr pNewInvolvedPatterns(new PatternList);
				const PatternList* pOldInvolvedPatterns = pNewTextStruct->GetInvolvedPatterns().get();
				for (unsigned int j = 0; j < pOldInvolvedPatterns->size(); j++)
				{
					const IPattern* pOldPattern = pOldInvolvedPatterns->operator[](j).get();
					int iPatternIndex = pOldPatterns->GetPatternIndex(pOldPattern);
					if (iPatternIndex == INVALID_IDX)
						LogMgr::DebugWarn(_T("654289"));
					pNewInvolvedPatterns->push_back(pNewPatterns->operator[](iPatternIndex));
				}
				pNewTextStruct->SetInvolvedPatterns(pNewInvolvedPatterns);

				PatternLoopListPtr pNewInvolvedPatternLoops(new PatternLoopList);
				const PatternLoopList* pOldInvolvedPatternLoops = pNewTextStruct->GetInvolvedPatternLoops().get();
				for (unsigned int j = 0; j < pOldInvolvedPatternLoops->size(); j++)
				{
					const IPatternLoop* pOldPatternLoop = pOldInvolvedPatternLoops->operator[](j).get();
					int iPatternLoopIndex = pPatternLoops_willCopy->GetPatternLoopIndex(pOldPatternLoop->GetID());
					if (iPatternLoopIndex == INVALID_IDX)
						LogMgr::DebugWarn(_T("429617"));
					pNewInvolvedPatternLoops->push_back(pNewPatternLoops->operator[](iPatternLoopIndex));
				}
				pNewTextStruct->SetInvolvedPatternLoops(pNewInvolvedPatternLoops);
			}
			pPartCadData->GetTextStructs()->SetModified(TRUE);
		}

		pAllPatterns->SetModified(TRUE);
		pAllPatternLoops->SetModified(TRUE);
		pPartCadData->SetModified(TRUE);
	}

	// 更新零件的加工特征。
	{
		// 拷贝新的轮廓加工特征，并逐个更新其中的数据。
		for (unsigned int i = 0; i < pPatternLoops_willCopy->size(); i++)
		{
			const IPatternLoop* pOldPatternLoop = pPatternLoops_willCopy->operator[](i).get();
			const LoopCutFeature* pOldLoopCutFeature = pPartCamData->GetLoopFeatureList()->FindByPatternLoopID(pOldPatternLoop->GetID()).get();
			LoopCutFeaturePtr pNewLoopCutFeature = boost::dynamic_pointer_cast<LoopCutFeature>(pOldLoopCutFeature->Clone());
			pPartCamData->GetLoopFeatureList()->push_back(pNewLoopCutFeature);

			// 更新“回路加工特征”中的回路对象。
			IPatternLoopPtr pNewPatternLoop = pNewPatternLoops->at(i);
			pNewLoopCutFeature->SetPatternLoop(pNewPatternLoop);

			// 更新“轮廓刀具特征”中的回路对象。
			LoopToolDataPtr pNewLoopToolData = pNewLoopCutFeature->GetLoopTool();
			pNewLoopToolData->SetPatternLoop(pNewPatternLoop);

			// 更新“回路起切特征”中的图形信息。
			{
				LoopStartCutDataPtr pNewLoopStaEndCut = pNewLoopCutFeature->GetLoopStartCutData();
				pNewLoopStaEndCut->SetPatternLoop(pNewPatternLoop);

				ILoopStartCutPtPtr pLoopStartCutPt = pNewLoopStaEndCut->GetLoopStartCutPt();
				LONGLONG iOldStartPatternId1 = pLoopStartCutPt->GetStartPatID();
				int iPatternIndex1 = pOldPatterns->GetIndexByID(iOldStartPatternId1);
				if (iPatternIndex1 == INVALID_IDX)
					LogMgr::DebugWarn(_T("522876"));
				const IPattern* pNewPattern1 = pNewPatterns->at(iPatternIndex1).get();
				pLoopStartCutPt->SetStartPatID(pNewPattern1->GetID());

				if (pLoopStartCutPt->GetSECutPtType() == SECUT_PT_CORNER)
				{
					CornerStartCutPtPtr pCornerSECutPt = boost::dynamic_pointer_cast<CornerStartCutPt>(pLoopStartCutPt);
					LONGLONG iOldStartPatternId2 = pCornerSECutPt->GetSecondPatternID();
					int iPatternIndex = pOldPatterns->GetIndexByID(iOldStartPatternId2);
					if (iPatternIndex == INVALID_IDX)
						LogMgr::DebugWarn(_T("227165"));
					const IPattern* pNewPattern2 = pNewPatterns->at(iPatternIndex).get();
					pCornerSECutPt->SetSecondPatternID(pNewPattern2->GetID());
				}

				// 更新缓存。
				LoopStartCutProcessor::UpdateCache(pNewLoopStaEndCut);
			}

			// 更新“回路角特征”中的图形信息。
			{
				LoopCornerDataPtr pNewLoopCorner = pNewLoopCutFeature->GetLoopCorner();
				pNewLoopCorner->SetPatternLoop(pNewPatternLoop);

				CornerListPtr pCornerList = pNewLoopCorner->GetCornerList();
				for (unsigned int k = 0; k < pCornerList->size(); k++)
				{
					ICornerPtr pCorner = pCornerList->operator[](k);

					LONGLONG iOldFirstPatternID = pCorner->GetFirstPatternID();
					int iPatternIndex1 = pOldPatterns->GetIndexByID(iOldFirstPatternID);
					if (iPatternIndex1 == INVALID_IDX)
						LogMgr::DebugWarn(_T("994372"));
					const IPattern* pNewPattern1 = pNewPatterns->at(iPatternIndex1).get();
					pCorner->SetFirstPatternID(pNewPattern1->GetID());

					LONGLONG iOldSecondPatternID = pCorner->GetSecondPatternID();
					int iPatternIndex2 = pOldPatterns->GetIndexByID(iOldSecondPatternID);
					if (iPatternIndex2 == INVALID_IDX)
						LogMgr::DebugWarn(_T("185429"));
					const IPattern* pNewPattern2 = pNewPatterns->at(iPatternIndex2).get();
					pCorner->SetSecondPatternID(pNewPattern2->GetID());
				}
			}

			// 更新“回路微连接特征”中的图形信息。
			{
				LoopMicroJointPtr pNewLoopMicroJoint = pNewLoopCutFeature->GetLoopMicroJoint();
				pNewLoopMicroJoint->SetPatternLoop(pNewPatternLoop);

				MicroJointListPtr pMicroJointList = pNewLoopMicroJoint->GetMicroJointList();
				for (unsigned int l = 0; l < pMicroJointList->size(); l++)
				{
					MicroJointPtr pMicroJoint = pMicroJointList->operator[](l);
					LONGLONG iOldPatternID = pMicroJoint->GetPatternID();
					int iPatternIndex1 = pOldPatterns->GetIndexByID(iOldPatternID);
					if (iPatternIndex1 == INVALID_IDX)
						LogMgr::DebugWarn(_T("492574"));
					const IPattern* pNewPattern2 = pNewPatterns->at(iPatternIndex1).get();
					pMicroJoint->SetPatternID(pNewPattern2->GetID());
				}
			}
		}

		pPartCamData->GetLoopFeatureList()->SetModified(TRUE);
		pPartCamData->SetModified(TRUE);
	}

	tuple<PatternListPtr, PatternLoopListPtr, TextStructListPtr> newCadData = make_tuple(pNewPatterns, pNewPatternLoops, pNewTextStructs);
	return newCadData;
}

void CanvasGeometryHelper::TransformPatternLoops(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	const vector<LONGLONG>& patternLoops_willTransform, const vector<LONGLONG>& textStructs_willTransform, const Matrix2D& transformMatrix)
{
	// 更新cad数据。
	GeometryFeatureHelper::TransformPatternLoops(pPartCadData, patternLoops_willTransform, textStructs_willTransform, transformMatrix);

	// 更新cam数据。
	for (unsigned int i = 0; i < patternLoops_willTransform.size(); i++) {
		LoopCutFeaturePtr pLoopCutFeature = pPartCamData->GetLoopFeatureList()->FindByPatternLoopID(patternLoops_willTransform[i]);
		LoopStartCutProcessor::UpdateCache(pLoopCutFeature->GetLoopStartCutData());
	}

	// 设置修改标记。
	pPartCamData->GetLoopFeatureList()->SetModified(TRUE);
	pPartCamData->SetModified(TRUE);
}

void CanvasGeometryHelper::ChangeSizeOfRectPolygonLoop(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, PolygonPatternLoopPtr pPolygonPatternLoop,
	double dNewLoopWidth, double dNewLoopHeight)
{
	// 更新cad数据。
	GeometryFeatureHelper::ChangeSizeOfRectPolygonLoop(pPartCadData, pPolygonPatternLoop, dNewLoopWidth, dNewLoopHeight);

	// 更新cam数据。
	LoopCutFeaturePtr pLoopCutFeature = pPartCamData->GetLoopFeatureList()->FindByPatternLoopID(pPolygonPatternLoop->GetID());
	LoopStartCutProcessor::UpdateCache(pLoopCutFeature->GetLoopStartCutData());

	// 设置修改标记。
	pPartCamData->GetLoopFeatureList()->SetModified(TRUE);
	pPartCamData->SetModified(TRUE);
}

void CanvasGeometryHelper::TransformPatternLoops_2(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, const vector<LONGLONG>& patternLoops_willTransform,
	const vector<LONGLONG>& textStructs_willTransform, const map<LONGLONG, Matrix2D>& transformMatrixs4Loops, const map<LONGLONG, Matrix2D>& transformMatrixs4Texts)
{
	// 更新cad数据。
	GeometryFeatureHelper::TransformPatternLoops_2(pPartCadData, patternLoops_willTransform, textStructs_willTransform, transformMatrixs4Loops, transformMatrixs4Texts);

	// 更新cam数据。
	for (unsigned int i = 0; i < patternLoops_willTransform.size(); i++) {
		LoopCutFeaturePtr pLoopCutFeature = pPartCamData->GetLoopFeatureList()->FindByPatternLoopID(patternLoops_willTransform[i]);
		LoopStartCutProcessor::UpdateCache(pLoopCutFeature->GetLoopStartCutData());
	}

	// 设置修改标记。
	pPartCamData->GetLoopFeatureList()->SetModified(TRUE);
	pPartCamData->SetModified(TRUE);
}

void CanvasGeometryHelper::DeletePatternsOnCanvas(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, const PatternList* pDeletingPatterns,
	const vector<LONGLONG>& textStructs_willDelete)
{
	PatternListPtr pAllPatterns = pPartCadData->GetPatternList();
	PatternLoopListPtr pAllPatternLoops = pPartCadData->GetPatternLoopList();

	// 找出所有涉及到的几何轮廓。
	map<LONGLONG, PatternListPtr> patternsInLoop; // 这个结构记录了要删除的几何特征分别在哪个几何轮廓中。
	PatternLoopListPtr pPatternLoops_affected = GeometryFeatureHelper::FindAffectedPatternLoops(pPartCadData.get(), pDeletingPatterns, patternsInLoop);

	// 更新零件的cad数据。
	PatternLoopListPtr pNewPatternLoops; // 新创建的几何轮廓。
	{
		// 这段代码把要删除的几何特征从零件的几何特征中删除，并从涉及到的几何轮廓中删除。
		for (unsigned int i = 0; i < pPatternLoops_affected->size(); i++) // 遍历每个涉及到的几何轮廓。
		{
			IPatternLoopPtr pPatternLoop = pPatternLoops_affected->operator[](i);
			PatternListPtr pPatterns = patternsInLoop[pPatternLoop->GetID()];
			pAllPatterns->RemovePatterns(pPatterns.get()); // 从零件几何特征集合中删除这些几何特征。
			for (unsigned int j = 0; j < pPatterns->size(); j++) // 从涉及到的轮廓中删除节点。
				pPatternLoop->GetPatternLoopNodes()->RemovePattern(pPatterns->operator[](j));
		}

		// 先从零件中删除所有涉及到的几何轮廓，再通过“涉及到的几何轮廓中剩下的几何特征”来创建新的轮廓并加入零件。
		PatternListPtr pPatternsWillGenerateNewLoops(new PatternList);
		for (unsigned int i = 0; i < pPatternLoops_affected->size(); i++) // 遍历每个涉及到的几何轮廓。
		{
			const IPatternLoop* pPatternLoop = pPatternLoops_affected->operator[](i).get();
			pPatternsWillGenerateNewLoops->AddPatterns(pPatternLoop->GetPatternList());
			pAllPatternLoops->DeleteLoopEx(pPatternLoop);
		}
		pNewPatternLoops = LoopBuilder::BuildPatternLoops(pPatternsWillGenerateNewLoops.get());
		if (pNewPatternLoops->size() > 0)
		{
			LoopProcessor::UpdateCache(pNewPatternLoops);
			pAllPatternLoops->insert(pAllPatternLoops->end(), pNewPatternLoops->begin(), pNewPatternLoops->end());
		}

		// 更新零件中所有几何轮廓的拓扑关系。
		LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get());
		pPartCadData->SetLoopTopologyItems(pLoopTopologyItems);

		// 删除文字对象。
		for (unsigned int i = 0; i < textStructs_willDelete.size(); i++)
		{
			pPartCadData->GetTextStructs()->DeleteTextStruct(textStructs_willDelete[i]);
		}

		pPartCadData->GetPatternList()->SetModified(TRUE);
		pPartCadData->GetPatternLoopList()->SetModified(TRUE);
		pPartCadData->GetTextStructs()->SetModified(TRUE);
		pPartCadData->SetModified(TRUE);
	}

	// 更新零件的加工特征。
	{
		// 删除涉及到的几何轮廓对应的轮廓工艺。
		for (unsigned int i = 0; i < pPatternLoops_affected->size(); i++)
		{
			const IPatternLoop* pPatternLoop = pPatternLoops_affected->at(i).get();
			pPartCamData->GetLoopFeatureList()->DeleteLoopCutFeatureByLoopID(pPatternLoop->GetID());
		}

		// 为新创建的几何轮廓分配加工特征。
		DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
		ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pPartCamData->GetParamConfigID());
		const LoopTopologyItemList* pLoopTopologyItems = pPartCadData->GetLoopTopologyItems();
		for (unsigned int i = 0; i < pNewPatternLoops->size(); i++)
		{
			IPatternLoopPtr pPatternLoop = pNewPatternLoops->at(i);
			LoopCutFeaturePtr pLoopFeature = LoopCutFeatureManager::AssignLoopFeature(pPatternLoop, pLoopTopologyItems, pParamConfig.get(), TRUE, TRUE, Matrix2D());
			pPartCamData->GetLoopFeatureList()->push_back(pLoopFeature);
		}

		pPartCamData->GetLoopFeatureList()->SetModified(TRUE);
		pPartCamData->SetModified(TRUE);
	}
}

void CanvasGeometryHelper::AddPattern2Canvas(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, IPatternPtr pNewPattern)
{
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pPartCamData->GetParamConfigID());

	PatternListPtr pAllPatterns = pPartCadData->GetPatternList();
	PatternLoopListPtr pAllPatternLoops = pPartCadData->GetPatternLoopList();

	if (pNewPattern->IsStdPattern())
	{
		// 更新零件的cad数据。
		pAllPatterns->push_back(pNewPattern);
		PatternLoopNodePtr pPatternLoopNode(new PatternLoopNode(pNewPattern, TRUE)); // here, the second param is not important.
		StdPatternLoopPtr pStdPatternLoop(new StdPatternLoop(pPatternLoopNode));
		LoopProcessor::UpdateCache(pStdPatternLoop);
		pAllPatternLoops->push_back(pStdPatternLoop);
		LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get());
		pPartCadData->SetLoopTopologyItems(pLoopTopologyItems); // 更新零件中所有几何轮廓的拓扑关系。

		// 更新零件的cam数据。
		LoopCutFeaturePtr pLoopFeature = LoopCutFeatureManager::AssignLoopFeature(pStdPatternLoop, pLoopTopologyItems.get(), pParamConfig.get(), TRUE, TRUE, Matrix2D());
		pPartCamData->GetLoopFeatureList()->push_back(pLoopFeature);
	}
	else if (pNewPattern->IsArrayPattern())
	{
		LogMgr::DebugWarn(_T("498366"));
		return;
	}
	else // pNewPattern是直线或者圆弧。
	{
		PatternLoopListPtr pOpenPatternLoops = pAllPatternLoops->GetOpenLoopLinkToPat(pNewPattern.get(), GEOM_TOLERANCE); // 搜索出能和pNewPattern连接的开放回路。
		if (pOpenPatternLoops->size() == 1) // 只有一个开放轮廓和pNewPattern能连接，那么就把pNewPattern加入这个轮廓。
		{
			// 更新零件的cad数据。
			pAllPatterns->push_back(pNewPattern);
			IPatternLoopPtr pTmpLoop = pOpenPatternLoops->front();
			PolygonPatternLoopPtr pPolygonPatternLoop = boost::dynamic_pointer_cast<PolygonPatternLoop>(pTmpLoop);
			pPolygonPatternLoop->AddPattern(pNewPattern, GEOM_TOLERANCE); // PolygonPatternLoop class will decide where this pattern will be added.
			LoopProcessor::UpdateCache(pPolygonPatternLoop);
			LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get());
			pPartCadData->SetLoopTopologyItems(pLoopTopologyItems); // 更新零件中所有几何轮廓的拓扑关系。

			// 更新零件的cam数据。
			pPartCamData->GetLoopFeatureList()->DeleteLoopCutFeatureByLoopID(pPolygonPatternLoop->GetID());
			LoopCutFeaturePtr pLoopFeature = LoopCutFeatureManager::AssignLoopFeature(pPolygonPatternLoop, pLoopTopologyItems.get(), pParamConfig.get(), TRUE, TRUE, Matrix2D());
			pPartCamData->GetLoopFeatureList()->push_back(pLoopFeature);
		}
		else // 有多个开放轮廓和pNewPattern能连接。
		{
			// 更新零件的cad数据。
			PatternLoopListPtr pNewPatternLoops(new PatternLoopList); // 新建的几何轮廓。
			{
				pAllPatterns->push_back(pNewPattern);

				// 搜集出这些开放轮廓中所有几何特征，把pNewPattern也加进去。
				PatternListPtr pPatList(new PatternList);
				for (unsigned int i = 0; i < pOpenPatternLoops->size(); i++)
				{
					const IPatternLoop* pPatternLoop = pOpenPatternLoops->operator[](i).get();
					PatternListPtr pTmpPatList = pPatternLoop->GetPatternList();
					pPatList->insert(pPatList->end(), pTmpPatList->begin(), pTmpPatList->end());
				}
				pPatList->push_back(pNewPattern);

				// 删除这些开放轮廓。
				for (unsigned int i = 0; i < pOpenPatternLoops->size(); i++)
				{
					const IPatternLoop* pPatternLoop = pOpenPatternLoops->operator[](i).get();
					pAllPatternLoops->DeleteLoop(pPatternLoop);
				}

				// 重新生成轮廓。
				if (pPatList->size() > 0)
				{
					pNewPatternLoops = LoopBuilder::BuildPolygonPatternLoops(pPatList, GEOM_TOLERANCE);
					LoopProcessor::UpdateCache(pNewPatternLoops);
					pAllPatternLoops->insert(pAllPatternLoops->end(), pNewPatternLoops->begin(), pNewPatternLoops->end());
				}

				// 更新零件中所有几何轮廓的拓扑关系。
				LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get());
				pPartCadData->SetLoopTopologyItems(pLoopTopologyItems);
			}

			// 更新零件的cam数据。
			for (unsigned int i = 0; i < pOpenPatternLoops->size(); i++)
			{
				const IPatternLoop* pPatternLoop = pOpenPatternLoops->at(i).get();
				pPartCamData->GetLoopFeatureList()->DeleteLoopCutFeatureByLoopID(pPatternLoop->GetID());
			}
			for (unsigned int i = 0; i < pNewPatternLoops->size(); i++)
			{
				IPatternLoopPtr pNewPatternLoop = pNewPatternLoops->at(i);
				LoopCutFeaturePtr pLoopFeature = LoopCutFeatureManager::AssignLoopFeature(pNewPatternLoop, pPartCadData->GetLoopTopologyItems(), pParamConfig.get(), TRUE, TRUE, Matrix2D());
				pPartCamData->GetLoopFeatureList()->push_back(pLoopFeature);
			}
		}
	}

	// 设置修改标记。
	pPartCadData->GetPatternList()->SetModified(TRUE);
	pPartCadData->GetPatternLoopList()->SetModified(TRUE);
	pPartCadData->SetModified(TRUE);
	pPartCamData->GetLoopFeatureList()->SetModified(TRUE);
	pPartCamData->SetModified(TRUE);
}

void CanvasGeometryHelper::AddText2Canvas(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, TextParamPtr pTextParam, const Vector2D& transferVect, Polygon2DListPtr pTextContours)
{
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pPartCamData->GetParamConfigID());

	PatternListPtr pAllPatterns = pPartCadData->GetPatternList();
	PatternLoopListPtr pAllPatternLoops = pPartCadData->GetPatternLoopList();

	Matrix2D transformMatrix;
	transformMatrix.SetTransfer(transferVect);

	// 创建几何轮廓和几何特征。
	PatternListPtr pNewPatterns(new PatternList);
	PatternLoopListPtr pNewPatternLoops(new PatternLoopList);
	for (unsigned int i = 0; i < pTextContours->size(); i++)
	{
		const Polygon2D* pTextContour = pTextContours->operator[](i).get();
		PolygonPatternLoopPtr pPolygonPatternLoop(new PolygonPatternLoop);
		for (unsigned int j = 0; j < pTextContour->size(); j++)
		{
			const Point2D& startPt = pTextContour->operator[](j == 0 ? pTextContour->size() - 1 : j - 1);
			const Point2D& endPt = pTextContour->operator[](j);

			// 创建几何特征。
			LinePatternPtr pLinePattern(new LinePattern(startPt, endPt));
			pLinePattern->Transform(transformMatrix);
			pLinePattern->UpdateCache();
			pAllPatterns->push_back(pLinePattern);
			pNewPatterns->push_back(pLinePattern);

			// 创建几何轮廓节点。
			PatternLoopNodePtr pPatternLoopNode(new PatternLoopNode(pLinePattern, TRUE));
			pPolygonPatternLoop->GetPatternLoopNodes()->push_back(pPatternLoopNode);
		}
		LoopProcessor::UpdateCache(pPolygonPatternLoop);
		pAllPatternLoops->push_back(pPolygonPatternLoop);
		pNewPatternLoops->push_back(pPolygonPatternLoop);
	}
	pPartCadData->SetLoopTopologyItems(LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get()));

	// 创建文字对象。
	TextStructPtr pTextStruct(new TextStruct(pTextParam, transformMatrix, pNewPatterns, pNewPatternLoops));
	pPartCadData->GetTextStructs()->push_back(pTextStruct);

	// 更新零件的cam数据。
	for (unsigned int i = 0; i < pNewPatternLoops->size(); i++)
	{
		IPatternLoopPtr pNewPatternLoop = pNewPatternLoops->at(i);
		LoopCutFeaturePtr pLoopFeature = LoopCutFeatureManager::AssignLoopFeature(pNewPatternLoop, pPartCadData->GetLoopTopologyItems(), pParamConfig.get(), TRUE, TRUE, Matrix2D());
		pPartCamData->GetLoopFeatureList()->push_back(pLoopFeature);
	}

	// 设置修改标记。
	pPartCadData->GetPatternList()->SetModified(TRUE);
	pPartCadData->GetPatternLoopList()->SetModified(TRUE);
	pPartCadData->GetTextStructs()->SetModified(TRUE);
	pPartCadData->SetModified(TRUE);
	pPartCamData->GetLoopFeatureList()->SetModified(TRUE);
	pPartCamData->SetModified(TRUE);
}

void CanvasGeometryHelper::ChangeParamOfTextStruct(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, LONGLONG iTextStructID, TextParamPtr pNewTextParam,
	const Polygon2DList* pNewTextContours)
{
	PatternListPtr pAllPatterns = pPartCadData->GetPatternList();
	PatternLoopListPtr pAllPatternLoops = pPartCadData->GetPatternLoopList();

	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, pPartCamData->GetParamConfigID());
	TextStructPtr pTextStruct = pPartCadData->GetTextStructs()->GetTextStructByID(iTextStructID);

	// 先删除原文字对象对应的几何特征和几何轮廓，以及加工工艺。
	{
		const PatternList* pInvolvedPatterns = pTextStruct->GetInvolvedPatterns().get();
		const PatternLoopList* pInvolvedPatternLoops = pTextStruct->GetInvolvedPatternLoops().get();
		pAllPatterns->RemovePatterns(pInvolvedPatterns);
		pAllPatternLoops->DeleteLoopList(pInvolvedPatternLoops);

		// 删除涉及到的几何轮廓对应的轮廓工艺。
		for (unsigned int i = 0; i < pInvolvedPatternLoops->size(); i++)
		{
			const IPatternLoop* pPatternLoop = pInvolvedPatternLoops->at(i).get();
			pPartCamData->GetLoopFeatureList()->DeleteLoopCutFeatureByLoopID(pPatternLoop->GetID());
		}
	}

	// 创建几何轮廓和几何特征。
	PatternListPtr pNewPatterns(new PatternList);
	PatternLoopListPtr pNewPatternLoops(new PatternLoopList);
	for (unsigned int i = 0; i < pNewTextContours->size(); i++)
	{
		const Polygon2D* pTextContour = pNewTextContours->operator[](i).get();
		PolygonPatternLoopPtr pPolygonPatternLoop(new PolygonPatternLoop);
		for (unsigned int j = 0; j < pTextContour->size(); j++)
		{
			const Point2D& startPt = pTextContour->operator[](j == 0 ? pTextContour->size() - 1 : j - 1);
			const Point2D& endPt = pTextContour->operator[](j);

			// 创建几何特征。
			LinePatternPtr pLinePattern(new LinePattern(startPt, endPt));
			pLinePattern->Transform(pTextStruct->GetTransformMatrix());
			pLinePattern->UpdateCache();
			pAllPatterns->push_back(pLinePattern);
			pNewPatterns->push_back(pLinePattern);

			// 创建几何轮廓节点。
			PatternLoopNodePtr pPatternLoopNode(new PatternLoopNode(pLinePattern, TRUE));
			pPolygonPatternLoop->GetPatternLoopNodes()->push_back(pPatternLoopNode);
		}
		LoopProcessor::UpdateCache(pPolygonPatternLoop);
		pAllPatternLoops->push_back(pPolygonPatternLoop);
		pNewPatternLoops->push_back(pPolygonPatternLoop);
	}
	pPartCadData->SetLoopTopologyItems(LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get()));

	// 修改文字对象。
	{
		pTextStruct->SetTextParam(pNewTextParam);
		pTextStruct->SetInvolvedPatterns(pNewPatterns);
		pTextStruct->SetInvolvedPatternLoops(pNewPatternLoops);
	}

	// 更新零件的cam数据。
	for (unsigned int i = 0; i < pNewPatternLoops->size(); i++)
	{
		IPatternLoopPtr pNewPatternLoop = pNewPatternLoops->at(i);
		LoopCutFeaturePtr pLoopFeature = LoopCutFeatureManager::AssignLoopFeature(pNewPatternLoop, pPartCadData->GetLoopTopologyItems(), pParamConfig.get(), TRUE, TRUE, Matrix2D());
		pPartCamData->GetLoopFeatureList()->push_back(pLoopFeature);
	}

	// 设置修改标记。
	pPartCadData->GetPatternList()->SetModified(TRUE);
	pPartCadData->GetPatternLoopList()->SetModified(TRUE);
	pTextStruct->SetModified(TRUE);
	pPartCadData->GetTextStructs()->SetModified(TRUE);
	pPartCadData->SetModified(TRUE);
	pPartCamData->GetLoopFeatureList()->SetModified(TRUE);
	pPartCamData->SetModified(TRUE);
}

LineArc2DListPtr CanvasGeometryHelper::CreateGeometryGrid(const LineArc2DList* pBaseLineArc2Ds, const GeometryGridParam* pGeometryGridParam)
{
	LineArc2DListPtr pLineArc2Ds_return(new LineArc2DList);

	Rect2D outerRect = pBaseLineArc2Ds->GetRectBox();
	double dRowSpacing = pGeometryGridParam->GetRowSpacing();
	double dColumnSpacing = pGeometryGridParam->GetColumnSpacing();
	int iRowNum = pGeometryGridParam->GetRowNum();
	int iColumnNum = pGeometryGridParam->GetColumnNum();
	GeometryGridDirType emGeometryGridDir = pGeometryGridParam->GetGeometryGridDir();

	// 注意这里的行列，如果阵列往左下延申，则往下数行数，往左数列数。
	for (int iRowIndex = 0; iRowIndex < iRowNum; iRowIndex++)
	{
		for (int iColumnIndex = 0; iColumnIndex < iColumnNum; iColumnIndex++)
		{
			if (iRowIndex == 0 && iColumnIndex == 0)
				continue;
			else
			{
				// 计算这个实例的变换矩阵。
				Matrix2D transformMatrix;
				if (emGeometryGridDir == GRID_RIGHT_TOP)
					transformMatrix.SetTransfer(Vector2D((outerRect.GetWidth() + dColumnSpacing) * iColumnIndex, (outerRect.GetHeight() + dRowSpacing) * iRowIndex));
				else if (emGeometryGridDir == GRID_RIGHT_BOTTOM)
					transformMatrix.SetTransfer(Vector2D((outerRect.GetWidth() + dColumnSpacing) * iColumnIndex, -(outerRect.GetHeight() + dRowSpacing) * iRowIndex));
				else if (emGeometryGridDir == GRID_LEFT_TOP)
					transformMatrix.SetTransfer(Vector2D(-(outerRect.GetWidth() + dColumnSpacing) * iColumnIndex, (outerRect.GetHeight() + dRowSpacing) * iRowIndex));
				else if (emGeometryGridDir == GRID_LEFT_BOTTOM)
					transformMatrix.SetTransfer(Vector2D(-(outerRect.GetWidth() + dColumnSpacing) * iColumnIndex, -(outerRect.GetHeight() + dRowSpacing) * iRowIndex));

				// 创建这个实例。
				LineArc2DListPtr pLineArc2Ds_instance = pBaseLineArc2Ds->Clone();
				pLineArc2Ds_instance->Transform(transformMatrix);
				pLineArc2Ds_return->insert(pLineArc2Ds_return->end(), pLineArc2Ds_instance->begin(), pLineArc2Ds_instance->end());
			}
		}
	}

	return pLineArc2Ds_return;
}

bool CanvasGeometryHelper::AlignCanvasLoop(const CWnd* pView, const CString& strEditorName, const PatternLoopList* pPatternLoops,
	const TextStructList* pTextStructs, AlignLoopStyle emAlignLoopStyle, PartPlacementPtr pPartPlacement)
{
	PartCadDataPtr pPartCadData = pPartPlacement->GetPart()->GetCadData();

	if (!pPatternLoops) {
		LogMgr::DebugWarn(_T("280942"));
		return false;
	}
	if (!pTextStructs) {
		LogMgr::DebugWarn(_T("997432"));
		return false;
	}
	if (pPatternLoops->size() < 2) { // 选中的轮廓数量小于2个，需提示用户重新选择。
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SelectMultiLoop2Align);
		MessageBox(pView->m_hWnd, str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
		return false;
	}

	// 要对齐的几何轮廓。
	vector<LONGLONG> patternLoops_willAlign;
	for (unsigned int i = 0; i < pPatternLoops->size(); i++) {
		patternLoops_willAlign.push_back(pPatternLoops->operator[](i)->GetID());
	}

	// 要对齐的文字。
	vector<LONGLONG> textStructs_willAlign;
	for (unsigned int i = 0; i < pTextStructs->size(); i++) {
		textStructs_willAlign.push_back(pTextStructs->operator[](i)->GetID());
	}

	// 执行命令。
	PartCamDataPtr pPartCamData = pPartPlacement->GetCamData();
	ICommandPtr pCommand(new CanvasLoopAlignCommand(EditorData(pView, strEditorName), pPartCadData, pPartCamData, patternLoops_willAlign, textStructs_willAlign, emAlignLoopStyle));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	return true;
}

END_CUTLEADER_NAMESPACE()

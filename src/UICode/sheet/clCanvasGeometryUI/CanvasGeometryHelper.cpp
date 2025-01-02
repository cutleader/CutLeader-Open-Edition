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

	// ��������ļ���������
	PatternListPtr pNewPatterns; // ��¡�����µļ���������
	PatternLoopListPtr pNewPatternLoops; // ��¡�����µļ���������
	TextStructListPtr pNewTextStructs(new TextStructList); // ��¡�����µ����ֶ���
	{
		// ��¡ͼ�μ��ϡ�
		pNewPatterns = boost::dynamic_pointer_cast<PatternList>(pOldPatterns->Clone());
		pNewPatterns->Transform(transformMatrix);
		pNewPatterns->UpdateCache();
		pAllPatterns->insert(pAllPatterns->end(), pNewPatterns->begin(), pNewPatterns->end());

		// ��¡���λ�·��
		pNewPatternLoops = boost::dynamic_pointer_cast<PatternLoopList>(pPatternLoops_willCopy->Clone());
		for (unsigned int i = 0; i < pNewPatternLoops->size(); i++) // ���»�·�ڵ������õļ���������
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

		// ������������м������������˹�ϵ��
		LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get());
		pPartCadData->SetLoopTopologyItems(pLoopTopologyItems);

		// ��¡���ֶ���
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

	// ��������ļӹ�������
	{
		// �����µ������ӹ�������������������е����ݡ�
		for (unsigned int i = 0; i < pPatternLoops_willCopy->size(); i++)
		{
			const IPatternLoop* pOldPatternLoop = pPatternLoops_willCopy->operator[](i).get();
			const LoopCutFeature* pOldLoopCutFeature = pPartCamData->GetLoopFeatureList()->FindByPatternLoopID(pOldPatternLoop->GetID()).get();
			LoopCutFeaturePtr pNewLoopCutFeature = boost::dynamic_pointer_cast<LoopCutFeature>(pOldLoopCutFeature->Clone());
			pPartCamData->GetLoopFeatureList()->push_back(pNewLoopCutFeature);

			// ���¡���·�ӹ��������еĻ�·����
			IPatternLoopPtr pNewPatternLoop = pNewPatternLoops->at(i);
			pNewLoopCutFeature->SetPatternLoop(pNewPatternLoop);

			// ���¡����������������еĻ�·����
			LoopToolDataPtr pNewLoopToolData = pNewLoopCutFeature->GetLoopTool();
			pNewLoopToolData->SetPatternLoop(pNewPatternLoop);

			// ���¡���·�����������е�ͼ����Ϣ��
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

				// ���»��档
				LoopStartCutProcessor::UpdateCache(pNewLoopStaEndCut);
			}

			// ���¡���·���������е�ͼ����Ϣ��
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

			// ���¡���·΢�����������е�ͼ����Ϣ��
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
	// ����cad���ݡ�
	GeometryFeatureHelper::TransformPatternLoops(pPartCadData, patternLoops_willTransform, textStructs_willTransform, transformMatrix);

	// ����cam���ݡ�
	for (unsigned int i = 0; i < patternLoops_willTransform.size(); i++) {
		LoopCutFeaturePtr pLoopCutFeature = pPartCamData->GetLoopFeatureList()->FindByPatternLoopID(patternLoops_willTransform[i]);
		LoopStartCutProcessor::UpdateCache(pLoopCutFeature->GetLoopStartCutData());
	}

	// �����޸ı�ǡ�
	pPartCamData->GetLoopFeatureList()->SetModified(TRUE);
	pPartCamData->SetModified(TRUE);
}

void CanvasGeometryHelper::ChangeSizeOfRectPolygonLoop(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, PolygonPatternLoopPtr pPolygonPatternLoop,
	double dNewLoopWidth, double dNewLoopHeight)
{
	// ����cad���ݡ�
	GeometryFeatureHelper::ChangeSizeOfRectPolygonLoop(pPartCadData, pPolygonPatternLoop, dNewLoopWidth, dNewLoopHeight);

	// ����cam���ݡ�
	LoopCutFeaturePtr pLoopCutFeature = pPartCamData->GetLoopFeatureList()->FindByPatternLoopID(pPolygonPatternLoop->GetID());
	LoopStartCutProcessor::UpdateCache(pLoopCutFeature->GetLoopStartCutData());

	// �����޸ı�ǡ�
	pPartCamData->GetLoopFeatureList()->SetModified(TRUE);
	pPartCamData->SetModified(TRUE);
}

void CanvasGeometryHelper::TransformPatternLoops_2(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, const vector<LONGLONG>& patternLoops_willTransform,
	const vector<LONGLONG>& textStructs_willTransform, const map<LONGLONG, Matrix2D>& transformMatrixs4Loops, const map<LONGLONG, Matrix2D>& transformMatrixs4Texts)
{
	// ����cad���ݡ�
	GeometryFeatureHelper::TransformPatternLoops_2(pPartCadData, patternLoops_willTransform, textStructs_willTransform, transformMatrixs4Loops, transformMatrixs4Texts);

	// ����cam���ݡ�
	for (unsigned int i = 0; i < patternLoops_willTransform.size(); i++) {
		LoopCutFeaturePtr pLoopCutFeature = pPartCamData->GetLoopFeatureList()->FindByPatternLoopID(patternLoops_willTransform[i]);
		LoopStartCutProcessor::UpdateCache(pLoopCutFeature->GetLoopStartCutData());
	}

	// �����޸ı�ǡ�
	pPartCamData->GetLoopFeatureList()->SetModified(TRUE);
	pPartCamData->SetModified(TRUE);
}

void CanvasGeometryHelper::DeletePatternsOnCanvas(PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, const PatternList* pDeletingPatterns,
	const vector<LONGLONG>& textStructs_willDelete)
{
	PatternListPtr pAllPatterns = pPartCadData->GetPatternList();
	PatternLoopListPtr pAllPatternLoops = pPartCadData->GetPatternLoopList();

	// �ҳ������漰���ļ���������
	map<LONGLONG, PatternListPtr> patternsInLoop; // ����ṹ��¼��Ҫɾ���ļ��������ֱ����ĸ����������С�
	PatternLoopListPtr pPatternLoops_affected = GeometryFeatureHelper::FindAffectedPatternLoops(pPartCadData.get(), pDeletingPatterns, patternsInLoop);

	// ���������cad���ݡ�
	PatternLoopListPtr pNewPatternLoops; // �´����ļ���������
	{
		// ��δ����Ҫɾ���ļ�������������ļ���������ɾ���������漰���ļ���������ɾ����
		for (unsigned int i = 0; i < pPatternLoops_affected->size(); i++) // ����ÿ���漰���ļ���������
		{
			IPatternLoopPtr pPatternLoop = pPatternLoops_affected->operator[](i);
			PatternListPtr pPatterns = patternsInLoop[pPatternLoop->GetID()];
			pAllPatterns->RemovePatterns(pPatterns.get()); // �������������������ɾ����Щ����������
			for (unsigned int j = 0; j < pPatterns->size(); j++) // ���漰����������ɾ���ڵ㡣
				pPatternLoop->GetPatternLoopNodes()->RemovePattern(pPatterns->operator[](j));
		}

		// �ȴ������ɾ�������漰���ļ�����������ͨ�����漰���ļ���������ʣ�µļ����������������µ����������������
		PatternListPtr pPatternsWillGenerateNewLoops(new PatternList);
		for (unsigned int i = 0; i < pPatternLoops_affected->size(); i++) // ����ÿ���漰���ļ���������
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

		// ������������м������������˹�ϵ��
		LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get());
		pPartCadData->SetLoopTopologyItems(pLoopTopologyItems);

		// ɾ�����ֶ���
		for (unsigned int i = 0; i < textStructs_willDelete.size(); i++)
		{
			pPartCadData->GetTextStructs()->DeleteTextStruct(textStructs_willDelete[i]);
		}

		pPartCadData->GetPatternList()->SetModified(TRUE);
		pPartCadData->GetPatternLoopList()->SetModified(TRUE);
		pPartCadData->GetTextStructs()->SetModified(TRUE);
		pPartCadData->SetModified(TRUE);
	}

	// ��������ļӹ�������
	{
		// ɾ���漰���ļ���������Ӧ���������ա�
		for (unsigned int i = 0; i < pPatternLoops_affected->size(); i++)
		{
			const IPatternLoop* pPatternLoop = pPatternLoops_affected->at(i).get();
			pPartCamData->GetLoopFeatureList()->DeleteLoopCutFeatureByLoopID(pPatternLoop->GetID());
		}

		// Ϊ�´����ļ�����������ӹ�������
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
		// ���������cad���ݡ�
		pAllPatterns->push_back(pNewPattern);
		PatternLoopNodePtr pPatternLoopNode(new PatternLoopNode(pNewPattern, TRUE)); // here, the second param is not important.
		StdPatternLoopPtr pStdPatternLoop(new StdPatternLoop(pPatternLoopNode));
		LoopProcessor::UpdateCache(pStdPatternLoop);
		pAllPatternLoops->push_back(pStdPatternLoop);
		LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get());
		pPartCadData->SetLoopTopologyItems(pLoopTopologyItems); // ������������м������������˹�ϵ��

		// ���������cam���ݡ�
		LoopCutFeaturePtr pLoopFeature = LoopCutFeatureManager::AssignLoopFeature(pStdPatternLoop, pLoopTopologyItems.get(), pParamConfig.get(), TRUE, TRUE, Matrix2D());
		pPartCamData->GetLoopFeatureList()->push_back(pLoopFeature);
	}
	else if (pNewPattern->IsArrayPattern())
	{
		LogMgr::DebugWarn(_T("498366"));
		return;
	}
	else // pNewPattern��ֱ�߻���Բ����
	{
		PatternLoopListPtr pOpenPatternLoops = pAllPatternLoops->GetOpenLoopLinkToPat(pNewPattern.get(), GEOM_TOLERANCE); // �������ܺ�pNewPattern���ӵĿ��Ż�·��
		if (pOpenPatternLoops->size() == 1) // ֻ��һ������������pNewPattern�����ӣ���ô�Ͱ�pNewPattern�������������
		{
			// ���������cad���ݡ�
			pAllPatterns->push_back(pNewPattern);
			IPatternLoopPtr pTmpLoop = pOpenPatternLoops->front();
			PolygonPatternLoopPtr pPolygonPatternLoop = boost::dynamic_pointer_cast<PolygonPatternLoop>(pTmpLoop);
			pPolygonPatternLoop->AddPattern(pNewPattern, GEOM_TOLERANCE); // PolygonPatternLoop class will decide where this pattern will be added.
			LoopProcessor::UpdateCache(pPolygonPatternLoop);
			LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get());
			pPartCadData->SetLoopTopologyItems(pLoopTopologyItems); // ������������м������������˹�ϵ��

			// ���������cam���ݡ�
			pPartCamData->GetLoopFeatureList()->DeleteLoopCutFeatureByLoopID(pPolygonPatternLoop->GetID());
			LoopCutFeaturePtr pLoopFeature = LoopCutFeatureManager::AssignLoopFeature(pPolygonPatternLoop, pLoopTopologyItems.get(), pParamConfig.get(), TRUE, TRUE, Matrix2D());
			pPartCamData->GetLoopFeatureList()->push_back(pLoopFeature);
		}
		else // �ж������������pNewPattern�����ӡ�
		{
			// ���������cad���ݡ�
			PatternLoopListPtr pNewPatternLoops(new PatternLoopList); // �½��ļ���������
			{
				pAllPatterns->push_back(pNewPattern);

				// �Ѽ�����Щ�������������м�����������pNewPatternҲ�ӽ�ȥ��
				PatternListPtr pPatList(new PatternList);
				for (unsigned int i = 0; i < pOpenPatternLoops->size(); i++)
				{
					const IPatternLoop* pPatternLoop = pOpenPatternLoops->operator[](i).get();
					PatternListPtr pTmpPatList = pPatternLoop->GetPatternList();
					pPatList->insert(pPatList->end(), pTmpPatList->begin(), pTmpPatList->end());
				}
				pPatList->push_back(pNewPattern);

				// ɾ����Щ����������
				for (unsigned int i = 0; i < pOpenPatternLoops->size(); i++)
				{
					const IPatternLoop* pPatternLoop = pOpenPatternLoops->operator[](i).get();
					pAllPatternLoops->DeleteLoop(pPatternLoop);
				}

				// ��������������
				if (pPatList->size() > 0)
				{
					pNewPatternLoops = LoopBuilder::BuildPolygonPatternLoops(pPatList, GEOM_TOLERANCE);
					LoopProcessor::UpdateCache(pNewPatternLoops);
					pAllPatternLoops->insert(pAllPatternLoops->end(), pNewPatternLoops->begin(), pNewPatternLoops->end());
				}

				// ������������м������������˹�ϵ��
				LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get());
				pPartCadData->SetLoopTopologyItems(pLoopTopologyItems);
			}

			// ���������cam���ݡ�
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

	// �����޸ı�ǡ�
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

	// �������������ͼ���������
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

			// ��������������
			LinePatternPtr pLinePattern(new LinePattern(startPt, endPt));
			pLinePattern->Transform(transformMatrix);
			pLinePattern->UpdateCache();
			pAllPatterns->push_back(pLinePattern);
			pNewPatterns->push_back(pLinePattern);

			// �������������ڵ㡣
			PatternLoopNodePtr pPatternLoopNode(new PatternLoopNode(pLinePattern, TRUE));
			pPolygonPatternLoop->GetPatternLoopNodes()->push_back(pPatternLoopNode);
		}
		LoopProcessor::UpdateCache(pPolygonPatternLoop);
		pAllPatternLoops->push_back(pPolygonPatternLoop);
		pNewPatternLoops->push_back(pPolygonPatternLoop);
	}
	pPartCadData->SetLoopTopologyItems(LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get()));

	// �������ֶ���
	TextStructPtr pTextStruct(new TextStruct(pTextParam, transformMatrix, pNewPatterns, pNewPatternLoops));
	pPartCadData->GetTextStructs()->push_back(pTextStruct);

	// ���������cam���ݡ�
	for (unsigned int i = 0; i < pNewPatternLoops->size(); i++)
	{
		IPatternLoopPtr pNewPatternLoop = pNewPatternLoops->at(i);
		LoopCutFeaturePtr pLoopFeature = LoopCutFeatureManager::AssignLoopFeature(pNewPatternLoop, pPartCadData->GetLoopTopologyItems(), pParamConfig.get(), TRUE, TRUE, Matrix2D());
		pPartCamData->GetLoopFeatureList()->push_back(pLoopFeature);
	}

	// �����޸ı�ǡ�
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

	// ��ɾ��ԭ���ֶ����Ӧ�ļ��������ͼ����������Լ��ӹ����ա�
	{
		const PatternList* pInvolvedPatterns = pTextStruct->GetInvolvedPatterns().get();
		const PatternLoopList* pInvolvedPatternLoops = pTextStruct->GetInvolvedPatternLoops().get();
		pAllPatterns->RemovePatterns(pInvolvedPatterns);
		pAllPatternLoops->DeleteLoopList(pInvolvedPatternLoops);

		// ɾ���漰���ļ���������Ӧ���������ա�
		for (unsigned int i = 0; i < pInvolvedPatternLoops->size(); i++)
		{
			const IPatternLoop* pPatternLoop = pInvolvedPatternLoops->at(i).get();
			pPartCamData->GetLoopFeatureList()->DeleteLoopCutFeatureByLoopID(pPatternLoop->GetID());
		}
	}

	// �������������ͼ���������
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

			// ��������������
			LinePatternPtr pLinePattern(new LinePattern(startPt, endPt));
			pLinePattern->Transform(pTextStruct->GetTransformMatrix());
			pLinePattern->UpdateCache();
			pAllPatterns->push_back(pLinePattern);
			pNewPatterns->push_back(pLinePattern);

			// �������������ڵ㡣
			PatternLoopNodePtr pPatternLoopNode(new PatternLoopNode(pLinePattern, TRUE));
			pPolygonPatternLoop->GetPatternLoopNodes()->push_back(pPatternLoopNode);
		}
		LoopProcessor::UpdateCache(pPolygonPatternLoop);
		pAllPatternLoops->push_back(pPolygonPatternLoop);
		pNewPatternLoops->push_back(pPolygonPatternLoop);
	}
	pPartCadData->SetLoopTopologyItems(LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get()));

	// �޸����ֶ���
	{
		pTextStruct->SetTextParam(pNewTextParam);
		pTextStruct->SetInvolvedPatterns(pNewPatterns);
		pTextStruct->SetInvolvedPatternLoops(pNewPatternLoops);
	}

	// ���������cam���ݡ�
	for (unsigned int i = 0; i < pNewPatternLoops->size(); i++)
	{
		IPatternLoopPtr pNewPatternLoop = pNewPatternLoops->at(i);
		LoopCutFeaturePtr pLoopFeature = LoopCutFeatureManager::AssignLoopFeature(pNewPatternLoop, pPartCadData->GetLoopTopologyItems(), pParamConfig.get(), TRUE, TRUE, Matrix2D());
		pPartCamData->GetLoopFeatureList()->push_back(pLoopFeature);
	}

	// �����޸ı�ǡ�
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

	// ע����������У�����������������꣬��������������������������
	for (int iRowIndex = 0; iRowIndex < iRowNum; iRowIndex++)
	{
		for (int iColumnIndex = 0; iColumnIndex < iColumnNum; iColumnIndex++)
		{
			if (iRowIndex == 0 && iColumnIndex == 0)
				continue;
			else
			{
				// �������ʵ���ı任����
				Matrix2D transformMatrix;
				if (emGeometryGridDir == GRID_RIGHT_TOP)
					transformMatrix.SetTransfer(Vector2D((outerRect.GetWidth() + dColumnSpacing) * iColumnIndex, (outerRect.GetHeight() + dRowSpacing) * iRowIndex));
				else if (emGeometryGridDir == GRID_RIGHT_BOTTOM)
					transformMatrix.SetTransfer(Vector2D((outerRect.GetWidth() + dColumnSpacing) * iColumnIndex, -(outerRect.GetHeight() + dRowSpacing) * iRowIndex));
				else if (emGeometryGridDir == GRID_LEFT_TOP)
					transformMatrix.SetTransfer(Vector2D(-(outerRect.GetWidth() + dColumnSpacing) * iColumnIndex, (outerRect.GetHeight() + dRowSpacing) * iRowIndex));
				else if (emGeometryGridDir == GRID_LEFT_BOTTOM)
					transformMatrix.SetTransfer(Vector2D(-(outerRect.GetWidth() + dColumnSpacing) * iColumnIndex, -(outerRect.GetHeight() + dRowSpacing) * iRowIndex));

				// �������ʵ����
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
	if (pPatternLoops->size() < 2) { // ѡ�е���������С��2��������ʾ�û�����ѡ��
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SelectMultiLoop2Align);
		MessageBox(pView->m_hWnd, str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
		return false;
	}

	// Ҫ����ļ���������
	vector<LONGLONG> patternLoops_willAlign;
	for (unsigned int i = 0; i < pPatternLoops->size(); i++) {
		patternLoops_willAlign.push_back(pPatternLoops->operator[](i)->GetID());
	}

	// Ҫ��������֡�
	vector<LONGLONG> textStructs_willAlign;
	for (unsigned int i = 0; i < pTextStructs->size(); i++) {
		textStructs_willAlign.push_back(pTextStructs->operator[](i)->GetID());
	}

	// ִ�����
	PartCamDataPtr pPartCamData = pPartPlacement->GetCamData();
	ICommandPtr pCommand(new CanvasLoopAlignCommand(EditorData(pView, strEditorName), pPartCadData, pPartCamData, patternLoops_willAlign, textStructs_willAlign, emAlignLoopStyle));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	return true;
}

END_CUTLEADER_NAMESPACE()

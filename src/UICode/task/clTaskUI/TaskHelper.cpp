#include "StdAfx.h"
#include "TaskHelper.h"

#include "DataBaseManager.h"
#include "LogMgr.h"
#include "Polygon2D.h"
#include "Polygon2DList.h"
#include "Text2ContourHelper.h"
#include "DataCenterItemLoader.h"
#include "DateHelper.h"
#include "FolderNode.h"
#include "clBaseDataResource.h"

#include "ParamConfigLoader.h"
#include "DefaultParamConfig.h"
#include "CLOptionInfo.h"
#include "ImpExpConfig.h"
#include "PartEditParam.h"

#include "CreatePartFromGeometryDlg.h"
#include "PartManager.h"
#include "TaskPartManagementBar.h"
#include "TaskPart.h"
#include "TaskItem.h"
#include "TaskPartList.h"
#include "LoopBuilder.h"
#include "PartBuilder.h"
#include "LoopProcessor.h"
#include "Part.h"
#include "PartCadData.h"
#include "PartCamDataList.h"
#include "SheetList.h"
#include "Sheet.h"
#include "clCanvasGeometryUIResource.h"
#include "PartPlacementList.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "PatternList.h"
#include "PatternLoopList.h"
#include "PolygonPatternLoop.h"
#include "LinePattern.h"
#include "PatternLoopNode.h"
#include "PatternLoopNodeList.h"
#include "TaskItemManager.h"
#include "SheetManager.h"
#include "SheetProcessor.h"
#include "MaterialStockWarningDlg.h"
#include "clDataManagerResource.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

void TaskHelper::CreatePartAndAdd2Task(const PatternList* pPatterns, const TextStructList* pTextStructs, TaskItemPtr pTaskItem)
{
	const CString& strDefaultFontFileName = CLOptionInfo::GetInstance()->GetPartEditParam()->GetDefaultFontFileName();

	CreatePartFromGeometryDlg dlg(pPatterns);
	if (dlg.DoModal() == IDOK)
	{
		// 创建零件。
		PartPtr pPart;
		{
			pPart.reset(new Part);
			pPart->SetName(dlg.GetPartName());

			// 创建CAD数据。
			PartCadDataPtr pPartCadData;
			{
				// 得到不在文字中的几何特征。
				PatternListPtr pPatterns_notInText(new PatternList);
				for (unsigned int m = 0; m < pPatterns->size(); m++)
				{
					const IPattern* pTmpPattern = pPatterns->operator[](m).get();
					bool bThisPatternInText = false;
					for (unsigned int i = 0; i < pTextStructs->size(); i++)
					{
						const TextStruct* pTextStruct = pTextStructs->operator[](i).get();
						if (pTextStruct->GetInvolvedPatterns()->GetPatternFromID(pTmpPattern->GetID()))
						{
							bThisPatternInText = true;
							break;
						}
					}
					if (!bThisPatternInText)
						pPatterns_notInText->push_back(pPatterns->operator[](m));
				}

				// 先克隆不在文字中的几何特征。
				PatternListPtr pPatterns_notInText_new = boost::dynamic_pointer_cast<PatternList>(pPatterns_notInText);
				pPatterns_notInText_new->UpdateCache();
				PatternLoopListPtr pPatternLoops_notInText_new = LoopBuilder::BuildPatternLoops(pPatterns_notInText_new.get());
				LoopProcessor::UpdateCache(pPatternLoops_notInText_new);

				// 克隆文字对象，并生成对应的几何特征和几何轮廓。
				TextStructListPtr pTextStructs_new(new TextStructList);
				PatternListPtr pPatterns_inText_new(new PatternList);
				PatternLoopListPtr pPatternLoops_inText_new(new PatternLoopList);
				for (unsigned int m = 0; m < pTextStructs->size(); m++)
				{
					const TextStruct* pTextStruct = pTextStructs->operator[](m).get();

					// 生成新的文字对象。
					TextStructPtr pNewTextStruct = boost::dynamic_pointer_cast<TextStruct>(pTextStruct->Clone());
					pTextStructs_new->push_back(pNewTextStruct);

					// 把文字对象转化为轮廓。
					const TextParam* pTextParam = pNewTextStruct->GetTextParam().get();
					const CString& strTextContent = pTextParam->GetTextContent();
					const CString& strFontFileName = pTextParam->GetFontFileName();
					double dTextHeight = pTextParam->GetTextHeight();
					double dTextWidth = pTextParam->GetTextWidth();
					double dTextTiltAngle = pTextParam->GetTextTiltAngle();
					Polygon2DListPtr pTextContours(new Polygon2DList);
					TextContourGenerationErrorCode emErrorCode = Text2ContourHelper::GenerateTextContours(strTextContent, strFontFileName, dTextHeight, dTextWidth, dTextTiltAngle,
						strDefaultFontFileName, pTextContours);
					if (emErrorCode != Succeed)
						LogMgr::DebugWarn(_T("979528"));

					// 为文字对象生成几何特征和几何轮廓。
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
							pLinePattern->Transform(pNewTextStruct->GetTransformMatrix());
							pLinePattern->UpdateCache();
							pPatterns_inText_new->push_back(pLinePattern);
							pNewPatterns->push_back(pLinePattern);

							// 创建几何轮廓节点。
							PatternLoopNodePtr pPatternLoopNode(new PatternLoopNode(pLinePattern, TRUE));
							pPolygonPatternLoop->GetPatternLoopNodes()->push_back(pPatternLoopNode);
						}
						LoopProcessor::UpdateCache(pPolygonPatternLoop);
						pPatternLoops_inText_new->push_back(pPolygonPatternLoop);
						pNewPatternLoops->push_back(pPolygonPatternLoop);
					}
					pNewTextStruct->SetInvolvedPatterns(pNewPatterns);
					pNewTextStruct->SetInvolvedPatternLoops(pNewPatternLoops);
				}

				// 更新轮廓拓扑关系。
				PatternListPtr pAllPatterns(new PatternList);
				pAllPatterns->insert(pAllPatterns->end(), pPatterns_notInText_new->begin(), pPatterns_notInText_new->end());
				pAllPatterns->insert(pAllPatterns->end(), pPatterns_inText_new->begin(), pPatterns_inText_new->end());
				PatternLoopListPtr pAllPatternLoops(new PatternLoopList);
				pAllPatternLoops->insert(pAllPatternLoops->end(), pPatternLoops_notInText_new->begin(), pPatternLoops_notInText_new->end());
				pAllPatternLoops->insert(pAllPatternLoops->end(), pPatternLoops_inText_new->begin(), pPatternLoops_inText_new->end());
				LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(pAllPatternLoops.get());

				// 创建cad对象。
				pPartCadData.reset(new PartCadData(pAllPatterns, pAllPatternLoops, pLoopTopologyItems));
				pPartCadData->SetTextStructs(pTextStructs_new);
				pPart->SetCadData(pPartCadData);
			}

			// 创建CAM数据。
			DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
			DefaultParamConfigPtr pDefaultParamConfig = ParamConfigLoader::LoadDflParamConfig(pExpLibDBCon);
			UINT iAssignLimit = CLOptionInfo::GetInstance()->GetImpExpConfig()->GetAssignLimit();
			PartCamDataPtr pPartCamData = PartBuilder::BuildPartCamData(pPartCadData, pDefaultParamConfig->GetParamConfigID(), iAssignLimit, FALSE);
			PartCamDataListPtr pPartCamDataList(new PartCamDataList);
			pPartCamDataList->push_back(pPartCamData);
			pPart->SetCamDataList(pPartCamDataList);
		}

		// 把零件保存到数据库。
		FolderNodePtr pFolderNode = dlg.GetSaveFolder();
		pPart->SetCreateTime(DateHelper::GetCurrentTime());
		pPart->SetModifyTime(DateHelper::GetCurrentTime());
		PartManager::SaveNewPart(pPart, pFolderNode);

		// 把零件添加到任务中。
		int iPartCount = dlg.GetPartCount();
		TaskPartPtr pTaskPart(new TaskPart(pPart, NestPriority::MaxPriority(), PartRotate_Free, iPartCount));
		pTaskItem->GetTaskPartList()->push_back(pTaskPart);

		// 更新任务零件列表。
		TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pTaskItem);
	}
}

void TaskHelper::CheckTaskItem(const TaskItem* pTaskItem)
{
	const SheetList* pSheetList = pTaskItem->GetSheetList().get();

	if (pSheetList->size() < 2) // 任务中板材数量不能小于2张，至少有一张底图板材和一张常规板材。
	{
		LogMgr::DebugWarn(_T("546278"));
		return;
	}

	// 检查底图板材。
	const Sheet* pCanvasSheet = pSheetList->front().get();
	if (!pCanvasSheet->GetIsCanvas()) // 第一个板材必须是底图。
	{
		LogMgr::DebugWarn(_T("253749"));
		return;
	}
	//if (pCanvasSheet->GetName() != MultiLanguageMgr::GetInstance()->TranslateString(IDS_Canvas)) // 底图板材的名称不能被修改，不过在多语言情况下不太好检查。
	//{
	//	LogMgr::DebugWarn(_T("465783"));
	//	return;
	//}
	if (pCanvasSheet->GetSheetCount() != 1) // 底图板材的数量只能是一张。
	{
		LogMgr::DebugWarn(_T("669832"));
		return;
	}
	if (pCanvasSheet->GetPartPlacements()->size() != 1) // 底图上只能有一个零件放置。
	{
		LogMgr::DebugWarn(_T("347803"));
		return;
	}
}

TaskItemPtr TaskHelper::LoadTaskItem(long long iTaskID)
{
	TaskItemPtr pTaskItem = TaskItemManager::LoadTaskItemFromDb(iTaskID);

	// 检查板材中的轮廓加工工艺。
	SheetListPtr pSheetList = pTaskItem->GetSheetList();
	for (unsigned int i = 0; i < pSheetList->size(); i++)
	{
		SheetPtr pSheet = pSheetList->operator [](i);
		SheetProcessor::CheckLoopCutFeature(pSheet);
		if (pSheet->IsModified())
			SheetManager::SaveExistSheet2SheetDbFile(pSheet);
	}

	return pTaskItem;
}

END_CUTLEADER_NAMESPACE()

#include "StdAfx.h"
#include "CanvasPatternGripEditCommand.h"

#include "DataBaseManager.h"

#include "ParamConfigLoader.h"

#include "PatternList.h"
#include "IPattern.h"
#include "PatternLoopList.h"
#include "clGeometryFeatureResource.h"
#include "LoopProcessor.h"
#include "PartCadData.h"
#include "LoopCutFeatureManager.h"
#include "PartCamData.h"
#include "LoopStartCutProcessor.h"
#include "LoopCutFeatureList.h"
#include "LoopCutFeature.h"
#include "PartBuilder.h"

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternGripEditCommand::CanvasPatternGripEditCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	LONGLONG iEditingPatternID, IPatternPtr pTmpPattern)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_GRIPEDIT_PAT), editorData, pPartCadData, pPartCamData)
{
	m_iEditingPatternID = iEditingPatternID;
	m_pTmpPattern = pTmpPattern;
}

CanvasPatternGripEditCommand::~CanvasPatternGripEditCommand(void)
{
}

void CanvasPatternGripEditCommand::Do()
{
	CloneCadAndCamData();

	// 修改几何特征。
	PatternListPtr pPatternList = m_pPartCadData->GetPatternList();
	IPatternPtr pEditingPattern = pPatternList->GetPatternFromID(m_iEditingPatternID);
	pEditingPattern->UpdateFrom(m_pTmpPattern);
	pEditingPattern->UpdateCache();
	pEditingPattern->SetModified(TRUE);

	// 更新几何轮廓。
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	PatternListPtr pPatList(new PatternList);
	pPatList->push_back(pEditingPattern);
	LoopProcessor::ChangePatInLoopList(pPatternLoopList, m_pPartCadData->GetLoopTopologyItems_FW(), pPatList);
	m_pPartCadData->GetPatternLoopList()->SetModified(TRUE);

	// 更新加工工艺。此处记得更新工艺中的缓存，比如修改圆的半径后，加工工艺都没有改变，不过缓存中数据需要更新。
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(DataBaseManager::GetInstance()->GetExpLibDBConnect(), m_pPartCamData->GetParamConfigID());
	LoopCutFeatureListPtr pLoopCutFeatures = m_pPartCamData->GetLoopFeatureList();
	LoopCutFeatureManager::CheckIntegrity(pPatternLoopList.get(), m_pPartCadData->GetLoopTopologyItems(), pLoopCutFeatures, pParamConfig.get(), TRUE, TRUE, TRUE, Matrix2D());
	for (unsigned int i = 0; i < pLoopCutFeatures->size(); i++)
		LoopStartCutProcessor::UpdateCache(pLoopCutFeatures->operator[](i)->GetLoopStartCutData());
}

void CanvasPatternGripEditCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()

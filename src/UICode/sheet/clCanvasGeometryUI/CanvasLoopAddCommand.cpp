#include "StdAfx.h"
#include "CanvasLoopAddCommand.h"

#include "DataBaseManager.h"

#include "ParamConfigLoader.h"

#include "PatternList.h"
#include "PatternLoopList.h"
#include "LoopProcessor.h"
#include "PartBuilder.h"
#include "PartCadData.h"
#include "PartCamData.h"
#include "LoopCutFeatureManager.h"
#include "LoopCutFeatureList.h"
#include "LoopStartCutProcessor.h"
#include "LoopCutFeature.h"
#include "clCanvasGeometryUIResource.h"
#include "TextStructList.h"

BEGIN_CUTLEADER_NAMESPACE()

CanvasLoopAddCommand::CanvasLoopAddCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	PatternListPtr pNewPatterns, PatternLoopListPtr pNewPatternLoops, TextStructListPtr pNewTextStructs)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_AddLoop2Canvas), editorData, pPartCadData, pPartCamData)
{
	m_pNewPatterns = pNewPatterns;
	m_pNewPatternLoops = pNewPatternLoops;
	m_pNewTextStructs = pNewTextStructs;
}

CanvasLoopAddCommand::~CanvasLoopAddCommand(void)
{
}

void CanvasLoopAddCommand::Do()
{
	CloneCadAndCamData();

	// 修改几何特征。
	m_pPartCadData->GetPatternList()->AddPatterns(m_pNewPatterns);
	m_pPartCadData->GetPatternList()->SetModified(TRUE);
	m_pPartCadData->GetPatternLoopList()->insert(m_pPartCadData->GetPatternLoopList()->end(), m_pNewPatternLoops->begin(), m_pNewPatternLoops->end());
	m_pPartCadData->GetPatternLoopList()->SetModified(TRUE);
	LoopTopologyItemListPtr pLoopTopologyItems = LoopProcessor::CalcLoopTopStruct(m_pPartCadData->GetPatternLoopList().get());
	m_pPartCadData->SetLoopTopologyItems(pLoopTopologyItems);

	// 添加文字对象。
	m_pPartCadData->GetTextStructs()->insert(m_pPartCadData->GetTextStructs()->end(), m_pNewTextStructs->begin(), m_pNewTextStructs->end());
	m_pPartCadData->GetTextStructs()->SetModified(TRUE);

	// 修改加工工艺。
	DBConnectPtr pExpLibDBCon = DataBaseManager::GetInstance()->GetExpLibDBConnect();
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(pExpLibDBCon, m_pPartCamData->GetParamConfigID());
	LoopCutFeatureManager::CheckIntegrity(m_pPartCadData->GetPatternLoopList().get(), m_pPartCadData->GetLoopTopologyItems(), m_pPartCamData->GetLoopFeatureList(),
		pParamConfig.get(), TRUE, TRUE, TRUE, Matrix2D());
}

void CanvasLoopAddCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()

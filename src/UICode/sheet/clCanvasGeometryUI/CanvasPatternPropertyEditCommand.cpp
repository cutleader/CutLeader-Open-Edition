#include "StdAfx.h"
#include "CanvasPatternPropertyEditCommand.h"

#include "DataPropItem.h"
#include "DataBaseManager.h"

#include "ParamConfigLoader.h"

#include "IPattern.h"
#include "PatternList.h"
#include "PatternLoopList.h"
#include "clGeometryFeatureResource.h"
#include "PatternProcessor.h"
#include "LoopProcessor.h"
#include "PartBuilder.h"
#include "PartCadData.h"
#include "LoopCutFeatureManager.h"
#include "PartCamData.h"
#include "LoopStartCutProcessor.h"
#include "LoopCutFeatureList.h"
#include "LoopCutFeature.h"

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternPropertyEditCommand::CanvasPatternPropertyEditCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	LONGLONG iEditingPatternID, const DataPropItem& newPropItem)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_EDITPROP_PAT), editorData, pPartCadData, pPartCamData)
{
	m_iEditingPatternID = iEditingPatternID;
	m_newPropItem = newPropItem;
}

CanvasPatternPropertyEditCommand::~CanvasPatternPropertyEditCommand(void)
{
}

void CanvasPatternPropertyEditCommand::Do()
{
	CloneCadAndCamData();

	// �޸ļ���������
	PatternListPtr pPatternList = m_pPartCadData->GetPatternList();
	IPatternPtr pEditingPattern = pPatternList->GetPatternFromID(m_iEditingPatternID);
	pEditingPattern->UpdateProperty(m_newPropItem);
	pEditingPattern->UpdateCache();
	pEditingPattern->SetModified(TRUE);

	// ���¼���������
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	PatternListPtr pPatList(new PatternList);
	pPatList->push_back(pEditingPattern);
	LoopProcessor::ChangePatInLoopList(pPatternLoopList, m_pPartCadData->GetLoopTopologyItems_FW(), pPatList);
	m_pPartCadData->GetPatternLoopList()->SetModified(TRUE);

	// ���¼ӹ����ա��˴��ǵø��¹����еĻ��棬�����޸�Բ�İ뾶�󣬼ӹ����ն�û�иı䣬����������������Ҫ���¡�
	ParamConfigPtr pParamConfig = ParamConfigLoader::LoadParamConfig(DataBaseManager::GetInstance()->GetExpLibDBConnect(), m_pPartCamData->GetParamConfigID());
	LoopCutFeatureListPtr pLoopCutFeatures = m_pPartCamData->GetLoopFeatureList();
	LoopCutFeatureManager::CheckIntegrity(pPatternLoopList.get(), m_pPartCadData->GetLoopTopologyItems(), pLoopCutFeatures, pParamConfig.get(), TRUE, TRUE, TRUE, Matrix2D());
	for (unsigned int i = 0; i < pLoopCutFeatures->size(); i++)
		LoopStartCutProcessor::UpdateCache(pLoopCutFeatures->operator[](i)->GetLoopStartCutData());
}

void CanvasPatternPropertyEditCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()

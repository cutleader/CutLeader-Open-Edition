#include "StdAfx.h"
#include "CanvasPatternEditCommandBase.h"

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

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternEditCommandBase::CanvasPatternEditCommandBase(const CString& strCommandName, const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData)
	: CommandBase(strCommandName, editorData)
{
	m_pPartCadData = pPartCadData;
	m_pPartCamData = pPartCamData;
}

CanvasPatternEditCommandBase::~CanvasPatternEditCommandBase(void)
{
}

void CanvasPatternEditCommandBase::CloneCadAndCamData()
{
	m_pPartCadData_backup = PartBuilder::CloneCadData(m_pPartCadData.get(), true);
	m_pPartCamData_backup = PartBuilder::CloneCamData(m_pPartCamData.get(), m_pPartCadData.get(), m_pPartCadData_backup.get(), true);
}

void CanvasPatternEditCommandBase::RestoreCadAndCamData()
{
	m_pPartCadData->SetPatternList(m_pPartCadData_backup->GetPatternList());
	m_pPartCadData->SetPatternLoopList(m_pPartCadData_backup->GetPatternLoopList());
	m_pPartCadData->SetLoopTopologyItems(m_pPartCadData_backup->GetLoopTopologyItems_FW());
	m_pPartCadData->SetTextStructs(m_pPartCadData_backup->GetTextStructs());
	m_pPartCadData->SetModified(TRUE);

	m_pPartCamData->SetLoopFeatureList(m_pPartCamData_backup->GetLoopFeatureList());
	m_pPartCamData->SetModified(TRUE);
}

END_CUTLEADER_NAMESPACE()

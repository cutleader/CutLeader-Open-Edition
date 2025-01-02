#include "StdAfx.h"
#include "SequenceHelper.h"

#include "LogMgr.h"
#include "MultiLanguageMgr.h"
#include "CommandManager.h"
#include "clBaseDataResource.h"

#include "IMaterialSize.h"
#include "SequenceConfigItem.h"

#include "FlyCutSequenceManager.h"
#include "FlyCutSequenceAddCommand.h"
#include "LoopInstanceList.h"
#include "ConfigData.h"
#include "PartInstance.h"
#include "PartInstanceList.h"
#include "LoopInstance.h"
#include "clCutSequenceResource.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PatternLoopList.h"
#include "Part.h"
#include "LoopCutFeature.h"
#include "LoopCutFeatureList.h"
#include "PartCadData.h"
#include "PartCamData.h"
#include "LoopToolData.h"
#include "CutSequenceList.h"
#include "ICutSequence.h"

BEGIN_CUTLEADER_NAMESPACE()

bool SequenceHelper::CreateFlyCutSequence(const CWnd* pView, const CString& strEditorName, LoopInstanceListPtr pSelectedLoopInsts,
	PartInstanceListPtr pPartInstances, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (pSelectedLoopInsts->size() == 0) {
		LogMgr::DebugWarn(_T("330095"));
		return false;
	}

	if (FlyCutSequenceManager::CheckLoopInsts(pConfigData.get(), pView, pSelectedLoopInsts.get())) // 可以创建飞行切割工序。
	{
		// 是否需要应用到其他相同零件。
		bool bApplyTo_other_sameParts = false;
		if (pConfigData->GetMatSize()->GetType() != MAT_SIZE_Canvas)
		{
			PartInstancePtr pPartInst_current = pSelectedLoopInsts->at(0)->GetPartInstance();
			CString strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_FLYCUT_APPLY_SAMEPARTS_TIP);
			if (pPartInstances->GetPartInstances_brother(pPartInst_current)->size() > 0 &&
				MessageBox(pView->m_hWnd, strMsg, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES) {
				bApplyTo_other_sameParts = true;
			}
		}

		// 把之前的工序替换为飞切。
		EditorData editorData(pView, strEditorName);
		ICommandPtr pCommand(new FlyCutSequenceAddCommand(editorData, pCutSequences, pConfigData, pPartInstances, pSelectedLoopInsts, bApplyTo_other_sameParts));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		return true;
	}

	return false;
}

CutSequenceListPtr SequenceHelper::GetCutPartSequences(const CutSequenceList* pAllCutSequences)
{
	CutSequenceListPtr pCutPartSequences(new CutSequenceList);

	for (unsigned int i = 0; i < pAllCutSequences->size(); i++) {
		ICutSequencePtr pCutSequence = pAllCutSequences->operator [](i);
		if (pCutSequence->IsCutPart() && pCutSequence->SequenceWillBeCut()) {
			pCutPartSequences->push_back(pCutSequence);
		}
	}

	return pCutPartSequences;
}

END_CUTLEADER_NAMESPACE()

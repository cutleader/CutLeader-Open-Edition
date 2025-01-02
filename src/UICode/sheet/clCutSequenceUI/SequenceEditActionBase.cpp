#include "StdAfx.h"
#include "SequenceEditActionBase.h"

#include "GlViewPort.h"
#include "ValueInputManager.h"

#include "SequenceConfigItem.h"
#include "IMaterialSize.h"

#include "CutNodeManager.h"
#include "clCutSequenceResource.h"
#include "ConfigData.h"
#include "PartPlacementEditAction.h"
#include "CanvasGeometryEditAction.h"
#include "CutSequenceManager.h"
#include "PartLayoutData.h"
#include "SequenceData.h"

BEGIN_CUTLEADER_NAMESPACE()

SequenceEditActionBase::SequenceEditActionBase(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData,
	ConfigDataPtr pConfigData, GlViewPortPtr pViewPort)
{
	m_pPartLayoutData = pPartLayoutData;
	m_pSequenceData = pSequenceData;
	m_pConfigData = pConfigData;
	m_pViewPort = pViewPort;

	// 更新切割节点缓存。
	RefreshCutNodes();
}

SequenceEditActionBase::~SequenceEditActionBase(void)
{
}

EditorData SequenceEditActionBase::GetEditorData() const
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ);
	EditorData editorData(CWnd::FromHandle(m_pViewPort->GetWnd()), str);
	return editorData;
}

BOOL SequenceEditActionBase::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
    {
		if (m_pConfigData->GetMatSize()->GetType() == MAT_SIZE_Canvas)
			CanvasGeometryEditAction::PostMessage2EnterCanvasLoopSelectAction();
		else
			PartPlacementEditAction::PostMessage2EnterPartPlacementSelectAction();
    }

    return ActionBase::OnKeyDown(nChar, nRepCnt, nFlags);
}

void SequenceEditActionBase::RefreshCutNodes()
{
	const IMaterialSize* pMaterialSize = m_pConfigData->GetMatSize();
	const SequenceConfigItem* pSeqConfigItem = m_pConfigData->GetSeqConfigItem();
	bool bCannotFindRefLoopsOnCanvas = false;
	Point2D referencePt = CutSequenceManager::CalcReferencePt(m_pPartLayoutData->GetPartPlacements().get(), pMaterialSize, m_pSequenceData->GetRefPtData().get(), bCannotFindRefLoopsOnCanvas);
	m_pCutNodeList = CutNodeManager::CutSequences2CutNodes(referencePt, m_pSequenceData->GetCutSequences().get());
}

END_CUTLEADER_NAMESPACE()

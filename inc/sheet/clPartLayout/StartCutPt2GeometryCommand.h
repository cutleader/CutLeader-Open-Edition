#pragma once

#include "clPartLayoutCommon.h"
#include "PatternPosition.h"
#include "CommandBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(ILoopStartCutPt)
DECLARE_CUTLEADER_CLASS(ILead)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)

BEGIN_CUTLEADER_NAMESPACE()

// �����������е��Ƶ�ͼ���ϡ�
// ע��
//  1) �����е���ͼ���ϵĺ��ڽ��ϵķֿ�����
class ClPartLayout_Export StartCutPt2GeometryCommand : public CommandBase
{
public:
	StartCutPt2GeometryCommand(EditorData editorData, LoopStartCutDataListPtr pGeomStaEndCuts, LONGLONG iNewPatID, PatternPosition newPatPos,
					LoopStartCutDataListPtr pCornerStaEndCuts, ILoopStartCutPtPtr pNewSECutPt, ILeadPtr pNewLeadIn, ILeadPtr pNewLeadOut);
	~StartCutPt2GeometryCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	/************************************************************************/
	// �����е���ͼ���ϵġ�

	LoopStartCutDataListPtr m_pGeomStaEndCuts;

	// �µ����е�λ�á�
	LONGLONG m_iNewPatID;
	PatternPosition m_newPatPos;

	// ����֮ǰ�����е�λ�á�
	IntegerListPtr m_pOldPatIDs;
	std::vector<PatternPosition> m_oldPatPosList;
	/************************************************************************/

	/************************************************************************/
	// �����е��ڽ��ϵġ�

	LoopStartCutDataListPtr m_pCornerStaEndCuts;

	// �����ݡ�
	std::vector<ILoopStartCutPtPtr> m_newSECutPts;
	std::vector<ILeadPtr> m_newLeadIns;
	std::vector<ILeadPtr> m_newLeadOuts;

	// �����ݡ�
	std::vector<ILoopStartCutPtPtr> m_oldSECutPts;
	std::vector<ILeadPtr> m_oldLeadIns;
	std::vector<ILeadPtr> m_oldLeadOuts;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()

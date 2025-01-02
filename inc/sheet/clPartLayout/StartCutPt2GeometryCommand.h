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

// 这个命令把起切点移到图形上。
// 注：
//  1) 老起切点在图形上的和在角上的分开处理。
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
	// 老起切点在图形上的。

	LoopStartCutDataListPtr m_pGeomStaEndCuts;

	// 新的起切点位置。
	LONGLONG m_iNewPatID;
	PatternPosition m_newPatPos;

	// 备份之前的起切点位置。
	IntegerListPtr m_pOldPatIDs;
	std::vector<PatternPosition> m_oldPatPosList;
	/************************************************************************/

	/************************************************************************/
	// 老起切点在角上的。

	LoopStartCutDataListPtr m_pCornerStaEndCuts;

	// 新数据。
	std::vector<ILoopStartCutPtPtr> m_newSECutPts;
	std::vector<ILeadPtr> m_newLeadIns;
	std::vector<ILeadPtr> m_newLeadOuts;

	// 老数据。
	std::vector<ILoopStartCutPtPtr> m_oldSECutPts;
	std::vector<ILeadPtr> m_oldLeadIns;
	std::vector<ILeadPtr> m_oldLeadOuts;
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clCutSequenceCommon.h"
#include "TaskEx.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(GenerateCutNodeTask)
DECLARE_CUTLEADER_CLASS(CutSequenceList)
DECLARE_CUTLEADER_CLASS(CutNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// 软件计算偏移时生成“切割节点”。
class ClCutSequence_Export GenerateCutNodeTask : public TaskEx
{
public:
	GenerateCutNodeTask(const Point2D& referencePt, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, CutSequenceListPtr pCutSequences,
		BOOL bCheckIntf, LeadConfigItemPtr pLeadConfigItem);
	~GenerateCutNodeTask(void);

public: // inherit from class TaskEx.
	virtual void Run();

public: // get/set functions.
	CutNodeListPtr GetCutNodes() const { return m_pCutNodes; }
    const std::set<LONGLONG>& GetPatternLoops_failedToOffset() const { return patternLoops_failedToOffset; }

private:
	Point2D m_referencePt;
	PartPlacementListPtr m_pPartPmts;
	PartInstanceListPtr m_pPartInsts;
	CutSequenceListPtr m_pCutSequences;

	// 参数配置。
	BOOL m_bCheckIntf;
	LeadConfigItemPtr m_pLeadConfigItem;

	// 生成的切割节点。
	CutNodeListPtr m_pCutNodes;

    // 此处记录偏移失败的几何轮廓。
    std::set<LONGLONG> patternLoops_failedToOffset;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clGeometryBaseCommon.h"
#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(GridCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// 创建“田字工序”的命令。
class ClCutSequence_Export GridCutSequenceAddCommand : public SequenceCommandBase
{
public:
	GridCutSequenceAddCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, PartInstanceListPtr pPartInstances, PartInstanceListPtr pPartInsts_4_grid,
				  RECT_CORNER iStartCorner, double dToolWidth);
	~GridCutSequenceAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 新建的“田字工序”
	GridCutSequencePtr m_pNewGridSeqUnit;
	LONGLONG m_iGridNextSeqID; // 田字工序插在这个工序前面，-1就插到最后。

	// 新分配的零件实例内部轮廓的工序。
	CutSequenceListPtr m_pInnerLoopSeqs;
	LONGLONG m_iNewNextSeqID; // 这些工序的后一个工序，-1的话这个工序在最后。
};

END_CUTLEADER_NAMESPACE()

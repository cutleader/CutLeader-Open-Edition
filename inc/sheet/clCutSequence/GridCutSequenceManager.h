#pragma once

#include "clCutSequenceCommon.h"
#include "Point2D.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(GridCutSequence)
DECLARE_CUTLEADER_CLASS(ConfigData)
DECLARE_CUTLEADER_CLASS(CutSequenceList)

BEGIN_CUTLEADER_NAMESPACE()

// “田字工序”管理器。
class ClCutSequence_Export GridCutSequenceManager
{
public:
	// 判断这些零件实例能否创建田字工序。
	static bool CheckPartInsts_4_gridSeq(ConfigDataPtr pConfigData, GlViewPortPtr pViewPort, PartInstanceListPtr pCandidate_PartInsts);

	// 计算田字工序的起切位置，此时工序尚未创建。
	static RECT_CORNER CalcStartCornerOfGridSeq(CutSequenceListPtr pAllCutSeqs, ConfigDataPtr pConfigData, PartInstanceListPtr pCandidate_PartInsts);

	// 偏移“田字工序”。
	static GridCutSequencePtr OffsetCutSequence(GridCutSequencePtr pGridSeq, PartInstanceListPtr pPartInsts_after_offset);

	// 判断这个田字工序是不是有效。
	static BOOL IsCutSequenceInvalid(GridCutSequencePtr pGridSeq, PartInstanceListPtr pAllPartInstances);

	// 根据新的“零件实例”集合更新“田字工序”中的一些数据。
	static void UpdateCutSequence(GridCutSequencePtr pGridSeq, PartInstanceListPtr pOldPartInsts, PartInstanceListPtr pNewPartInsts);
};

END_CUTLEADER_NAMESPACE()

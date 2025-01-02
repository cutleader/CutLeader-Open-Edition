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

// �����ֹ��򡱹�������
class ClCutSequence_Export GridCutSequenceManager
{
public:
	// �ж���Щ���ʵ���ܷ񴴽����ֹ���
	static bool CheckPartInsts_4_gridSeq(ConfigDataPtr pConfigData, GlViewPortPtr pViewPort, PartInstanceListPtr pCandidate_PartInsts);

	// �������ֹ��������λ�ã���ʱ������δ������
	static RECT_CORNER CalcStartCornerOfGridSeq(CutSequenceListPtr pAllCutSeqs, ConfigDataPtr pConfigData, PartInstanceListPtr pCandidate_PartInsts);

	// ƫ�ơ����ֹ��򡱡�
	static GridCutSequencePtr OffsetCutSequence(GridCutSequencePtr pGridSeq, PartInstanceListPtr pPartInsts_after_offset);

	// �ж�������ֹ����ǲ�����Ч��
	static BOOL IsCutSequenceInvalid(GridCutSequencePtr pGridSeq, PartInstanceListPtr pAllPartInstances);

	// �����µġ����ʵ�������ϸ��¡����ֹ����е�һЩ���ݡ�
	static void UpdateCutSequence(GridCutSequencePtr pGridSeq, PartInstanceListPtr pOldPartInsts, PartInstanceListPtr pNewPartInsts);
};

END_CUTLEADER_NAMESPACE()

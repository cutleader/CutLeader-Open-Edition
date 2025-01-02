#pragma once

#include "clCutSequenceCommon.h"
#include "Point2D.h"
#include "Rect2D.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(ConfigData)
DECLARE_CUTLEADER_CLASS(ICutSequence)
DECLARE_CUTLEADER_CLASS(FlyCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// �������и�򡱹�������
class ClCutSequence_Export FlyCutSequenceManager
{
public:
	// �ж���Щ��·ʵ���ܷ񴴽������и��
	// ע��
	// 1) ��Ҫ��Щ����ʵ����������Ҫһ����ȫ���Ǿ��λ���Բ�Ρ�Ҫ�γ�һ�����С����⣬���ڰ���»�Ҫ����Щ����ʵ�����ڿף���������һ�����ʵ����
	static bool CheckLoopInsts(const ConfigData* pConfigData, const CWnd* pSheetView, const LoopInstanceList* pCandidate_LoopInsts);

	// ��������и�������λ�ã���ʱ������δ�������ο��˱��滻�ĵ�һ����������е㡣
	// ע��
	// 1) ��cutSeqs_willBeReplaced���й�����ο�����Щ�����еĵ�һ����������е㡣��cutSeqs_willBeReplaced��û�й�����ο���������е���ʼ�ǡ�
	static RECT_CORNER CalcStartCornerOfFlyCutSeq(const std::map<int, ICutSequencePtr>& cutSeqs_willBeReplaced, ConfigDataPtr pConfigData, LoopInstanceListPtr pCandidate_LoopInsts);

	// ƫ�ơ������и�򡱡�
	static FlyCutSequencePtr OffsetCutSequence(FlyCutSequencePtr pFlyCutSeq, PartInstanceListPtr pPartInsts_after_offset);

	// �ж�������й����ǲ�����Ч��
	static BOOL IsCutSequenceInvalid(FlyCutSequencePtr pFlyCutSeq, PartInstanceListPtr pAllPartInstances);

	// �����µġ����ʵ�������ϸ��¡������и���е�һЩ���ݡ�
	static void UpdateCutSequence(FlyCutSequencePtr pFlyCutSeq, PartInstanceListPtr pOldPartInsts, PartInstanceListPtr pNewPartInsts);
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clGeometryBaseCommon.h"
#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(GridCutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// ���������ֹ��򡱵����
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
	// �½��ġ����ֹ���
	GridCutSequencePtr m_pNewGridSeqUnit;
	LONGLONG m_iGridNextSeqID; // ���ֹ�������������ǰ�棬-1�Ͳ嵽���

	// �·�������ʵ���ڲ������Ĺ���
	CutSequenceListPtr m_pInnerLoopSeqs;
	LONGLONG m_iNewNextSeqID; // ��Щ����ĺ�һ������-1�Ļ�������������
};

END_CUTLEADER_NAMESPACE()

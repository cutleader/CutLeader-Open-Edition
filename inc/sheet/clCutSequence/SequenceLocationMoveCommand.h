#pragma once

#include "SequenceCommandBase.h"

DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(ICutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// �ѹ������������׼���򡱺��档
class ClCutSequence_Export SequenceLocationMoveCommand : public SequenceCommandBase
{
public:
	SequenceLocationMoveCommand(EditorData editorData, CutSequenceListPtr pCutSequences, ConfigDataPtr pConfigData, CutSequenceListPtr pMovCutSeqs, ICutSequencePtr pBaseCutSeq, PartInstanceListPtr pAllPartInst);
	virtual ~SequenceLocationMoveCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// Ҫ����λ�õĹ���
	CutSequenceListPtr m_pMovCutSeqs;

	// ���������������档
	// ע��
	//  1) Ϊ��ʱ��������ʼλ�á�
	//  2) ������������ƶ��Ĺ����С�
	ICutSequencePtr m_pBaseCutSequence;

	// �����ƶ�֮ǰ��λ�á�
	std::vector<int> m_iOldPosList;

	// ���е����ʵ����
	PartInstanceListPtr m_pAllPartInsts;
};

END_CUTLEADER_NAMESPACE()

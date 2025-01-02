#pragma once

#include "LoopSequence.h"

DECLARE_CUTLEADER_CLASS(GridLoopSequence)
DECLARE_CUTLEADER_CLASS(AvoidData)

BEGIN_CUTLEADER_NAMESPACE()

// Grid loop sequence, ���л�·����
// ע:
//   1) the cut order within the grid is decided by cad data.
class ClCutSequence_Export GridLoopSequence : public LoopSequence
{
public:
	GridLoopSequence(void);
	GridLoopSequence(LoopInstancePtr pLoopInstance);
	virtual ~GridLoopSequence(void);

public: // implement IData interface.
	// clone the loop sequence unit.
	// notes:
	//   1) "m_pLoopInstance" should be cloned.
	virtual IDataPtr Clone() const override;

public: // implement interface ICutSequence.
	virtual CutSequenceType GetCutSequenceType() const override { return CutSequence_GridLoop; }

public: // get/set functions.
	void SetAvoidData(AvoidDataPtr pAvoidData) { m_pAvoidData = pAvoidData; }
	AvoidDataPtr GetAvoidData() const { return m_pAvoidData; }

private:
	// �����ڲ�������ʱ��Ҫ�ο������ݡ�
	AvoidDataPtr m_pAvoidData;
};

END_CUTLEADER_NAMESPACE()

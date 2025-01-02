#pragma once

#include "clGeometryBaseCommon.h"
#include "clCutSequenceCommon.h"
#include "ComCutGroupBase.h"

DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(AvoidData)
DECLARE_CUTLEADER_CLASS(CanvasComCutGroup)

BEGIN_CUTLEADER_NAMESPACE()

// �����顣
//  1) ����������˿��������ߵ�һ������ʵ����
class CanvasComCutGroup : public ComCutGroupBase
{
public:
	CanvasComCutGroup(void);
	CanvasComCutGroup(LoopInstanceListPtr pLoopInstances, AvoidDataPtr pAvoidData, DIRECTION cutDir, double dOverCut, double dToolWidth);
	virtual ~CanvasComCutGroup(void);

public:
	// ��¡�����顣
	// ע:
	//   1) do not need to clone "m_pLoopInstList", so if we want to change it, we should do this outside.
	virtual IDataPtr Clone() const override;

	virtual MicroJointDatasOfComCutGroupPtr GetMicroJointDatas() const override;

public:
	void SetLoopInsts(LoopInstanceListPtr pLoopInstList) { m_pLoopInstList = pLoopInstList; }
	LoopInstanceListPtr GetLoopInstances() const { return m_pLoopInstList; }

protected:
	// ���������ߵ�һ������ʵ����
	LoopInstanceListPtr m_pLoopInstList;
};

END_CUTLEADER_NAMESPACE()

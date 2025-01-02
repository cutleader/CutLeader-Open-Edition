#pragma once

#include "ClTaskCommon.h"
#include "ClData.h"
#include "NestPriority.h"
#include "ClNestJobCommon.h"

DECLARE_CUTLEADER_CLASS(Part)

BEGIN_CUTLEADER_NAMESPACE()

// �������
class ClTask_Export TaskPart : public ClData
{
public:
	TaskPart(PartPtr pPart, const NestPriority& nestPriority, PartRotateType emPartRotate, int iPlanningCount);
	virtual ~TaskPart(void);

public: // set/get functions.
	const Part* GetPart() const { return m_pPart.get(); }
    PartPtr GetPart_FW() const { return m_pPart; }
	void SetPart(PartPtr pPart) { m_pPart = pPart; }

    const NestPriority& GetNestPriority() const { return m_nestPriority; }
    void SetNestPriority(const NestPriority& nestPriority) { m_nestPriority = nestPriority; }

	PartRotateType GetPartRotateType() const { return m_emPartRotate; }
    void SetPartRotateType(PartRotateType emPartRotate) { m_emPartRotate = emPartRotate; }

	int GetPlanningCount() const { return m_iPlanningCount; }
    void SetPlanningCount(int iPlanningCount) { m_iPlanningCount = iPlanningCount; }

    // ����������ע�ͣ�Ҫע����õ�ʱ����
    int GetNestedCount() const { return m_iNestedCount; }
    void SetNestedCount(int iNestedCount) { m_iNestedCount = iNestedCount; }

private:
	PartPtr m_pPart;

    // �Զ��Ű����ȼ�
    NestPriority m_nestPriority;

    // �Զ��Ű���ת�Ƕ�
	PartRotateType m_emPartRotate;

	// �ƻ�������Ҳ����ϣ���ӹ���������
	int m_iPlanningCount;

    // �����������������ֻ�ڱ�������ʱ���£���������ʱҪע��ʱ����
    int m_iNestedCount;
};

END_CUTLEADER_NAMESPACE()

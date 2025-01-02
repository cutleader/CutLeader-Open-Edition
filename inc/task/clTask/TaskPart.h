#pragma once

#include "ClTaskCommon.h"
#include "ClData.h"
#include "NestPriority.h"
#include "ClNestJobCommon.h"

DECLARE_CUTLEADER_CLASS(Part)

BEGIN_CUTLEADER_NAMESPACE()

// 任务零件
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

    // 看看变量的注释，要注意调用的时机。
    int GetNestedCount() const { return m_iNestedCount; }
    void SetNestedCount(int iNestedCount) { m_iNestedCount = iNestedCount; }

private:
	PartPtr m_pPart;

    // 自动排版优先级
    NestPriority m_nestPriority;

    // 自动排版旋转角度
	PartRotateType m_emPartRotate;

	// 计划数量，也就是希望加工的数量。
	int m_iPlanningCount;

    // 已排数量，这个数据只在保存任务时更新，所以用它时要注意时机。
    int m_iNestedCount;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "ClData.h"
#include "clExpertLibCommon.h"

DECLARE_CUTLEADER_CLASS(ToolInstance)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLib_Export ToolAssignRule : public ClData
{
public:
	ToolAssignRule(bool bForInnerLoop, double dWidthRange_of_loop, double dHeightRange_of_loop, ToolInstancePtr pToolInstance);
	~ToolAssignRule();

public:
    bool IsForInnerLoop() const { return m_bForInnerLoop; }
    void IsForInnerLoop(bool bForInnerLoop) { m_bForInnerLoop = bForInnerLoop; }

    double GetWidthRange_of_loop() const { return m_dWidthRange_of_loop; }
    void SetWidthRange_of_loop(double dWidthRange_of_loop) { m_dWidthRange_of_loop = dWidthRange_of_loop; }

    double GetHeightRange_of_loop() const { return m_dHeightRange_of_loop; }
    void SetHeightRange_of_loop(double dHeightRange_of_loop) { m_dHeightRange_of_loop = dHeightRange_of_loop; }

    ToolInstancePtr GetToolInstance() const { return m_pToolInstance; }
    void SetToolInstance(ToolInstancePtr pToolInstance) { m_pToolInstance = pToolInstance; }

private:
    // 内轮廓或外轮廓
    bool m_bForInnerLoop;

    // 轮廓要小于这个range
    double m_dWidthRange_of_loop;
    double m_dHeightRange_of_loop;

    // 分配的刀具
    ToolInstancePtr m_pToolInstance;
};

END_CUTLEADER_NAMESPACE()

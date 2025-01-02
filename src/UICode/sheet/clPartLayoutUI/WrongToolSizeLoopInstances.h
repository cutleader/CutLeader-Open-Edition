#pragma once

#include "clPartLayoutUICommon.h"
#include <set>


DECLARE_CUTLEADER_CLASS(WrongToolSizeLoopInstances)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(GlViewPort)


BEGIN_CUTLEADER_NAMESPACE()


// 该类代表有着不合适刀具尺寸的轮廓实例。
class ClPartLayoutUI_Export WrongToolSizeLoopInstances
{
public:
    WrongToolSizeLoopInstances();
    ~WrongToolSizeLoopInstances();

public:
    LoopInstanceListPtr GetLoopInstances_wrongToolSize() const { return m_pLoopInstances_wrongToolSize; }

public:
    // 初始化该对象。
    // 注：
    // 1）该函数用于初始化该对象，不过该函数并不是必须调用的。
    // 2）pAllLoopInstances：板材上所有的轮廓实例。
    // 3）patternLoops_failedToOffset：没有能成功偏移的几何轮廓。
    void Init(const LoopInstanceList* pAllLoopInstances, const std::set<LONGLONG>& patternLoops_failedToOffset);

    // 更新当前对象。
    // 注：
    // 1）该函数用于对该对象进行更新。
    // 1）pLoopInstances_willBeChecked：需要对这些轮廓进行偏移测试。
    void Update(const LoopInstanceList* pLoopInstances_willBeChecked, double dToolSize);

    // 绘制偏移有问题的轮廓实例。
    void Draw(GlViewPortPtr pViewPort);

private:
    LoopInstanceListPtr m_pLoopInstances_wrongToolSize;
};

END_CUTLEADER_NAMESPACE()

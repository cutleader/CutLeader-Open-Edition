#pragma once

#include "ICutSequence.h"

DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// 实际切割零件的工序。
class ClCutSequence_Export ICutPartSequence : virtual public ICutSequence
{
public:
	virtual ~ICutPartSequence() {}

public:
	// “切割工序”是否加工了该“回路实例”。
	virtual BOOL LoopHasBeenCut(const LoopInstance* pLoopInstance) const = 0;

	// “切割工序”加工的所有“回路实例”。
	virtual LoopInstanceListPtr GetLoopInstanceList() const = 0;

	// 得到“切割工序”加工的回路实例，这个回路实例上面是可能有引入引出线的。
	// 注：
	//  1) 这几种情况下的“回路实例”上面没有引入引出线。
	//     a) 从过切点切割的共边工序。
	//     b) 不是根节点的“桥接节点”。
	//  2) 没有则返回空值。
	virtual LoopInstancePtr GetLoopHasLead() const = 0;

	// 得到工序所涉及的“零件实例”。
	// 注：
	//   1) 当然工序可能只切割了“零件实例”的一部分。
	virtual PartInstanceListPtr GetPartInstances() const = 0;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clCutSequenceCommon.h"
#include "Matrix2D.h"
#include "IData.h"

BEGIN_CUTLEADER_NAMESPACE()

// the type of Cut Node.
enum CutNodeType
{
	CutNode_DoPierce	= 0,
	CutNode_UnPierce	= 1,

	CutNode_TraceLine	= 2,

	CutNode_CutLine		= 3,
	CutNode_CutArc		= 4,

    CutNode_SelectTool	= 6,
	CutNode_LoopInfo	= 7,

    CutNode_Reset       = 8, // 为一个客户定制，复位刀具

    CutNode_CoolingPoint       = 9, // 冷却点
};

// “切割节点”接口。
// 注:
//   1) set the sequence unit ID as the parent ID.
class ClCutSequence_Export ICutNode : virtual public IData
{
public:
    virtual ~ICutNode() {}

public:
	// get the cut node type.
	virtual CutNodeType GetCutNodeType() const = 0;

	// whether this cut node can be drawn. e.g., Condition cut node cannot be drawn.
	virtual BOOL CanDraw() const = 0;

	// some cut nodes can be transformed.
	virtual void Transform(Matrix2D mat) = 0;
};

END_CUTLEADER_NAMESPACE()

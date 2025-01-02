#pragma once

#include "clCutSequenceCommon.h"
#include "Rect2D.h"

BEGIN_CUTLEADER_NAMESPACE()

enum SIM_SHAPE_TYPE
{
	SIM_SHAPE_CUT			= 0,
	SIM_SHAPE_TRACE			= 1,
};

// 模拟“切割节点”的形状。
// 注：
//   1) “模拟形状”中的基本图形是从“切割节点”克隆而来。
class ClCutSequence_Export ISequenceSimulateShape
{
public:
    virtual ~ISequenceSimulateShape() {}

public:
	virtual SIM_SHAPE_TYPE GetSimShapeType() const = 0;
	virtual void Transform(const Matrix2D& mat) = 0;
	virtual Rect2D GetRect() const = 0;
};

END_CUTLEADER_NAMESPACE()

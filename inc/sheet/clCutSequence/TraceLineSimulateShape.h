#pragma once

#include "SequenceSimulateShapeBase.h"

DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(TraceLineSimulateShape)

BEGIN_CUTLEADER_NAMESPACE()

// 模拟“快速移动”的形状。
class ClCutSequence_Export TraceLineSimulateShape : public SequenceSimulateShapeBase
{
public:
	TraceLineSimulateShape(Line2DPtr pLine2D, const Rect2D& rectOfLine2D);
	virtual ~TraceLineSimulateShape(void);

public: // implement interface ISequenceSimulateShape.
	virtual SIM_SHAPE_TYPE GetSimShapeType() const override { return SIM_SHAPE_TRACE; }
	virtual void Transform(const Matrix2D& mat);

public: // get/set functions.
	const Line2D* GetLine2D() const { return m_pLine2D.get(); }

private:
	Line2DPtr m_pLine2D;
};

END_CUTLEADER_NAMESPACE()

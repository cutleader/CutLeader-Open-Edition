#pragma once

#include "clCutSequenceCommon.h"
#include "ISequenceSimulateShape.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export SequenceSimulateShapeBase : public ISequenceSimulateShape
{
public:
	SequenceSimulateShapeBase(void);
	virtual ~SequenceSimulateShapeBase(void);

public: // implement interface ISequenceSimulateShape.
	virtual Rect2D GetRect() const override { return m_rect; }

protected:
	// the rect of the line or arc which was simulated.
	Rect2D m_rect;
};

END_CUTLEADER_NAMESPACE()

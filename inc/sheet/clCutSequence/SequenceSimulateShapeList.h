#pragma once

#include "clCutSequenceCommon.h"
#include "Matrix2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ISequenceSimulateShape)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutSequence_Export SequenceSimulateShapeList : public std::vector<ISequenceSimulateShapePtr>
{
public:
	SequenceSimulateShapeList(void);
	~SequenceSimulateShapeList(void);

public:
	void Transform(const Matrix2D& mat);
};

END_CUTLEADER_NAMESPACE()

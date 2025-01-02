#pragma once

#include "clGeometryBaseCommon.h"
#include "RealPair.h"
#include "Matrix2D.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

// this class can be used for repeating data.
// notes:
//   1) e.g., for a "3 instance" LineArrayPattern, if we know the pierce info of first inst,
//      we can use "RepeatData" to figure out the other two base on the first inst.
class ClGeometryBase_Export RepeatData
{
public:
	RepeatData(void);
	RepeatData(std::vector<Matrix2D> matList);

	~RepeatData(void);

public:
	const std::vector<Matrix2D>& GetMatrix() const { return m_matList; }
	void SetMatrix(std::vector<Matrix2D> matList) { m_matList = matList; }

	void AddMatrix(Matrix2D mat) { m_matList.push_back(mat); }

private:
	// the Matrix list, each Matrix can calculate a new "instance".
	std::vector<Matrix2D> m_matList;
};

END_CUTLEADER_NAMESPACE()

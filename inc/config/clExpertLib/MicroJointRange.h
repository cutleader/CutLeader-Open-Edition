#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(MicroJointRangeInfo)
DECLARE_CUTLEADER_CLASS(MicroJointRange)

BEGIN_CUTLEADER_NAMESPACE()

// now we define 3 ranges, a range object represent the data in one tab.
// notes:
//   1) a MicroJointRangeInfo object occupy two rows in table expLib_bmjrangeinfo.
class ClExpertLib_Export MicroJointRange : public ClData
{
public:
	MicroJointRange();
	virtual ~MicroJointRange();

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetHoleRange(MicroJointRangeInfoPtr holeRange) { m_holeRange = holeRange; }
	MicroJointRangeInfoPtr GetHoleRange() const { return m_holeRange; }

	void SetPerimeterRange(MicroJointRangeInfoPtr perimeterRange) { m_perimeterRange = perimeterRange; }
	MicroJointRangeInfoPtr GetPerimeterRange() const { return m_perimeterRange; }

	void SetIndex(int index) { m_index = index; }
	int GetIndex() const { return m_index; }

private:
	// the setting for Hole.
	MicroJointRangeInfoPtr m_holeRange;

	// the setting for Perimeter.
	MicroJointRangeInfoPtr m_perimeterRange;

	// the range index, (1,2,3)
	int m_index;
};

END_CUTLEADER_NAMESPACE()

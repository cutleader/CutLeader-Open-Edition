#pragma once

#include "clGeometryFeatureCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// “图形位置”对象。
class ClGeometryFeature_Export PatternPosition
{
public:
	PatternPosition(void);
	PatternPosition(int iIndex, double dPercent);
	~PatternPosition(void);

public: // get/set functions
	void SetIndex(int iIndex) { m_iIndex = iIndex; }
	int GetIndex() const { return m_iIndex; }

	void SetPercent(double dPercent) { m_dPercent = dPercent; }
	double GetPercent() const { return m_dPercent; }

private:
	// the base geom2D index of the pattern.
	int m_iIndex;

	// the percent.
	double m_dPercent;
};

END_CUTLEADER_NAMESPACE()

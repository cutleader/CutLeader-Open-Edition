#pragma once

#include "clGeometryBaseCommon.h"
#include "ILineArc2D.h"

BEGIN_CUTLEADER_NAMESPACE()

// the basic implementation for interface ILineArc2D.
class ClGeometryBase_Export LineArc2DBase : public ILineArc2D
{
public:
	LineArc2DBase();
	~LineArc2DBase();

public: // get/set functions.
	const Point2D& GetBasePoint() const { return m_basePt; }
	void SetBasePoint(const Point2D& newVal)
	{ 
		m_basePt = newVal;
		UpdateCache();
	}

	const Vector2D& GetBaseVector() const { return m_baseVector; }
	void SetBaseVector(const Vector2D& newVal)
	{ 
		m_baseVector = newVal;
		UpdateCache();
	}

protected:
	// the base point.
	// notes:
	//   1) for arc, this is the center point of the arc.
	//   2) for line, this is the start point.
	Point2D m_basePt;

	// the base vector of the geom.
	// notes:
	//   1) for arc, this is from center point to start point. circle do not need this variable. 
	//      BUT anyway, we still set it.
	//   2) for line. this can decide the end point.
	Vector2D m_baseVector;

	// buffer the end pt.
	Point2D m_endPt;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "clGeometryBaseCommon.h"
#include "Point2D.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(Triangle2D)
DECLARE_CUTLEADER_CLASS(LineArc2DList)

BEGIN_CUTLEADER_NAMESPACE()

// this class can stand for a triangle.
/*
                        top point
						   8               -|-     
						  8 8               |      
						 8   8              |      
						8     8             |      
					   8       8            |      
					  8         88          | height    
					 8            8         |      
					8              8        |      
				   8                8       |      
				  8                  8      |      
				 8          8         8     |      
				888888888888888888888888   ---    
						    8  (center point) (m_centerPt)
			                                       
				|      width		   |           
				------------------------           
				|                      |  
*/

class ClGeometryBase_Export Triangle2D
{
public:
	Triangle2D(void);
	Triangle2D(Point2D centerPt, Vector2D baseVect, double dOrthoRatio = 1.5);
	~Triangle2D(void);

public: // get/set functions.
	double GetWidth() const { return m_baseVector.Length(); }
	void SetWidth(double dVal) { m_baseVector.Magnitude(dVal); }

	double GetHeight() const { return GetWidth()*m_dOrthoRatio; }

	// get height vector.
	// notes:
	//   1) rotate the base vector with 90 degree by CCW, we will get height direction.
	Vector2D GetHeightVector() const;

public:
	// notes:
	//   1) if the three points are CCW, the area is positive, otherwise it is negative.
	static double TriangleArea(const Point2D& pt1, const Point2D& pt2, const Point2D& pt3);

public:
	void Transform(const Matrix2D& mat);

	virtual Triangle2DPtr Clone();

	// get line/arc shapes of the triangle.
	LineArc2DListPtr GetLineArcs() const;

	// get three vertexes.
	void GetVertexes(Point2D& pt1, Point2D& pt2, Point2D& pt3) const;

	Rect2D GetRectBox() const;

private:
	// the center point of the triangle.
	Point2D m_centerPt;

	// the base vector of the triangle.
	Vector2D m_baseVector;

	// the ratio of the triangle.
	// notes:
	//   1) m_dOrthoRatio = height/width.
	double m_dOrthoRatio;
};

END_CUTLEADER_NAMESPACE()

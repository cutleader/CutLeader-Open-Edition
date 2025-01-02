#pragma once

#include "clUtilityCommon.h"
#include "baseConst.h"
#include "math.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class encapsulates some basic math functions.
class ClUtility_Export MathEx
{
public:
	/************************************************************************/
	// about the dAngle.

	// range the dAngle from -PI to PI
	static double GetSignedAng(double dAngle, double dTol = SYSTEM_TOLERANCE);

	// range the dAngle from 0 to 2*PI
	static double GetUnsignedAng(double dAngle, double dTol = SYSTEM_TOLERANCE) {
		double dRounds = (dAngle / PI) / 2;
		double dAng = PI * 2 * (dRounds - floor(dRounds));
		if (fabs(dAng - 2 * PI) < dTol) {
			return 0;
		}
		return dAng;
	}

	// range the dAngle from -2*PI to 2*PI
	static double GetSignedAngEx(double dAngle, double dTol = SYSTEM_TOLERANCE);

	// get the angle of the vector(pt1 to pt2), the range is from -PI to PI.
	static double GetAngle(double dPointX1, double dPointY1, double dPointX2, double dPointY2) {
		return atan2(dPointY2 - dPointY1, dPointX2 - dPointX1);
	}

	static BOOL CheckAngleRange(double dAngle, double dStartAngle, double dSweepAngle, double dTol = SYSTEM_TOLERANCE) {
		if (fabs(fabs(dSweepAngle) - 2 * PI) < dTol) {
			return TRUE;
		}
		else {
			if (dSweepAngle >= 0) {
				return (GetUnsignedAng((dAngle - dStartAngle), dTol) < GetUnsignedAng(dSweepAngle, dTol) + dTol);
			}
			else {
				return (GetUnsignedAng((-dAngle + dStartAngle), dTol) < GetUnsignedAng(-dSweepAngle, dTol) + dTol);
			}
		}
	}

	static double GetCosFromTriangle(double d1, double d2, double d3);

	// whether the dAngle is near axis(0,90,180,270,360).
	// notes: "dAngle" is radian format.
	static BOOL AngleNearAxis(double dAngle);
	/************************************************************************/


	/************************************************************************/
	// about arc.

	// calculate arc's center point, radius, start dAngle, move dAngle from three points.
	// notes:
	//   1) return false if 3 point cannot build an arc. e.g. there are on one line.
	static BOOL GetArcFrom3Point(double dPoint1X, double dPoint1Y, double dPoint2X, double dPoint2Y, double dPoint3X, double dPoint3Y, double& dCenterX, double& dCenterY, 
		double& dRadius, double& dStartAngle, double& dMoveAngle);

	// get the center point of arc.
	static void GetArcCenter(double dStartPtX, double dStartPtY, double dEndPtX, double dEndPtY, double dSweepAngle, double& dCenterX, double& dCenterY);
	/************************************************************************/


	/************************************************************************/
	// equal.

	// check whether these two values are equal.
	static bool Equal(double d1, double d2, double dTol = SYSTEM_TOLERANCE) { return fabs(d1-d2) <= dTol; }

	// check whether the difference between two values are equal to "dDiffer".
	static bool EqualA(double d1, double d2, double dDiffer);

	// check whether these two points are very close.
	static BOOL PointEqual(double dX1, double dY1, double dX2, double dY2);

	static BOOL ValBetween(double dVal, double dStart, double dEnd);
	/************************************************************************/


	// rotate the point(dOriginX,dOriginY) based on point(centerX,centerY) and dAngle.
	static void RotatePoint(double dCenterX, double dCenterY, double dAngle, double dOriginX, double dOriginY, double& dPointX, double& dPointY);

	// get the area of the three points.
	/* 参数：
	*  bSigned: 返回值要不要带符号。如果3个点逆时针排布，返回负值。
	*/
	static double GetTriangleArea(double dPtX1, double dPtY1, double dPtX2, double dPtY2, double dPtX3, double dPtY3, BOOL bSigned);
};

END_CUTLEADER_NAMESPACE()

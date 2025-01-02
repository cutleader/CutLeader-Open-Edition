#pragma once

#include "clUtilityCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class will convert the unit between the service and the logic env.
// notes:
//   1) before perform the conversion, you should call init() function to init the resolution value at first.
class ClUtility_Export UnitHelper
{
public:
	static void Init(int iResolution) { m_iResolution = iResolution; }

	static double PixelsToInches(int pixels);
	static double PixelsToCentimeters(int pixels);

	static int PointsToPixels(int points);
	static int InchesToPixels(double inches);
	static int CentimeterToPixels(double centimeters);

	// The function adjusts a paper, taking the hardware margin of the printer into account.
	static void AdjustPixelsToPaper(CDC* pDC, CRect& rect);

private:
	static int m_iResolution;
};

END_CUTLEADER_NAMESPACE()

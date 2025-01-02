#pragma once

#include "ccpBridgeCommon.h"
#include "Point2DEx.h"
#include "Matrix2DEx.h"
#include "Rect2DEx.h"

BEGIN_CUTLEADER_NAMESPACE()

[ComVisible(true)]
public enum class GEOMITEM_TYPE_EX
{
	GEOMITEM_EX_LINE 	= 0,
	GEOMITEM_EX_ARC		= 1,
};

[InterfaceType(ComInterfaceType::InterfaceIsDual)]
[ComVisible(true)]
public interface class GeomItemEx
{
public:
	virtual GEOMITEM_TYPE_EX GetType();
	virtual GeomItemEx^ Clone();

	virtual Point2DEx^ GetStartPoint();
	virtual void SetStartPoint(Point2DEx^ pt);
	virtual Point2DEx^ GetEndPoint();

	virtual Point2DEx^ GetPointByPercent(double dPercent);
	virtual double GetPercentByPoint(Point2DEx^ pt);

	virtual void Transform(Matrix2DEx^ mat);
	virtual void Reverse();

	virtual double Distance(Point2DEx^ pt);

	virtual Rect2DEx^ GetRectBox();
};

END_CUTLEADER_NAMESPACE()
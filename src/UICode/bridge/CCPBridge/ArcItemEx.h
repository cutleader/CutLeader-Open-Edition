#pragma once

#include "ccpBridgeCommon.h"
#include "GeomItemEx.h"
#include "Arc2D.h"

/************************************************************************/
// declare class.
DECLARE_CUTLEADER_CLASS(Arc2D)
/************************************************************************/

BEGIN_CUTLEADER_NAMESPACE()

[ClassInterface(ClassInterfaceType::AutoDual)]
[ComVisible(true)]
public ref class ArcItemEx : public GeomItemEx
{
public:
	ArcItemEx(void);
	ArcItemEx(Point2DEx^ centerPt, double dRadius, double dStartAngle, double dMoveAngle);
	ArcItemEx(Arc2DPtr* ppArcItem);

	~ArcItemEx();
	!ArcItemEx();

public: // implement interface GeomItemEx.
	virtual GEOMITEM_TYPE_EX GetType() { return GEOMITEM_TYPE_EX::GEOMITEM_EX_ARC; }
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

public:
	Point2DEx^ GetCenterPt();
	void SetCenterPt(Point2DEx^ centerPt);

	double GetRadius() { return (*m_ppArcItem)->GetRadius(); }
	void SetRadius(double dRadius) { (*m_ppArcItem)->SetRadius(dRadius); }

	void SetBaseAngle(double dAngle) { (*m_ppArcItem)->SetBaseAngle(dAngle); }
	double GetBaseAngle() { return (*m_ppArcItem)->GetBaseAngle(); }

	double GetSweepAngle() { return (*m_ppArcItem)->GetSweepAngle(); }
	void SetSweepAngle(double dAngle) { (*m_ppArcItem)->SetSweepAngle(dAngle); }

public:
	Arc2DPtr GetArcItem() { return *m_ppArcItem; }

private:
	Arc2DPtr* m_ppArcItem;
};

END_CUTLEADER_NAMESPACE()
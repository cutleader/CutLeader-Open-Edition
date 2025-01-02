#pragma once

#include "ccpBridgeCommon.h"
#include "Vector2DEx.h"
#include "GeomItemEx.h"
#include "Line2D.h"

/************************************************************************/
// declare class.
DECLARE_CUTLEADER_CLASS(Line2D)
/************************************************************************/

BEGIN_CUTLEADER_NAMESPACE()

[ClassInterface(ClassInterfaceType::AutoDual)]
[ComVisible(true)]
public ref class LineItemEx : public GeomItemEx
{
public:
	LineItemEx(void);
	LineItemEx(Point2DEx^ startPt, Point2DEx^ endPt);
	LineItemEx(Line2DPtr* ppLineItem);

	~LineItemEx();
	!LineItemEx();

public: // implement interface GeomItemEx.
	virtual GEOMITEM_TYPE_EX GetType() { return GEOMITEM_TYPE_EX::GEOMITEM_EX_LINE; }
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
	Vector2DEx^ GetBaseVector();
	void SetEndPoint(Point2DEx^ pt);

	double GetLength() { return (*m_ppLineItem)->GetLength(); }
	void SetLength(double dLength) { (*m_ppLineItem)->SetLength(dLength); }

	double GetBaseAngle() { return (*m_ppLineItem)->GetBaseAngle(); }
	void SetBaseAngle(double dAngle) { (*m_ppLineItem)->SetBaseAngle(dAngle); }

public:
	Line2DPtr GetLineItem() { return *m_ppLineItem; }

private:
	Line2DPtr* m_ppLineItem;
};

END_CUTLEADER_NAMESPACE()
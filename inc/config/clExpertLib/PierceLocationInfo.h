#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(PierceLocationInfo)

BEGIN_CUTLEADER_NAMESPACE()

// this divides the four quadrants into 16 sections.调整值的时候需要注意，下面根据值判断在不在角上。
typedef enum tagPIERCE_POSITION
{	
	PIERCE_NONE		= 0,
	PIERCE_0		= 1,
	PIERCE_PI_8		= 2,
	PIERCE_PI_4		= 3,
	PIERCE_3PI_8	= 4,
	PIERCE_PI_2		= 5,
	PIERCE_5PI_8	= 6,
	PIERCE_3PI_4	= 7,
	PIERCE_7PI_8	= 8,
	PIERCE_PI		= 9,
	PIERCE_9PI_8	= 10,
	PIERCE_5PI_4	= 11,
	PIERCE_11PI_8	= 12,
	PIERCE_3PI_2	= 13,
	PIERCE_13PI_8	= 14,
	PIERCE_7PI_4	= 15,
	PIERCE_15PI_8	= 16
} PIERCE_POSITION;

typedef enum tagPIERCE_SHAPETYPE
{	
	PIERCE_ROUND			= 1,
	PIERCE_RECT				= 2,
	PIERCE_OBROUND			= 3,
	PIERCE_DOUBLED			= 4,
	PIERCE_SINGLED			= 5,
	PIERCE_TRIANGLE			= 6,
	PIERCE_RCRECT			= 7,
	PIERCE_RADSLOT			= 8,
	PIERCE_INSIDEPOLYGON	= 9,
	PIERCE_EXTERIORPOLYGON	= 10
} PIERCE_SHAPETYPE;

// this class describe the pierce location for a shape.
class ClExpertLib_Export PierceLocationInfo : public ClData
{
public:
	PierceLocationInfo(void);
	PierceLocationInfo(PIERCE_SHAPETYPE shape, PIERCE_POSITION position);

	~PierceLocationInfo(void);

public:
	static double PositionToAngle(PIERCE_POSITION position);
	static PIERCE_POSITION PierceLocationInfo::AngleToPosition(double angle);

	// e.g., we can normalize 40 degree to PI/4, but for RoundHole we should return 0.
	// notes: we can do this by another way:
	//        step 1): call AngleToPosition()
	//        step 2): call PositionToAngle()
	static double NormalizeAngle(PIERCE_SHAPETYPE shape, double dAngle);

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetPierceShape(PIERCE_SHAPETYPE shape) { m_shape = shape; }
	PIERCE_SHAPETYPE GetPierceShape() { return m_shape; }

	void SetPiercePos(PIERCE_POSITION pos) { m_position = pos; }
	PIERCE_POSITION GetPiercePos() { return m_position; }

public:
	bool IsAtCorner() const;

private:
	// the shape
	PIERCE_SHAPETYPE m_shape;

	// the start location
	PIERCE_POSITION m_position;
};

END_CUTLEADER_NAMESPACE()

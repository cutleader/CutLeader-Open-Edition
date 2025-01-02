#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(CornerRangeInfo)

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<CString>> StringListPtr;

typedef enum tagCornerRangeType
{
	// 0-90 degree
	CORNER_RANGE_1		= 0,

	// 90 degree
	CORNER_RANGE_2		= 1,

	// 90-180 degree
	CORNER_RANGE_3		= 2,

	// 180-270 degree
	CORNER_RANGE_4		= 3,

	// 270 degree
	CORNER_RANGE_5		= 4,

	// 270-360 degree
	CORNER_RANGE_6		= 5,
} CORNER_RANGE_TYPE;

// 每个角度范围对应的角特征。
class ClExpertLib_Export CornerRangeInfo : public ClData
{
public:
	CornerRangeInfo();
	virtual ~CornerRangeInfo();

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetCornerRangeType(CORNER_RANGE_TYPE rangeType) { m_rangeType = rangeType; }
	CORNER_RANGE_TYPE GetCornerRangeType() const { return m_rangeType; }

	void SetCornerType(CornerType cornerType) { m_iCornerType = cornerType; }
	CornerType GetCornerType() const { return m_iCornerType; }

	void SetCornerValue(double dVal) { m_dVal = dVal; }
	double GetCornerValue() const { return m_dVal; }

    void EnableCornerLineLengthControl(BOOL bEnableCornerLineLengthControl) { m_bEnableCornerLineLengthControl = bEnableCornerLineLengthControl; }
    BOOL EnableCornerLineLengthControl() const { return m_bEnableCornerLineLengthControl; }

    void SetCornerLineLength(double dCornerLineLength) { m_dCornerLineLength = dCornerLineLength; }
    double GetCornerLineLength() const { return m_dCornerLineLength; }

public: // static functions.
	// 得到所有角特征类型的名字。
	static StringListPtr GetAllCornerType(BOOL bAddNoneCorner = TRUE);

	// parse the corner type by string name.
	static CornerType GetCornerTypeByStr(CString str);

	// parse the Corner type name by corner type.
	static CString GetNameByType(CornerType cornerType);

public:
	// get the angle value according to the range.
	// notes: such angle is just used for showing preview.
	double GetRangeAngle() const;

	// get the low and up angle of the range.
	double GetLowAngle() const;
	double GetUpAngle() const;

	// whether the angle is within the range.
	BOOL AngleInRange(double dAngle) const;

private:
	CORNER_RANGE_TYPE m_rangeType;
	CornerType m_iCornerType;
	double m_dVal;

    // 关于角两侧线长的设置，一旦开启，就和角度范围一起作为条件。
    BOOL m_bEnableCornerLineLengthControl;
    double m_dCornerLineLength; // 角上线长（角两侧的线长）至少有一个要小于这个值。
};

END_CUTLEADER_NAMESPACE()

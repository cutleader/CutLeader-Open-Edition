#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(MicroJointRangeInfo)

BEGIN_CUTLEADER_NAMESPACE()

enum MicroJointPosition
{
	MICRO_NONE		= 0,

	ONE_BOTTOM		= 1, // 轮廓下侧分配一个微连。
	ONE_RIGHT		= 2, // 轮廓右侧分配一个微连。
	ONE_TOP			= 3, // 轮廓上侧分配一个微连。
	ONE_LEFT		= 4, // 轮廓左侧分配一个微连。

	TWO_VERTICAL	= 5, // 轮廓上侧和下侧各分配一个微连。
	TWO_HORIZONTAL	= 6, // 轮廓左侧和右侧各分配一个微连。

	THREE_BOTTOM	= 7, // 轮廓上侧分配一个微连，下侧分配两个微连。
	THREE_RIGHT		= 8, // 轮廓左侧分配一个微连，右侧分配两个微连。
	THREE_TOP		= 9, // 轮廓下侧分配一个微连，上侧分配两个微连。
	THREE_LEFT		= 10,// 轮廓右侧分配一个微连，左侧分配两个微连。 

	FOUR_FULL		= 11,// 轮廓上下左右侧各分配一个微连。

	SIX_DOWN		= 12,
	SIX_UP			= 13
};

// a MicroJointRangeInfo object represent a half data of a tab.
// notes:
//   1) a MicroJointRangeInfo object occupy one row in table expLib_bmjrangeinfo.
class ClExpertLib_Export MicroJointRangeInfo : public ClData
{
public:
	MicroJointRangeInfo();
	~MicroJointRangeInfo();

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void IsHole(BOOL bHole) { m_bHole = bHole; }
	BOOL IsHole() const { return m_bHole; }

	void SetJointWidth(double dJointWidth) { m_dJointWidth = dJointWidth; }
	double GetJointWidth() const { return m_dJointWidth; }

	// in this function, we also should set "m_MicroJointPos".
	void SetQuantity(int iQuantity);
	int GetQuantity() const { return m_iQuantity; }

	void SetMicroJointPos(MicroJointPosition emMicroJointPos) { m_emMicroJointPos = emMicroJointPos; }
	MicroJointPosition GetMicroJointPos() const { return m_emMicroJointPos; }

	void SetRangeX(double dRangeX) { m_dRangeX = dRangeX; }
	double GetRangeX() const { return m_dRangeX; }

	void SetRangeY(double dRangeY) { m_dRangeY = dRangeY; }
	double GetRangeY() const { return m_dRangeY; }

public: // other functions.
	// get the angles by the "m_iMicroJointPos"
	// notes: 
	//   1) this function return a list of angle, actually the angles follow the CCW direction.
	std::vector<double> GetAngles();

private:
	// 1: hole, 0: perimeter
	BOOL m_bHole;

	// the width of the micro joint.
	double m_dJointWidth;

	// notes:
	//   1) Max value is 4 for hole range.
	//   2) Max value is 6 for perimeter range.
	int m_iQuantity;

	// the positions of the micro joint.
	MicroJointPosition m_emMicroJointPos;

	// the range size.
	double m_dRangeX;
	double m_dRangeY;
};

END_CUTLEADER_NAMESPACE()

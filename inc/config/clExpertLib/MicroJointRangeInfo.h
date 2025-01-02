#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(MicroJointRangeInfo)

BEGIN_CUTLEADER_NAMESPACE()

enum MicroJointPosition
{
	MICRO_NONE		= 0,

	ONE_BOTTOM		= 1, // �����²����һ��΢����
	ONE_RIGHT		= 2, // �����Ҳ����һ��΢����
	ONE_TOP			= 3, // �����ϲ����һ��΢����
	ONE_LEFT		= 4, // ����������һ��΢����

	TWO_VERTICAL	= 5, // �����ϲ���²������һ��΢����
	TWO_HORIZONTAL	= 6, // ���������Ҳ������һ��΢����

	THREE_BOTTOM	= 7, // �����ϲ����һ��΢�����²��������΢����
	THREE_RIGHT		= 8, // ����������һ��΢�����Ҳ��������΢����
	THREE_TOP		= 9, // �����²����һ��΢�����ϲ��������΢����
	THREE_LEFT		= 10,// �����Ҳ����һ��΢��������������΢���� 

	FOUR_FULL		= 11,// �����������Ҳ������һ��΢����

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

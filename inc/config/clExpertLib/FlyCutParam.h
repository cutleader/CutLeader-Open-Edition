#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(FlyCutParam)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLib_Export FlyCutParam : public ClData
{
public:
	FlyCutParam(void);
	virtual ~FlyCutParam(void);

public: // implements IData interface.
	virtual IDataPtr Clone() const override;

public: // get/set functions.
	void IsVerticalCut(bool bVerticalCut) { m_bVerticalCut = bVerticalCut; }
	bool IsVerticalCut() const { return m_bVerticalCut; }

	double GetMax_turing_radius() const { return m_dMax_turing_radius; }
	void SetMax_turing_radius(double dMax_turing_radius) { m_dMax_turing_radius = dMax_turing_radius; }

private:
	// 选好起始角后，就按照竖直的方向开始切。
	bool m_bVerticalCut;

	// 最大转弯半径。
	double m_dMax_turing_radius;
};

END_CUTLEADER_NAMESPACE()

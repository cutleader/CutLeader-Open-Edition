#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(ChainCutParam)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLib_Export ChainCutParam : public ClData
{
public:
	ChainCutParam(void);
	~ChainCutParam(void);

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void IsCutAlongContour(bool bCutAlongContour) { m_bCutAlongContour = bCutAlongContour; }
	bool IsCutAlongContour() { return m_bCutAlongContour; }

	void SetAvoidDis(double dAvoidDis) { m_dAvoidDis = dAvoidDis; }
	double GetAvoidDis() { return m_dAvoidDis; }

private:
	// 是否沿着零件轮廓线做避让
	bool m_bCutAlongContour;

	// the avoid distance, this defines the distance of the cut header to the actual cutout.
	// notes:
	//   1) use this value to calculate AvoidRect object. please refer to AvoidRectInfo class.
	double m_dAvoidDis;
};

END_CUTLEADER_NAMESPACE()

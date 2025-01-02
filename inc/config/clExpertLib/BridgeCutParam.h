#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(BridgeCutParam)

BEGIN_CUTLEADER_NAMESPACE()

// 桥接规则。
class ClExpertLib_Export BridgeCutParam : public ClData
{
public:
	BridgeCutParam(void);
	~BridgeCutParam(void);

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetBdgeWidth(double dBdgeWidth) { m_dBdgeWidth = dBdgeWidth; }
	double GetBdgeWidth() { return m_dBdgeWidth; }

private:
	// 桥的宽度。
	double m_dBdgeWidth;
};

END_CUTLEADER_NAMESPACE()

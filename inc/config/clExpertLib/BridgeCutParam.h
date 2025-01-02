#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(BridgeCutParam)

BEGIN_CUTLEADER_NAMESPACE()

// �Žӹ���
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
	// �ŵĿ�ȡ�
	double m_dBdgeWidth;
};

END_CUTLEADER_NAMESPACE()

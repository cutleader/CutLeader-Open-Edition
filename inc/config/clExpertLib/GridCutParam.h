#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(GridCutParam)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLib_Export GridCutParam : public ClData
{
public:
	GridCutParam(void);
	virtual ~GridCutParam(void);

public: // implements IData interface.
	virtual IDataPtr Clone() const override;

public: // get/set functions.
	void IsVertical_4_inner(bool bVertical_4_inner) { m_bVertical_4_inner = bVertical_4_inner; }
	bool IsVertical_4_inner() const { return m_bVertical_4_inner; }

	void IsCCW_4_outer(bool bCCW_4_outer) { m_bCCW_4_outer = bCCW_4_outer; }
	bool IsCCW_4_outer() const { return m_bCCW_4_outer; }

private:
	// 内部切割先纵向切割。
	bool m_bVertical_4_inner;

	// 外框按照逆时针切割。
	bool m_bCCW_4_outer;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "IRptEntity.h"

BEGIN_CUTLEADER_NAMESPACE()

// the interface for all entities in report.
class RptEntityBase : virtual public IRptEntity
{
public:
	RptEntityBase(void);
	~RptEntityBase(void);

public: // implement IRptEntity interface.
	virtual void GetBorderRect(double& dLeft, double& dRight, double& dTop, double& dBottom) const;
	virtual void SetBorderRect(double dLeft, double dRight, double dTop, double dBottom);

protected:
	// the report entity position.
	// notes:
	//   1) in inch unit.
	double m_dLeft;
	double m_dRight;
	double m_dTop;
	double m_dBottom;
};

END_CUTLEADER_NAMESPACE()

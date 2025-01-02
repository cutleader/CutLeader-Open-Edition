#pragma once

#include "clReportPrinterCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// 报表实体类型。
typedef enum tagRptEntityType
{
	RPT_ENTITY_TEXT				= 1,
	RPT_ENTITY_IMG				= 2,
	RPT_ENTITY_TABLE			= 3,
	RPT_ENTITY_LINE				= 4,
	RPT_ENTITY_BOX				= 5,
	RPT_ENTITY_ELLIPSE			= 6,
	RPT_ENTITY_LABEL			= 7,
	RPT_ENTITY_BARCODE			= 8,
} RPT_ENTITY_TYPE;

// 报表实体对象。
class IRptEntity
{
public:
	virtual RPT_ENTITY_TYPE GetType() const = 0;

	virtual void GetBorderRect(double& dLeft, double& dRight, double& dTop, double& dBottom) const = 0;
	virtual void SetBorderRect(double dLeft, double dRight, double dTop, double dBottom) = 0;
};

END_CUTLEADER_NAMESPACE()

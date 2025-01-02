#pragma once

#include "BorderRptEntityBase.h"

DECLARE_CUTLEADER_CLASS(LabelRptEntity)

BEGIN_CUTLEADER_NAMESPACE()

// ����ǩ������ʵ�塣
class ClReportPrinter_Export LabelRptEntity : public BorderRptEntityBase
{
public:
	LabelRptEntity(void);
	~LabelRptEntity(void);

public: // implement IRptEntity interface.
	virtual RPT_ENTITY_TYPE GetType() const { return RPT_ENTITY_LABEL; }
};

END_CUTLEADER_NAMESPACE()

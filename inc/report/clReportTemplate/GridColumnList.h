#pragma once

#include "clReportTemplateCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(GridColumn)
DECLARE_CUTLEADER_CLASS(GridColumnList)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export GridColumnList : public std::vector<GridColumnPtr>,
					   public ClData
{
public:
	GridColumnList(void);
	~GridColumnList(void);

public: // functions from IData interface.
	virtual BOOL IsModified() const override;
	virtual IDataPtr Clone() const override;

public:
	GridColumnPtr GetItemByID(LONGLONG iId);
	void DeleteColumn(LONGLONG iColumnID);
};

END_CUTLEADER_NAMESPACE()

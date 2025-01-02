#pragma once

#include "ClData.h"
#include "clExpertLibCommon.h"

DECLARE_CUTLEADER_CLASS(ToolInstance)
DECLARE_CUTLEADER_CLASS(ToolInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLib_Export ToolInstanceList : public std::vector<ToolInstancePtr>,
                                          public ClData
{
public:
	ToolInstanceList(void);
	~ToolInstanceList(void);

public: // implements IData interface.
    virtual BOOL IsModified() const override;
    virtual IDataPtr Clone() const override;

public:
	ToolInstancePtr GetToolByID(LONGLONG iToolID) const;
    ToolInstancePtr GetToolByName(CString strToolName) const;
    void DeleteToolByID(LONGLONG iToolID);
};

END_CUTLEADER_NAMESPACE()

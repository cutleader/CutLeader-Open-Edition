#pragma once

#include "clMaterialLibCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(MaterialSizeList)

BEGIN_CUTLEADER_NAMESPACE()

class ClMaterialLib_Export MaterialSizeList : public ClData, public std::vector<IMaterialSizePtr>
{
public:
	MaterialSizeList(void);
	virtual ~MaterialSizeList(void);

public: // implement IData interface
	IDataPtr Clone() const override;

public:
	void DeleteItemByID(LONGLONG iId);

	// get the material size object by the ID.
	IMaterialSizePtr GetItemByID(LONGLONG iId) const;
};

END_CUTLEADER_NAMESPACE()

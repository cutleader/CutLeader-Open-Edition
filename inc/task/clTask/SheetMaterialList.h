#pragma once

#include "ClTaskCommon.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(SheetMaterial)
DECLARE_CUTLEADER_CLASS(SheetMaterialList)


BEGIN_CUTLEADER_NAMESPACE()

class ClTask_Export SheetMaterialList : public std::vector<SheetMaterialPtr>
{
public:
	SheetMaterialList(void);
	~SheetMaterialList(void);

public:
    const SheetMaterial* GetItemBySheetID(LONGLONG iSheetID) const;
};

END_CUTLEADER_NAMESPACE()

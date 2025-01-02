#pragma once

#include "ClTaskCommon.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(NestResultOnSheetMaterial)


BEGIN_CUTLEADER_NAMESPACE()

class ClTask_Export NestResultListOnSheetMaterial : public std::vector<NestResultOnSheetMaterialPtr>
{
public:
	NestResultListOnSheetMaterial(void);
	~NestResultListOnSheetMaterial(void);

public:
    NestResultOnSheetMaterialPtr GetItemBySheetMaterialID(LONGLONG iSheetMaterialID) const;
    NestResultOnSheetMaterialPtr GetItemByID(LONGLONG iID) const;
};

END_CUTLEADER_NAMESPACE()

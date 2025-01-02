#pragma once

#include "clNestJobCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(NestMaterial)
DECLARE_CUTLEADER_CLASS(NestMaterialList)

BEGIN_CUTLEADER_NAMESPACE()

class ClNestJob_Export NestMaterialList : public std::vector<NestMaterialPtr>
{
public:
	NestMaterialList(void);
	~NestMaterialList(void);

public:
	// filter out the remnant nest mat.
	NestMaterialListPtr GetNestMatList(BOOL bRemnant);
};

END_CUTLEADER_NAMESPACE()

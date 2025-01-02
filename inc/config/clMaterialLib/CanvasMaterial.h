#pragma once

#include "clMaterialLibCommon.h"
#include "MaterialSizeBase.h"

DECLARE_CUTLEADER_CLASS(CanvasMaterial)

BEGIN_CUTLEADER_NAMESPACE()

// 这种材料用于底图。
// 注：
// 1) 这种材料尺寸无限大、数量无限多。
// 2) 每种材料厚度有一个这种材料尺寸，在创建材料厚度时创建，只能用于底图，一般来说在界面上不可见。
class ClMaterialLib_Export CanvasMaterial : public MaterialSizeBase
{
public:
	CanvasMaterial();
	CanvasMaterial(LONGLONG iID, LONGLONG iParentID, CString strName, BOOL bRemnant, double dWidth, double dHeight, int iTotalNum, int iAvailNum, CString strComment);
	~CanvasMaterial(void);

public: // implement IData interface
	IDataPtr Clone() const override;

public: // implement IMaterialSize interface
	virtual MAT_SIZE_TYPE GetType() const override { return MAT_SIZE_Canvas; }
};

END_CUTLEADER_NAMESPACE()

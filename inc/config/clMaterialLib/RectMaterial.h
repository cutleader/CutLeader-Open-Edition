#pragma once

#include "clMaterialLibCommon.h"
#include "MaterialSizeBase.h"

DECLARE_CUTLEADER_CLASS(RectMaterial)

BEGIN_CUTLEADER_NAMESPACE()

// ¾ØÐÎ²ÄÁÏ¡£
class ClMaterialLib_Export RectMaterial : public MaterialSizeBase
{
public:
	RectMaterial(void);
	RectMaterial(LONGLONG iID, LONGLONG iParentID, CString strName, BOOL bRemnant, double dWidth, double dHeight, 
			int iTotalNum, int iAvailNum, CString strComment);
	~RectMaterial(void);

public: // implement IData interface
	IDataPtr Clone() const override;

public: // implement IMaterialSize interface
	virtual MAT_SIZE_TYPE GetType() const override { return MAT_SIZE_RECT; }
};

END_CUTLEADER_NAMESPACE()

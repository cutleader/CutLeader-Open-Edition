#pragma once

#include "clNestJobCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(IMaterialSize)

BEGIN_CUTLEADER_NAMESPACE()

// “排版材料”对象。
class ClNestJob_Export NestMaterial : public ClData
{
public:
	NestMaterial(void);
	NestMaterial(IMaterialSizePtr pMaterialSize);

	~NestMaterial(void);

public: // set/get functions.
	void SetMatSize(IMaterialSizePtr pMaterialSize) { m_pMatSize = pMaterialSize; }
	IMaterialSizePtr GetMatSize() const { return m_pMatSize; }

	void SetIsEachMaterialMustHasSameLayout(bool b) { m_bEachMaterialMustHasSameLayout = b; }
	bool GetIsEachMaterialMustHasSameLayout() const { return m_bEachMaterialMustHasSameLayout; }

private:
	IMaterialSizePtr m_pMatSize;

	// 是否要求每个材料上的布局都一样。
	// 注：
	//  1) 当材料来自任务中的板材，并且数量大于一时，就要求每个板子上的布局一样。
	bool m_bEachMaterialMustHasSameLayout;
};

END_CUTLEADER_NAMESPACE()

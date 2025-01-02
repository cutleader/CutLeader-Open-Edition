#pragma once

#include "ClTaskCommon.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(TaskPartsNestResult)
DECLARE_CUTLEADER_CLASS(NestResultListOnSheetMaterial)
DECLARE_CUTLEADER_CLASS(SheetList)


BEGIN_CUTLEADER_NAMESPACE()

// 排版任务中零件，得到的排版结果。
class ClTask_Export TaskPartsNestResult
{
public:
	TaskPartsNestResult(NestResultListOnSheetMaterialPtr pNestResultListOnSheetMaterial, SheetListPtr pNewSheetList);
	~TaskPartsNestResult(void);

public:
    const NestResultListOnSheetMaterial* GetNestResultListOnSheetMaterial() const { return m_pNestResultListOnSheetMaterial.get(); }
    const SheetList* GetNewSheetList() const { return m_pNewSheetList.get(); }

public:
    int GetPartNestedCount(LONGLONG iPartID) const;

private:
    // 在“板材材料”上的排版结果。
    NestResultListOnSheetMaterialPtr m_pNestResultListOnSheetMaterial;

    // 用材料库中新材料排出的板材。
    SheetListPtr m_pNewSheetList;
};

END_CUTLEADER_NAMESPACE()

#pragma once

#include "ClTaskCommon.h"
#include "ClData.h"


DECLARE_CUTLEADER_CLASS(NestResultOnSheetMaterial)
DECLARE_CUTLEADER_CLASS(SheetMaterial)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()


// 在一个板材材料上的排版结果。
class NestResultOnSheetMaterial : public ClData
{
public:
    NestResultOnSheetMaterial(SheetMaterialPtr pSheetMaterial, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstList);
	~NestResultOnSheetMaterial(void);

public:
    const SheetMaterial* GetSheetMaterial() const { return m_pSheetMaterial.get(); }
    PartPlacementListPtr GetPartPmtList() const { return m_pPartPlacementList; }
    PartInstanceListPtr GetPartInstList() const { return m_pPartInstanceList; }

public:
    int GetPartNestedCount(LONGLONG iPartID) const;

private:
    // 板材材料
    SheetMaterialPtr m_pSheetMaterial;

    // 排版结果
    PartPlacementListPtr m_pPartPlacementList;
    PartInstanceListPtr m_pPartInstanceList;
};

END_CUTLEADER_NAMESPACE()

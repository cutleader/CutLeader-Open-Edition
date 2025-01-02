#pragma once

#include "ClTaskCommon.h"
#include "ClData.h"


DECLARE_CUTLEADER_CLASS(NestResultOnSheetMaterial)
DECLARE_CUTLEADER_CLASS(SheetMaterial)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()


// ��һ����Ĳ����ϵ��Ű�����
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
    // ��Ĳ���
    SheetMaterialPtr m_pSheetMaterial;

    // �Ű���
    PartPlacementListPtr m_pPartPlacementList;
    PartInstanceListPtr m_pPartInstanceList;
};

END_CUTLEADER_NAMESPACE()

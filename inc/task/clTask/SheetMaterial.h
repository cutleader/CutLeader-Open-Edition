#pragma once

#include "ClTaskCommon.h"
#include "ClData.h"


DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(PolygonMaterial)
DECLARE_CUTLEADER_CLASS(SheetMaterial)
DECLARE_CUTLEADER_CLASS(LineArc2DList)


BEGIN_CUTLEADER_NAMESPACE()


// “板材材料”，即用现有板材的剩余区域生成的材料信息。
class ClTask_Export SheetMaterial : public ClData
{
public:
    SheetMaterial(SheetPtr pSheet, IMaterialSizePtr pMaterialSize, const std::vector<PolygonMaterialPtr>& polyMaterials, LineArc2DListPtr pLineArc2Ds_ofParts);
	virtual ~SheetMaterial(void);

public:
	SheetPtr GetSheet() const { return m_pSheet; }
	const IMaterialSize* GetMatSize() const { return m_pMaterialSize.get(); }
    const LineArc2DList* GetLineArc2Ds_ofParts() const { return m_pLineArc2Ds_ofParts.get(); }
    const std::vector<PolygonMaterialPtr>& GetPolyMaterials() const { return m_polyMaterials; }

private:
	// 板材和及其相关数据。
	SheetPtr m_pSheet;
	IMaterialSizePtr m_pMaterialSize;
    LineArc2DListPtr m_pLineArc2Ds_ofParts; // 板材上已排零件的基本图形

	// 从板材生成的异形材料。
    std::vector<PolygonMaterialPtr> m_polyMaterials;
};

END_CUTLEADER_NAMESPACE()

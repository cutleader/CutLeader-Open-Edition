#pragma once

#include "ClTaskCommon.h"
#include "ClData.h"


DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(PolygonMaterial)
DECLARE_CUTLEADER_CLASS(SheetMaterial)
DECLARE_CUTLEADER_CLASS(LineArc2DList)


BEGIN_CUTLEADER_NAMESPACE()


// ����Ĳ��ϡ����������а�ĵ�ʣ���������ɵĲ�����Ϣ��
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
	// ��ĺͼ���������ݡ�
	SheetPtr m_pSheet;
	IMaterialSizePtr m_pMaterialSize;
    LineArc2DListPtr m_pLineArc2Ds_ofParts; // �������������Ļ���ͼ��

	// �Ӱ�����ɵ����β��ϡ�
    std::vector<PolygonMaterialPtr> m_polyMaterials;
};

END_CUTLEADER_NAMESPACE()

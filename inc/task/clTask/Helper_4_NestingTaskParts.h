#pragma once

#include "ClTaskCommon.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(SheetList)
DECLARE_CUTLEADER_CLASS(PolygonMaterial)
DECLARE_CUTLEADER_CLASS(SheetMaterialList)
DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(TaskPartsNestResult)
DECLARE_CUTLEADER_CLASS(SheetMaterial)


BEGIN_CUTLEADER_NAMESPACE()

class ClTask_Export Helper_4_NestingTaskParts
{
public:
    // �Ӱ�ĵ��������򴴽�����β��ϡ�
    // ע��
    //  1) ��Ҫ�ѡ�������Ķ���β��ϡ����ڼ��ϵ����
	//  2) ���pSheet�õĲ����Ͼ�����Ч����Ҳ��Ҫ�����Ƿ�����½��Ķ���β����С�
    static std::vector<PolygonMaterialPtr> CreatePolyMaterials_fromRemnantRegions(const Sheet* pSheet);

    // �����Ű�����
    static TaskPartsNestResultPtr GenerateTaskPartsNestResult(const SheetList* pSheetList, const TaskItem* pTaskItem, const SheetMaterialList* pSheetMaterials);

    // �õ�����õ��ġ���Ĳ��ϡ���Ҫ�ǰ��pSheet��ͨ�����Ͽ���ϴ����ģ��ͷ��ؿա�
    static SheetMaterialPtr GetHostSheetMaterial(const Sheet* pSheet, const SheetMaterialList* pSheetMaterials);
};

END_CUTLEADER_NAMESPACE()
